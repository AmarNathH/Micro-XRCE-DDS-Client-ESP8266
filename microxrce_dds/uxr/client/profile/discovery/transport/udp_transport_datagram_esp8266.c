#include <uxr/client/config.h>
#include "udp_transport_datagram_internal.h"
#include "../esp8266_discovery_internal.h"


#if defined(PLATFORM_NAME_ESP8266)

bool uxr_init_udp_transport_datagram(uxrUDPTransportDatagram *transport)
{
    return initDiscoveryESP8266(transport->udp_instance);
}

bool uxr_close_udp_transport_datagram(uxrUDPTransportDatagram *transport)
{
    return closeDiscoveryESP8266(transport->udp_instance);
}

bool uxr_udp_send_datagram_to(uxrUDPTransportDatagram *transport, const uint8_t *buf, size_t len, const char *ip, uint16_t port)
{
    return sendDiscoveryESP8266(transport->udp_instance, buf, len, ip, port);
}

bool uxr_udp_recv_datagram(uxrUDPTransportDatagram *transport, uint8_t **buf, size_t *len, int timeout)
{
    return readDiscoveryESP8266(transport->udp_instance, transport->buffer, buf, len, timeout);
}

void uxr_bytes_to_ip(const uint8_t* bytes, char* ip)
{
    bytesToIp(bytes,ip);
}

#endif // PLATFORM_NAME_ESP8266