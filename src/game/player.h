#ifndef UDPSOCKET_PLAYER_H
#define UDPSOCKET_PLAYER_H

#include <time.h>
#include <netinet/in.h>

#define MAX_PLAYERS 1024

typedef struct {
    int used;
    struct sockaddr_in addr;
    char username[16];

    int move_x;
    int move_y;

    float x;
    float y;

    time_t last_seen;
} Player;

extern Player players[MAX_PLAYERS];

int add_player(const struct sockaddr_in *addr, const char *username);
int find_player(const struct sockaddr_in *addr);

#endif
