#define DURATION_IMPLEMENTATION
#include "duration.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

int main() {
    char buf[64];

    // test duration_parse
    assert(duration_parse(NULL)    == 0);
    assert(duration_parse("")      == 0);
    assert(duration_parse("10x")   == 0);
    assert(duration_parse("5s")    == 5);
    assert(duration_parse("3m")    == 180);
    assert(duration_parse("1h")    == 3600);
    assert(duration_parse("1h30m") == 5400);
    assert(duration_parse("1d")    == 86400);
    assert(duration_parse("1w")    == 604800);
    assert(duration_parse("1h junk") == 3600);

    // tests for duration_parse with spaces and decimal points
    assert(duration_parse("  2h 15m ") == 8100);
    assert(duration_parse("1.5h") == 5400);
    assert(duration_parse("2.5h") == 9000);
    assert(duration_parse("1.5d") == 129600);
    assert(duration_parse("1,5h") == 5400);
    assert(duration_parse("2,5minutes") == 150);
    assert(duration_parse("1.5 hrs") == 5400);
    assert(duration_parse("2.5minutes") == 150);
    assert(duration_parse("3.5 days") == 302400);
    assert(duration_parse("3,5 days") == 302400);


    // test duration_format
    duration_format(5, buf, sizeof(buf), false);
    assert(strcmp(buf, "5s") == 0);

    duration_format(180, buf, sizeof(buf), false);
    assert(strcmp(buf, "3m") == 0);

    duration_format(3600, buf, sizeof(buf), false);
    assert(strcmp(buf, "1h") == 0);

    duration_format(5400, buf, sizeof(buf), false);
    assert(strcmp(buf, "1h30m") == 0);

    duration_format(86400, buf, sizeof(buf), false);
    assert(strcmp(buf, "1d") == 0);

    duration_format(604800, buf, sizeof(buf), false);
    assert(strcmp(buf, "1w") == 0);

    // test duration_format (human readable)
    duration_format(1, buf, sizeof(buf), true);
    assert(strcmp(buf, "1 second") == 0);

    duration_format(5400, buf, sizeof(buf), true);
    assert(strcmp(buf, "1 hour 30 minutes") == 0);

    duration_format(0, buf, sizeof(buf), true);
    assert(strcmp(buf, "0 seconds") == 0);

    // smoke test guards against invalid output buffers
    duration_format(1, NULL, 0, false);
    duration_format(1, buf, 0, false);

    printf("All tests passed!\n");
    return 0;
}