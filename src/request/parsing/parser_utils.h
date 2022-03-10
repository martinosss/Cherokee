#ifndef TEST_REQUEST_H
#define TEST_REQUEST_H

#include <stdio.h>
#include <regex.h>
#include "../request.h"
#include "../../network/network.h"

void method_uri_version_filler(char*, req_t *req);
void body_filler(char*, req_t *req);
void incoming_request_general_info_filler(char**, char*, req_t *req);
char **word_parser(char*);
unsigned long countHttpColumn(char *httpRequest);
char **httpRequestToArray(char *httpRequest);
void show_parsed_headers(req_t *req);
void generate_request_header(req_t *req);
char *str_substring(char *str, int start, int end);
char *remove_spaces(char *str);
char **parse_key_multiples_content(char *key_content);
int count_multiples_arguments(char *key_content);
char *get_keyword(char *str, int end_keyword_index);
char *parse_key_content(char *str);
int count_str_spaces(char *str);

#endif //TEST_REQUEST_H
