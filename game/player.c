#include "player.h"
#include <string.h>
#include <time.h>

Player players[MAX_PLAYERS];

int add_player(const struct sockaddr_in *addr, const char *username) {
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (!players[i].used) {
            players[i].used = 1;
            players[i].addr = *addr;

            strncpy(players[i].username, username, sizeof(players[i].username) - 1);
            players[i].username[sizeof(players[i].username) - 1] = '\0';

            players[i].last_seen = time(NULL);
            players[i].move_x = 0;
            players[i].move_y = 0;
            players[i].x = 0.0f;
            players[i].y = 0.0f;

            return i;
        }
    }
    return -1;
}

int find_player(const struct sockaddr_in *addr) {
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (players[i].used &&
            players[i].addr.sin_addr.s_addr == addr->sin_addr.s_addr &&
            players[i].addr.sin_port == addr->sin_port) {
            return i;
            }
    }
    return -1;
}