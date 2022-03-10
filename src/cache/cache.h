#ifndef CHEROKEE_CACHE_H
#define CHEROKEE_CACHE_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../response/response.h"
#include "../utils/utils.h"

typedef struct cache_s {
    char *path;
    http_res_t http_res;
    int expire_time;
    time_t cached_at;
    struct cache_s *next;
} cache_t;

void *cache_manager_thread(void *_cache_manager);
void set_cache_response(cache_t **cache, routes_config_t *routes_config, http_res_t http_res);
http_res_t get_response_from_cache(cache_t *cache, routes_config_t *routes_config);
void *remove_cache_by_path(cache_t **cache, char *path);

#endif //CHEROKEE_CACHE_H
