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
    if (key_pressed == XK_b) {
        printf("Quitting\n");
        wm_state->is_running = false;
        return;
    }
    if (key.subwindow == None || key.subwindow == wm_state->event.xkey.window)
        return;
    key.window = key.subwindow;
    XSendEvent(key.display, key.subwindow, False, KeyPressMask, (XEvent *)&key);
    XFlush(key.display);
}

void handle_mouse_press(wm_state_t *wm_state)
{
    Window window = wm_state->event.xbutton.subwindow;

    if (window == None || window == wm_state->event.xbutton.root)
        return;
    wm_state->mouse->dragging = true;
    wm_state->mouse->window = window;
    wm_state->mouse->start.x = (short)wm_state->event.xbutton.x_root;
    wm_state->mouse->start.y = (short)wm_state->event.xbutton.y_root;
    XGetWindowAttributes(
        wm_state->event.xbutton.display,
        wm_state->mouse->window,
        &wm_state->mouse->window_attr
    );
}

void handle_mouse_release(wm_state_t *wm_state)
{
    wm_state->mouse->dragging = false;
}

void handle_mouse_motion(wm_state_t *wm_state)
{
    XEvent evt = wm_state->event;
    XPoint delta;

    if (!wm_state->mouse->dragging)
        return;
    delta.x = (short)(evt.xbutton.x_root - wm_state->mouse->start.x);
    delta.y = (short)(evt.xbutton.y_root - wm_state->mouse->start.y);
    XMoveWindow(
        evt.xbutton.display,
        wm_state->mouse->window,
        wm_state->mouse->window_attr.x + delta.x,
        wm_state->mouse->window_attr.y + delta.y
    );
}
