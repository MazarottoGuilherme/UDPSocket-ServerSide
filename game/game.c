#include "game.h"

#include <stdio.h>
#include <arpa/inet.h>

#include "player.h"
#include "../protocol/protocol.h"
#include "../net/net.h"

void server_tick(float dt) {
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (!players[i].used)
            continue;

        players[i].x += players[i].move_x * SPEED * dt;
        players[i].y += players[i].move_y * SPEED * dt;

        players[i].move_x = 0;
        players[i].move_y = 0;
    }
}

void handle_packet(char *data, ssize_t size, struct sockaddr_in *from) {
    PacketHeader *header = (PacketHeader *) data;

    switch (header->type) {
        case PKT_LOGIN: {
            if (size < sizeof(PacketLogin)) return;

            PacketLogin *pkt = (PacketLogin *) data;

            int id = find_player(from);
            if (id >= 0) return;

            int new_id = add_player(from, pkt->username);
            if (new_id < 0) {
                return;
            }

            printf("LOGIN: %s (%s:%d)\n",
                   pkt->username,
                   inet_ntoa(from->sin_addr),
                   ntohs(from->sin_port));

            PacketLoginACK ack;
            ack.type = PKT_LOGIN_ACK;
            ack.id = new_id;

            net_send_to(&ack, sizeof(ack), from);

            break;
        }

        case PKT_INPUT: {
            if (size < sizeof(PacketInput)) return;

            int id = find_player(from);
            if (id < 0) return;

            PacketInput *pkt = (PacketInput *) data;

            players[id].move_x = pkt->dx;
            players[id].move_y = pkt->dy;
            players[id].last_seen = time(NULL);
            break;
        }

        case PKT_PING: {
            int id = find_player(from);
            if (id >= 0) {
                players[id].last_seen = time(NULL);
            }
            break;
        }
        case PKT_LOGOUT: {
            PacketLogout *pkt = (PacketLogout *) data;
            players[pkt->id].used = 0;

            for (int i = 0; i < MAX_PLAYERS; i++) {
                if (!players[i].used) continue;
                net_send_to(pkt, sizeof(*pkt), &players[i].addr);
            }

            break;
        }


        default:
            break;
    }
}

void send_world_state() {
    PacketState pkt;

    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (!players[i].used)
            continue;

        pkt.type = PKT_STATE;
        pkt.player_id = i;
        pkt.x = players[i].x;
        pkt.y = players[i].y;

        for (int j = 0; j < MAX_PLAYERS; j++) {
            if (!players[j].used)
                continue;

            net_send_to(&pkt, sizeof(pkt), &players[j].addr);
        }
    }
}
