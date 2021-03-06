#include <ctest.h>
#include <input_parser.h>
#include <stdio.h>
#include <stdlib.h>
#include <step_handler.h>
#include <string.h>

byte test_handler_result(struct HandlerError res, text expected) {
    byte pass = (res.str == NULL && expected == NULL) || (res.str != NULL && expected != NULL && strcmp(res.str, expected) == 0);
    if (!pass) {
        printf("\nБыла вызвана ошибка: %s\n", res.str);
        printf("  А ожидалось: %s\n", expected);
    }
    if (res.generated) free((void *) res.str);
    return pass;
}
byte test_board(char board[8][8], text effects) {
    byte x = effects[0] - 'a';
    byte y = 7 - (effects[1] - '1');
    byte x2 = effects[3] - 'a';
    byte y2 = 7 - (effects[4] - '1');
    byte yeah = board[y][x] == effects[2];
    byte yeah2 = board[y2][x2] == effects[5];
    if (!(yeah && yeah2)) printf("\n");
    if (!yeah) printf("В первой клетке [%u/%u] ожидалось '%c', а оказалось '%c'\n", x, y, effects[2], board[y][x]);
    if (!yeah2) printf("Во второй клетке [%u/%u] ожидалось '%c', а оказалось '%c'\n", x2, y2, effects[5], board[y2][x2]);
    return yeah && yeah2;
}

CTEST(handler_suite, success1) {
    char board[8][8] = {"rnbqkbnr", "pppppppp", "P       ", "        ", "        ", "        ", " PPPPPPP", "RNBQKBNR"};
    int step_n = 0;
    ASSERT_TRUE(test_handler_result(handler(board, parser("Pa6xb7P", 0), NULL, &step_n), NULL) && test_board(board, "a6 b7P"));
}
CTEST(handler_suite, success2) {
    char board[8][8] = {"rnbqkbnr", "pppppppp", "P       ", "        ", "        ", "        ", " PPPPPPP", "RNBQKBNR"};
    int step_n = 0;
    ASSERT_TRUE(test_handler_result(handler(board, parser("Pb2-b4", 0), NULL, &step_n), NULL) && test_board(board, "b2 b4P"));
    ASSERT_TRUE(test_handler_result(handler(board, parser("Pb7-b5", 0), NULL, &step_n), NULL) && test_board(board, "b7 b5p"));
}
CTEST(handler_suite, success3) {
    char board[8][8] = {"rnbqkbnr", "pppppppp", "        ", "        ", "        ", "        ", "PPPPPPPP", "RNBQKBNR"};
    int step_n = 0;
    ASSERT_TRUE(test_handler_result(handler(board, parser("Pd2-d4", 0), NULL, &step_n), NULL) && test_board(board, "d2 d4P"));
    ASSERT_TRUE(test_handler_result(handler(board, parser("Pe7-e5", 0), NULL, &step_n), NULL) && test_board(board, "e7 e5p"));
    ASSERT_TRUE(test_handler_result(handler(board, parser("Pd4xe5P", 0), NULL, &step_n), NULL) && test_board(board, "d4 e5P"));
    ASSERT_TRUE(test_handler_result(handler(board, parser("Pd7-d6", 0), NULL, &step_n), NULL) && test_board(board, "d7 d6p"));
    ASSERT_TRUE(test_handler_result(handler(board, parser("Pe5xd6P", 0), NULL, &step_n), NULL) && test_board(board, "e5 d6P"));
    ASSERT_TRUE(test_handler_result(handler(board, parser("Qd8xd6P", 0), NULL, &step_n), NULL) && test_board(board, "d8 d6q"));
}

