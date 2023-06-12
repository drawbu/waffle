#include <stdio.h>
#include <stdlib.h>

#include <X11/Xlib.h>
#include <X11/keysym.h>

#include "waffle/events.h"

void handle_key_press(wm_state_t *wm_state)
{
    XKeyEvent key = wm_state->event.xkey;
    KeySym key_pressed = XLookupKeysym(&key, 0);
    unsigned int modifiers = key.state;

    if (modifiers & Mod4Mask && key_pressed == XK_Return) {
        printf("Opening kitty\n");
        (void)system("kitty &");
        return;
    }
    if (modifiers & Mod4Mask && key_pressed == XK_Escape) {
        printf("Quitting\n");
        wm_state->is_running = false;
        return;
    }
    if (key.subwindow == None || key.subwindow == wm_state->event.xkey.root)
        return;
    key.window = key.subwindow;
    XSendEvent(key.display, key.subwindow, False, KeyPressMask, (XEvent *)&key);
}

static
void get_windows(Display *display, Window root)
{
    unsigned int count;
    Window root_return;
    Window parent_return;
    Window *windows = NULL;
    XWindowAttributes wa;

    if (!XQueryTree(display, root, &root_return, &parent_return, &windows, &count)
        || windows == NULL
    ) {
        printf("Failed to query windows tree\n");
        return;
    }
    for (unsigned int i = 0; i < count; i++) {
        Window window = windows[i];

        if (!XGetWindowAttributes(display, window, &wa))
            continue;
        if (wa.map_state != IsViewable)
            continue;
        XSelectInput(
            display, window,
            EnterWindowMask | LeaveWindowMask | SubstructureNotifyMask | SubstructureRedirectMask
        );
    }
    XFree(windows);
}
