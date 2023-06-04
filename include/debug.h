#ifndef DEBUG_MODE_H
    #define DEBUG_MODE_H

    #define NOTHING /* Nothing */

    #ifdef DEBUG_MODE

        #include <stdio.h>
        #include "colors.h"

        #define HEAD_FMT BOLD BLUE "%s" RESET ":" PURPLE BOLD "%d" RESET ": "
        #define COLORED(s) HEAD_FMT s "\n"

static const int START = sizeof (HEAD_FMT) - 1;

        #define HEAD __FILE_NAME__, __LINE__
        #define DEBUG(fmt, ...) (printf(COLORED(fmt), HEAD, __VA_ARGS__))
        #define DEBUG_MSG(string) (printf(COLORED(string), HEAD))

        #define DEBUG_CALL(func, ...) func(__VA_ARGS__)

        #define DEBUG_IF(cond, fmt, ...) ((cond) ? DEBUG(fmt, __VA_ARGS__) : 0)
        #define DEBUG_MSG_IF(cond, fmt) ((cond) ? DEBUG_MSG(fmt) : 0)

        #define DEBUG_USED NOTHING
        #define DEBUG_EXPR(...) __VA_ARGS__

    #else
        #define DEBUG(...) NOTHING
        #define DEBUG_MSG(...) NOTHING

        #define DEBUG_CALL(...) NOTHING
        #define DEBUG_IF(...) NOTHING
        #define DEBUG_MSG_IF(...) NOTHING

        #define DEBUG_USED __attribute__((unused))
        #define DEBUG_EXPR(...) /* nop */
    #endif

#endif /* !DEBUG_MODE_H */
