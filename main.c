#include <stdio.h>
#include <stdlib.h>

typedef const char* text;

FILE* file;
char Board[8][8] = {
  "rnbqkbnr",
  "pppppppp",
  "        ",
  "        ",
  "        ",
  "        ",
  "PPPPPPPP",
  "RNBQKBNR"
};

void print(text Str) {
    fprintf(file, "%s\n", Str);
}
void Builder() {
    print("<!DOCTYPE html>");
    print("<html>");
    print("<head>");
    print("  <meta charset=\"utf-8\">");
    print("  <title>chessviz</title>");
    print("  <style type=\"text/css\">");
    print("    table.chessboard {");
    print("      border: 5px solid #333;");
    print("      border-collapse: collapse;");
    print("      height: 320px;");
    print("      margin: 20px;");
    print("      width: 320px;");
    print("    }");
    print("    table.chessboard caption {");
    print("      text-align: left;");
    print("    }");
    print("    table.chessboard td {");
    print("      background-color: #fff;");
    print("      font-size: 25px;");
    print("      height: 40px;");
    print("      text-align: center;");
    print("      vertical-align: middle;");
    print("      width: 40px;");
    print("    }");
    print("    table.chessboard tr:nth-child(odd) td:nth-child(even),");
    print("    table.chessboard tr:nth-child(even) td:nth-child(odd) {");
    print("      background-color: #999;");
    print("    }");
    print("    table.chessboard .white.king:before   { content: \"\\2654\"; }");
    print("    table.chessboard .white.queen:before  { content: \"\\2655\"; }");
    print("    table.chessboard .white.rook:before   { content: \"\\2656\"; }");
    print("    table.chessboard .white.bishop:before { content: \"\\2657\"; }");
    print("    table.chessboard .white.knight:before { content: \"\\2658\"; }");
    print("    table.chessboard .white.pawn:before   { content: \"\\2659\"; }");
    print("    table.chessboard .black.king:before   { content: \"\\265A\"; }");
    print("    table.chessboard .black.queen:before  { content: \"\\265B\"; }");
    print("    table.chessboard .black.rook:before   { content: \"\\265C\"; }");
    print("    table.chessboard .black.bishop:before { content: \"\\265D\"; }");
    print("    table.chessboard .black.knight:before { content: \"\\265E\"; }");
    print("    table.chessboard .black.pawn:before   { content: \"\\265F\"; }");
    print("  </style>");
    print("</head>");
    print("<body>");
    print("  <table class=\"chessboard\">");
    print("    <caption>1. e2-e4</caption>");
    for(int Y = 0; Y < 8; Y++) {
        print("    <tr>");
    	for(int X = 0; X < 8; X++) {
            char Tile = Board[Y][X];
            switch(Tile) {
            case ' ': print("      <td></td>"); break;
            case 'r': print("      <td><span class=\"black rook\"></span></td>"); break;
            case 'n': print("      <td><span class=\"black knight\"></span></td>"); break;
            case 'b': print("      <td><span class=\"black bishop\"></td>"); break;
            case 'q': print("      <td><span class=\"black queen\"></td>"); break;
            case 'k': print("      <td><span class=\"black king\"></td>"); break;
            case 'p': print("      <td><span class=\"black pawn\"></td>"); break;
            case 'R': print("      <td><span class=\"white rook\"></span></td>"); break;
            case 'N': print("      <td><span class=\"white knight\"></span></td>"); break;
            case 'B': print("      <td><span class=\"white bishop\"></td>"); break;
            case 'Q': print("      <td><span class=\"white queen\"></td>"); break;
            case 'K': print("      <td><span class=\"white king\"></td>"); break;
            case 'P': print("      <td><span class=\"white pawn\"></td>"); break;
            }
        }
        print("    </tr>");
    }
    print("  </table>");
    print("  После каждого хода обновляйте страницу");
    print("</body>");
    print("</html>");
}

