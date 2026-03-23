# c-duration

A small single-header C library to parse and format durations.

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

## API

```c
long duration_parse(const char *s);
void duration_format(long seconds, char *buf, int bufsize, bool human_readable);
```

### duration_parse

- Returns total seconds parsed from `s`.
- Returns `0` when `s == NULL`.
- Supports integer and decimal values with `.` or `,` (for example: `1.5h`, `1,5h`, `2.5minutes`).
- Parses left-to-right and stops at the first invalid token.
- Example: `"1h junk"` returns `3600`.
- Whitespace between chunks is allowed.

### duration_format

- Writes formatted output into `buf`.
- If `human_readable == false`, output is compact (`1h30m`).
- If `human_readable == true`, output uses words and spacing (`1 hour 30 minutes`).
- If `seconds <= 0`, output is `0s` (compact) or `0 seconds` (human readable).
- If `buf == NULL` or `bufsize <= 0`, the function returns immediately.

## Configuration

You can override year length before including the header:

```c
#define DURATION_YEAR_DAYS 360
#define DURATION_IMPLEMENTATION
#include "duration.h"
```

## Build and Test

```sh
make test
```