#include "handler.h"

custom_handlers_t *get_custom_handlers() {
    static custom_handlers_t customHandlers[] = {
            { .name = "@get_lama", .handler = &get_lama },
            { .name = "@post_lama", .handler = &post_lama }
    };
    return customHandlers;
}

