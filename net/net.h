#ifndef UDPSOCKET_NET_H
#define UDPSOCKET_NET_H

#include <stddef.h>
#include <netinet/in.h>

void net_init(void);
void net_poll(void);
void net_send_to(const void* data, size_t size, const struct sockaddr_in* addr);
void net_shutdown(void);

#endif