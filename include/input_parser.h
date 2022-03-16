#pragma once

#include <stdlib.h>
#include <stdio.h>

typedef const char *text;

struct Step {
    char figure, x, y, type, x2, y2, figure2, gen;
    text err, src;
};

struct Step parser(text str);
