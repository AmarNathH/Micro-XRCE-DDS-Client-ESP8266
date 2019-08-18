#include "esp8266_discovery_internal.h"

#include <sys/types.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <lwip/inet.h>

//******* INTERNAL *************
int isValidIp4(const char *str)
{
  int segs = 0;  /* Segment count. */
  int chcnt = 0; /* Character count within segment. */
  int accum = 0; /* Accumulator for segment. */

  /* Catch NULL pointer. */
  if (str == nullptr)
    return 0;

  /* Process every character in string. */
  while (*str != '\0')
  {
    /* Segment changeover. */

    if (*str == '.')
    {
      /* Must have some digits in segment. */

      if (chcnt == 0)
        return 0;

      /* Limit number of segments. */

      if (++segs == 4)
        return 0;

      /* Reset segment values and restart loop. */

      chcnt = accum = 0;
      str++;
      continue;
    }
    /* Check numeric. */

    if ((*str < '0') || (*str > '9'))
      return 0;

    /* Accumulate and check segment. */

    if ((accum = accum * 10 + *str - '0') > 255)
      return 0;

    /* Advance other segment specific stuff and continue loop. */

    chcnt++;
    str++;
  }

  /* Check enough segments and enough characters in last segment. */

  if (segs != 3)
    return 0;

  if (chcnt == 0)
    return 0;

  /* Address okay. */
  return 1;
}

//*****************EXTERNAL************************

// For UDP Discovery
bool initDiscoveryESP8266(void *udp_instance)
{
  // printf("Running Discovery init\n");
  bool rv = false;
  if (udp_instance == nullptr)
  {
    return false;
  }

  ((WiFiUDP *)(udp_instance));

  rv = true;

  return rv;
}

bool sendDiscoveryESP8266(void *udp_instance, const uint8_t *buf, size_t len, const char *ip, uint16_t port)
{
  // printf("Running Discovery send\n");
  bool rv = true;

  ((WiFiUDP *)(udp_instance))->begin(port);

  // printf("Before Using ip: %s\n", ip);

  if (isValidIp4(ip)) // Error at this point after first attempt
  {
    ssize_t tx_len;
    // printf("IP:%s PORT:%i\n", ip, port);

    ((WiFiUDP *)(udp_instance))->beginPacket(ip, port);
    tx_len = ((WiFiUDP *)(udp_instance))->write(buf, len);
    ((WiFiUDP *)(udp_instance))->endPacket();

    // printf("Data sent:%i\n", buf);

    if (0 > tx_len)
      rv = false;
  }

  // printf("After using ip: %s\n", ip);
  return rv;
}

bool readDiscoveryESP8266(void *udp_instance, uint8_t *udp_buffer, uint8_t **buf, size_t *len, int timeout)
{
  // printf("Running Discovery read\n");
  uint8_t rv = 0;
  uint32_t pre_time = millis();

  while (rv <= 0 && (millis() - pre_time < (uint32_t)timeout))
  {
    ((WiFiUDP *)(udp_instance))->parsePacket();
    rv = ((WiFiUDP *)(udp_instance))->available();
  }

  if (rv > *len)
  {
    rv = *len;
  }

  if (0 < rv)
  {
    ssize_t bytes_received = ((WiFiUDP *)(udp_instance))->read(udp_buffer, *len);
    // printf("Bytes received:%i\n", bytes_received);
    *len = (size_t)bytes_received;
    *buf = udp_buffer;
  }

  return rv;
}

bool closeDiscoveryESP8266(void *udp_instance)
{
  // printf("Running Discovery close\n");

  ((WiFiUDP *)(udp_instance))->stop();
  return true;
}

void bytesToIp(const uint8_t *bytes, char *ip)
{
  uint32_t addr = (uint32_t)(*bytes + (*(bytes + 1) << 8) + (*(bytes + 2) << 16) + (*(bytes + 3) << 24));
  char *internal_ip = inet_ntoa(addr);
  // printf("The ip address is:%s", internal_ip);
  strcpy(ip, internal_ip);
}
