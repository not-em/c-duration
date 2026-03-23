#define DURATION_IMPLEMENTATION
#include "duration.h"
#include <stdio.h>
#include <assert.h>

int main() {
    assert(duration_parse("5s")    == 5);
    assert(duration_parse("3m")    == 180);
    assert(duration_parse("1h")    == 3600);
    assert(duration_parse("1h30m") == 5400);
    assert(duration_parse("1d")    == 86400);
    assert(duration_parse("1w")    == 604800);

    printf("All tests passed!\n");
    return 0;
}