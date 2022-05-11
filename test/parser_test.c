#include <ctest.h>
#include <input_parser.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_step(struct Step res) {
    printf("\n");
    if (res.err)
        printf("Ошибка: %s\n", res.err);
    else {
        printf("Ходящая фигура: %c %c%c\n", res.figure, res.x, res.y);
        printf("Куда топает: %c %c%c\n", res.figure2, res.x2, res.y2);
        printf("Тип хода: %c\n", res.type);
    }
}

byte test_success_step(struct Step res, text chars) {
    if (res.err) {
        print_step(res);
        if (res.gen) free((void *) res.err);
        return 0;
    }
    byte test = res.x == chars[0] && res.y == chars[1] && res.x2 == chars[2] && res.y2 == chars[3] && res.figure == chars[4] && res.figure2 == chars[5] && res.type == chars[6];
    if (!test) print_step(res);
    return test;
}

byte test_fail_step(struct Step res, text str_error) {
    if (res.err) {
        byte test = strcmp(res.err, str_error) == 0;
        if (!test) print_step(res);
        if (res.gen) free((void *) res.err);
        return test;
    }
    print_step(res);
    return 0;
}

CTEST(parser_suite, success1) {
    ASSERT_TRUE(test_success_step(parser("a1-b6", 0), "a1b6\0\0-"));
}
CTEST(parser_suite, success2) {
    ASSERT_TRUE(test_success_step(parser("e2-a1", 0), "e2a1\0\0-"));
}
CTEST(parser_suite, success3) {
    ASSERT_TRUE(test_success_step(parser("f7xc2", 0), "f7c2\0\0x"));
}
CTEST(parser_suite, success4) {
    ASSERT_TRUE(test_success_step(parser("h6-d8", 0), "h6d8\0\0-"));
}
CTEST(parser_suite, success5) {
    ASSERT_TRUE(test_success_step(parser("g8xh3", 0), "g8h3\0\0x"));
}
CTEST(parser_suite, success6) {
    ASSERT_TRUE(test_success_step(parser("d3-f4", 0), "d3f4\0\0-"));
}
CTEST(parser_suite, success7) {
    ASSERT_TRUE(test_success_step(parser("c4xe7", 0), "c4e7\0\0x"));
}
CTEST(parser_suite, success8) {
    ASSERT_TRUE(test_success_step(parser("b5xg5", 0), "b5g5\0\0x"));
}
CTEST(parser_suite, success9) {
    ASSERT_TRUE(test_success_step(parser("Kb5xg5", 0), "b5g5K\0x"));
}
CTEST(parser_suite, success10) {
    ASSERT_TRUE(test_success_step(parser("b5xg5K", 0), "b5g5\0Kx"));
}
CTEST(parser_suite, success11) {
    ASSERT_TRUE(test_success_step(parser("Kb5xg5B", 0), "b5g5KBx"));
}
CTEST(parser_suite, success12) {
    ASSERT_TRUE(test_success_step(parser("Nb5xg5R", 0), "b5g5NRx"));
}
CTEST(parser_suite, success13) {
    ASSERT_TRUE(test_success_step(parser("Pb5-g5Q", 0), "b5g5PQ-"));
}
CTEST(parser_suite, success14) {
    ASSERT_TRUE(test_success_step(parser("Pb5-g5+", 0), "b5g5P+-"));
}
CTEST(parser_suite, success15) {
    ASSERT_TRUE(test_success_step(parser("Pb5-g5#", 0), "b5g5P#-"));
}
CTEST(parser_suite, success16) {
    ASSERT_TRUE(test_success_step(parser("Qa1xh8++", 0), "a1h8Q+x"));
}

