#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef const char* text;

struct VectorNode {
    text step;
    struct VectorNode* child;
};
struct Vector {
    struct VectorNode* first;
    struct VectorNode* last;
    int size;
};

struct Vector* vector_new()
{
    struct Vector* new = (struct Vector*)malloc(sizeof(struct Vector));
    new->first = new->last = 0;
    new->size = 0;
    return new;
}
void vector_add(struct Vector* vec, text str)
{
    struct VectorNode* new
            = (struct VectorNode*)malloc(sizeof(struct VectorNode));
    int len = strlen(str) + 1;
    char* step = (char*)malloc(len);
    memcpy(step, str, len);
    new->step = step;
    new->child = 0;
    if (vec->size++)
        vec->last = vec->last->child = new;
    else
        vec->last = vec->first = new;
}
void vector_clear(struct Vector* vec)
{
    struct VectorNode* node = vec->first;
    while (node) {
        struct VectorNode* next = node->child;
        free((void*)node->step);
        free(node);
        node = next;
    }
    vec->first = vec->last = 0;
    vec->size = 0;
}
void vector_delete(struct Vector* vec)
{
    vector_clear(vec);
    free(vec);
}
void vector_print(struct Vector* vec)
{
    printf("%15lu -> %15lu %15lu | Vector (%u):\n",
           (size_t)vec,
           (size_t)vec->first,
           (size_t)vec->last,
           vec->size);
    struct VectorNode* node = vec->first;
    int N = 0;
    while (node) {
        printf("%15lu -> %15lu %15lu | %2u: %s\n",
               (size_t)node,
               (size_t)node->step,
               (size_t)node->child,
               N++,
               node->step);
        node = node->child;
    }
}
void vector_test()
{
    struct Vector* vec = vector_new();
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

int builder(char board[8][8], struct Vector* history)
{
    FILE* file;
    if ((file = fopen("result.html", "w")) == NULL) {
        printf("Не удалось открыть файл на запись\n");
        return 1;
    }
    fprintf(file,
            "<!DOCTYPE html>"
            "<html>\n"
            "<head>\n"
            "  <meta charset=\"utf-8\">\n"
            "  <title>chessviz</title>\n"
            "  <style type=\"text/css\">\n"
            "    table.chessboard {\n"
            "      border: 5px solid #333;\n"
            "      border-collapse: collapse;\n"
            "      height: 361px;\n"
            "      margin: 20px;\n"
            "      width: 361px;\n"
            "    }\n"
            "    table.chessboard caption {\n"
            "      text-align: left;\n"
            "    }\n"
            "    table.chessboard td {\n"
            "      background-color: #fff;\n"
            "      font-size: 25px;\n"
            "      height: 40px;\n"
            "      text-align: center;\n"
            "      vertical-align: middle;\n"
            "      width: 40px;\n"
            "    }\n"
            "    table.chessboard tr:nth-child(even) td:nth-child(even),\n"
            "    table.chessboard tr:nth-child(odd) td:nth-child(odd) {\n"
            "      background-color: #999;\n"
            "    }\n"
            "    table.chessboard tr:nth-child(9) td:nth-child(n) {\n"
            "      height: 20px;\n"
            "      border-top: 3px solid #555;\n"
            "    }\n"
            "    table.chessboard tr:nth-child(n) td:nth-child(1) {\n"
            "      width: 20px;\n"
            "      border-right: 3px solid #555;\n"
            "    }\n"
            "    table.chessboard tr:nth-child(9) td:nth-child(1) {\n"
            "      border: none;\n"
            "    }\n"
            "    table.chessboard tr:nth-child(9) td:nth-child(n),\n"
            "    table.chessboard tr:nth-child(n) td:nth-child(1) {\n"
            "      background-color: #ffd800;\n"
            "      color: #fffbe6;\n"
            "      font-size: 16px;\n"
            "      text-shadow: 1px 1px 1px #a25f2a;\n"
            "    }\n"
            "    table.chessboard .white.king:before   { content: \"\\2654\"; "
            "}\n"
            "    table.chessboard .white.queen:before  { content: \"\\2655\"; "
            "}\n"
            "    table.chessboard .white.rook:before   { content: \"\\2656\"; "
            "}\n"
            "    table.chessboard .white.bishop:before { content: \"\\2657\"; "
            "}\n"
            "    table.chessboard .white.knight:before { content: \"\\2658\"; "
            "}\n"
            "    table.chessboard .white.pawn:before   { content: \"\\2659\"; "
            "}\n"
            "    table.chessboard .black.king:before   { content: \"\\265A\"; "
            "}\n"
            "    table.chessboard .black.queen:before  { content: \"\\265B\"; "
            "}\n"
            "    table.chessboard .black.rook:before   { content: \"\\265C\"; "
            "}\n"
            "    table.chessboard .black.bishop:before { content: \"\\265D\"; "
            "}\n"
            "    table.chessboard .black.knight:before { content: \"\\265E\"; "
            "}\n"
            "    table.chessboard .black.pawn:before   { content: \"\\265F\"; "
            "}\n"
            "    div { margin-left: 20px; }\n"
            "  </style>\n"
            "</head>\n"
            "<body>\n"
            "  <table class=\"chessboard\">\n");
    struct VectorNode* node = history->first;
    int n = 0;
    while (node) {
        fprintf(file,
                "    <caption>%u. %s</caption>\n",
                n++ / 2 + 1,
                node->step);
        node = node->child;
    }
    for (int y = 0; y < 8; y++) {
        fprintf(file, "    <tr><td>%u</td>\n", 8 - y);
        for (int x = 0; x < 8; x++) {
            char tile = board[y][x];
            text line = "";
            switch (tile) {
            case 'r':
                line = "<span class=\"black rook\"></span>";
                break;
            case 'n':
                line = "<span class=\"black knight\"></span>";
                break;
            case 'b':
                line = "<span class=\"black bishop\">";
                break;
            case 'q':
                line = "<span class=\"black queen\">";
                break;
            case 'k':
                line = "<span class=\"black king\">";
                break;
            case 'p':
                line = "<span class=\"black pawn\">";
                break;
            case 'R':
                line = "<span class=\"white rook\"></span>";
                break;
            case 'N':
                line = "<span class=\"white knight\"></span>";
                break;
            case 'B':
                line = "<span class=\"white bishop\">";
                break;
            case 'Q':
                line = "<span class=\"white queen\">";
                break;
            case 'K':
                line = "<span class=\"white king\">";
                break;
            case 'P':
                line = "<span class=\"white pawn\">";
                break;
            }
            fprintf(file, "      <td>%s</td>\n", line);
        }
        fprintf(file, "    </tr>\n");
    }
    fprintf(file,
            "    "
            "<tr><td></td><td>A</td><td>B</td><td>C</td><td>D</td><td>E</"
            "td><td>F</td><td>G</td><td>H</td></tr>\n"
            "  </table>\n"
            "  <div>После каждого хода обновляйте страницу</div>\n"
            "</body>\n"
            "</html>");
    fclose(file);
    printf("Создание шахматной доски выполнено успешно\n");
    return 0;
}

struct Step {
    char figure, x, y, type, x2, y2, figure2, gen;
    text err, src;
};

struct Step error(text err)
{
    struct Step step = {.err = err, .gen = 0};
    return step;
}
struct Step error2(int n, char c)
{
    text errors[]
            = {"Ожидался ввод клетки ходящей фигуры или её тип",
               "Ожидался ввод клетки ходящей фигуры",
               "Ожидался ввод типа хода",
               "Ожидался ввод клетки, куда походит фигура",
               "Ожидался ввод типа съедаемой фигуры или шах или мат или en "
               "passant"};
    char* err = (char*)malloc(100);
    sprintf(err, "%s, а было введено: '%c'", errors[n], c);
    struct Step step = {.err = err, .gen = 1};
    return step;
}
struct Step parser(text str)
{
    int pos = 0, R = 0;
    struct Step step
            = {.figure = 0,
               .x = 0,
               .y = 0,
               .type = 0,
               .x2 = 0,
               .y2 = 0,
               .figure2 = 0,
               .err = 0,
               .src = str};
    while (str[pos] && R < 5) {
        char let = str[pos++];
        if (let == ' ')
            continue;
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
                return error2(R, let);
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
                    return error(
                            "Вы уже ввели букву клетки ходящей фигуры или её "
                            "тип");
                step.x = let;
                R = 1;
            } else if (R == 2)
                return error2(R, let);
            else if (R == 3) {
                if (step.x2)
                    return error(
                            "Вы уже ввели букву клетки, куда ходит фигура");
                step.x2 = let;
            } else {
            }
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
                    return error(
                            "Вы ещё не ввели букву клетки ходящей фигуры или "
                            "её тип");
                step.y = let;
                R = 2;
            } else if (R == 2)
                return error2(R, let);
            else if (R == 3) {
                if (!step.x2)
                    return error(
                            "Вы ещё не ввели букву клетки, куда ходит фигура");
                step.y2 = let;
                R = 4;
            } else {
            }
            break;
        case '-':
        case 'x':
            if (R == 2)
                step.type = let;
            else
                return error2(R, let);
            R = 3;
            break;
        case '+':
        case '#':
            if (R == 4)
                step.figure2 = let;
            else
                return error2(R, let);
            R = 5;
            break;
        }
    }
    return step;
}

