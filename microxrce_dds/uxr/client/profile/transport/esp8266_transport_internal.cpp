#include "esp8266_transport_internal.h"

#include <sys/types.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

static const char *agent_address;
static uint16_t agent_port;

// For UDP Transport
bool initUDPESP8266(void *udp_instance, const char *ip, uint16_t port)
{
  bool rv = false;
  
  if (udp_instance == nullptr)
  {
    return false;
  }

  ((WiFiUDP *)(udp_instance))->begin(port);
  agent_address = ip;
  agent_port = port;

  rv = true;

  return rv;
}

bool closeUDPESP8266(void *udp_instance)
{
  ((WiFiUDP *)(udp_instance))->stop();
  return true;
}

size_t writeUDPESP8266(void *udp_instance, const uint8_t *buf, size_t len)
{
  size_t tx_len = 0;

  ((WiFiUDP *)(udp_instance))->beginPacket(agent_address, agent_port);
  tx_len = ((WiFiUDP *)(udp_instance))->write(buf, len);
  ((WiFiUDP *)(udp_instance))->endPacket();

  return tx_len;
}

size_t readUDPESP8266(void *udp_instance, uint8_t *buf, size_t len, int timeout)
{
  size_t rv = 0;
  uint32_t pre_time = millis();

  while (rv <= 0 && (millis() - pre_time < (uint32_t)timeout))
  {
    ((WiFiUDP *)(udp_instance))->parsePacket();
    rv = ((WiFiUDP *)(udp_instance))->available();
  }

  if (rv > len)
  {
    rv = len;
  }

  if (0 < rv)
  {
    ((WiFiUDP *)(udp_instance))->read(buf, len);
  }

  return rv;
}
