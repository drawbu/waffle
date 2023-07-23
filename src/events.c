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

void handle_map_request(wm_state_t *wm_state)
{
    XMapRequestEvent event = wm_state->event.xmaprequest;

    DEBUG_MSG("Map of the windows requested");
    XMapWindow(event.display, event.window);
    XSelectInput(
        event.display, event.window,
        EnterWindowMask
        | LeaveWindowMask
        | SubstructureRedirectMask
        | SubstructureNotifyMask
    );
}

void handle_map_notify(wm_state_t *wm_state)
{
    static int magic = -1;
    unsigned long border_color = 0xff0000;
    unsigned long background_color = 0x000000;
    Window bordered_window;
    XSetWindowAttributes attributes;
    XWindowAttributes win_attr;
    XMapEvent event = wm_state->event.xmap;

    DEBUG("state of magic: %d", magic);
    if (--magic & 1)
        return;
    XGetWindowAttributes(event.display, event.window, &win_attr);
    attributes.border_pixel = border_color;
    attributes.background_pixel = background_color;

    bordered_window = XCreateWindow(
        event.display, 
        RootWindow(event.display, 0), 
        -2, -2, win_attr.width, win_attr.height, 2,
        CopyFromParent, InputOutput, CopyFromParent, CWBackPixel | CWBorderPixel,
        &attributes
    );

    XMapWindow(event.display, bordered_window);
    XReparentWindow(event.display, event.window, bordered_window, -2, -2);
    XMapWindow(event.display, event.window);
    XFlush(event.display);
}

