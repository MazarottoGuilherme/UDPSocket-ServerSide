#ifndef UDPSOCKET_GAME_H
#define UDPSOCKET_GAME_H

#include <sys/types.h>
#include <netinet/in.h>

#define TICK_RATE 60
#define TICK_DT   (1.0f / TICK_RATE)
#define SPEED     100.0f

void server_tick(float dt);
void handle_packet(char *data, ssize_t size, struct sockaddr_in *from);
void send_world_state();
#endif //UDPSOCKET_GAME_H