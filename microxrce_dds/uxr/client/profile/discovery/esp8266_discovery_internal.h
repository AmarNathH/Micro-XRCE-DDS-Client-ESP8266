#ifndef _SRC_C_PROFILE_DISCOVERY_ESP8266_DISCOVERY_INTERNAL_H_
#define _SRC_C_PROFILE_DISCOVERY_ESP8266_DISCOVERY_INTERNAL_H_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif
// For UDP Datagram
bool initDiscoveryESP8266(void* udp_instance);
bool sendDiscoveryESP8266(void* udp_instance, const uint8_t *buf, size_t len, const char *ip, uint16_t port);
bool readDiscoveryESP8266(void* udp_instance, uint8_t *udp_buffer ,uint8_t **buf, size_t *len, int timeout);
bool closeDiscoveryESP8266(void* udp_instance);
void bytesToIp(const uint8_t* bytes, char* ip);

#ifdef __cplusplus
}
#endif

#endif //_SRC_C_PROFILE_DISCOVERY_ESP8266_DISCOVERY_INTERNAL_H_