#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/epoll.h>

#include "thread_pool.h"

#define PORT 7000
#define MAX_EVENTS 64


typedef struct server_s {
    int listen_fd;
    int epoll_fd;
    struct epoll_event events[MAX_EVENTS];

} server_t;


void set_nonblock(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        perror("Failed to get file descriptor flags");
        exit(EXIT_FAILURE);
    }

    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
        perror("Failed to set file descriptor flags");
        exit(EXIT_FAILURE);
    }
}


void create_server_socket(server_t *server) {
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_fd == -1) {
        perror("Failed to create socket");
    }

    int option = 1;
    setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
    set_nonblock(socket_fd);

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(PORT),
        .sin_addr.s_addr = INADDR_ANY
    };

    if (bind(socket_fd, (struct sockaddr*) &addr, sizeof(addr)) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }
    printf("Binding successful\n");

    if (listen(socket_fd, SOMAXCONN) == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    server->listen_fd = socket_fd;
}


void accept_connection(server_t *server) {
    struct sockaddr_in client;
    socklen_t len = sizeof(client);
    int client_fd = accept(server->listen_fd, (struct sockaddr*) &client, &len);

    if (client_fd == -1) {
        perror("accept");
        return;
    }
    set_nonblock(client_fd);

    printf("Client connected\n");

    struct epoll_event event = {
        .events = EPOLLIN,
        .data.fd = client_fd,
    };
    epoll_ctl(server->epoll_fd, EPOLL_CTL_ADD, client_fd, &event);
}


void read_client_data(server_t *server, int client_fd) {
    const int max_bytes = 8;
    char buffer[max_bytes];
    int bytes = read(client_fd, buffer, max_bytes);

    if (bytes <= 0) {
        close(client_fd);
    } else {
        printf("Received bytes: %d\n", bytes);
        printf("Received data: %.*s\n", bytes, buffer);
    }
}


void handle_events(server_t *server, int num_events) {
    for (int i = 0; i < num_events; i++) {
        int event_fd = server->events[i].data.fd;

        if (event_fd == server->listen_fd) {
            accept_connection(server);
        } else {
            read_client_data(server, event_fd);
        }
    }
}


void wait_for_events(server_t *server) {
    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }
    server->epoll_fd = epoll_fd;

    struct epoll_event event = {
        .events = EPOLLIN,
        .data.fd = server->listen_fd
    };

    int ret = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server->listen_fd, &event);
    if (ret == -1) {
        perror("Failed to add server socket listener file descriptor to epoll");
    }

    while (1) {
        printf("Waiting for events\n");
        int num_events = epoll_wait(epoll_fd, server->events, MAX_EVENTS, -1);

        if (num_events == -1) {
            if (errno == EINTR) continue;
            perror("epoll_wait");
            exit(EXIT_FAILURE);
        }

        handle_events(server, num_events);
    }
}


int main(void) {
    // thread_pool_t thread_pool = thread_pool_create(5);
    // server_t server;
    // create_server_socket(&server);
    // wait_for_events(&server);
    // close(server.listen_fd);
    // struct sockaddr_in client;
    // int x;
    // scanf("%d", &x);
    // unsigned long abc[x];
    printf("Size: %lu, %lu", sizeof(struct sockaddr_in), sizeof(struct sockaddr));

    return 0;
}
