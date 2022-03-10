#include "response.h"
#include "../utils/utils.h"

http_res_t build_http_response(res_t *res) {
    //TODO add all response headers
    http_res_t http_res;
    if (!res->file_size || res->file_size == -1) res->file_size = strlen(res->body);

    set_response_content_length(res, res->file_size);

    http_res.content = malloc(sizeof (char) * (strlen(res->version) + 1 +
            strlen(res->status) + 1 +
            strlen(res->content_type) + 1 +
            strlen(res->body) + 1 +
            strlen(res->server) + 1 +
            strlen(res->content_length)));
    int header_size = sprintf(http_res.content, "%s %s\n%s\n%s\n%s\n\n", res->version, res->status, res->content_type, res->server, res->content_length);

    memcpy(
           http_res.content + header_size,
           res->body,
           res->file_size
    );

    http_res.length = header_size + res->file_size;
    return http_res;
}

res_t *build_response() {
    res_t *res = malloc(sizeof(res_t));

//    time_t timestamp = time(NULL);
//    struct tm * timeInfos = gmtime(&timestamp);

     //TODO format and add into header
//    printf( "Date and UTC time : %04d/%02d/%02d %02d:%02d:%02d\n",
//            timeInfos->tm_year+1900, timeInfos->tm_mon+1, timeInfos->tm_mday,
//            timeInfos->tm_hour, timeInfos->tm_min, timeInfos->tm_sec
//    );

    res->version = "HTTP/1.1";
    res->server = "Server: Cherokee/1.0";
    set_response_status(res, "200");
    res->body = NULL;
    set_response_content_type(res, "text/plain");
    res->content_length = NULL;
    res->file_size = -1;

    return res;
}

void set_response_body(res_t *res, char *body) {
    if (body != NULL) {
        res->body = body;
    }
}

void set_response_status(res_t *res, char *status) {
    if (status != NULL) {
        res->status = status;
    }
}

void set_response_content_type(res_t *res, char *content_type) {
    if (content_type != NULL) {
        char *key = "Content-Type: ";

        res->content_type = malloc(sizeof(char) * (strlen(key) + strlen(content_type)));
        sprintf(res->content_type, "%s%s", key, content_type);
    }
}

void set_response_content_length(res_t *res, int length) {
        char *key = "Content-Length: ";
        res->content_length = malloc(sizeof(char) * (strlen(key) + str_count(length) + 1));
        sprintf(res->content_length, "%s%d", key, length);
}

