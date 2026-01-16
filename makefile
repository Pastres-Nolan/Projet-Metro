CC      = gcc
CFLAGS  = -std=c99 -Wall -Wextra -Iinclude # -g -O0 # (pour Valgrind, valgrind --leak-check=full --track-origins=yes ./metro)
NAME    = metro
BUILD   = builds

SRC     = src/main.c src/menu.c src/graphe.c src/station.c src/itineraire.c src/tri_degre.c

OBJ     = $(patsubst src/%.c,$(BUILD)/%.o,$(SRC))

all: $(BUILD) $(NAME)

# Règle de compilation finale
$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(BUILD)/$(NAME) $(OBJ)

# Compilation des .c vers .o
$(BUILD)/%.o: src/%.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

# Création du dossier builds si inexistant
$(BUILD):
	mkdir -p $(BUILD)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(BUILD)/$(NAME)

re: fclean all

.PHONY: all clean fclean re
