LIBS =
CFLAGS = -Wall -Werror
SRC = $(wildcard *.c)

test: $(SRC)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

