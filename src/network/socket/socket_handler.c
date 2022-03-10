#include "../network.h"
#include "../../request/parsing/parser_utils.h"
#include "../../handler/handler.h"

void socket_send_response(int socket, http_res_t http_res) {
    send(socket, http_res.content, http_res.length, 0);
}

void *socket_query_fill_req(req_t *req, char *query) {
    char **array = httpRequestToArray(query);

    incoming_request_general_info_filler(array, query, req);
    return NULL;
}

void *socket_handler_end(req_t *req) {
    printf("disconnect client\n");
    req->config->socket_thread_count -= 1;
    close(req->socket);

    return NULL;
}

void *socket_handler(void *_req) {
    req_t *req = (req_t *)_req;
    char query[1024];
    res_t *res = NULL;
    http_res_t http_res;

    printf("connected client, waiting for query...\n");

    req->config->socket_thread_count += 1;

    memset(query, 0, 1024);

    recv(req->socket, query, 1024, 0);

    if (strlen(query) == 0) return socket_handler_end(req);

    printf("received query...\n");

    socket_query_fill_req(req, query);

    http_res = get_response_from_cache(req->cache_manager->cache, get_route_config_by_path(req->config->routes, req->uri));

    if (http_res.content != NULL) {
        socket_send_response(req->socket, http_res);

        return socket_handler_end(req);
    }

    res = handle_query(req);

    http_res = build_http_response(res);

    set_cache_response(&req->cache_manager->cache, get_route_config_by_path(req->config->routes, req->uri), http_res);

    socket_send_response(req->socket, http_res);

    printf("response send\n");

    return socket_handler_end(req);
} 
