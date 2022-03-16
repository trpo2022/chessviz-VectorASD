#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef const char *text;

struct VectorNode {
    text step;
    struct VectorNode *child;
};
struct Vector {
    struct VectorNode *first;
    struct VectorNode *last;
    int size;
};

struct Vector *vector_new();
void vector_add(struct Vector *vec, text str);
void vector_clear(struct Vector *vec);
void vector_delete(struct Vector *vec);
void vector_print(struct Vector *vec);
void vector_test();
