#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <step_handler.h>

text caserson(char s, char p) {
    text figures[6][4] = {
        {"p", "пешка", "пешку", "пешкой"},
        {"r", "ладья", "ладью", "ладьёй"},
        {"n", "конь", "коня", "конём"},
        {"b", "слон", "слона", "слоном"},
        {"q", "ферзь", "ферзя", "ферзем"},
        {"k", "король", "короля", "королём"}};
    s = tolower(s);
    for (int i = 0; i < 6; i++)
        if (figures[i][0][0] == s) return figures[i][p + 1];
    return "???";
}

struct HandlerError error_parser_const_pool(text err) {
    struct HandlerError res = {err, 0};
    return res;
}
struct HandlerError error_caserson(text err, char tile, char tile2) {
    char *str = malloc(256);
    sprintf(str, err, caserson(tile, 2), tile, caserson(tile2, 0), tile2);
    struct HandlerError res = {str, 1};
    return res;
}

struct HandlerError handler(char board[8][8], struct Step step, struct Vector *history, int *step_n) {
    if (step.err) {
        if (step.gen) free((void *) step.err);
        return error_parser_const_pool("Осторожнее с тестированием этой функции (parser error) :/");
    }
    int x = step.x - 'a', y = 7 - (step.y - '1');
    int x2 = step.x2 - 'a', y2 = 7 - (step.y2 - '1');
    char is_white = (*step_n) % 2 == 0;
    char tile = board[y][x];
    char tile_is_white = tile >= 'A' && tile <= 'Z';
    char tile2 = board[y2][x2];
    char tile2_is_white = tile2 >= 'A' && tile2 <= 'Z';
    if (tile == ' ') return error_parser_const_pool("Вы не можете ходить пустой клеткой");
    if (x == x2 && y == y2) return error_parser_const_pool("Вы не можете ходить в ту же самую клетку");
    if (is_white && !tile_is_white) return error_parser_const_pool("Вы не можете ходить чёрными фигурами");
    if ((!is_white) && tile_is_white) return error_parser_const_pool("Вы не можете ходить белыми фигурами");
    if (tile2 == ' ') {
        if (step.type == 'x') return error_parser_const_pool("Вы не можете ходить в режиме 'x'. Используйте '-'");
    } else {
        if (is_white == tile2_is_white) return error_parser_const_pool("Вы не можете рубить свои же фигуры");
        if (step.type == '-') return error_parser_const_pool("Вы не можете рубить в режиме '-'. Используйте 'x'");
    }
    if (step.figure && step.figure != toupper(tile)) return error_caserson("Вы хотели походить %s '%c', но на поле оказалась %s '%c'", step.figure, tile);
    if (step.figure2 && step.type == '-')
        return error_parser_const_pool("Вы не можете выбирать, кого хочете рубить, в режиме '-'. Используйте 'x'");

    char dx = x2 - x, dy = y2 - y;
    switch (tolower(tile)) {
    case 'p':
        if (step.type == '-') {
            if ((dx != 0 || dy != -1) && is_white)
                return error_parser_const_pool("Белая пешка может ходить только вверх на одну клетку");
            if ((dx != 0 || dy != 1) && !is_white)
                return error_parser_const_pool("Чёрная пешка может ходить только вниз на одну клетку");
        } else {
            if ((dy != -1 || (dx != -1 && dx != 1)) && is_white)
                return error_parser_const_pool("Белая пешка может рубить только вверх по диагонали на одну клетку");
            if ((dy != 1 || (dx != -1 && dx != 1)) && !is_white)
                return error_parser_const_pool("Чёрная пешка может рубить только вниз по диагонали на одну клетку");
        }
        break;
    case 'r':
        if (dx == 0) {
        } else if (dy == 0) {
        } else
            return error_parser_const_pool("Тура не может перемещаться по диагонали");
    }

    board[y][x] = ' ';
    board[y2][x2] = tile;

    if (history) vector_add(history, step.src);
    (*step_n)++;

    struct HandlerError success = {NULL, 0};
    return success;
}
