#include "game.h"

#include <stdio.h>
#include <arpa/inet.h>

#include "player.h"
#include "../protocol/protocol.h"
#include "../net/net.h"
#include "../db/login/loginUser.h"

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

// pacotes aqui
void handle_packet(char *data, ssize_t size, struct sockaddr_in *from) {
    PacketHeader *header = (PacketHeader *) data;

    switch (header->type) {
        case PKT_LOGIN: {
            if (size < sizeof(PacketLogin)) return;
            PacketLogin *pkt = (PacketLogin *) data;
            if (find_player(from) >= 0)
            return;
            Usuario user = buscar_usuario(pkt->email, pkt->password);

            PacketLoginACK ack;
            ack.type = PKT_LOGIN_ACK;

            if (user.id > 0) {
                int slot = add_player(from, user.id);
                if (slot < 0) return;

                players[slot].id = user.id;

                ack.id = user.id;
                strncpy(ack.username, user.username, sizeof(ack.username)-1);
                ack.username[sizeof(ack.username)-1] = '\0';
                printf("LOGIN OK: %s (%s:%d)\n",
                user.username,
                inet_ntoa(from->sin_addr),
                ntohs(from->sin_port));
            } else {
                ack.id = -1;
                ack.username[0] = '\0';
            }

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
