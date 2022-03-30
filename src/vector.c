#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector.h>

struct Vector *vector_new() {
    struct Vector *new = (struct Vector *) malloc(sizeof(struct Vector));
    new->first = new->last = 0;
    new->size = 0;
    return new;
}
void vector_add(struct Vector *vec, text str) {
    struct VectorNode *new = (struct VectorNode *) malloc(sizeof(struct VectorNode));
    int len = strlen(str) + 1;
    char *step = (char *) malloc(len);
    memcpy(step, str, len);
    new->step = step;
    new->child = 0;
    if (vec->size++)
        vec->last = vec->last->child = new;
    else
        vec->last = vec->first = new;
}
void vector_clear(struct Vector *vec) {
    struct VectorNode *node = vec->first;
    while (node) {
        struct VectorNode *next = node->child;
        free((void *) node->step);
        free(node);
        node = next;
    }
    vec->first = vec->last = 0;
    vec->size = 0;
}
void vector_delete(struct Vector *vec) {
    vector_clear(vec);
    free(vec);
}
void vector_print(struct Vector *vec) {
    printf("%15lu -> %15lu %15lu | Vector (%u):\n", (size_t) vec, (size_t) vec->first, (size_t) vec->last, vec->size);
    struct VectorNode *node = vec->first;
    int N = 0;
    while (node) {
        printf("%15lu -> %15lu %15lu | %2u: %s\n", (size_t) node, (size_t) node->step, (size_t) node->child, N++, node->step);
        node = node->child;
    }
}
void vector_test() {
    struct Vector *vec = vector_new();
    vector_print(vec);
    vector_add(vec, "Test string");
    vector_add(vec, "Yeah Meow Woof!");
    vector_add(vec, "I is love ТРПО?");
    vector_add(vec, "I is love ТРПО!");
    vector_print(vec);
    vector_clear(vec);
    vector_print(vec);
    vector_delete(vec);
}
