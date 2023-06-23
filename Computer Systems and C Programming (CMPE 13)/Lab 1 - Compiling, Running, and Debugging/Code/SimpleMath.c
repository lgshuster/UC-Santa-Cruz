#include "BOARD.h"
#include "SimpleMath.h"

int Bigger(int x, int y);

int Bigger(int x, int y) {
    if (x > y) {
        return x;
    } else if (x < y) {
        return y;
    } else {
        return x;
    }

}

int AddTwoNumbers(int x, int y) {
    return x + y;
}

int IncrementByOne(int x) {
    return AddTwoNumbers(x, 1);
}

