#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <errno.h>
#include <string.h> // za strerror

#define DEVICE_COUNT 6

int main() {
    struct pollfd fds[DEVICE_COUNT];
    char device_name[20];
    char buffer[1];
    int opened_devices = 0;

    // Open all devices for reading
    for (int i = 0; i < DEVICE_COUNT; i++) {
        snprintf(device_name, sizeof(device_name), "/dev/shofer%d", i);
        fds[i].fd = open(device_name, O_RDONLY);
        if (fds[i].fd < 0) {
            printf("Failed to open %s: %s\n", device_name, strerror(errno));
            continue;
        }
        fds[i].events = POLLIN;
        opened_devices++;
        printf("Successfully opened %s\n", device_name);
    }

    if (opened_devices == 0) {
        printf("No devices were opened successfully. Exiting.\n");
        return 1;
    }

    printf("Waiting for input on %d devices...\n", opened_devices);

    while (1) {
        int ret = poll(fds, DEVICE_COUNT, -1);
        if (ret < 0) {
            perror("poll");
            break;
        }
        
        for (int i = 0; i < DEVICE_COUNT; i++) {
            if (fds[i].fd < 0) continue; // Skip unopened devices
            
            if (fds[i].revents & POLLIN) {
                ssize_t n = read(fds[i].fd, buffer, 1);
                if (n > 0) {
                    printf("Read from device %d: %c\n", i, buffer[0]);
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
