#include <board_builder.h>
#include <stdio.h>
#include <stdlib.h>

int board_print_html(char board[8][8], struct Vector *history) {
    FILE *file;
    if ((file = fopen("result.html", "w")) == NULL) {
        printf("Не удалось открыть файл на запись\n");
        return 1;
    }
    fprintf(file, "<!DOCTYPE html>"
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
                  "    table.chessboard .white.king:before   { content: \"\\2654\"; }\n"
                  "    table.chessboard .white.queen:before  { content: \"\\2655\"; }\n"
                  "    table.chessboard .white.rook:before   { content: \"\\2656\"; }\n"
                  "    table.chessboard .white.bishop:before { content: \"\\2657\"; }\n"
                  "    table.chessboard .white.knight:before { content: \"\\2658\"; }\n"
                  "    table.chessboard .white.pawn:before   { content: \"\\2659\"; }\n"
                  "    table.chessboard .black.king:before   { content: \"\\265A\"; }\n"
                  "    table.chessboard .black.queen:before  { content: \"\\265B\"; }\n"
                  "    table.chessboard .black.rook:before   { content: \"\\265C\"; }\n"
                  "    table.chessboard .black.bishop:before { content: \"\\265D\"; }\n"
                  "    table.chessboard .black.knight:before { content: \"\\265E\"; }\n"
                  "    table.chessboard .black.pawn:before   { content: \"\\265F\"; }\n"
                  "    div { margin-left: 20px; }\n"
                  "  </style>\n"
                  "</head>\n"
                  "<body>\n"
                  "  <table class=\"chessboard\">\n");
    struct VectorNode *node = history->first;
    int n = 0;
    while (node) {
        fprintf(file, "    <caption>%u. %s</caption>\n", n++ / 2 + 1, node->step);
        node = node->child;
    }
    for (int y = 0; y < 8; y++) {
        fprintf(file, "    <tr><td>%u</td>\n", 8 - y);
        for (int x = 0; x < 8; x++) {
            char tile = board[y][x];
            text line = "";
            switch (tile) {
            case 'r': line = "<span class=\"black rook\"></span>"; break;
            case 'n': line = "<span class=\"black knight\"></span>"; break;
            case 'b': line = "<span class=\"black bishop\">"; break;
            case 'q': line = "<span class=\"black queen\">"; break;
            case 'k': line = "<span class=\"black king\">"; break;
            case 'p': line = "<span class=\"black pawn\">"; break;
            case 'R': line = "<span class=\"white rook\"></span>"; break;
            case 'N': line = "<span class=\"white knight\"></span>"; break;
            case 'B': line = "<span class=\"white bishop\">"; break;
            case 'Q': line = "<span class=\"white queen\">"; break;
            case 'K': line = "<span class=\"white king\">"; break;
            case 'P': line = "<span class=\"white pawn\">"; break;
            }
            fprintf(file, "      <td>%s</td>\n", line);
        }
        fprintf(file, "    </tr>\n");
    }
    fprintf(file, "    <tr><td></td><td>A</td><td>B</td><td>C</td><td>D</td><td>E</td><td>F</td><td>G</td><td>H</td></tr>\n"
                  "  </table>\n"
                  "  <div>После каждого хода обновляйте страницу</div>\n"
                  "</body>\n"
                  "</html>");
    fclose(file);
    return 0;
}

void board_print_plain(char board[8][8], struct Vector *history) {
    struct VectorNode *node = history->first;
    int n = 0;
    while (node) {
        printf("    %u. %s\n", n++ / 2 + 1, node->step);
        node = node->child;
    }
    for (int y = 0; y < 8; y++) {
        printf("%d|", 8 - y);
        for (int x = 0; x < 8; x++) printf("%c", board[y][x]);
        printf("|\n");
    }
    printf(" *~~~~~~~~*\n  abcdefgh\n");
    printf("Создание шахматной доски выполнено успешно\n");
    printf("XD\n");
}