char lower(char S) {
    return S >= 'A' && S <= 'Z' ? S - 'A' + 'a' : S;
}
struct Step_s {
    char Figure, X, Y, Type, X2, Y2, Figure2;
    text Err;
};

struct Step_s Error(text Err) {
    struct Step_s Step = {.Err = Err};
    return Step;
}
struct Step_s Error2(int N, char C) {
    text Errors[] = {
        "Ожидался ввод клетки ходящей фигуры или её тип",
        "Ожидался ввод клетки ходящей фигуры",
        "Ожидался ввод типа хода",
        "Ожидался ввод клетки, куда походит фигура",
        "Ожидался ввод типа съедаемой фигуры или шах или мат или en passant"
    };
    char *Err = (char*) malloc(100);
    sprintf(Err, "%s, а было введено: '%c'", Errors[N], C);
    struct Step_s Step = {.Err = Err};
    return Step;
}
struct Step_s Parser(text Str) {
    int Pos = 0, R = 0;
    struct Step_s Step = {.Figure = 0, .X = 0, .Y = 0, .Type = 0, .X2 = 0, .Y2 = 0, .Figure2 = 0, .Err = 0};
    while (Str[Pos] && R < 5) {
    	char Let = Str[Pos++];
    	if (Let == ' ') continue;
    	printf("W%d: %c\n", R, Let);
    	switch(Let) {
    	case 'K': case 'Q': case 'R': case 'B': case 'N': case 'P':
    	    if (R == 0) Step.Figure = Let;
    	    else if (R == 4) Step.Figure2 = Let;
    	    else return Error2(R, Let); 
    	    R += 1;
    	    break;
    	case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h':
    	    if (R == 0 || R == 1) {
    	        if (Step.X) return Error("Вы уже ввели букву клетки ходящей фигуры или её тип");
    	        Step.X = Let;
    	    } else if (R == 2) return Error2(R, Let);
    	    else if (R == 3) {
    	        if (Step.X2) return Error("Вы уже ввели букву клетки, куда ходит фигура");
    	        Step.X2 = Let;
    	    } else {}
    	    break;
    	case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8':
    	    if (R == 0 || R == 1) {
    	        if (!Step.X) return Error("Вы ещё не ввели букву клетки ходящей фигуры или её тип");
    	        Step.Y = Let;
    	        R = 2;
    	    } else if (R == 2) return Error2(R, Let);
    	    else if (R == 3) {
    	        if (!Step.X2) return Error("Вы ещё не ввели букву клетки, куда ходит фигура");
    	        Step.Y2 = Let;
    	        R = 4;
    	    } else {}
    	    break;
    	case '-': case 'x':
    	    if (R == 2) Step.Type = Let;
    	    else return Error2(R, Let);
    	    R = 3;
    	    break;
    	case '+': case '#':
    	    if (R == 4) Step.Figure2 = Let;
    	    else return Error2(R, Let);
    	    R = 5;
    	}
    }
    return Step;
}

int main(int argc, text* args)
{
    printf("Аргументы:\n");
    for (int i = 0; i < argc; i++) {
        printf("%u %s\n", i, args[i]);
    }
    if ((file = fopen("result.html", "w")) == NULL) {
        printf("Не удалось открыть файл на запись\n");
        return 1;
    }
    Builder();
    fclose(file);
    printf("Создание шахматной доски выполнено успешно\n");
    int StepN = 1;
    while (1) {
        char Str[100];
        printf("%d. ", StepN);
        scanf("%s", (char*) &Str);
        if (!Str[1]) break;
        struct Step_s Res = Parser(Str);
        if (Res.Err) printf("Ошибка формата ввода:\n  %s\n", Res.Err);
        else {
            printf("Ходящая фигура: %c %c%c\n", Res.Figure, Res.X, Res.Y);
            printf("Куда топает: %c %c%c\n", Res.Figure2, Res.X2, Res.Y2);
            printf("Тип хода: %c\n", Res.Type);
        }
        StepN += 1;
    }
    printf("HAPPY END!!!\n");
    return 0;
}
