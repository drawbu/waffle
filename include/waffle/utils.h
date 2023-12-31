#ifndef UTILS_H_
    #define UTILS_H_

    #include <X11/Xlib.h>

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

void map_windows(Display *display, Window root);
void set_window_on_top(Display *display, Window window);

static const vec_t DIRECTION_OFFSET[DIRECTION_COUNT] = {
    [ TOP_LEFT ] = { .x = 1, .y = 1, .width = -1, .height = -1 },
    [ TOP_RIGHT ] = { .x = 0, .y = 1, .width = 1, .height = -1 },
    [ BOTTOM_LEFT ] = { .x = 1, .y = 0, .width = -1, .height = 1 },
    [ BOTTOM_RIGHT ] = { .x = 0, .y = 0, .width = 1, .height = 1 },
};


#endif /* !UTILS_H_ */
