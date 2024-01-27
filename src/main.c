#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <X11/Xlib.h>

#include "colors.h"
#include "debug.h"
#include "wm.h"

static
event_callback_t EVENT_TABLE[LASTEvent] = {
    [KeyPress] = &handle_key_press,
    [ButtonPress] = &handle_mouse_press,
    [ButtonRelease] = &handle_mouse_release,
    [MotionNotify] = &handle_mouse_motion,

    [EnterNotify] = &handle_enter,
    [LeaveNotify] = &handle_leave,

    [MapRequest] = &handle_map_request,
    [ConfigureRequest] = &handle_configure_request
};

static
int wm_run(Display *display)
{
    Window root = DefaultRootWindow(display);
    event_callback_t event_callback;
    wm_t wm = {
        .is_running = true,
        .mouse = &(mouse_mov_t){ 0 }
    };

    map_windows(display, root);
    if (setup_grab(display, root) < 0)
        return -1;
    printf("%sWaffle%s is running!\n", YELLOW, RESET);
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
    return 0;
}

static
int wm_start(void)
{
    Display *display = XOpenDisplay(NULL);

    if (display == NULL) {
        fprintf(stderr, "Unable to open display: [%s]\n", XDisplayName(NULL));
        return -1;
    }
    DEBUG("Display size: [%d, %d]",
        XDisplayWidth(display, 0), XDisplayHeight(display, 0));
    if (wm_run(display) < 0)
        return -1;
    DEBUG_MSG("Closing server");
    XCloseDisplay(display);
    return 0;
}

static
int hot_reload_run(char *prog_name)
{
    char command[256] = { '\0' };
    int status = snprintf(command, 64, "make %s", prog_name);

    DEBUG("%sRunning hot loader hook...%s", YELLOW, RESET);
    if (status == -1)
        return EXIT_FAILURE;
    status = system(command);
    DEBUG("-> got status: %s%d%s", YELLOW, status, RESET);
    if (status == -1)
        return EXIT_FAILURE;
    status = execlp(prog_name, prog_name, "hot-reload", NULL);
    DEBUG(
        "%sGOT STATUS [%d] WHILE TRYING TO HOT RELOAD!!!%s",
        RED, status, RESET
    );
    return EXIT_FAILURE;
}

int main(int argc, char **argv)
{
    bool ret;
    bool hot_reload = argc == 2 && !strcmp(argv[1], "hot-reload");

    setbuf(stdout, NULL);
#ifdef DEBUG_MODE
    if (hot_reload) {
        DEBUG("%shot-reload%s hook set to [%s%s%s]",
            YELLOW, RESET, CYAN, argv[0], RESET);
    }
#endif
    ret = wm_start();
    if (hot_reload)
        hot_reload_run(argv[0]);
    return ret;
}

