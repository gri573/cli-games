LDLIBS += -lncurses
CFLAGS += -Wall -Wextra -Ofast
#CFLAGS += -ggdb -fsanitize=address
all: colorsort

colorsort: colorsort.c
	$(CC) $(CFLAGS) $^ $(LDLIBS) -o $@
