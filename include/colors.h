#ifndef COLORS_H_
    #define COLORS_H_

    #include <stddef.h>

    #define COL_FMT(n) "\e[" #n "m"

    #define RESET COL_FMT(0)
    #define BOLD COL_FMT(1)

    #define RED COL_FMT(31)
    #define GREEN COL_FMT(32)
    #define YELLOW COL_FMT(33)
    #define BLUE COL_FMT(34)
    #define PURPLE COL_FMT(35)
    #define CYAN COL_FMT(36)

static const size_t COLORS_STRING_SIZE = sizeof (BLUE) - 1;
static const size_t RESET_STRING_SIZE = sizeof (RESET) - 1;

#endif /* !COLORS_H_ */
