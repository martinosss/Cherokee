#include <string.h>
#include <stdlib.h>
#include "./parser_utils.h"

unsigned long countHttpColumn(char *httpRequest) {
    int httpColumnCount = 0;

    for (unsigned long i = 0; strlen(httpRequest) > i; i++) {
        if (httpRequest[i] == '\n') {
            httpColumnCount++;
        }
    }

    return httpColumnCount;
}

char **httpRequestToArray(char *httpRequest) {
    unsigned long httpColumnCount = countHttpColumn(httpRequest);
    char **httpRequestArray = malloc(sizeof(char *) * httpColumnCount);
    unsigned long lastIndex = 0;
    int numberOfElements = 0;

    for (unsigned long i = 0; i < strlen(httpRequest); i++) {
        if (httpRequest[i] == '\n') {
            httpRequestArray[numberOfElements] = malloc(sizeof(char) * (i - lastIndex) + 1);
            for (unsigned long y = 0; y < (i - lastIndex); y++) {
                httpRequestArray[numberOfElements][y] = httpRequest[lastIndex + y];
            }
            numberOfElements++;
            lastIndex = i + 1;
        }
    }

    return httpRequestArray;
}

void show_parsed_headers(req_t *req) {
    printf("== HEADERS ==\n");
    printf("METHOD : %s\n", req->method);
    printf("URI : %s\n", req->uri);
    printf("VERSION : %s\n", req->version);
    printf("HOST: %s\n", req->header->host);
    printf("REFERER: %s\n", req->header->referrer);
    printf("USERAGENT: %s\n", req->header->user_agent);
    printf("CONNECTION: %s\n", req->header->connection);
    printf("ACCEPT: \n");
    for (int i = 0; i < req->header->accept_arguments; ++i) {
        printf(">%s\n", req->header->accept[i]);
    }
    printf("ACCEPT CHARSET: \n");
    for (int i = 0; i < req->header->accept_charset_arguments; ++i) {
        printf(">%s\n", req->header->accept_charset[i]);
    }
    printf("ACCEPT LANGUAGE: \n");
    for (int i = 0; i < req->header->accept_language_arguments; ++i) {
        printf(">%s\n", req->header->accept_language[i]);
    }
    printf("TRANSFERT ENCODING: %s\n", req->header->transfer_encoding);
    printf("TRAILER: %s\n", req->header->trailer);
    printf("TE: \n");
    for (int i = 0; i < req->header->te_arguments; ++i) {
        printf(">%s\n", req->header->te[i]);
    }
    printf("BODY: %s\n", req->body);
    printf("======\n");
}

void generate_request_header(req_t *req) {
    req->header = malloc(sizeof(header_t));
    req->header->host = NULL;
    req->header->referrer = NULL;
    req->header->user_agent = NULL;
    req->header->connection = NULL;
    req->header->accept = NULL;
    req->header->accept_charset = NULL;
    req->header->accept_language = NULL;
    req->header->transfer_encoding = NULL;
    req->header->trailer = NULL;
    req->header->te = NULL;
    req->header->accept_arguments = -1;
    req->header->accept_language_arguments = -1;
    req->header->accept_charset_arguments = -1;
    req->header->te_arguments = -1;
}

char **word_parser(char *line) {
    int nb_word = 1;
    for(int i = 0; i != (int)strlen(line); i++) {
        if(line[i] == ' ')
            nb_word += 1;
    }
    char **parsed_line = malloc(sizeof(char *) * nb_word);
    char * token = strtok(line, " ");
    int j = 0;
    while(token != NULL) {
        parsed_line[j] = malloc (sizeof (char) * strlen(token));
        parsed_line[j] = token;
        token = strtok(NULL, " ");
        j+=1;
    }
    return parsed_line;
}

char *str_substring(char *str, int start, int end) {
    char *substring = malloc(sizeof(char) * (end - start) + 1);

    for (int i = 0; i < (end - start); ++i) {
        substring[i] = str[i + start];
    }

    substring[strlen(substring)] = '\0';

    return substring;
}

char *remove_spaces(char *str) {
    int copy_index = 0;
    char *new_str = malloc(sizeof (char) * strlen(str) + 1);

    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] != ' ' && str[i] != '\n') {
            new_str[copy_index] = str[i];
            copy_index++;
        }
    }
    new_str[strlen(str)] = '\0';

    return new_str;
}

char **parse_key_multiples_content(char *key_content) {
    key_content = parse_key_content(key_content);
    char **multiples_content = malloc(sizeof(char *) * count_multiples_arguments(key_content) + 1);
    char *parsed_content = strtok(key_content, ",");

    int i = 0;
    while (parsed_content != NULL)
    {
        multiples_content[i] = malloc(sizeof(char) * strlen(parsed_content) + 1);
        strcpy(multiples_content[i], parsed_content);
        parsed_content = strtok(NULL, ",");
        i++;
    }

    if (i == 0) {
        multiples_content[0] = malloc(sizeof(char) * strlen(key_content) + 1);
        multiples_content[0] = key_content;
    }

    return multiples_content;
}

int count_multiples_arguments(char *key_content) {
    int arguments = 0;
    for (int i = 0; i < (int)strlen(key_content); ++i) {
        if (key_content[i] == ',') {
            arguments++;
        }
    }

    return arguments + 1;
}

char *get_keyword(char *str, int end_keyword_index) {
    char *new_str = malloc(sizeof(char) * end_keyword_index);

    strncpy(new_str, str, end_keyword_index);

    return new_str;
}

char *parse_key_content(char *str) {
    int copy_index = 0;
    char *new_str = malloc(sizeof (char) * strlen(str) + 1);

    for (int i = 0; i < (int)strlen(str); ++i) {
        if (str[i] != ' ' && str[i] != ':') {
            new_str[copy_index] = str[i];
            copy_index++;
        }
    }

    new_str[strlen(str)] = '\0';

    return new_str;
}

int count_str_spaces(char *str) {
    int spaces = 0;

    for (int i = 0; i < (int)strlen(str); ++i) {
        if (str[i] == ' ') {
            spaces++;
        }
    }

    return spaces;
}