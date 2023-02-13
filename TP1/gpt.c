#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define SUM 0
#define MULTIPLY 1
#define OUT_INT 0
#define OUT_STR 1

int create_and_send_message(const struct sockaddr *dest_addr, socklen_t addrlen, int *ints, size_t no_int, char operation, char output_format, int *result) {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket");
        exit(1);
    }

    if (connect(sock, dest_addr, addrlen) < 0) {
        perror("connect");
        exit(1);
    }

    unsigned char header = 0;
    header = header | operation;
    header = header | (output_format << 3);

    unsigned char *message = malloc(4 * no_int + 1);
    message[0] = header;
    for (size_t i = 0; i < no_int; i++) {
        *(int *)(message + (i + 1) * 4) = htonl(ints[i]);
    }

    ssize_t sent = send(sock, message, 4 * no_int + 1, 0);
    if (sent < 0) {
        perror("send");
        exit(1);
    }
    free(message);

    unsigned char response[260];
    ssize_t received = recv(sock, response, 260, 0);
    if (received < 0) {
        perror("recv");
        exit(1);
    }

    if (response[0] == 0) {
        // error, return -1
        return -1;
    }

    if (output_format == OUT_INT) {
        *result = ntohl(*(int *)(response + 1));
    } else {
        // output format is string, cannot assign result as int
        return -1;
    }

    close(sock);
    return 0;
}
