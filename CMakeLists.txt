cmake_minimum_required(VERSION 3.17)
project(cherokee C)

set(CMAKE_C_STANDARD 99)

add_executable(cherokee src/main.c src/network/server/server_handler.c src/network/network.h src/network/server/server_initialization.c src/utils/utils.h src/network/socket/socket_handler.c src/utils/lib/json/cJSON.c src/utils/lib/json/cJSON.h src/utils/parse_config.c src/utils/processing_file.c src/request/parsing/parser_utils.h src/request/parsing/request_parser.c src/request/parsing/utils.c src/response/response.h src/response/response.c src/handler/handler.c src/handler/handler.h src/utils/utils.c src/handler/lama_handler.c src/handler/handler_config.c src/network/socket/socket_queue.c src/cache/cache.h src/cache/cache.c src/logger/logger.c)