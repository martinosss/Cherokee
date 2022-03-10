#ifndef CHEROKEE_REQUEST_H
#define CHEROKEE_REQUEST_H

#include <stdio.h>
#include <regex.h>

typedef struct header_s {
    char *host;
    char *referrer;
    char *user_agent;
    char *connection;
    char **accept;
    int  accept_arguments;
    char **accept_charset;
    int  accept_charset_arguments;
    char **accept_language;
    int  accept_language_arguments;
    char *transfer_encoding;
    char *trailer;
    char **te;
    int  te_arguments;
} header_t;

#endif //CHEROKEE_REQUEST_H
