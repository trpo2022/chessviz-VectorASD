CFLAGS = -Wall -Werror -I ./include
SRC = $(wildcard src/*.c)
test: $(SRC)
	gcc -o $@ $^ $(CFLAGS)

