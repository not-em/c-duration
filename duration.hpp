
#ifndef DURATION_HPP
#define DURATION_HPP

#include <cctype>
#include <climits>
#include <cstdlib>
#include <optional>
#include <string>
#include <string_view>

// --- Configuration ---
#ifndef DURATION_YEAR_DAYS
#define DURATION_YEAR_DAYS 365
#endif

// --- Public API ---
// Unlike the C version, all functions here are inline — no DURATION_IMPLEMENTATION guard needed.
namespace duration {

long parse(std::string_view s);
std::string format(long seconds, bool human_readable = false);

// --- Implementation ---
namespace detail {

inline bool unit_matches(std::string_view token, std::string_view unit) {
    if (token.size() != unit.size()) return false;
    for (size_t i = 0; i < token.size(); i++) {
        if (std::tolower((unsigned char)token[i]) != std::tolower((unsigned char)unit[i]))
            return false;
    }
    return true;
}

inline std::optional<double> parse_unit(std::string_view token) {
    auto matches = [&](std::string_view u) { return unit_matches(token, u); };

    if (matches("s") || matches("sec") || matches("secs") || matches("second") || matches("seconds"))
        return 1.0;
    if (matches("m") || matches("min") || matches("mins") || matches("minute") || matches("minutes"))
        return 60.0;
    if (matches("h") || matches("hr") || matches("hrs") || matches("hour") || matches("hours"))
        return 3600.0;
    if (matches("d") || matches("day") || matches("days"))
        return 86400.0;
    if (matches("w") || matches("wk") || matches("wks") || matches("week") || matches("weeks"))
        return 604800.0;
    if (matches("y") || matches("yr") || matches("yrs") || matches("year") || matches("years"))
        return 86400.0 * DURATION_YEAR_DAYS;

    return std::nullopt;
}

inline long round_to_long(double value) {
    if (value >= (double)LONG_MAX) return LONG_MAX;
    if (value <= (double)LONG_MIN) return LONG_MIN;
    return value >= 0.0 ? (long)(value + 0.5) : (long)(value - 0.5);
}

// Advances s past the parsed number and returns its value, or nullopt on failure.
inline std::optional<double> parse_value(std::string_view &s) {
    size_t i = 0;
    bool saw_digit = false;

    if (i < s.size() && s[i] == '-') i++;

    while (i < s.size() && std::isdigit((unsigned char)s[i])) {
        saw_digit = true;
        i++;
    }

    if (i < s.size() && (s[i] == '.' || s[i] == ',')) {
        i++;
        while (i < s.size() && std::isdigit((unsigned char)s[i])) {
            saw_digit = true;
            i++;
        }
    }

    if (!saw_digit || i == 0 || i >= 64) return std::nullopt;

    char numbuf[64];
    for (size_t j = 0; j < i; j++)
        numbuf[j] = (s[j] == ',') ? '.' : s[j];
    numbuf[i] = '\0';

    char *end = nullptr;
    double parsed = std::strtod(numbuf, &end);
    if (end == numbuf || *end != '\0') return std::nullopt;

    s = s.substr(i);
    return parsed;
}

} // namespace detail

inline long parse(std::string_view s) {
    double total = 0.0;

    while (!s.empty()) {
        while (!s.empty() && s[0] == ' ') s = s.substr(1);
        if (s.empty()) break;

        auto value = detail::parse_value(s);
        if (!value) break;

        while (!s.empty() && s[0] == ' ') s = s.substr(1);

        size_t unit_len = 0;
        while (unit_len < s.size() && std::isalpha((unsigned char)s[unit_len]))
            unit_len++;

        if (unit_len == 0) break;

        auto multiplier = detail::parse_unit(s.substr(0, unit_len));
        if (!multiplier) break;

        total += *value * *multiplier;
        s = s.substr(unit_len);
    }

    return detail::round_to_long(total);
}

inline std::string format(long seconds, bool human_readable) {
    if (seconds <= 0)
        return human_readable ? "0 seconds" : "0s";

    static constexpr const char *short_units[] = {"y", "w", "d", "h", "m", "s"};
    static constexpr const char *long_units[]  = {"year", "week", "day", "hour", "minute", "second"};
    static constexpr long divisors[] = {
        86400L * DURATION_YEAR_DAYS,
        604800L,
        86400L,
        3600L,
        60L,
        1L
    };

    std::string result;
    for (int i = 0; i < 6; i++) {
        if (seconds >= divisors[i]) {
            long count = seconds / divisors[i];
            seconds -= count * divisors[i];

            if (human_readable && !result.empty()) result += ' ';
            result += std::to_string(count);

            if (human_readable) {
                result += ' ';
                result += long_units[i];
                if (count != 1) result += 's';
            } else {
                result += short_units[i];
            }
        }
    }

    return result.empty() ? (human_readable ? "0 seconds" : "0s") : result;
}

} // namespace duration

#endif // DURATION_HPP
