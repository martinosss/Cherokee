#include "utils.h"

config_t *get_config() {
    config_t *config = malloc(sizeof(config_t));

    config->json = open_file("./../src/config.json").content;
    config->routes = get_routes_config(config);
    config->max_thread = get_config_max_thread(config);
    config->request_timeout = get_config_request_timeout(config);
    config->content_types = get_config_content_types(config);
    config->socket_thread_count = 0;
    config->log_file = get_config_file(config, "logFile");
    config->error_file = get_config_file(config, "errorFile");
    config->expose_port = get_port_expose(config);

    return config;
}

char *get_config_file(config_t *config, char *file_type) {
    cJSON *thread = cJSON_GetObjectItem(cJSON_Parse(config->json), file_type);

    return cJSON_GetObjectItem(thread, "path")->valuestring;;
}t

int get_port_expose(config_t *config) {
    return cJSON_GetObjectItem(cJSON_Parse(config->json), "port")->valueint;
}

int get_config_request_timeout(config_t *config) {
    cJSON *thread = cJSON_GetObjectItem(cJSON_Parse(config->json), "request");

    return cJSON_GetObjectItem(thread, "timeout")->valueint;
}

int get_config_max_thread(config_t *config) {
    cJSON *thread = cJSON_GetObjectItem(cJSON_Parse(config->json), "thread");

    return cJSON_GetObjectItem(thread, "max")->valueint;
}

content_types_config_t *get_config_content_types(config_t *config) {
    content_types_config_t *content_types_config = malloc(sizeof(content_types_config_t));
    content_types_config_t *current_content_types_config = content_types_config;

    int i;
    cJSON *root = cJSON_Parse(config->json);
    cJSON *element;
    cJSON *content_types = cJSON_GetObjectItem(root, "contentTypes");
    int n = cJSON_GetArraySize(content_types);

    for (i = 0; i < n; i++) {
        if (content_types_config == NULL) {
            content_types_config = malloc(sizeof (content_types_config_t));
        }
        element = cJSON_GetArrayItem(content_types, i);
        content_types_config->extension = cJSON_GetObjectItem(element, "extension")->valuestring;
        content_types_config->mime_type = cJSON_GetObjectItem(element, "mimeType")->valuestring;

        if (cJSON_GetArrayItem(content_types, i + 1) != NULL) {
            content_types_config->next = malloc(sizeof(content_types_config_t));
        } else {
            content_types_config->next = NULL;
        }
        content_types_config = content_types_config->next;
    }

    return current_content_types_config;
}

routes_config_t *get_routes_config(config_t *config) {
    routes_config_t *routes_config = malloc(sizeof(routes_config_t));
    routes_config_t *current_routes_config = routes_config;
    cJSON *root = cJSON_Parse(config->json);

    int i;
    cJSON *element;
    cJSON *routes = cJSON_GetObjectItem(root, "routes");
    int n = cJSON_GetArraySize(routes);
    for (i = 0; i < n; i++) {
        if (routes_config == NULL) {
            routes_config = malloc(sizeof (routes_config_t));
        }
        routes_config->cache_time = -1;
        element = cJSON_GetArrayItem(routes, i);
        routes_config->handler = cJSON_GetObjectItem(element, "handler")->valuestring;
        routes_config->path = cJSON_GetObjectItem(element, "path")->valuestring;
        routes_config->method = cJSON_GetObjectItem(element, "method")->valuestring;

        cJSON *cache_time = cJSON_GetObjectItem(element, "cache");

        if (cache_time != NULL) {
            routes_config->cache_time = cache_time->valueint;
        }

        if (cJSON_GetArrayItem(routes, i + 1) != NULL) {
            routes_config->next = malloc(sizeof(routes_config_t));
        } else {
            routes_config->next = NULL;
        }
        routes_config = routes_config->next;
    }

    return current_routes_config;
}

