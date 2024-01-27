#include "debug.h"
#include "waffle/events.h"

void handle_configure_request(wm_state_t *wm_state)
{
    XConfigureRequestEvent *evt = &wm_state->event.xconfigurerequest;
    Screen *scr = ScreenOfDisplay(evt->display, 0);
    XWindowChanges changes = {
        .x = (scr->width - evt->width) >> 1,
        .y = (scr->height - evt->height) >> 1,
        .width = evt->width,
        .height = evt->height,
        .border_width = evt->border_width,
        .sibling = evt->above,
        .stack_mode = evt->detail
    };

    evt->value_mask = CWX | CWY;
    DEBUG("Setting window position @ [%d, %d, %d, %d]",
        changes.x, changes.y, changes.width, changes.height);
    XConfigureWindow(evt->display, evt->window,
        (unsigned int)evt->value_mask, &changes);
}
