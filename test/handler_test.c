#include <ctest.h>
#include <input_parser.h>
#include <stdio.h>
#include <stdlib.h>
#include <step_handler.h>
#include <string.h>

byte test_handler_result(struct HandlerError res, text expected) {
    byte pass = strcmp(res.str, expected) == 0;
    if (!pass) {
        printf("\nБыла вызвана ошибка: %s\n", res.str);
        printf("  А ожидалось: %s\n", expected);
    }
    if (res.generated) free((void *) res.str);
    return pass;
}

CTEST(handler_suite, fail1) {
    char board[8][8] = {"rnbqkbnr", "pppppppp", "        ", "        ", "        ", "        ", "PPPPPPPP", "RNBQKBNR"};
    int step_n = 0;
    ASSERT_TRUE(test_handler_result(handler(board, parser("h8*", 0), NULL, &step_n), "Осторожнее с тестированием этой функции (parser error) :/"));
}
CTEST(handler_suite, fail2) {
    char board[8][8] = {"rnbqkbnr", "pppppppp", "        ", "        ", "        ", "        ", "PPPPPPPP", "RNBQKBNR"};
    int step_n = 0;
    ASSERT_TRUE(test_handler_result(handler(board, parser("a3-a4", 0), NULL, &step_n), "Вы не можете ходить пустой клеткой"));
}
