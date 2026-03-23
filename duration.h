

#ifndef DURATION_H
#define DURATION_H

#include <stdbool.h>
#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Configuration ---
#ifndef DURATION_YEAR_DAYS
#define DURATION_YEAR_DAYS 365
#endif

// --- Declarations ---
long duration_parse(const char *s);
void duration_format(long seconds, char *buf, int bufsize, bool human_readable);

// --- Implementation ---
#ifdef DURATION_IMPLEMENTATION

static bool duration_unit_matches(const char *token, size_t token_len, const char *unit) {
    size_t i = 0;

    while (unit[i] != '\0') {
        if (i >= token_len) return false;
        if (tolower((unsigned char)token[i]) != tolower((unsigned char)unit[i])) return false;
        i++;
    }

    return i == token_len;
}

static bool duration_parse_unit(const char *token, size_t token_len, double *multiplier) {
    if (duration_unit_matches(token, token_len, "s") ||
        duration_unit_matches(token, token_len, "sec") ||
        duration_unit_matches(token, token_len, "secs") ||
        duration_unit_matches(token, token_len, "second") ||
        duration_unit_matches(token, token_len, "seconds")) {
        *multiplier = 1.0;
        return true;
    }

    if (duration_unit_matches(token, token_len, "m") ||
        duration_unit_matches(token, token_len, "min") ||
        duration_unit_matches(token, token_len, "mins") ||
        duration_unit_matches(token, token_len, "minute") ||
        duration_unit_matches(token, token_len, "minutes")) {
        *multiplier = 60.0;
        return true;
    }

    if (duration_unit_matches(token, token_len, "h") ||
        duration_unit_matches(token, token_len, "hr") ||
        duration_unit_matches(token, token_len, "hrs") ||
        duration_unit_matches(token, token_len, "hour") ||
        duration_unit_matches(token, token_len, "hours")) {
        *multiplier = 3600.0;
        return true;
    }

    if (duration_unit_matches(token, token_len, "d") ||
        duration_unit_matches(token, token_len, "day") ||
        duration_unit_matches(token, token_len, "days")) {
        *multiplier = 86400.0;
        return true;
    }

    if (duration_unit_matches(token, token_len, "w") ||
        duration_unit_matches(token, token_len, "wk") ||
        duration_unit_matches(token, token_len, "wks") ||
        duration_unit_matches(token, token_len, "week") ||
        duration_unit_matches(token, token_len, "weeks")) {
        *multiplier = 604800.0;
        return true;
    }

    if (duration_unit_matches(token, token_len, "y") ||
        duration_unit_matches(token, token_len, "yr") ||
        duration_unit_matches(token, token_len, "yrs") ||
        duration_unit_matches(token, token_len, "year") ||
        duration_unit_matches(token, token_len, "years")) {
        *multiplier = 86400.0 * DURATION_YEAR_DAYS;
        return true;
    }

    return false;
}

static long duration_round_to_long(double value) {
    if (value >= (double)LONG_MAX) return LONG_MAX;
    if (value <= (double)LONG_MIN) return LONG_MIN;

    if (value >= 0.0) return (long)(value + 0.5);
    return (long)(value - 0.5);
}

static bool duration_parse_value(const char **input, double *value_out) {
    const char *s = *input;
    const char *start = s;
    bool saw_digit = false;

    if (*s == '-') s++;

    while (isdigit((unsigned char)*s)) {
        saw_digit = true;
        s++;
    }

    if (*s == '.' || *s == ',') {
        s++;
        while (isdigit((unsigned char)*s)) {
            saw_digit = true;
            s++;
        }
    }

    if (!saw_digit) return false;

    size_t len = (size_t)(s - start);
    if (len == 0 || len >= 64) return false;

    char numbuf[64];
    for (size_t i = 0; i < len; i++) {
        numbuf[i] = (start[i] == ',') ? '.' : start[i];
    }
    numbuf[len] = '\0';

    char *end = NULL;
    double parsed = strtod(numbuf, &end);
    if (end == numbuf || *end != '\0') return false;

    *value_out = parsed;
    *input = s;
    return true;
}

long duration_parse(const char *s) {
    if (s == NULL) return 0;

    double total = 0.0;

    while (*s != '\0') {
        while (*s == ' ') s++;
        if (*s == '\0') break;

        double value;
        if (!duration_parse_value(&s, &value)) break;

        while (*s == ' ') s++;

        const char *unit_start = s;
        size_t unit_len = 0;
        double multiplier = 0.0;

        while (isalpha((unsigned char)*s)) {
            s++;
            unit_len++;
        }

        if (unit_len == 0) break;
        if (!duration_parse_unit(unit_start, unit_len, &multiplier)) break;

        total += value * multiplier;

        while (*s == ' ') s++;
    }

    return duration_round_to_long(total);
}

void duration_format(long seconds, char *buf, int bufsize, bool human_readable) {
    if (buf == NULL || bufsize <= 0) return;

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

    if (seconds <= 0) {
        snprintf(buf, bufsize, human_readable ? "0 seconds" : "0s");
        return;
    }

    int pos = 0;
    buf[0] = '\0';

    for (int i = 0; i < 6; i++) {
        if (seconds >= values[i]) {
            long count = seconds / values[i];
            int written;

            seconds -= count * values[i];

            if (human_readable) {
                written = snprintf(
                    buf + pos,
                    bufsize - pos,
                    "%s%ld %s%s",
                    pos > 0 ? " " : "",
                    count,
                    units[i],
                    count == 1 ? "" : "s"
                );
            } else {
                written = snprintf(buf + pos, bufsize - pos, "%ld%s", count, units[i]);
            }

            if (written < 0) {
                buf[0] = '\0';
                return;
            }

            if (written >= bufsize - pos) {
                pos = bufsize - 1;
                break;
            }

            pos += written;
        }
    }

    if (pos == 0) {
        snprintf(buf, bufsize, human_readable ? "0 seconds" : "0s");
    }
}

#endif // DURATION_IMPLEMENTATION
#endif // DURATION_H