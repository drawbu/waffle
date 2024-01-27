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
int wm_run(wm_t *wm)
{
    event_callback_t event_callback;

    while (wm->is_running) {
        XNextEvent(wm->display, &wm->event);
        event_callback = EVENT_TABLE[wm->event.type];
        if (!event_callback) {
            DEBUG("Missing callback for event [%d]", wm->event.type);
            continue;
        }
        event_callback(wm);
    }
    return 0;
}

static
int wm_init(wm_t *wm)
{
    wm->display = XOpenDisplay(NULL);
    if (wm->display == NULL) {
        fprintf(stderr, "Unable to open display: [%s]\n", XDisplayName(NULL));
        return -1;
    }
    DEBUG("Display size: [%d, %d]",
        XDisplayWidth(wm->display, 0), XDisplayHeight(wm->display, 0));
    wm->root = DefaultRootWindow(wm->display);
    wm_map_windows(wm);
    if (wm_setup_grab(wm) < 0)
        return -1;
    printf("%sWaffle%s is running!\n", YELLOW, RESET);
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

static
void wm_finalize(wm_t *wm) {
    DEBUG_MSG("Closing server");
    wm_remove_grab(wm);
    XCloseDisplay(wm->display);
}

int main(int argc, char **argv)
{
    bool hot_reload = argc == 2 && !strcmp(argv[1], "hot-reload");
    wm_t wm = { .is_running = true };

    setbuf(stdout, NULL);
#ifdef DEBUG_MODE
    if (hot_reload) {
        DEBUG("%shot-reload%s hook set to [%s%s%s]",
            YELLOW, RESET, CYAN, argv[0], RESET);
    }
#endif
    if (wm_init(&wm) < 0)
        return EXIT_FAILURE;
    if (wm_run(&wm) < 0)
        return EXIT_FAILURE;
    wm_finalize(&wm);
    if (hot_reload)
        hot_reload_run(argv[0]);
    return EXIT_SUCCESS;
}

