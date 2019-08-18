#include <Arduino.h>

#include "helloworld.h"

#include <uxr/client/client.h>
#include <ucdr/microcdr.h>

#include <spi_flash.h>

#include <stdio.h> //printf
// #include <string.h>   //strcmp
// #include <stdlib.h>   //atoi
// #include <inttypes.h> // printing int64_t

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#define STREAM_HISTORY 2
#define BUFFER_SIZE UXR_CONFIG_UDP_TRANSPORT_MTU *STREAM_HISTORY

#define SSID "Base Station"
#define PASSWD "robotics@rignitc"
#define MAX_TOPICS 1000

#define MAX_AGENTS 10

uxrAgentAddress agent;

void connectToWifi()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWD);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  printf("\n Connected to: %s \n", SSID);
  printf("IP Address: %s \n", WiFi.localIP().toString().c_str());
  return;
}

void on_agent_found(const uxrAgentAddress *address, void *args)
{
  (void)args;
  agent.ip = address->ip;
  agent.port = address->port;
  printf("Found agent => ip: %s, port: %d\n", agent.ip, agent.port);
  return;
}

void setup()
{
  Serial.begin(115200);

  WiFiUDP udp_instance;

  //Initialise the agent object
  agent.ip = "0.0.0.0";
  agent.port = 0;

  connectToWifi();

  uxr_discovery_agents_default(2, 1000, on_agent_found, NULL, &udp_instance);

  // Transport
  uxrUDPTransport transport;
  uxrUDPPlatform udp_platform;
  udp_platform.udp_instance = &udp_instance;

  uxrSession session;

  uint8_t output_reliable_stream_buffer[BUFFER_SIZE];
  uint8_t input_reliable_stream_buffer[BUFFER_SIZE];

  const char *participant_xml = "<dds>"
                                "<participant>"
                                "<rtps>"
                                "<name>default_xrce_participant</name>"
                                "</rtps>"
                                "</participant>"
                                "</dds>";
  const char *topic_xml = "<dds>"
                          "<topic>"
                          "<name>HelloWorldTopic</name>"
                          "<dataType>HelloWorld</dataType>"
                          "</topic>"
                          "</dds>";
  const char *publisher_xml = "";
  const char *datawriter_xml = "<dds>"
                               "<data_writer>"
                               "<topic>"
                               "<kind>NO_KEY</kind>"
                               "<name>HelloWorldTopic</name>"
                               "<dataType>HelloWorld</dataType>"
                               "</topic>"
                               "</data_writer>"
                               "</dds>";

  uint32_t count = 0;
  const char * ip_copy = strdup(agent.ip); // copy of agent.ip should be taken

  while (count != MAX_TOPICS)
  {
    if (!uxr_init_udp_transport(&transport, &udp_platform, ip_copy, agent.port)) //copy of agent.ip should be passed
    {
      Serial.print("Error at create transport.\n");
    }
    else
    {
      Serial.print("Created transport.\n");
    }

    // Session
    uxr_init_session(&session, &transport.comm, 0xAAAABBBB);
    if (!uxr_create_session(&session))
    {
      printf("Error at create session.\n");
    }
    else
    {
      printf("Created Session.\n");
    }

    // Streams
    uxrStreamId reliable_out = uxr_create_output_reliable_stream(&session, output_reliable_stream_buffer, BUFFER_SIZE, STREAM_HISTORY);
    uxr_create_input_reliable_stream(&session, input_reliable_stream_buffer, BUFFER_SIZE, STREAM_HISTORY);

    // Create entities
    uxrObjectId participant_id = uxr_object_id(0x01, UXR_PARTICIPANT_ID);
    uint16_t participant_req = uxr_buffer_create_participant_xml(&session, reliable_out, participant_id, 0, participant_xml, UXR_REPLACE);
    uxrObjectId topic_id = uxr_object_id(0x01, UXR_TOPIC_ID);
    uint16_t topic_req = uxr_buffer_create_topic_xml(&session, reliable_out, topic_id, participant_id, topic_xml, UXR_REPLACE);
    uxrObjectId publisher_id = uxr_object_id(0x01, UXR_PUBLISHER_ID);
    uint16_t publisher_req = uxr_buffer_create_publisher_xml(&session, reliable_out, publisher_id, participant_id, publisher_xml, UXR_REPLACE);
    uxrObjectId datawriter_id = uxr_object_id(0x01, UXR_DATAWRITER_ID);
    uint16_t datawriter_req = uxr_buffer_create_datawriter_xml(&session, reliable_out, datawriter_id, publisher_id, datawriter_xml, UXR_REPLACE);

    //Send create entities message and wait its status
    uint8_t status[4];
    uint16_t requests[4] = {participant_req, topic_req, publisher_req, datawriter_req};

    if (!uxr_run_session_until_all_status(&session, 1000, requests, status, 4))
    {
      //printf("The request is: participant: %i topic: %i publisher: %i darawriter: %i\n", requests[0], requests[1], requests[2], requests[3]);
      printf("Error at create entities: participant: %i topic: %i publisher: %i darawriter: %i\n", status[0], status[1], status[2], status[3]);
      // printf("Time:%" PRId64 "\n", uxr_millis());
    }
    else
    {
      printf("Created Entities. \n");
    }

    // Write topics
    bool connected = true;
    while (connected && count < MAX_TOPICS)
    {
      helloworld topic = {++count, "Hello DDS world!"};

      ucdrBuffer ub;
      uint32_t topic_size = helloworld_size_of_topic(&topic, 0);
      uxr_prepare_output_stream(&session, reliable_out, datawriter_id, &ub, topic_size);
      helloworld_serialize_topic(&ub, &topic);

      printf("Send topic: %s, id: %i\n", topic.message, topic.index);
      connected = uxr_run_session_until_confirm_delivery(&session, 1000);
    }

    if (!(count == MAX_TOPICS))
    {
      if (!(WiFi.status() == WL_CONNECTED))
      {
        printf("%s not found... Reconnecting\n", SSID);
        connectToWifi();
      }
      else if (!connected)
      {
        printf("Master Missing!!!\n");
        printf("Reconnecting...\n");
      }
    }
  }
  // Delete resources
  uxr_delete_session(&session);
  uxr_close_udp_transport(&transport);
}

void loop()
{
}