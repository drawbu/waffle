#ifndef UTILS_H_
    #define UTILS_H_

    #include <X11/Xlib.h>

typedef struct {
    int x;
    int y;
} vec_t;

static inline
void set_window_on_top(Display *display, Window window)
{
    XRaiseWindow(display, window);
    XFlush(display);
}

#endif /* !UTILS_H_ */
