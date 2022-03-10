#ifndef CHEROKEE_HANDLER_H
#define CHEROKEE_HANDLER_H

#include "../response/response.h"
#include "../network/network.h"

typedef struct custom_handlers_s {
    char *name;
    void *(*handler)(req_t *req, res_t *res);
} custom_handlers_t;

res_t *handle_query(req_t *req);
custom_handlers_t *get_custom_handlers();
routes_config_t *get_route_config_by_path(routes_config_t *_routes_config, char *path);

void *get_lama(req_t *req, res_t *res);
void *post_lama(req_t *req, res_t *res);

#endif //CHEROKEE_HANDLER_H
