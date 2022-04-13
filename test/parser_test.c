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

byte test_step_success(struct Step res, text chars) {
    if (res.err) {
        print_step(res);
        if (res.gen) free((void *) res.err);
        return 0;
    }
    byte test = res.x == chars[0] && res.y == chars[1] && res.x2 == chars[2] && res.y2 == chars[3] && res.figure == chars[4] && res.figure2 == chars[5] && res.type == chars[6];
    if (!test) print_step(res);
    return test;
}

byte test_step_fail(struct Step res, text str_error) {
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
    ASSERT_TRUE(test_step_success(parser("a1-b6", 0), "a1b6\0\0-"));
}

CTEST(parser_suite, success2) {
    ASSERT_TRUE(test_step_success(parser("a1-Xb6", 0), "a1b6\0\0-"));
}

CTEST(parser_suite, fail1) {
    ASSERT_TRUE(test_step_fail(parser("", 0), "Ожидался ввод клетки ходящей фигуры или её тип, но неожиданно встречен конец :/"));
}
CTEST(parser_suite, fail2) {
    ASSERT_TRUE(test_step_fail(parser("a1", 0), "Ожидался ввод типа хода, но неожиданно встречен конец :/"));
}
CTEST(parser_suite, fail3) {
    ASSERT_TRUE(test_step_fail(parser("a1-", 0), "Ожидался ввод клетки, куда походит фигура, но неожиданно встречен конец :/"));
}
CTEST(parser_suite, fail4) {
    ASSERT_TRUE(test_step_fail(parser("a1-b", 0), "Ожидался ввод клетки, куда походит фигура, но неожиданно встречен конец :/"));
}
