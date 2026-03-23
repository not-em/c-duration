# c-duration

A small single-header library to parse and format durations. Available in both C (`duration.h`) and C++ (`duration.hpp`).

## Features

- Parse compact duration strings like `1h30m`.
- Parse decimal values (dot or comma) and flexible unit names like `1.5 hrs`, `2,5minutes`, `3.5 days`.
- Format seconds into compact (`1h30m`) or human-readable (`1 hour 30 minutes`) output.
- Simple compile-time configuration for year length.

## Supported Units

- `s` second
- `m` minute
- `h` hour
- `d` day
- `w` week
- `y` year (`DURATION_YEAR_DAYS`, default `365`)

Aliases are also accepted:

- seconds: `sec`, `secs`, `second`, `seconds`
- minutes: `min`, `mins`, `minute`, `minutes`
- hours: `hr`, `hrs`, `hour`, `hours`
- days: `day`, `days`
- weeks: `wk`, `wks`, `week`, `weeks`
- years: `yr`, `yrs`, `year`, `years`

## Quick Start

### C

```c
#define DURATION_IMPLEMENTATION
#include "duration.h"

#include <stdio.h>

int main(void) {
	char buf[64];
	long seconds = duration_parse("1h30m");

	duration_format(seconds, buf, sizeof(buf), false);
	printf("compact: %s\n", buf); // 1h30m

	duration_format(seconds, buf, sizeof(buf), true);
	printf("human: %s\n", buf);   // 1 hour 30 minutes

	return 0;
}
```

### C++

```cpp
#include "duration.hpp"
#include <iostream>

int main() {
	long seconds = duration::parse("1h30m");

	std::cout << duration::format(seconds)       << "\n"; // 1h30m
	std::cout << duration::format(seconds, true) << "\n"; // 1 hour 30 minutes
}
```

The C++ header is fully self-contained — no `#define` guard needed. All functions are in the `duration` namespace and return `std::string`.

## API

### C (`duration.h`)

```c
long duration_parse(const char *s);
void duration_format(long seconds, char *buf, int bufsize, bool human_readable);
```

### C++ (`duration.hpp`)

```cpp
namespace duration {
    long        parse (std::string_view s);
    std::string format(long seconds, bool human_readable = false);
}
```

### parse / duration_parse

- Returns total seconds parsed from the input string.
- Supports integer and decimal values with `.` or `,` (for example: `1.5h`, `1,5h`, `2.5minutes`).
- Parses left-to-right and stops at the first invalid token.
- Example: `"1h junk"` returns `3600`.
- Whitespace between chunks is allowed.
- C version returns `0` when passed `NULL`.

### format / duration_format

- If `human_readable` is `false`, output is compact (`1h30m`).
- If `human_readable` is `true`, output uses words and spacing (`1 hour 30 minutes`).
- If `seconds <= 0`, returns `"0s"` (compact) or `"0 seconds"` (human readable).
- C version: writes into `buf`; returns immediately if `buf == NULL` or `bufsize <= 0`.

## Configuration

Override year length before including either header:

```c
#define DURATION_YEAR_DAYS 360
```

## Build and Test

```sh
make test      # C only
make test_cpp  # C++ only
make all       # both
```