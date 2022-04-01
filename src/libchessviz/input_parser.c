#include <input_parser.h>
#include <stdio.h>
#include <stdlib.h>

struct Step error_const_pool(text err) {
    struct Step step = {.err = err, .gen = 0};
    return step;
}
struct Step error_generated(int n, char c) {
    text errors[] = {
        "Ожидался ввод клетки ходящей фигуры или её тип",
        "Ожидался ввод клетки ходящей фигуры",
        "Ожидался ввод типа хода",
        "Ожидался ввод клетки, куда походит фигура",
        "Ожидался ввод типа съедаемой фигуры или шах или мат или en "
        "passant"};
    char *err = (char *) malloc(100);
    sprintf(err, "%s, а было введено: '%c'", errors[n], c);
    struct Step step = {.err = err, .gen = 1};
    return step;
}
struct Step parser(text str) {
    int pos = 0, R = 0;
    struct Step step = {.figure = 0, .x = 0, .y = 0, .type = 0, .x2 = 0, .y2 = 0, .figure2 = 0, .err = 0, .src = str};
    while (str[pos] && R < 5) {
        char let = str[pos++];
        if (let == ' ') continue;
        printf("W%d: %c\n", R, let);
        switch (let) {
        case 'K':
        case 'Q':
        case 'R':
        case 'B':
        case 'N':
        case 'P':
            if (R == 0)
                step.figure = let;
            else if (R == 4)
                step.figure2 = let;
            else
                return error_generated(R, let);
            R += 1;
            break;
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
        case 'g':
        case 'h':
            if (R == 0 || R == 1) {
                if (step.x)
                    return error_const_pool("Вы уже ввели букву клетки ходящей фигуры или её тип");
                step.x = let;
                R = 1;
            } else if (R == 2)
                return error_generated(R, let);
            else if (R == 3) {
                if (step.x2)
                    return error_const_pool("Вы уже ввели букву клетки, куда ходит фигура");
                step.x2 = let;
            } else
                return error_generated(R, let);
            break;
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
            if (R == 0 || R == 1) {
                if (!step.x)
                    return error_const_pool("Вы ещё не ввели букву клетки ходящей фигуры или её тип");
                step.y = let;
                R = 2;
            } else if (R == 2)
                return error_generated(R, let);
            else if (R == 3) {
                if (!step.x2)
                    return error_const_pool("Вы ещё не ввели букву клетки, куда ходит фигура");
                step.y2 = let;
                R = 4;
            } else
                return error_generated(R, let);
            break;
        case '-':
        case 'x':
            if (R == 2)
                step.type = let;
            else
                return error_generated(R, let);
            R = 3;
            break;
        case '+':
        case '#':
            if (R == 4)
                step.figure2 = let;
            else
                return error_generated(R, let);
            R = 5;
            break;
        }
    }
    return step;
}
