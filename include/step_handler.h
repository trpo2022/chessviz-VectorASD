#include <input_parser.h>
#include <vector.h>

struct HandlerError {
    text str;
    byte generated;
};

text caserson(char s, char p);
struct HandlerError handler(char board[8][8], struct Step step, struct Vector *history, int *step_n);
