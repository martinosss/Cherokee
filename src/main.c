#include "network/network.h"
#include "utils/utils.h"

int main() {
    setvbuf(stdout, NULL, _IONBF, 0);
    config_t *config = get_config();
    server_t server = server_initialization(config);

    if (!server.is_started) {
        return 1;
    }

    printf("Server started on port %d, waiting for connections... \n", config->expose_port);

    server_handle_socket(server, config);

    return 0;
}
