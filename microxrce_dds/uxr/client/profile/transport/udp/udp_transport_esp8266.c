#include "udp_transport_esp8266.h"
#include <uxr/client/profile/transport/udp/udp_transport_internal.h>
#include "../esp8266_transport_internal.h"

bool uxr_init_udp_platform(uxrUDPPlatform *platform, const char *ip, uint16_t port)
{
  return initUDPESP8266(platform->udp_instance, ip, port);
}

bool uxr_close_udp_platform(uxrUDPPlatform *platform)
{
  return closeUDPESP8266(platform->udp_instance);
}

size_t uxr_write_udp_data_platform(uxrUDPPlatform *platform, const uint8_t *buf, size_t len, uint8_t *errcode)
{
  size_t rv = 0;
  (void)(errcode);
  rv = writeUDPESP8266(platform->udp_instance, buf, len);

  return rv;
}

size_t uxr_read_udp_data_platform(uxrUDPPlatform *platform, uint8_t *buf, size_t len, int timeout, uint8_t *errcode)
{
  size_t rv = 0;
  (void)(errcode);
  rv = readUDPESP8266(platform->udp_instance, buf, len, timeout);

  return rv;
}