CTEST(parser_suite, fail1) {
    ASSERT_TRUE(test_fail_step(parser("", 0), "Ожидался ввод клетки ходящей фигуры или её тип, но неожиданно встречен конец :/"));
}
CTEST(parser_suite, fail2) {
    ASSERT_TRUE(test_fail_step(parser("a1", 0), "Ожидался ввод типа хода, но неожиданно встречен конец :/"));
}
CTEST(parser_suite, fail3) {
    ASSERT_TRUE(test_fail_step(parser("a1-", 0), "Ожидался ввод клетки, куда походит фигура, но неожиданно встречен конец :/"));
}
CTEST(parser_suite, fail4) {
    ASSERT_TRUE(test_fail_step(parser("a1-b", 0), "Ожидался ввод клетки, куда походит фигура, но неожиданно встречен конец :/"));
}
CTEST(parser_suite, fail5) {
    ASSERT_TRUE(test_fail_step(parser("aa", 0), "Вы уже ввели букву клетки ходящей фигуры или её тип"));
}
CTEST(parser_suite, fail6) {
    ASSERT_TRUE(test_fail_step(parser("h8-hh", 0), "Вы уже ввели букву клетки, куда ходит фигура"));
}
CTEST(parser_suite, fail7) {
    ASSERT_TRUE(test_fail_step(parser("1", 0), "Вы ещё не ввели букву клетки ходящей фигуры или её тип"));
}
CTEST(parser_suite, fail8) {
    ASSERT_TRUE(test_fail_step(parser("h8-1", 0), "Вы ещё не ввели букву клетки, куда ходит фигура"));
}
CTEST(parser_suite, fail9) {
    ASSERT_TRUE(test_fail_step(parser("h8-hK", 0), "Ожидался ввод клетки, куда походит фигура, а было введено: 'K'"));
}
CTEST(parser_suite, fail10) {
    ASSERT_TRUE(test_fail_step(parser("h8g", 0), "Ожидался ввод типа хода, а было введено: 'g'"));
}
CTEST(parser_suite, fail11) {
    ASSERT_TRUE(test_fail_step(parser("a8-b8g", 0), "Ожидался ввод типа съедаемой фигуры или шах или мат или en passant, а было введено: 'g'"));
}
CTEST(parser_suite, fail12) {
    ASSERT_TRUE(test_fail_step(parser("a8-b8/", 0), "Ожидался ввод типа съедаемой фигуры или шах или мат или en passant, а было введено: '/'"));
}
CTEST(parser_suite, fail13) {
    ASSERT_TRUE(test_fail_step(parser("a76", 0), "Ожидался ввод типа хода, а было введено: '6'"));
}
CTEST(parser_suite, fail14) {
    ASSERT_TRUE(test_fail_step(parser("a7-b65", 0), "Ожидался ввод типа съедаемой фигуры или шах или мат или en passant, а было введено: '5'"));
}
CTEST(parser_suite, fail15) {
    ASSERT_TRUE(test_fail_step(parser("h8-g7-", 0), "Ожидался ввод типа съедаемой фигуры или шах или мат или en passant, а было введено: '-'"));
}
CTEST(parser_suite, fail16) {
    ASSERT_TRUE(test_fail_step(parser("h8-*", 0), "Ожидался ввод клетки, куда походит фигура, а было введено: '*'"));
}
CTEST(parser_suite, fail17) {
    ASSERT_TRUE(test_fail_step(parser("a#", 0), "Ожидался ввод клетки ходящей фигуры, а было введено: '#'"));
}
CTEST(parser_suite, fail18) {
    ASSERT_TRUE(test_fail_step(parser("a6+a6", 0), "Ожидался ввод типа хода, а было введено: '+'"));
}
CTEST(parser_suite, fail19) {
    ASSERT_TRUE(test_fail_step(parser("h8*", 0), "Ожидался ввод типа хода, а было введено: '*'"));
}
CTEST(parser_suite, fail20) {
    ASSERT_TRUE(test_fail_step(parser("z1-a1", 0), "Символ 'z' за пределами доски"));
}
CTEST(parser_suite, fail21) {
    ASSERT_TRUE(test_fail_step(parser("a9-a1", 0), "Символ '9' за пределами доски"));
}
CTEST(parser_suite, fail22) {
    ASSERT_TRUE(test_fail_step(parser("a1-t1", 0), "Символ 't' за пределами доски"));
}
CTEST(parser_suite, fail23) {
    ASSERT_TRUE(test_fail_step(parser("a1-a0", 0), "Символ '0' за пределами доски"));
}
CTEST(parser_suite, fail24) {
    ASSERT_TRUE(test_fail_step(parser("a1-a1y", 0), "Символ 'y' за пределами доски"));
}
