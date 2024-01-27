#include <stdio.h>

#include <X11/Xlib.h>

#include "debug.h"
#include "utils.h"

void map_windows(Display *display, Window root)
{
    unsigned int count;
    Window root_return;
    Window parent_return;
    Window *windows = NULL;
    XWindowAttributes wa;

    if (!XQueryTree(display, root, &root_return, &parent_return, &windows, &count)
        || windows == NULL
    ) {
        printf("Failed to query windows tree\n");
        return;
    }
    DEBUG("%d windows found", count);
    for (unsigned int i = 0; i < count; i++) {
        Window window = windows[i];

        if (!XGetWindowAttributes(display, window, &wa))
            continue;
        if (wa.map_state != IsViewable)
            continue;
        XSelectInput(
            display, window,
            EnterWindowMask | LeaveWindowMask | SubstructureRedirectMask
        );
    }
    XFree(windows);
}

void set_window_on_top(Display *display, Window window)
{
    XRaiseWindow(display, window);
    XFlush(display);
}
