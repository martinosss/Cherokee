#include <limits.h>
#include "utils.h"

int str_count(int n) {
    if (n < 0) return str_count((n == INT_MIN) ? INT_MAX: -n);
    if (n < 10) return 1;
    return 1 + str_count(n / 10);
}

char *get_content_type_by_extension(config_t *config, char *extension) {
    content_types_config_t *content_types = config->content_types;
    char *mime_type = NULL;

    while (content_types != NULL) {
        if (strcmp(content_types->extension, extension) == 0) {
            mime_type = content_types->mime_type;
        }
        content_types = content_types->next;
    }

    return mime_type;
}