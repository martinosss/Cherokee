#ifndef CHEROKEE_NETWORK_H
#define CHEROKEE_NETWORK_H

#include <pthread.h>
#include <stdio.h>
#include <netinet/in.h>
#include "../utils/utils.h"
#include <unistd.h>
#include <string.h>
#include "../request/request.h"
#include "../logger/logger.h"
#include "../cache/cache.h"

typedef struct server_s {
    int socket_desc;
    int client_socket;
    int socket_length;
    int is_started;
    struct  sockaddr_in server;
} server_t;

typedef struct socket_queue_s {
    int socket;
    struct socket_queue_s *next;
} socket_queue_t;

typedef struct cache_manager_s {
    server_t server;
    cache_t *cache;
} cache_manager_t;

typedef struct socket_queue_manager_s {
    config_t *config;
    cache_manager_t *cache_manager;
    socket_queue_t *queue;
} socket_queue_manager_t;

typedef struct req_s {
    int socket;
    config_t *config;
    char *method;
    char *uri;
    char *version;
    char *host;
    header_t *header;
    char *body;
    cache_manager_t *cache_manager;
} req_t;

server_t server_initialization(config_t *config);
int server_handle_socket(server_t server, config_t *config);
void *socket_handler(void *sockets_void);
void socket_enqueue(socket_queue_t **queue, int socket);
int socket_dequeue(socket_queue_t *queue);
int server_init_socket_handler(config_t *config, int socket, socket_queue_manager_t *socket_queue_manager);
void *queue_manager_thread(void *_queue_manager);
int get_socket_queue_size(socket_queue_t *queue);

#endif //CHEROKEE_NETWORK_H
