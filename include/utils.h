#ifndef UTILS_H_
    #define UTILS_H_

    #include <X11/Xlib.h>

    #define ATTR(key) __attribute__((key))

typedef struct {
    int x;
    int y;
    int width;
    int height;
} vec_t;

typedef enum {
    TOP_LEFT,
    TOP_RIGHT,
    BOTTOM_LEFT,
    BOTTOM_RIGHT,
    DIRECTION_COUNT
} direction_t;

#endif /* !UTILS_H_ */
