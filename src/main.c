#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>

#include "game/game.h"
#include "game/player.h"
#include "net/net.h"

int main(void) {

    net_init();

    memset(players, 0, sizeof(players));

    struct timespec last_time;
    clock_gettime(CLOCK_MONOTONIC, &last_time);

    double accumulator = 0.0;

    while (1) {

        net_poll();

        struct timespec now;
        clock_gettime(CLOCK_MONOTONIC, &now);

        double frame_time =
            (now.tv_sec - last_time.tv_sec) +
            (now.tv_nsec - last_time.tv_nsec) / 1e9;

        last_time = now;
        accumulator += frame_time;

        while (accumulator >= TICK_DT) {
            server_tick(TICK_DT);
            accumulator -= TICK_DT;
        }

        send_world_state();

        usleep(1000);
    }

    net_shutdown();
    return 0;
}
