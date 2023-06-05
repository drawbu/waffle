#ifndef UTILS_H_
    #define UTILS_H_

    #include <X11/Xlib.h>

static inline
void set_window_on_top(Display *display, Window window)
{
    XRaiseWindow(display, window);
    XFlush(display);
}

#endif /* !UTILS_H_ */
