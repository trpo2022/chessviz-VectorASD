APP_NAME = test
LIB_NAME = libyeah
PROJECT = chessviz

CFLAGS = -Wall -Werror -I include -MMD

APP_SRC = $(wildcard src/$(PROJECT)/*.c)
LIB_SRC = $(wildcard src/lib$(PROJECT)/*.c)
APP_OBJ = $(APP_SRC:src/%.c=obj/src/%.o)
LIB_OBJ = $(LIB_SRC:src/%.c=obj/src/%.o)
OBJ = $(APP_OBJ) $(LIB_OBJ)
DEPS = $(OBJ:.o=.d)

APP_PATH = bin/$(APP_NAME)
LIB_PATH = obj/src/lib$(PROJECT)/$(LIB_NAME).a

.PHONY: all
all: $(OBJ) $(APP_PATH)

-include $(DEPS)

$(APP_PATH): $(APP_OBJ) $(LIB_PATH)
	gcc $^ -o $@

$(LIB_PATH): $(LIB_OBJ)
	ar rcs $@ $^

$(OBJ):
	gcc $(CFLAGS) -o $@ -c $(@:obj/src/%.o=src/%.c)

.PHONY: clean
clean:
	rm -f $(APP_PATH) $(LIB_PATH) $(OBJ) $(DEPS)
