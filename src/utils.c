#include <X11/Xlib.h>

void set_window_on_top(Display *display, Window window)
{
    XRaiseWindow(display, window);
    XFlush(display);
}
