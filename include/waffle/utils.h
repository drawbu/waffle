#ifndef UTILS_H_
    #define UTILS_H_

    #include <X11/Xlib.h>

typedef struct {
    int x;
    int y;
    int width;
    int height;
} vec_t;

static inline
void set_window_on_top(Display *display, Window window)
{
    XRaiseWindow(display, window);
    XFlush(display);
}

#define TOP_LEFT (0)
#define TOP_RIGHT (1)
#define BOTTOM_LEFT (2)
#define BOTTOM_RIGHT (3)

static const vec_t DIRECTION_OFFSET[] = {
    [ TOP_LEFT ] =     { .x = 1, .y = 1, .width = -1, .height = -1 },
    [ TOP_RIGHT ] =    { .x = 0, .y = 1, .width = 1,  .height = -1 },
    [ BOTTOM_LEFT ] =  { .x = 1, .y = 0, .width = -1, .height = 1 },
    [ BOTTOM_RIGHT ] = { .x = 0, .y = 0, .width = 1,  .height = 1 },
};

#endif /* !UTILS_H_ */
