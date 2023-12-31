#ifndef QL_DEBUG_H
    #define QL_DEBUG_H

    #include <stdio.h>

    #include "colors.h"

    #ifdef DEBUG_MODE
        #define HEAD __FILE_NAME__, __LINE__

        #define HEAD_FMT_FILE BOLD BLUE "%s" RESET
        #define HEAD_FMT_LINE ":" BOLD PURPLE "%d" RESET

        #define HEAD_FMT HEAD_FMT_FILE HEAD_FMT_LINE "  "

        #define DEBUG_MSG(msg) printf(HEAD_FMT "%s\n", HEAD, msg)
        #define DEBUG(fmt, ...) printf(HEAD_FMT fmt "\n", HEAD, __VA_ARGS__)

        #define DEBUG_CALL(func, ...) func(__VA_ARGS__)

        #define DEBUG_USED
    #else
        #define DEBUG_MODE 0

        #define OMIT /* omitted */

        #define DEBUG_MSG(msg) OMIT
        #define DEBUG(fmt, ...) OMIT

        #define DEBUG_CALL(func, ...) OMIT
        #define DEBUG_USED __attribute__((unused))
    #endif

#endif