char lower(char s)
{
    return s >= 'A' && s <= 'Z' ? s - 'A' + 'a' : s;
}
char upper(char s)
{
    return s >= 'a' && s <= 'z' ? s - 'a' + 'A' : s;
}

text caserson(char s, char p)
{
    text figures[6][4]
            = {{"p", "пешка", "пешку", "пешкой"},
               {"r", "ладья", "ладью", "ладьёй"},
               {"n", "конь", "коня", "конём"},
               {"b", "слон", "слона", "слоном"},
               {"q", "ферзь", "ферзя", "ферзем"},
               {"k", "король", "короля", "королём"}};
    s = lower(s);
    for (int i = 0; i < 6; i++)
        if (figures[i][0][0] == s)
            return figures[i][p + 1];
    return "???";
}

void error_h(text err)
{
    printf("Ошибка игровой механики:\n  %s\n", err);
}
void error_h2(text err, char tile, char tile2)
{
    char str[150];
    sprintf(str, err, caserson(tile, 2), tile, caserson(tile2, 0), tile2);

    printf("Ошибка игровой механики:\n  %s\n", str);
}

void handler(
        char board[8][8], struct Step step, struct Vector* history, int* step_n)
{
    int x = step.x - 'a', y = 7 - (step.y - '1');
    int x2 = step.x2 - 'a', y2 = 7 - (step.y2 - '1');
    char is_white = (*step_n) % 2 == 0;
    char tile = board[y][x];
    char tile_is_white = tile >= 'A' && tile <= 'Z';
    char tile2 = board[y2][x2];
    char tile2_is_white = tile2 >= 'A' && tile2 <= 'Z';
    if (tile == ' ')
        return error_h("Вы не можете ходить пустой клеткой");
    if (x == x2 && y == y2)
        return error_h("Вы не можете ходить в ту же самую клетку");
    if (is_white && !tile_is_white)
        return error_h("Вы не можете ходить чёрными фигурами");
    if ((!is_white) && tile_is_white)
        return error_h("Вы не можете ходить белыми фигурами");
    if (tile2 == ' ') {
        if (step.type == 'x')
            return error_h("Вы не можете ходить в режиме 'x'. Используйте '-'");
    } else {
        if (is_white == tile2_is_white)
            return error_h("Вы не можете рубить свои же фигуры");
        if (step.type == '-')
            return error_h("Вы не можете рубить в режиме '-'. Используйте 'x'");
    }
    if (step.figure && step.figure != upper(tile))
        return error_h2(
                "Вы хотели походить %s '%c', но на поле оказалась %s '%c'",
                step.figure,
                tile);
    if (step.figure2 && step.type == '-')
        return error_h(
                "Вы не можете выбирать, кого хочете рубить, в режиме '-'. "
                "Используйте 'x'");

    char dx = x2 - x, dy = y2 - y;
    switch (lower(tile)) {
    case 'p':
        if (step.type == '-') {
            if ((dx != 0 || dy != -1) && is_white)
                return error_h(
                        "Белая пешка может ходить только вверх на одну клетку");
            if ((dx != 0 || dy != 1) && !is_white)
                return error_h(
                        "Чёрная пешка может ходить только вниз на одну клетку");
        } else {
            if ((dy != -1 || (dx != -1 && dx != 1)) && is_white)
                return error_h(
                        "Белая пешка может рубить только вверх по диагонали на "
                        "одну клетку");
            if ((dy != 1 || (dx != -1 && dx != 1)) && !is_white)
                return error_h(
                        "Чёрная пешка может рубить только вниз по диагонали на "
                        "одну клетку");
        }
        break;
    case 'r':
        if (dx == 0) {
        } else if (dy == 0) {
        } else
            return error_h("Тура не может перемещаться по диагонали");
    }

    if (step.type == '-')
        printf("Вы успешно походили\n");
    else
        printf("Вы успешно срубили у соперника фигуру\n");
    board[y][x] = ' ';
    board[y2][x2] = tile;

    vector_add(history, step.src);
    (*step_n)++;
    builder(board, history);
}

