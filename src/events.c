#include <stdio.h>
#include <stdlib.h>

#include <X11/Xlib.h>
#include <X11/keysym.h>

#include "waffle/events.h"
#include "waffle/utils.h"
#include "debug.h"

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
    wm_state->mouse->button = wm_state->event.xbutton.button;
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
    if (wm_state->mouse->button == 1) {
        delta.x = (short) (evt.xbutton.x_root - wm_state->mouse->start.x);
        delta.y = (short) (evt.xbutton.y_root - wm_state->mouse->start.y);
        set_window_on_top(evt.xbutton.display, wm_state->mouse->window);
        XMoveWindow(
            evt.xbutton.display,
            wm_state->mouse->window,
            wm_state->mouse->window_attr.x + delta.x,
            wm_state->mouse->window_attr.y + delta.y
        );
    } else if (wm_state->mouse->button == 3) {
        delta.x = (short) (evt.xbutton.x_root - wm_state->mouse->start.x);
        delta.y = (short) (evt.xbutton.y_root - wm_state->mouse->start.y);
        set_window_on_top(evt.xbutton.display, wm_state->mouse->window);
        XResizeWindow(
            evt.xbutton.display,
            wm_state->mouse->window,
            wm_state->mouse->window_attr.width + delta.x,
            wm_state->mouse->window_attr.height + delta.y
        );
    }
}
