#include "../request.h"
#include <string.h>
#include "./parser_utils.h"
#include <stdio.h>

void update_header_content(char* key_content, req_t *req, char *key) {
    if (strcmp(key, "Accept") == 0) {
        req->header->accept = parse_key_multiples_content(key_content);
        req->header->accept_arguments = count_multiples_arguments(key_content);
    } else if (strcmp(key, "Accept-Charset") == 0) {
        req->header->accept_charset = parse_key_multiples_content(key_content);
        req->header->accept_charset_arguments = count_multiples_arguments(key_content);
    } else if (strcmp(key, "Accept-Language") == 0) {
        req->header->accept_language = parse_key_multiples_content(key_content);
        req->header->accept_language_arguments = count_multiples_arguments(key_content);
    } else if (strcmp(key, "Referrer") == 0) {
        req->header->referrer = parse_key_content(key_content);
    } else if (strcmp(key, "TE") == 0) {
        req->header->te = parse_key_multiples_content(key_content);
        req->header->te_arguments = count_multiples_arguments(key_content);
    } else if (strcmp(key, "User-Agent") == 0) {
        req->header->user_agent = parse_key_content(key_content);
    } else if (strcmp(key, "Connection") == 0) {
        req->header->connection = parse_key_content(key_content);
    } else if (strcmp(key, "Transfer-Encoding") == 0) {
        req->header->transfer_encoding = parse_key_content(key_content);
    } else if (strcmp(key, "Trailer") == 0) {
        req->header->trailer = parse_key_content(key_content);
    } else if (strcmp(key, "Host") == 0) {
        req->header->host = remove_spaces(key_content);
    }
}

void header_filter_content(char** request_array, req_t * req, char *key, unsigned long httpColumnCount) {
    int is_body = 0;
    for (int i = 0; i < (int)httpColumnCount; ++i) {
        if (is_body) {
            break;
        }
        for (int j = 0; j < (int)strlen(request_array[i]); ++j) {
            if (request_array[i][j] == '\n' && request_array[i][j - 1] == 13 && request_array[i][j - 2] == '\n') {
                is_body = 1;
            }

            if (request_array[i][j] == ':') {
                char *key_word = get_keyword(request_array[i], j);

                if (strcmp(key_word, key) == 0) {
                    char *key_content = str_substring(request_array[i], j + 1, (int) strlen(request_array[i]));
                    update_header_content(key_content, req, key_word);
                }
                break;
            }
        }
    }
}

void method_uri_version_filler(char* request_line, req_t * req) {
    char **parsed_line = malloc(sizeof(char *) * count_str_spaces(request_line) + 1);
    int pushed_arguments = 0;
    int started_arguments_index = 0;
    int i;

    for (i = 0; i < (int)strlen(request_line); ++i) {
        if (request_line[i] == ' ') {
            parsed_line[pushed_arguments] = str_substring(request_line, started_arguments_index, i);
            started_arguments_index = i + 1;
            pushed_arguments++;
        }
    }
    parsed_line[pushed_arguments] = str_substring(request_line, started_arguments_index, i - 1);
    
    req->method = parsed_line[0];
    req->uri = parsed_line[1];
    req->version = parsed_line[2];
}

void header_filler(char** request_array, req_t *req, unsigned long httpColumnCount) {
    char headers_options[10][18] =
            { "Accept",
              "Accept-Charset",
              "Accept-Language",
              "Host",
              "Referrer",
              "TE",
              "User-Agent",
              "Connection",
              "Transfer-Encoding",
              "Trailer"
            };
    for (int i = 0; i < 10; i++) {
        header_filter_content(request_array, req, headers_options[i], httpColumnCount);
    }
}

void body_filler(char* request_line, req_t *req) {
    char *body_line = NULL;

    for (int i = 0; i < (int)strlen(request_line); ++i) {
        if (request_line[i] == '\n' && request_line[i - 1] == 13 && request_line[i - 2] == '\n') {
            body_line = str_substring(request_line, i + 1, (int)strlen(request_line));
        }
    }
    req->body = body_line;
}

void incoming_request_general_info_filler(char** request, char* request_str, req_t *req) {
    unsigned long httpColumnCount = countHttpColumn(request_str);

    generate_request_header(req);

    method_uri_version_filler(request[0], req);
    header_filler(request, req, httpColumnCount);
    body_filler(request_str, req);
}
