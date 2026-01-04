CC      = gcc
CFLAGS  = -std=c99 -Wall -Wextra -Iinclude # -g -O0 # (pour Valgrind, valgrind --leak-check=full --track-origins=yes ./metro)
NAME    = metro

SRC     = src/main.c src/menu.c src/edges.c src/chargement_reseau.c src/algorithmes.c src/degre_tri.c

OBJ     = $(SRC:.c=.o)

all: $(NAME)

# Règle de compilation finale
$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

# Compilation des .c vers .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
