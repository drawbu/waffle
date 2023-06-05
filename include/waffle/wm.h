#ifndef WM_H_
    #define WM_H_

    #include <stdbool.h>
    #include <X11/Xlib.h>

    #include "debug.h"

typedef struct {
    bool dragging;
    unsigned int button;
    Window window;
    XWindowAttributes window_attr;
    XPoint start;
} mouse_mov_t;

typedef struct {
    mouse_mov_t *mouse;
    bool is_running;
    XEvent event;
} wm_state_t;

static inline
void setup_grab(Display *display, Window root_id)
{
    static int grab_event_pointer = (
        PointerMotionMask
        | ButtonPressMask
        | ButtonReleaseMask
        | EnterWindowMask
        | LeaveWindowMask
    );

    DEBUG("GEP: %d", grab_event_pointer);
    XGrabPointer(
        display, root_id, False,
        grab_event_pointer, GrabModeAsync, GrabModeAsync,
        None, None, CurrentTime
    );
    XGrabKeyboard(
        display, root_id, True,
        GrabModeAsync, GrabModeAsync, CurrentTime
    );
}

static inline
void remove_grab(Display *display)
{
    XUngrabPointer(display, CurrentTime);
    XUngrabKeyboard(display, CurrentTime);
}

#endif /* !WM_H_ */
