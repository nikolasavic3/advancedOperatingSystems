#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <time.h>
#include <errno.h>
#include <string.h>

#define DEVICE_COUNT 6

int main() {
    struct pollfd fds[DEVICE_COUNT];
    char device_name[20];
    char data;
    int opened_devices = 0;
    
    srand(time(NULL));

    // Open all devices for writing
    for (int i = 0; i < DEVICE_COUNT; i++) {
        snprintf(device_name, sizeof(device_name), "/dev/shofer%d", i);
        fds[i].fd = open(device_name, O_WRONLY);
        if (fds[i].fd < 0) {
            printf("Failed to open %s: %s\n", device_name, strerror(errno));
            continue;
        }
        fds[i].events = POLLOUT;
        opened_devices++;
        printf("Successfully opened %s\n", device_name);
    }

    if (opened_devices == 0) {
        printf("No devices were opened successfully. Exiting.\n");
        return 1;
    }

    printf("Ready to write to %d devices...\n", opened_devices);

    while (1) {
        sleep(5); // Wait 5 seconds
        int ret = poll(fds, DEVICE_COUNT, 0);
        if (ret < 0) {
            perror("poll");
            break;
        }

        // Brojač koliko naprava je spremno za pisanje
        int ready_devices = 0;
        for (int i = 0; i < DEVICE_COUNT; i++) {
            if (fds[i].fd < 0) continue; // Preskoči zatvorene naprave
            
            if (fds[i].revents & POLLOUT) {
                ready_devices++;
            }
        }

        if (ready_devices > 0) {
            // Nasumično odaberi jednu od spremnih naprava
            int selected = rand() % ready_devices;
            int current = 0;
            
            for (int i = 0; i < DEVICE_COUNT; i++) {
                if (fds[i].fd < 0) continue;
                
                if (fds[i].revents & POLLOUT) {
                    if (current == selected) {
                        data = 'A' + (rand() % 26); // Random letter
                        if (write(fds[i].fd, &data, 1) > 0) {
                            printf("Wrote '%c' to device %d\n", data, i);
                        }
                        break;
                    }
                    current++;
                }
            }
        }
    }

    // Close all successfully opened devices
    for (int i = 0; i < DEVICE_COUNT; i++) {
        if (fds[i].fd >= 0) {
            close(fds[i].fd);
        }
    }

    return 0;
}
