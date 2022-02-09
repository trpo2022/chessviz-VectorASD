LIBS =
CFLAGS = -Wall
SRC = $(wildcard *.c)

test: $(SRC)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

