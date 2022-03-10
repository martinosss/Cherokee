#include "../response/response.h"
#include "handler.h"

void call_handler_by_name(routes_config_t *route, req_t *req, res_t *res) {
    custom_handlers_t *handlers = get_custom_handlers();

    for (int i = 0; handlers[i].name != NULL; i++) {
        if (strcmp(handlers[i].name, route->handler) == 0) {
            handlers[i].handler(req, res);
            break;
        }
    }
}

res_t *handle_query(req_t *req) {
    routes_config_t *routes = req->config->routes;
    res_t *res = build_response();
    file_t *file = NULL;

    set_response_status(res, "404");
    while (routes != NULL) {
        if (strcmp(req->method, routes->method) == 0 && strcmp(req->uri, routes->path) == 0) {
            if (routes->handler[0] == '@') {
                call_handler_by_name(routes, req, res);
            } else {
                file = processing_file(req->config, routes->handler);
            }
            break;
        }
        routes = routes->next;
    }

    if (file != NULL) {
        set_response_status(res, file->status);
        set_response_body(res, file->content);
        set_response_content_type(res, file->content_type);
        res->file_size = file->content_size;
    }

    if (strcmp(res->status, "404") == 0) {
        set_response_body(res, "Requested resource not found :(");
    }

    return res;
}

routes_config_t *get_route_config_by_path(routes_config_t *_routes_config, char *path) {
    routes_config_t *routes_config = _routes_config;
    routes_config_t *current_routes = NULL;

    while (routes_config != NULL) {
        if (strcmp(routes_config->path, path) == 0) {
            current_routes = routes_config;
            break;
        }
        routes_config = routes_config->next;
    }

    return current_routes;
}

