#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <step_handler.h>

text caserson(char s, char p) {
    text figures[6][5] = {
        //F   И.Падеж.   Д.П.     Т.П.      Р.П.
        {"p", "пешка", "пешку", "пешкой", "пешки"},
        {"r", "тура", "туру", "турой", "туры"},
        {"n", "конь", "коня", "конём", "коня"},
        {"b", "слон", "слона", "слоном", "слона"},
        {"q", "королева", "королеву", "королевой", "королевы"},
        {"k", "король", "короля", "королём", "короля"}};
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
    sprintf(str, err, caserson(tile, 2), tile, caserson(tile2, 3), tile2);
    struct HandlerError res = {str, 1};
    return res;
}

#define max(a, b) ({ a > b ? a : b; })
#define sign(a) ({ a > 0 ? 1 : a == 0 ? 0 : -1; })

struct HandlerError Tracer(char board[8][8], int x, int y, int dx, int dy) {
    int steps = max(abs(dx), abs(dy));
    dx = sign(dx);
    dy = sign(dy);
    int walk_figure = board[y][x];
    for (int i = 0; i < steps; i++) {
        x += dx;
        y += dy;
        if (x < 0 || y < 0 || x > 7 || y > 7) {
            char *str = malloc(128);
            sprintf(str, "Внезапный выход за границу: %u %u", x, y);
            struct HandlerError err = {str, 0};
            return err;
        }
        char figure = board[y][x];
        if (figure != ' ') {
            char *str = malloc(128);
            figure = tolower(figure);
            sprintf(str, "Внезапно на пути %s %s при ходьбе %s", figure == 'p' || figure == 'r' ? "встречена" : "встречен", caserson(figure, 0), caserson(walk_figure, 2));
            struct HandlerError err = {str, 0};
            return err;
        }
    }
    struct HandlerError success = {NULL, 0};
    return success;
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
    if (step.figure && step.figure != toupper(tile)) return error_caserson("Вы хотели походить %s '%c', но на поле оказалась фигура %s '%c'", step.figure, tile);
    if (step.figure2 && step.type == '-')
        return error_parser_const_pool("Вы не можете выбирать, кого хочете рубить, в режиме '-'. Используйте 'x'");

    char dx = x2 - x, dy = y2 - y;
    switch (tolower(tile)) {
    case 'p':
        if (step.type == '-') {
            if (!(dx == 0 && (dy == -1 || (dy == -2 && y == 6))) && is_white)
                return error_parser_const_pool("Белая пешка может ходить только вверх на одну клетку");
            if (!(dx == 0 && (dy == 1 || (dy == 2 && y == 1))) && !is_white)
                return error_parser_const_pool("Чёрная пешка может ходить только вниз на одну клетку");
        } else {
            if ((dy != -1 || (dx != -1 && dx != 1)) && is_white)
                return error_parser_const_pool("Белая пешка может рубить только вверх по диагонали на одну клетку");
            if ((dy != 1 || (dx != -1 && dx != 1)) && !is_white)
                return error_parser_const_pool("Чёрная пешка может рубить только вниз по диагонали на одну клетку");
        }
        break;
    case 'r': {
        if (dx != 0 && dy != 0) return error_parser_const_pool("Тура не может перемещаться по диагонали");
        struct HandlerError err = Tracer(board, x, y, dx, dy);
        if (err.str) return err;
        break;
    }
    case 'n':
        dx = abs(dx);
        dy = abs(dy);
        if (!((dx == 1 && dy == 2) || (dx == 2 && dy == 1))) return error_parser_const_pool("Конь может ходить только буквой Г");
        break;
    case 'b': {
        if (abs(dx) != abs(dy)) return error_parser_const_pool("Слон может перемещаться только ровно по диагонали");
        struct HandlerError err = Tracer(board, x, y, dx, dy);
        if (err.str) return err;
        break;
    }
    case 'q': {
        if (dx != 0 && dy != 0 && abs(dx) != abs(dy)) return error_parser_const_pool("Ферзь не может идти не по ровной диагонали");
        struct HandlerError err = Tracer(board, x, y, dx, dy);
        if (err.str) return err;
        break;
    }
    case 'k':
        if (abs(dx) > 1 || abs(dy) > 1) return error_parser_const_pool("Король может ходить только по соседним 8 клеткам");
        break;
    }

    board[y][x] = ' ';
    board[y2][x2] = tile;

    if (history) vector_add(history, step.src);
    (*step_n)++;

    struct HandlerError success = {NULL, 0};
    return success;
}
