#include "../network.h"
#include "../../logger/logger.h"


server_t server_initialization(config_t *config) {
    server_t server;
    int options = 1;
    server.socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(server.socket_desc, SOL_SOCKET, SO_REUSEADDR, &options, sizeof(options));

    if (server.socket_desc == -1) {
        close(server.socket_desc);
        printf("Socket start error\n");
        write_log(config->error_file, "Socket start error\n");
        server.is_started = 0;
        return server;
    }
    server.server.sin_family = AF_INET;
    server.server.sin_addr.s_addr = INADDR_ANY;
    server.server.sin_port = htons(config->expose_port);
    if (bind(server.socket_desc, (struct sockaddr *) &server.server, sizeof(server.server)) < 0) {
        printf("Server start error\n");
        write_log(config->error_file, "Server start error\n");
        close(server.socket_desc);
        server.is_started = 0;
        printf("Retry...\n");
        sleep(1);
        return server_initialization(config);
    }

    server.socket_length = sizeof(struct sockaddr_in);

    listen(server.socket_desc, 3);

    server.is_started = 1;

    return server;
}
