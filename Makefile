NAME = philo

SRCS = main.c \
		utils.c

OBJS = $(SRCS:.c=.o)

CC = cc
RM = rm -rf
CFLAGS = -Wall -Werror -Wextra
IFLAGS = -I.

.PHONY: all clean fclean re

all: $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(IFLAGS) -c -o $@ $<

$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all