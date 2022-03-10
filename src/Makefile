CC=			gcc

NAME=		cherokee

SRC=		main.c \
			request/parsing/request_parser.c \
			network/server/server_handler.c \
			network/server/server_initialization.c \
			network/socket/socket_handler.c \
			handler/handler.c \
			handler/lama_handler.c \
			handler/handler_config.c \
			request/parsing/utils.c \
			response/response.c \
			utils/lib/json/cJSON.c \
			utils/parse_config.c \
			utils/processing_file.c \
			utils/utils.c \
			network/socket/socket_queue.c \
			logger/logger.c \
			cache/cache.c


RM=			rm -f

OBJ=		$(SRC:%.c=%.o)

CFLAGS=		-Wall -Wextra -Werror


all	:	$(OBJ)
		$(CC) $(CFLAGS) $(OBJ) -pthread -o $(NAME)

clean:
		$(RM) $(OBJ)

fclean:	clean
		$(RM) $(NAME)

re: fclean all
