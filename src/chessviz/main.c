#include <board_builder.h>
#include <input_parser.h>
#include <stdio.h>
#include <stdlib.h>
#include <step_handler.h>
#include <string.h>

int main(int argc, text *args) {
    char board[8][8] = {
        "rnbqkbnr",
        "pppppppp",
        "        ",
        "        ",
        "        ",
        "        ",
        "PPPPPPPP",
        "RNBQKBNR"};
    struct Vector *history = vector_new();

    board_print_plain(board, history);
    int error_code = board_print_html(board, history);
    if (error_code) return error_code;
    int step_n = 0;
    while (1) {
        char str[100];
        printf("Ход %s\n", step_n % 2 ? "чёрного" : "белого");
        printf("%d. ", step_n / 2 + 1);
        scanf("%s", (char *) &str);
        if (!str[1]) break;
        struct Step res = parser(str);
        if (res.err) {
            printf("Ошибка формата ввода:\n  %s\n", res.err);
            if (res.gen) free((void *) res.err);
        } else {
            printf("Ходящая фигура: %c %c%c\n", res.figure, res.x, res.y);
            printf("Куда топает: %c %c%c\n", res.figure2, res.x2, res.y2);
            printf("Тип хода: %c\n", res.type);
            handler(board, res, history, &step_n);
        }
        board_print_plain(board, history);
        board_print_html(board, history);
    }
    vector_delete(history);
    printf("HAPPY END!!!\n");
    
    if (1)
    
    
    
    
    {
        
        
        
        
        
        		printf("ads,kpasodkads\n");
            printf("ads,kpasodkads\n");
        
         	printf("ads,kpasodkads\n");
    }
    
    
    return 0;
}
