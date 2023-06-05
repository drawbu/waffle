#include <X11/cursorfont.h>
#include "waffle/events.h"
#include "waffle/wm.h"

void handle_enter(wm_state_t *wm_state)
{
    static Cursor default_cursor = 0;

    DEBUG("%sEvent%s: Enter", YELLOW, RESET);
    if (!default_cursor)
        default_cursor = XCreateFontCursor(
                wm_state->event.xbutton.display, XC_gumby);
    XDefineCursor(
        wm_state->event.xbutton.display,
        wm_state->event.xcrossing.window,
        default_cursor
    );
    XWarpPointer(
        wm_state->event.xbutton.display, None,
        wm_state->event.xbutton.root, 0, 0, 0, 0,
        wm_state->mouse->start.x,
        wm_state->mouse->start.y
    );
    XFlush(wm_state->event.xbutton.display);
}

void handle_leave(wm_state_t *wm_state)
{
    static Cursor default_cursor = 0;

    DEBUG("%sEvent%s: Leave", YELLOW, RESET);
    if (!default_cursor)
        default_cursor = XCreateFontCursor(
            wm_state->event.xbutton.display, XC_boat);
    XDefineCursor(
        wm_state->event.xbutton.display,
        wm_state->event.xcrossing.window,
        default_cursor
    );
}
