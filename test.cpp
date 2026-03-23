#include "duration.hpp"
#include <cassert>
#include <cstdio>

int main() {
    using duration::parse;
    using duration::format;

    // test parse
    assert(parse("")        == 0);
    assert(parse("10x")    == 0);
    assert(parse("5s")     == 5);
    assert(parse("3m")     == 180);
    assert(parse("1h")     == 3600);
    assert(parse("1h30m")  == 5400);
    assert(parse("1d")     == 86400);
    assert(parse("1w")     == 604800);
    assert(parse("1h junk") == 3600);

    // spaces and decimal/comma separators
    assert(parse("  2h 15m ") == 8100);
    assert(parse("1.5h")      == 5400);
    assert(parse("2.5h")      == 9000);
    assert(parse("1.5d")      == 129600);
    assert(parse("1,5h")      == 5400);
    assert(parse("2,5minutes") == 150);
    assert(parse("1.5 hrs")   == 5400);
    assert(parse("2.5minutes") == 150);
    assert(parse("3.5 days")  == 302400);
    assert(parse("3,5 days")  == 302400);

    // test format (compact)
    assert(format(5)      == "5s");
    assert(format(180)    == "3m");
    assert(format(3600)   == "1h");
    assert(format(5400)   == "1h30m");
    assert(format(86400)  == "1d");
    assert(format(604800) == "1w");
    assert(format(0)      == "0s");

    // test format (human readable)
    assert(format(1,    true) == "1 second");
    assert(format(5400, true) == "1 hour 30 minutes");
    assert(format(0,    true) == "0 seconds");

    printf("All tests passed!\n");
    return 0;
}
