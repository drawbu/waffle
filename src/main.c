#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

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
    map_windows(display, root);
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

int wm_start(void)
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

static 
int hot_reload_run(char *prog_name)
{   
    char command[256] = { '\0' };
    char *argv[2] = { prog_name, NULL };
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

    if (hot_reload)
        DEBUG(
            "%shot-reload%s hook set to [%s%s%s]",
            YELLOW, RESET, CYAN, argv[0], RESET
        );
    ret = wm_start();
    if (hot_reload)
        hot_reload_run(argv[0]);
    return ret;
}

