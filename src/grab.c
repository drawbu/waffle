#include <X11/Xlib.h>
#include <X11/keysym.h>

#include "debug.h"
#include "utils.h"
#include "wm.h"

static char SELECT_INPUT_ERRORED = 0;

static
int hook_bad_access(
    Display *display __attribute__((unused)), XErrorEvent *error)
{
    if (error->error_code == BadAccess)
        SELECT_INPUT_ERRORED = 1;
    return 0;
}

static
int attempt_input_selection_setup(Display *display, Window root_id)
{
    XSetErrorHandler(hook_bad_access);
    XSelectInput(display, root_id,
        EnterWindowMask | LeaveWindowMask| SubstructureRedirectMask);
    XSync(display, false);
    XSetErrorHandler(NULL);

    if (SELECT_INPUT_ERRORED) {
        fprintf(stderr,
            "Another Window Manager is running on this display [%s]\n"
            "Cannot continue, exiting...\n",
            XDisplayName(NULL));
        return -1;
    }

    return 0;
}

int setup_grab(Display *display, Window root_id)
{
    static unsigned int grab_event_pointer = (
        PointerMotionMask
        | ButtonPressMask
        | ButtonReleaseMask
    );

    DEBUG("GEP: %d", grab_event_pointer);
    if (attempt_input_selection_setup(display, root_id) < 0)
        return -1;
    XGrabButton(
        display, Button1Mask | Button3Mask, Mod4Mask,
        root_id, False, grab_event_pointer,
        GrabModeAsync, GrabModeSync, None, None);
    XGrabKeyboard(display, root_id, True,
        GrabModeAsync, GrabModeAsync, CurrentTime);
    return 0;
}

void remove_grab(Display *display, Window root_id)
{
    XUngrabButton(display, AnyButton, AnyModifier, root_id);
    XUngrabKeyboard(display, CurrentTime);
}

