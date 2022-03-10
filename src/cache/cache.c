
#include "cache.h"
#include "../network/network.h"

void clear_expired_cache(cache_manager_t *cache_manager) {
    cache_t *cache = cache_manager->cache;

    while (cache != NULL) {
        if (difftime(time(NULL), cache->cached_at) > cache->expire_time) {
            printf("Remove query %s from cache\n", cache->path);
            remove_cache_by_path(&cache_manager->cache, cache->path);
        }
        cache = cache->next;
    }
}

void *cache_manager_thread(void *_cache_manager) {
    printf("Cache thread started\n");
    cache_manager_t *cache_manager = (cache_manager_t *)_cache_manager;

    while (cache_manager->server.is_started) {
        clear_expired_cache(cache_manager);
        sleep(1);
    }

    return NULL;
}

void set_cache_response(cache_t **cache, routes_config_t *routes_config, http_res_t http_res) {
    if (routes_config != NULL && routes_config->cache_time > 0) {
        cache_t *new_cache;
        new_cache = (cache_t *) malloc(sizeof(cache_t));

        new_cache->expire_time = routes_config->cache_time;
        new_cache->http_res = http_res;
        new_cache->path = routes_config->path;
        new_cache->cached_at = time(NULL);
        new_cache->next = *cache;
        *cache = new_cache;

        printf("Query %s added to cache\n", routes_config->path);
    }
}

http_res_t get_response_from_cache(cache_t *_cache, routes_config_t *routes_config) {
    cache_t *cache = _cache;
    http_res_t http_res;
    http_res.content = NULL;

    if (routes_config == NULL || routes_config->cache_time < 0) return http_res;

    while (cache != NULL) {
        if (strcmp(routes_config->path, cache->path) == 0) {
            http_res = cache->http_res;
            break;
        }
        cache = cache->next;
    }

    if (http_res.content != NULL) {
        printf("Get query %s from cache\n", routes_config->path);
    }

    return http_res;
}


void *remove_cache_by_path(cache_t **cache, char *path) {
    cache_t *current_cache = *cache;
    cache_t *temp_node = NULL;

    if (strcmp(current_cache->path, path) == 0) {
        temp_node = (*cache)->next;
        free(*cache);
        *cache = temp_node;

        return NULL;
    }

    while (current_cache != NULL) {
        if (current_cache->next != NULL) {
            if (strcmp(current_cache->next->path, path) == 0) {
                temp_node = current_cache->next;
            }
        }
        current_cache = current_cache->next;
    }

    current_cache->next = temp_node->next;
    free(temp_node);

    return NULL;
}

