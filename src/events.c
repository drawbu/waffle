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
}

void handle_mouse_press(wm_state_t *wm_state)
{
    Window window = wm_state->event.xbutton.subwindow;

    if (window == None || window == wm_state->event.xbutton.root)
        return;
    wm_state->mouse->button = wm_state->event.xbutton.button;
    wm_state->mouse->window = window;
    wm_state->mouse->start.x = (short)wm_state->event.xbutton.x_root;
    wm_state->mouse->start.y = (short)wm_state->event.xbutton.y_root;
    XGetWindowAttributes(
        wm_state->event.xbutton.display,
        wm_state->mouse->window,
        &wm_state->mouse->window_attr
    );
    bool right = (
        wm_state->event.xbutton.x_root - wm_state->mouse->window_attr.x
        > wm_state->mouse->window_attr.width / 2);
    bool bottom = (
        wm_state->event.xbutton.y_root - wm_state->mouse->window_attr.y
        > wm_state->mouse->window_attr.height / 2 );
    wm_state->mouse->side = (right << 0) + (bottom << 1);
}

void handle_mouse_release(wm_state_t *wm_state)
{
    DEBUG_CALL(debug_mouse_motion, wm_state, false);
    DEBUG_CALL(debug_win_rect, 0, 0, false);
    wm_state->mouse->button = None;
    if (wm_state->mouse->dragging) {
        wm_state->mouse->dragging = false;
        return;
    }
}

void handle_mouse_motion(wm_state_t *wm_state)
{
    static vec_t vec;
    static XPoint delta;
    mouse_mov_t *mouse = wm_state->mouse;
    XEvent evt = wm_state->event;

    DEBUG_CALL(debug_mouse_motion, wm_state, true);
    if (mouse->button == None)
        return;
    mouse->dragging = true;
    if (mouse->button == 1) {
        get_motion_delta(&delta, &evt, mouse);
        set_window_on_top(evt.xbutton.display, mouse->window);
        vec.x = mouse->window_attr.x + delta.x;
        vec.y = mouse->window_attr.y + delta.y;
        XMoveWindow(evt.xbutton.display, mouse->window, vec.x, vec.y);
    } else if (mouse->button == 3) {
        vec_t move = DIRECTION_OFFSET[wm_state->mouse->side];

        get_motion_delta(&delta, &evt, mouse);
        set_window_on_top(evt.xbutton.display, mouse->window);
        vec.x = mouse->window_attr.x + move.x * delta.x;
        vec.y = mouse->window_attr.y + move.y * delta.y;
        vec.width = mouse->window_attr.width + move.width * delta.x;
        vec.height = mouse->window_attr.height + move.height * delta.y;
        if (vec.width < 40 || vec.height < 40)
            return;
        DEBUG("width=%d, height=%d", vec.width, vec.height);
        XMoveResizeWindow(
            evt.xbutton.display, mouse->window,
            vec.x, vec.y, vec.width, vec.height
        );
    }
    DEBUG_CALL(debug_win_rect, evt.xbutton.display, mouse->window, true);
}
