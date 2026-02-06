#include "net.h"
#include "../protocol/protocol.h"
#include "../game/game.h"

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdio.h>

static int socket_desc = -1;

static struct sockaddr_in server_addr;
static struct sockaddr_in client_addr;
static socklen_t client_addr_len = sizeof(client_addr);

static char client_message[2000];

void net_init(void) {
    socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (socket_desc < 0) {
        perror("socket");
        exit(1);
    }

    if (fcntl(socket_desc, F_SETFL, O_NONBLOCK) < 0) {
        perror("fcntl");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2000);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        exit(1);
    }
}

void net_poll(void) {
    while (1) {
        ssize_t received = recvfrom(
            socket_desc,
            client_message,
            sizeof(client_message),
            0,
            (struct sockaddr*)&client_addr,
            &client_addr_len
        );

        if (received < 0) {
            if (errno == EWOULDBLOCK || errno == EAGAIN)
                break;
            perror("recvfrom");
            break;
        }

        if ((size_t)received < sizeof(PacketHeader))
            continue;

        handle_packet(client_message, received, &client_addr);
    }
}

void net_send_to(const void* data, size_t size, const struct sockaddr_in* addr) {
    sendto(
        socket_desc,
        data,
        size,
        0,
        (const struct sockaddr*)addr,
        sizeof(*addr)
    );
}

void net_shutdown(void) {
    if (socket_desc >= 0) {
        close(socket_desc);
        socket_desc = -1;
    }
}
#include "net.h"
#include "../protocol/protocol.h"
#include "../game/game.h"

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdio.h>

static int socket_desc = -1;

static struct sockaddr_in server_addr;
static struct sockaddr_in client_addr;
static socklen_t client_addr_len = sizeof(client_addr);

static char client_message[2000];

void net_init(void) {
    socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (socket_desc < 0) {
        perror("socket");
        exit(1);
    }

    if (fcntl(socket_desc, F_SETFL, O_NONBLOCK) < 0) {
        perror("fcntl");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2000);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        exit(1);
    }
}

void net_poll(void) {
    while (1) {
        ssize_t received = recvfrom(
            socket_desc,
            client_message,
            sizeof(client_message),
            0,
            (struct sockaddr*)&client_addr,
            &client_addr_len
        );

        if (received < 0) {
            if (errno == EWOULDBLOCK || errno == EAGAIN)
                break;
            perror("recvfrom");
            break;
        }

        if ((size_t)received < sizeof(PacketHeader))
            continue;

        handle_packet(client_message, received, &client_addr);
    }
}

void net_send_to(const void* data, size_t size, const struct sockaddr_in* addr) {
    sendto(
        socket_desc,
        data,
        size,
        0,
        (const struct sockaddr*)addr,
        sizeof(*addr)
    );
}

void net_shutdown(void) {
    if (socket_desc >= 0) {
        close(socket_desc);
        socket_desc = -1;
    }
}