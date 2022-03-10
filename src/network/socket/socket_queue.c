#include "../network.h"

void *queue_manager_thread(void *_queue_manager) {
    printf("Queue thread started\n");
    socket_queue_manager_t *queue_manager = (socket_queue_manager_t *)_queue_manager;

    while (queue_manager->cache_manager->server.is_started) {
        while (queue_manager->config->socket_thread_count < queue_manager->config->max_thread
        && queue_manager->queue != NULL) {
            printf("Process socket from queue...\n");
            int socket_queue_size = get_socket_queue_size(queue_manager->queue);
            int socket = socket_dequeue(queue_manager->queue);

            if (socket_queue_size == 1 && get_socket_queue_size(queue_manager->queue) == 1) {
                queue_manager->queue = NULL;
            }

            server_init_socket_handler(queue_manager->config, socket, queue_manager);
        }
        sleep(1);
    }

    return NULL;
}

void socket_enqueue(socket_queue_t **queue, int socket) {
    socket_queue_t *new_queue;
    new_queue = (socket_queue_t *) malloc(sizeof(socket_queue_t));

    new_queue->socket = socket;
    new_queue->next = *queue;
    *queue = new_queue;
}

int socket_dequeue(socket_queue_t *queue) {
    int socket = 0;
    if (queue->next == NULL) {
        socket = queue->socket;

        free(queue);
        return socket;
    }

    socket_queue_t *current_queue = queue;
    while (current_queue->next->next != NULL) {
        current_queue = current_queue->next;
    }
    socket = current_queue->next->socket;
    free(current_queue->next);
    current_queue->next = NULL;

    return socket;
}

int get_socket_queue_size(socket_queue_t *queue) {
    socket_queue_t *head_queue = queue;
    int i = 0;

    while (head_queue != NULL) {
        i++;
        head_queue = head_queue->next;
    }

    return i;
}
