#ifndef _UXR_CLIENT_UDP_TRANSPORT_ESP8266_H_
#define _UXR_CLIENT_UDP_TRANSPORT_ESP8266_H_

#include <uxr/client/config.h>

#if defined(PLATFORM_NAME_ESP8266)

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct uxrUDPPlatform
{
  int socket_fd;
  void* udp_instance;
} uxrUDPPlatform;

#ifdef __cplusplus
}
#endif

#endif //PLATFORM NAME ESP8266

#endif //_UXR_CLIENT_UDP_TRANSPORT_ESP8266_H_