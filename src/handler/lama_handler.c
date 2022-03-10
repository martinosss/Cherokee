#include "handler.h"

void *get_lama(req_t *req, res_t *res) {
    res->body = "<div style=\"color: blue\">"
                    "Lama"
                    "</br>"
                    "<img src=\"http://localhost:8080/lama\" />"
                "</div>";
    set_response_content_type(res, get_content_type_by_extension(req->config, "html"));
    res->status = "200";

    return NULL;
}

void *post_lama(req_t *req, res_t *res) {
    if (req->body == NULL || strlen(req->body) == 0) {
        res->status = "400";
        res->body = "Bad request";
        return NULL;
    }
    res->body = malloc(sizeof(char) * 10 + strlen(req->body) + 1);
    sprintf(res->body, "Post Body: %s", req->body);
    res->status = "200";

    return NULL;
}