int main(int argc, text* args)
{
    printf("Аргументы:\n");
    for (int i = 0; i < argc; i++) {
        printf("%u %s\n", i, args[i]);
    }
    char board[8][8]
            = {"rnbqkbnr",
               "pppppppp",
               "        ",
               "        ",
               "        ",
               "        ",
               "PPPPPPPP",
               "RNBQKBNR"};
    struct Vector* history = vector_new();
    int error_code = builder(board, history);
    if (error_code)
        return error_code;
    int step_n = 0;
    while (1) {
        char str[100];
        printf("Ход %s\n", step_n % 2 ? "чёрного" : "белого");
        printf("%d. ", step_n / 2 + 1);
        scanf("%s", (char*)&str);
        if (!str[1])
            break;
        struct Step res = parser(str);
        if (res.err) {
            printf("Ошибка формата ввода:\n  %s\n", res.err);
            if (res.gen)
                free((void*)res.err);
        } else {
            printf("Ходящая фигура: %c %c%c\n", res.figure, res.x, res.y);
            printf("Куда топает: %c %c%c\n", res.figure2, res.x2, res.y2);
            printf("Тип хода: %c\n", res.type);
            handler(board, res, history, &step_n);
        }
    }
    vector_delete(history);
    printf("HAPPY END!!!\n");
    return 0;
}
