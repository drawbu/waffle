#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <stdbool.h>

#include "colors.h"
#include "debug.h"
#include "waffle/wm.h"
#include "waffle/events.h"

void wm_run(Display *display)
{
    Window root = DefaultRootWindow(display);
    event_callback_t event_callback;
    wm_state_t wm = {
        .is_running = true,
        .mouse = &(mouse_mov_t){ 0 }
    };

    printf("%sWaffle%s is running!\n", RED, RESET);
    setup_grab(display, root);
    while (wm.is_running) {
        XNextEvent(display, &wm.event);
        event_callback = EVENT_TABLE[wm.event.type];
        if (!event_callback) {
            DEBUG("Missing callback for event [%d]", wm.event.type);
            continue;
        }
        event_callback(&wm);
    }
    remove_grab(display, root);
}

int main(void)
{
    Display *display = XOpenDisplay(NULL);

    if (display == NULL) {
        fprintf(stderr, "Unable to open display\n");
        return EXIT_FAILURE;
    }
    DEBUG("Display size: [%d, %d]",
        XDisplayWidth(display, 0), XDisplayHeight(display, 0));
    wm_run(display);
    DEBUG_MSG("Closing server");
    XCloseDisplay(display);
    return EXIT_SUCCESS;
}
