#ifndef WM_H_
    #define WM_H_

    #include <stdbool.h>
    
    #include <X11/X.h>
    #include <X11/Xlib.h>
    #include <X11/keysym.h>
    
    #include "debug.h"
    #include "utils.h"

typedef struct {
    bool dragging;
    unsigned int button;
    direction_t side;
    Window window;
    XWindowAttributes window_attr;
    XPoint start;
} mouse_mov_t;

typedef struct {
    mouse_mov_t *mouse;
    bool is_running;
    XEvent event;
    Window focused_window;
} wm_state_t;

static inline
void setup_grab(Display *display, Window root_id)
{
    static int grab_event_pointer = (
        PointerMotionMask
        | ButtonPressMask
        | ButtonReleaseMask
    );

    DEBUG("GEP: %d", grab_event_pointer);
    XSelectInput(
        display, root_id,
        EnterWindowMask
        | LeaveWindowMask
        | SubstructureRedirectMask
        | SubstructureNotifyMask
    );
    XGrabButton(
        display, Button1Mask | Button3Mask, Mod4Mask,
        root_id, False, grab_event_pointer,
        GrabModeAsync, GrabModeSync, None, None
    );
    XGrabKeyboard(
        display, root_id, True,
        GrabModeAsync, GrabModeAsync, CurrentTime
    );
}

static inline
void remove_grab(Display *display, Window root_id)
{
    XUngrabButton(display, AnyButton, AnyModifier, root_id);
    XUngrabKeyboard(display, CurrentTime);
}

#endif /* !WM_H_ */
