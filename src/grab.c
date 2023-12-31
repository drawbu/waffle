#include <X11/Xlib.h>
#include <X11/keysym.h>

#include "debug.h"
#include "waffle/utils.h"
#include "waffle/wm.h"

void setup_grab(Display *display, Window root_id)
{
    static unsigned int grab_event_pointer = (
        PointerMotionMask
        | ButtonPressMask
        | ButtonReleaseMask
    );

    DEBUG("GEP: %d", grab_event_pointer);
    XSelectInput(
        display, root_id,
        EnterWindowMask | LeaveWindowMask| SubstructureRedirectMask
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

void remove_grab(Display *display, Window root_id)
{
    XUngrabButton(display, AnyButton, AnyModifier, root_id);
    XUngrabKeyboard(display, CurrentTime);
}