CTEST(handler_suite, morf) {
    char board[8][8] = {"        ", "PPPPpppp", "        ", "k      K", "        ", "        ", "ppppPPPP", "        "};
    int step_n = 0;
    ASSERT_TRUE(test_handler_result(handler(board, parser("Pa7-a8", 0), NULL, &step_n), "Вы не выбрали, в кого должна превращаться пешка"));
    ASSERT_TRUE(test_handler_result(handler(board, parser("Pa7-a8P", 0), NULL, &step_n), "Пешка не может превращаться в саму себя"));
    ASSERT_TRUE(test_handler_result(handler(board, parser("Pa7-a8K", 0), NULL, &step_n), "Ещё один король? Серьёзно? XD"));
    ASSERT_TRUE(test_handler_result(handler(board, parser("Pa7-a8Q", 0), NULL, &step_n), NULL) && test_board(board, "a7 a8Q"));
    ASSERT_TRUE(test_handler_result(handler(board, parser("Pa2-a1Q", 0), NULL, &step_n), NULL) && test_board(board, "a2 a1q"));
    ASSERT_TRUE(test_handler_result(handler(board, parser("Pb7-b8R", 0), NULL, &step_n), NULL) && test_board(board, "b7 b8R"));
    ASSERT_TRUE(test_handler_result(handler(board, parser("Pb2-b1R", 0), NULL, &step_n), NULL) && test_board(board, "b2 b1r"));
    ASSERT_TRUE(test_handler_result(handler(board, parser("Pc7-c8N", 0), NULL, &step_n), NULL) && test_board(board, "c7 c8N"));
    ASSERT_TRUE(test_handler_result(handler(board, parser("Pc2-c1N", 0), NULL, &step_n), NULL) && test_board(board, "c2 c1n"));
    ASSERT_TRUE(test_handler_result(handler(board, parser("Pd7-d8B", 0), NULL, &step_n), NULL) && test_board(board, "d7 d8B"));
    ASSERT_TRUE(test_handler_result(handler(board, parser("Pe7-e8B", 0), NULL, &step_n), "Вы не можете выбирать, кого хочете рубить, в режиме '-'. Используйте 'x'"));
    ASSERT_TRUE(test_handler_result(handler(board, parser("Pd2-d1B", 0), NULL, &step_n), NULL) && test_board(board, "d2 d1b"));
    ASSERT_TRUE(test_handler_result(handler(board, parser("Pe2-e1B", 0), NULL, &step_n), "Вы не можете выбирать, кого хочете рубить, в режиме '-'. Используйте 'x'"));
    ASSERT_TRUE(test_handler_result(handler(board, parser("Qa8xa1Q", 0), NULL, &step_n), "Внезапно на пути встречен король при ходьбе королевой"));
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
CTEST(handler_suite, fail3) {
    char board[8][8] = {"rnbqkbnr", "pppppppp", "        ", "        ", "        ", "        ", "PPPPPPPP", "RNBQKBNR"};
    int step_n = 0;
    ASSERT_TRUE(test_handler_result(handler(board, parser("a1-a1", 0), NULL, &step_n), "Вы не можете ходить в ту же самую клетку"));
}
CTEST(handler_suite, fail4) {
    char board[8][8] = {"rnbqkbnr", "pppppppp", "        ", "        ", "        ", "        ", "PPPPPPPP", "RNBQKBNR"};
    int step_n = 0;
    ASSERT_TRUE(test_handler_result(handler(board, parser("a7-a6", 0), NULL, &step_n), "Вы не можете ходить чёрными фигурами"));
}
CTEST(handler_suite, fail5) {
    char board[8][8] = {"rnbqkbnr", "pppppppp", "        ", "        ", "        ", "        ", "PPPPPPPP", "RNBQKBNR"};
    int step_n = 1;
    ASSERT_TRUE(test_handler_result(handler(board, parser("a2-a3", 0), NULL, &step_n), "Вы не можете ходить белыми фигурами"));
}
CTEST(handler_suite, fail6) {
    char board[8][8] = {"rnbqkbnr", "pppppppp", "        ", "        ", "        ", "        ", "PPPPPPPP", "RNBQKBNR"};
    int step_n = 0;
    ASSERT_TRUE(test_handler_result(handler(board, parser("a2xa3", 0), NULL, &step_n), "Вы не можете ходить в режиме 'x'. Используйте '-'"));
}
CTEST(handler_suite, fail7) {
    char board[8][8] = {"rnbqkbnr", "pppppppp", "P       ", "        ", "        ", "        ", " PPPPPPP", "RNBQKBNR"};
    int step_n = 0;
    ASSERT_TRUE(test_handler_result(handler(board, parser("a6-b7", 0), NULL, &step_n), "Вы не можете рубить в режиме '-'. Используйте 'x'"));
}
CTEST(handler_suite, fail8) {
    char board[8][8] = {"rnbqkbnr", "pppppppp", "P       ", "        ", "        ", "        ", " PPPPPPP", "RNBQKBNR"};
    int step_n = 0;
    ASSERT_TRUE(test_handler_result(handler(board, parser("a1-b2", 0), NULL, &step_n), "Вы не можете рубить свои же фигуры"));
}
CTEST(handler_suite, fail9_14) {
    char board[8][8] = {"rnbqkbnr", "pppppppp", "P       ", "        ", "        ", "        ", " PPPPPPP", "RNBQKBNR"};
    int step_n = 0;
    ASSERT_TRUE(test_handler_result(handler(board, parser("Ra6xb7", 0), NULL, &step_n), "Вы хотели походить турой 'R', но на поле оказалась фигура пешки 'P'"));
    ASSERT_TRUE(test_handler_result(handler(board, parser("Pb1-b3", 0), NULL, &step_n), "Вы хотели походить пешкой 'P', но на поле оказалась фигура коня 'N'"));
    ASSERT_TRUE(test_handler_result(handler(board, parser("Qc1-b3", 0), NULL, &step_n), "Вы хотели походить королевой 'Q', но на поле оказалась фигура слона 'B'"));
    ASSERT_TRUE(test_handler_result(handler(board, parser("Bd1-b3", 0), NULL, &step_n), "Вы хотели походить слоном 'B', но на поле оказалась фигура королевы 'Q'"));
    ASSERT_TRUE(test_handler_result(handler(board, parser("Ne1-b3", 0), NULL, &step_n), "Вы хотели походить конём 'N', но на поле оказалась фигура короля 'K'"));
    ASSERT_TRUE(test_handler_result(handler(board, parser("Kh1-b3", 0), NULL, &step_n), "Вы хотели походить королём 'K', но на поле оказалась фигура туры 'R'"));
}
CTEST(handler_suite, fail15) {
    char board[8][8] = {"rnbqkbnr", "pppppppp", "        ", "        ", "        ", "        ", "PPPPPPPP", "RNBQKBNR"};
    int step_n = 0;
    ASSERT_TRUE(test_handler_result(handler(board, parser("a1-b3K", 0), NULL, &step_n), "Вы не можете выбирать, кого хочете рубить, в режиме '-'. Используйте 'x'"));
}

CTEST(handler_suite, testP) {
    char board[8][8] = {"rnbqkbnr", "pppppppp", "P       ", "        ", "        ", "        ", " PPPPPPP", "RNBQKBNR"};
    int step_n = 0, step_n2 = 1;
    ASSERT_TRUE(test_handler_result(handler(board, parser("Pb2-b5", 0), NULL, &step_n), "Белая пешка может ходить только вверх на одну клетку"));
    ASSERT_TRUE(test_handler_result(handler(board, parser("Pa6xa7", 0), NULL, &step_n), "Белая пешка может рубить только вверх по диагонали на одну клетку"));
    ASSERT_TRUE(test_handler_result(handler(board, parser("Pb7-c6", 0), NULL, &step_n2), "Чёрная пешка может ходить только вниз на одну клетку"));
    ASSERT_TRUE(test_handler_result(handler(board, parser("Pa7xa6", 0), NULL, &step_n2), "Чёрная пешка может рубить только вниз по диагонали на одну клетку"));
}
CTEST(handler_suite, testR) {
    char board[8][8] = {"rnbqkbnr", "pppppppp", "        ", "        ", "   R    ", "        ", "PPPPPPPP", " NBQKBNR"};
    int step_n = 0;
    ASSERT_TRUE(test_handler_result(handler(board, parser("Rd4-e5", 0), NULL, &step_n), "Тура не может перемещаться по диагонали"));
    ASSERT_TRUE(test_handler_result(handler(board, parser("Rd4xd8", 0), NULL, &step_n), "Внезапно на пути встречена пешка при ходьбе турой"));
    ASSERT_TRUE(test_handler_result(handler(board, parser("Rd4xd2", 0), NULL, &step_n), "Вы не можете рубить свои же фигуры"));
    ASSERT_TRUE(test_handler_result(handler(board, parser("Rd4xd7", 0), NULL, &step_n), NULL));
}
CTEST(handler_suite, testN) {
    char board[8][8] = {"rnbqkbnr", "pppppppp", "        ", "        ", "   N    ", "        ", "PPPPPPPP", "R BQKBNR"};
    int step_n = 0;
    ASSERT_TRUE(test_handler_result(handler(board, parser("Nd4-d6", 0), NULL, &step_n), "Конь может ходить только буквой Г"));
    ASSERT_TRUE(test_handler_result(handler(board, parser("Nd4-f4", 0), NULL, &step_n), "Конь может ходить только буквой Г"));
    ASSERT_TRUE(test_handler_result(handler(board, parser("Nd4-e6", 0), NULL, &step_n), NULL));
    ASSERT_TRUE(test_handler_result(handler(board, parser("Nb8-c6", 0), NULL, &step_n), NULL));
}
CTEST(handler_suite, testB) {
    char board[8][8] = {"rnbqkbnr", "pppppppp", "        ", "        ", "   B    ", "        ", "PPPPPPPP", "RN QKBNR"};
    int step_n = 0;
    ASSERT_TRUE(test_handler_result(handler(board, parser("Bd4-d6", 0), NULL, &step_n), "Слон может перемещаться только ровно по диагонали"));
    ASSERT_TRUE(test_handler_result(handler(board, parser("Bd4-f4", 0), NULL, &step_n), "Слон может перемещаться только ровно по диагонали"));
    ASSERT_TRUE(test_handler_result(handler(board, parser("Bd4-b6", 0), NULL, &step_n), NULL));
    ASSERT_TRUE(test_handler_result(handler(board, parser("Bc8-a6", 0), NULL, &step_n), "Внезапно на пути встречена пешка при ходьбе слоном"));
}
CTEST(handler_suite, testQ) {
    char board[8][8] = {"rnbqkbnr", "pppppppp", "        ", "        ", "   Q    ", "        ", "PPPPPPPP", "RNB KBNR"};
    int step_n = 0;
    ASSERT_TRUE(test_handler_result(handler(board, parser("Qd4-e6", 0), NULL, &step_n), "Королева не может идти не по ровной диагонали"));
    ASSERT_TRUE(test_handler_result(handler(board, parser("Qd4xd8", 0), NULL, &step_n), "Внезапно на пути встречена пешка при ходьбе королевой"));
    ASSERT_TRUE(test_handler_result(handler(board, parser("Qd4xd7", 0), NULL, &step_n), NULL));
    ASSERT_TRUE(test_handler_result(handler(board, parser("Qd8-d6", 0), NULL, &step_n), "Внезапно на пути встречен королева при ходьбе королевой"));
    ASSERT_TRUE(test_handler_result(handler(board, parser("Qd8xd7", 0), NULL, &step_n), NULL));
}
CTEST(handler_suite, testK) {
    char board[8][8] = {"rnbq bnr", "pppppppp", "k       ", "    K   ", "        ", "        ", "PPPPPPPP", "RNBQ BNR"};
    int step_n = 0;
    ASSERT_TRUE(test_handler_result(handler(board, parser("Ke5xf7", 0), NULL, &step_n), "Король может ходить только по соседним 8 клеткам"));
    ASSERT_TRUE(test_handler_result(handler(board, parser("Ke5-f6", 0), NULL, &step_n), NULL));
    ASSERT_TRUE(test_handler_result(handler(board, parser("Ka6-b5", 0), NULL, &step_n), NULL));
}
