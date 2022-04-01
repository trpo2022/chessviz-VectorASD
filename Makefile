APP_NAME = test

CFLAGS = -Wall -Werror -I include -MMD
SRC = $(wildcard src/*.c)
OBJ = $(SRC:src/%.c=obj/%.o)
DEPS = $(OBJ:.o=.d)

.PHONY: all
all: $(OBJ) $(APP_NAME)

-include $(DEPS)

$(APP_NAME):
	gcc -o $@ $(OBJ)

$(OBJ):
	gcc $(CFLAGS) -o $@ -c $(@:obj/%.o=src/%.c)

.PHONY: clean
clean:
	rm -f $(OBJ) $(DEPS)
