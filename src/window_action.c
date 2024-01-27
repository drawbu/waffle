#include <stdio.h>

#include <X11/Xlib.h>

#include "debug.h"
#include "utils.h"
#include "wm.h"

void wm_map_windows(wm_t *wm)
{
    unsigned int count;
    Window root_return;
    Window parent_return;
    Window *windows = NULL;
    XWindowAttributes wa;

    if (!XQueryTree(wm->display, wm->root,
            &root_return, &parent_return, &windows, &count)
        || windows == NULL
    ) {
        printf("Failed to query windows tree\n");
        return;
    }
    DEBUG("%d windows found", count);
    for (unsigned int i = 0; i < count; i++) {
        Window window = windows[i];

        if (!XGetWindowAttributes(wm->display, window, &wa))
            continue;
        if (wa.map_state != IsViewable)
            continue;
        XSelectInput(
            wm->display, window,
            EnterWindowMask | LeaveWindowMask | SubstructureRedirectMask
        );
    }
    XFree(windows);
}

void set_window_on_top(Display *display, Window win)
{
    XRaiseWindow(display, win);
    XFlush(display);
}
