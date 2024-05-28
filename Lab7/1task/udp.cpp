#define _POSIX_C_SOURCE 201112L

#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>

#define BUFF_SIZE 1024

static void handle_error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <server port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // create socket
    int socket_fd = socket(AF_INET, SOCK_DGRAM, 0); // IPv4, UDP
    if (socket_fd == -1) {
        handle_error("socket");
    }

    int port = atoi(argv[1]);

    // create address for socket
    struct sockaddr_in serv_address;
    serv_address.sin_family = AF_INET;
    serv_address.sin_port = htons(port);
    serv_address.sin_addr.s_addr = htonl(INADDR_ANY);

    // bind socket to the new address
    if (bind(socket_fd, (struct sockaddr *) &serv_address, sizeof(serv_address)) == -1) {
        handle_error("bind");
    }

    // get info about the server
    char hostname[50];
    char service[10];
    getnameinfo((struct sockaddr *) &serv_address, sizeof(serv_address), hostname,
                sizeof(hostname), service, sizeof(service), 0);
    printf("Server %s %s\n", hostname, service);

    char buffer[BUFF_SIZE];

    while (1) {
        struct sockaddr_in client_address;
        socklen_t client_addr_len = sizeof(client_address);

        // receive messages from clients
        ssize_t recieved = recvfrom(socket_fd, buffer, BUFF_SIZE, 0, (struct sockaddr *) &client_address, &client_addr_len);
        if (recieved == -1) {
            handle_error("recvfrom");
        }

        // echo messages back to clients
        ssize_t sent = sendto(socket_fd, buffer, recieved, 0, (struct sockaddr *) &client_address, client_addr_len);
        if (sent == -1) {
            handle_error("sendto");
        }
    }
}