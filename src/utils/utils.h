#ifndef CHEROKEE_UTILS_H
#define CHEROKEE_UTILS_H

#include "stdlib.h"
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include "lib/json/cJSON.h"

typedef struct file_s {
    const char *path;
    char *content_type;
    char *content;
    char *status;
    int content_size;
} file_t;

typedef struct open_file_s {
    char *content;
    int size;
} open_file_t;

typedef struct routes_config_s {
    char *path;
    char *handler;
    char *method;
    int cache_time;
    struct routes_config_s *next;
} routes_config_t;

typedef struct content_types_config_s {
    char *extension;
    char *mime_type;
    struct content_types_config_s *next;
} content_types_config_t;

typedef struct config_s {
    char *json;
    routes_config_t *routes;
    content_types_config_t *content_types;
    int max_thread;
    int request_timeout;
    int socket_thread_count;
    char *log_file;
    char *error_file;
    int expose_port;
} config_t;

file_t *processing_file(config_t *config, const char* path);
char *get_extension(const char *filename);

open_file_t open_file(const char *path);
config_t *get_config();
routes_config_t *get_routes_config(config_t *config);
int get_config_max_thread(config_t *config);
int str_count(int n);
content_types_config_t *get_config_content_types(config_t *config);
int get_config_request_timeout(config_t *config);
char *get_content_type_by_extension(config_t *config, char *extension);
char *get_config_file(config_t *config, char *file_type);
int get_port_expose(config_t *config);

#endif //CHEROKEE_UTILS_H
