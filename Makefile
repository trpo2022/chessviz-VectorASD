APP_NAME = test
LIB_NAME = libyeah

CFLAGS = -Wall -Werror -I include -MMD
APP_SRC = $(wildcard src/*.c)
LIB_SRC = $(wildcard src/lib/*.c)
SRC = $(APP_SRC) $(LIB_SRC)
APP_OBJ = $(APP_SRC:src/%.c=obj/%.o)
LIB_OBJ = $(LIB_SRC:src/%.c=obj/%.o)
OBJ = $(APP_OBJ) $(LIB_OBJ)
DEPS = $(OBJ:.o=.d)

APP_PATH = bin/$(APP_NAME)
LIB_PATH = bin/lib/$(LIB_NAME).a

.PHONY: all
all: $(OBJ) $(APP_PATH)

-include $(DEPS)

$(APP_PATH): $(APP_OBJ) $(LIB_PATH)
	gcc $^ -o $@

$(LIB_PATH): $(LIB_OBJ)
	ar rcs $@ $^

$(OBJ):
	gcc $(CFLAGS) -o $@ -c $(@:obj/%.o=src/%.c)

.PHONY: clean
clean:
	rm -f $(OBJ) $(DEPS)
