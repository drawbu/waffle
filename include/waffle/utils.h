#ifndef UTILS_H_
    #define UTILS_H_

    #include <stdio.h>
    #include <X11/Xlib.h>
    #include "debug.h"

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

static inline
void set_window_on_top(Display *display, Window window)
{
    XRaiseWindow(display, window);
    XFlush(display);
}

static const vec_t DIRECTION_OFFSET[DIRECTION_COUNT] = {
    [ TOP_LEFT ] = { .x = 1, .y = 1, .width = -1, .height = -1 },
    [ TOP_RIGHT ] = { .x = 0, .y = 1, .width = 1, .height = -1 },
    [ BOTTOM_LEFT ] = { .x = 1, .y = 0, .width = -1, .height = 1 },
    [ BOTTOM_RIGHT ] = { .x = 0, .y = 0, .width = 1, .height = 1 },
};

static inline
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
            EnterWindowMask 
            | LeaveWindowMask
            | SubstructureNotifyMask
            | SubstructureRedirectMask
        );
    }
    XFree(windows);
}

#endif /* !UTILS_H_ */
