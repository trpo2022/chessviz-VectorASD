#include <stdio.h>
#include <stdlib.h>

typedef const char* text;

int Builder(char Board[8][8])
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
            "      height: 320px;\n"
            "      margin: 20px;\n"
            "      width: 320px;\n"
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
            "    table.chessboard tr:nth-child(odd) td:nth-child(even),\n"
            "    table.chessboard tr:nth-child(even) td:nth-child(odd) {\n"
            "      background-color: #999;\n"
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
            "  </style>\n"
            "</head>\n"
            "<body>\n"
            "  <table class=\"chessboard\">\n"
            "    <caption>1. e2-e4</caption>\n");
    for (int Y = 0; Y < 8; Y++) {
        fprintf(file, "    <tr>\n");
        for (int X = 0; X < 8; X++) {
            char Tile = Board[Y][X];
            text Line = "";
            switch (Tile) {
            case 'r':
                Line = "<span class=\"black rook\"></span>";
                break;
            case 'n':
                Line = "<span class=\"black knight\"></span>";
                break;
            case 'b':
                Line = "<span class=\"black bishop\">";
                break;
            case 'q':
                Line = "<span class=\"black queen\">";
                break;
            case 'k':
                Line = "<span class=\"black king\">";
                break;
            case 'p':
                Line = "<span class=\"black pawn\">";
                break;
            case 'R':
                Line = "<span class=\"white rook\"></span>";
                break;
            case 'N':
                Line = "<span class=\"white knight\"></span>";
                break;
            case 'B':
                Line = "<span class=\"white bishop\">";
                break;
            case 'Q':
                Line = "<span class=\"white queen\">";
                break;
            case 'K':
                Line = "<span class=\"white king\">";
                break;
            case 'P':
                Line = "<span class=\"white pawn\">";
                break;
            }
            fprintf(file, "      <td>%s</td>\n", Line);
        }
        fprintf(file, "    </tr>\n");
    }
    fprintf(file,
            "  </table>\n"
            "  После каждого хода обновляйте страницу\n"
            "</body>\n"
            "</html>");
    fclose(file);
    printf("Создание шахматной доски выполнено успешно\n");
    return 0;
}

char lower(char S)
{
    return S >= 'A' && S <= 'Z' ? S - 'A' + 'a' : S;
}
struct Step {
    char Figure, X, Y, Type, X2, Y2, Figure2, Gen;
    text Err;
};

struct Step Error(text Err)
{
    struct Step step = {.Err = Err, .Gen = 0};
    return step;
}
struct Step Error2(int N, char C)
{
    text Errors[]
            = {"Ожидался ввод клетки ходящей фигуры или её тип",
               "Ожидался ввод клетки ходящей фигуры",
               "Ожидался ввод типа хода",
               "Ожидался ввод клетки, куда походит фигура",
               "Ожидался ввод типа съедаемой фигуры или шах или мат или en "
               "passant"};
    char* Err = (char*)malloc(100);
    sprintf(Err, "%s, а было введено: '%c'", Errors[N], C);
    struct Step step = {.Err = Err, .Gen = 1};
    return step;
}
struct Step Parser(text Str)
{
    int Pos = 0, R = 0;
    struct Step step
            = {.Figure = 0,
               .X = 0,
               .Y = 0,
               .Type = 0,
               .X2 = 0,
               .Y2 = 0,
               .Figure2 = 0,
               .Err = 0};
    while (Str[Pos] && R < 5) {
        char Let = Str[Pos++];
        if (Let == ' ')
            continue;
        printf("W%d: %c\n", R, Let);
        switch (Let) {
        case 'K':
        case 'Q':
        case 'R':
        case 'B':
        case 'N':
        case 'P':
            if (R == 0)
                step.Figure = Let;
            else if (R == 4)
                step.Figure2 = Let;
            else
                return Error2(R, Let);
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
                if (step.X)
                    return Error(
                            "Вы уже ввели букву клетки ходящей фигуры или её "
                            "тип");
                step.X = Let;
            } else if (R == 2)
                return Error2(R, Let);
            else if (R == 3) {
                if (step.X2)
                    return Error(
                            "Вы уже ввели букву клетки, куда ходит фигура");
                step.X2 = Let;
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
                if (!step.X)
                    return Error(
                            "Вы ещё не ввели букву клетки ходящей фигуры или "
                            "её тип");
                step.Y = Let;
                R = 2;
            } else if (R == 2)
                return Error2(R, Let);
            else if (R == 3) {
                if (!step.X2)
                    return Error(
                            "Вы ещё не ввели букву клетки, куда ходит фигура");
                step.Y2 = Let;
                R = 4;
            } else {
            }
            break;
        case '-':
        case 'x':
            if (R == 2)
                step.Type = Let;
            else
                return Error2(R, Let);
            R = 3;
            break;
        case '+':
        case '#':
            if (R == 4)
                step.Figure2 = Let;
            else
                return Error2(R, Let);
            R = 5;
        }
    }
    return step;
}

int main(int argc, text* args)
{
    printf("Аргументы:\n");
    for (int i = 0; i < argc; i++) {
        printf("%u %s\n", i, args[i]);
    }
    char Board[8][8]
            = {"rnbqkbnr",
               "pppppppp",
               "        ",
               "        ",
               "        ",
               "        ",
               "PPPPPPPP",
               "RNBQKBNR"};
    int error_code = Builder(Board);
    if (error_code)
        return error_code;
    int StepN = 0;
    while (1) {
        char Str[100];
        printf("Ход %s\n", StepN % 2 ? "чёрного" : "белого");
        printf("%d. ", StepN / 2 + 1);
        scanf("%s", (char*)&Str);
        if (!Str[1])
            break;
        struct Step Res = Parser(Str);
        if (Res.Err) {
            printf("Ошибка формата ввода:\n  %s\n", Res.Err);
            if (Res.Gen)
                free((void*)Res.Err);
        } else {
            printf("Ходящая фигура: %c %c%c\n", Res.Figure, Res.X, Res.Y);
            printf("Куда топает: %c %c%c\n", Res.Figure2, Res.X2, Res.Y2);
            printf("Тип хода: %c\n", Res.Type);
            StepN += 1;
        }
    }
    printf("HAPPY END!!!\n");
    return 0;
}
