#include "../network.h"

req_t *setup_socket_req(config_t *config, int socket, cache_manager_t *cache_manager) {
    struct timeval tv;
    tv.tv_sec = config->request_timeout;
    tv.tv_usec = 0;
    setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
    req_t *req = malloc(sizeof(req_t));
    req->config = config;
    req->socket = socket;
    req->cache_manager = cache_manager;

    return req;
}

int server_init_socket_handler(config_t *config, int socket, socket_queue_manager_t *socket_queue_manager) {
    int thread_creator;
    pthread_t thread_id;

    if (config->socket_thread_count >= config->max_thread) {
        printf("Max thread achieved, add socket to queue\n");
        socket_enqueue(&socket_queue_manager->queue, socket);
        return 0;
    }

    thread_creator = pthread_create(&thread_id, NULL, socket_handler, setup_socket_req(config, socket, socket_queue_manager->cache_manager));
    if (thread_creator < 0) {
        return 1;
    }
    return 0;
}

int server_handle_socket(server_t server, config_t *config) {
    pthread_t query_thread_id;
    pthread_t cache_thread_id;
    socket_queue_manager_t *socket_queue_manager = malloc(sizeof (socket_queue_manager_t));
    socket_queue_manager->queue = NULL;
    socket_queue_manager->config = config;
    socket_queue_manager->cache_manager = malloc(sizeof (cache_manager_t));
    socket_queue_manager->cache_manager->cache = NULL;
    socket_queue_manager->cache_manager->server = server;
    pthread_create(&query_thread_id, NULL, queue_manager_thread, socket_queue_manager);
    pthread_create(&cache_thread_id, NULL, cache_manager_thread, socket_queue_manager->cache_manager);
    write_log(config->log_file, "Server started");

    while (1) {
        while ((server.client_socket = accept(server.socket_desc,
                                                        (struct sockaddr *) &server.client_socket,
                                                        (socklen_t *) &server.socket_length))) {
            write_log(config->log_file, "Receive new connexion\n");
            int thread = server_init_socket_handler(config, server.client_socket, socket_queue_manager);
            if (thread < 0) {
                write_log(config->error_file, "Error while create thread\n");
                return 1;
            }
        }
    }
}