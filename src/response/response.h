#ifndef CHEROKEE_RESPONSE_H
#define CHEROKEE_RESPONSE_H

#include <string.h>
#include <stdio.h>

typedef struct res_s {
  char *version;
  char *status;
  char *body;
  char *server;
  char *content_type;
  char *content_length;
  char *date;
  int file_size;
} res_t;

typedef struct http_res_s {
   int length;
   char *content;
} http_res_t;

res_t *build_response();
void set_response_body(res_t *res, char *body);
void set_response_status(res_t *res, char *status);
void set_response_content_type(res_t *res, char *content_type);
void set_response_content_length(res_t *res, int length);
http_res_t build_http_response(res_t *res);

#endif //CHEROKEE_RESPONSE_H
