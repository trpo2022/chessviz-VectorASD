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
        struct Step step = parser(str, 0);
        if (step.err) {
            printf("Ошибка формата ввода:\n  %s\n", step.err);
            if (step.gen) free((void *) step.err);
        } else {
            printf("Ходящая фигура: %c %c%c\n", step.figure, step.x, step.y);
            printf("Куда топает: %c %c%c\n", step.figure2, step.x2, step.y2);
            printf("Тип хода: %c\n", step.type);
            struct HandlerError err = handler(board, step, history, &step_n);
            if (err.str) {
                printf("Ошибка игровой механики:\n  %s\n", err.str);
                if (err.generated) free((void *) step.err);
            } else
                printf(step.type == '-' ? "Вы успешно походили\n\n" : "Вы успешно у соперника срубили фигуру\n\n");
        }
        board_print_plain(board, history);
        board_print_html(board, history);
    }
    vector_delete(history);
    printf("HAPPY END!!!\n");
    return 0;
}
