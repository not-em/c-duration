

#ifndef DURATION_H
#define DURATION_H

// --- Configuration ---
#ifndef DURATION_YEAR_DAYS
#define DURATION_YEAR_DAYS 365
#endif

// --- Declarations ---
long duration_parse(const char *s);
void duration_format(long seconds, char *buf, int bufsize);

// --- Implementation ---
#ifdef DURATION_IMPLEMENTATION

long duration_parse(const char *s) {
    long total = 0;

    while (*s != '\0') {
        while (*s == ' ') s++;
        if (*s == '\0') break;

        char *end;
        long value = strtol(s, &end, 10);
        if (end == s) break;
        s = end;

        while (*s == ' ') s++;

        if      (*s == 's') { total += value;                    s++; }
        else if (*s == 'm') { total += value * 60;               s++; }
        else if (*s == 'h') { total += value * 3600;             s++; }
        else if (*s == 'd') { total += value * 86400;            s++; }
        else if (*s == 'w') { total += value * 604800;           s++; }
        else if (*s == 'y') { total += value * 86400 *
                              DURATION_YEAR_DAYS;                s++; }
        else break;
    }

    return total;
}

void duration_format(long seconds, char *buf, int bufsize, bool human_readable) {
    // if human_readable is true, use "year", "week", etc. instead of "y", "w", etc.
    const char *units[] = {
        human_readable ? "year" : "y",
        human_readable ? "week" : "w",
        human_readable ? "day" : "d",
        human_readable ? "hour" : "h",
        human_readable ? "minute" : "m",
        human_readable ? "second" : "s"
    };
    long values[] = {
        86400 * DURATION_YEAR_DAYS,
        604800,
        86400,
        3600,
        60,
        1
    };

    int pos = 0;
    for (int i = 0; i < 6; i++) {
        if (seconds >= values[i]) {
            long count = seconds / values[i];
            seconds -= count * values[i];
            pos += snprintf(buf + pos, bufsize - pos, "%ld%s", count, units[i]);
            if (pos >= bufsize) break;
        }
    }

    if (pos == 0) {
        snprintf(buf, bufsize, "0s");
    }
}

#endif // DURATION_IMPLEMENTATION
#endif // DURATION_H