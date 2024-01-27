#include <stdio.h>

#include <X11/Xlib.h>

#include "colors.h"
#include "debug.h"
#include "wm.h"

DEBUG_USED
void debug_mouse_motion(wm_t *wm_state DEBUG_USED, bool mode)
{
    static bool save = true;

    if (save == mode)
        return;
    DEBUG("%sEvent%s: Mouse motion", YELLOW, RESET);
    save = mode;
    if (save) {
        DEBUG("%sEvent%s: Mouse released", YELLOW, RESET);
        return;
    }
    DEBUG("-> Mouse button: %s%d%s", BLUE, wm_state->mouse->button, RESET);
}

DEBUG_USED
void debug_win_rect(Display *display DEBUG_USED, Window win, bool mode)
{
    static bool save = true;
    XWindowAttributes attr;

    if (mode == save)
        return;
    save = mode;
    if (!save)
        return;
    XGetWindowAttributes(display, win, &attr);
    DEBUG(
        "New window Rect: [%d, %d, %d, %d]",
        attr.x, attr.y, attr.width, attr.height
    );
}
