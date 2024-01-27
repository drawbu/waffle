#include <stdio.h>
#include <stdlib.h>

#include <X11/Xlib.h>
#include <X11/keysym.h>

#include "wm.h"
#include "debug.h"

void handle_key_press(wm_t *wm_state)
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

void handle_map_request(wm_t *wm_state)
{
    XMapRequestEvent event = wm_state->event.xmaprequest;

    DEBUG_MSG("Map of the windows requested");
    XMapWindow(event.display, event.window);
    XSelectInput(
        event.display, event.window,
        EnterWindowMask | LeaveWindowMask | SubstructureRedirectMask
    );
}
