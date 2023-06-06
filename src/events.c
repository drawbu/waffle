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
direction_t retrieve_mouse_side(mouse_mov_t *mouse, XEvent *event)
{
    int rel_x = event->xbutton.x_root - mouse->window_attr.x;
    int rel_y = event->xbutton.y_root - mouse->window_attr.y;
    bool right = rel_x > mouse->window_attr.width / 2;
    bool bottom = rel_y > mouse->window_attr.height / 2;
    return (bottom << 1) | right;
}

void handle_mouse_press(wm_state_t *wm_state)
{
    Window window = wm_state->event.xbutton.subwindow;
    mouse_mov_t *mouse = wm_state->mouse;

    if (window == None || window == wm_state->event.xbutton.root)
        return;
    mouse->button = wm_state->event.xbutton.button;
    mouse->window = window;
    mouse->start.x = (short)wm_state->event.xbutton.x_root;
    mouse->start.y = (short)wm_state->event.xbutton.y_root;
    XGetWindowAttributes(
        wm_state->event.xbutton.display,
        mouse->window,
        &mouse->window_attr
    );
    wm_state->mouse->side = retrieve_mouse_side(
        wm_state->mouse, &wm_state->event);
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

static
void move_window(mouse_mov_t *mouse, XEvent *evt)
{
    static XPoint delta;
    static vec_t vec;

    get_motion_delta(&delta, evt, mouse);
    vec.x = mouse->window_attr.x + delta.x;
    vec.y = mouse->window_attr.y + delta.y;
    set_window_on_top(evt->xbutton.display, mouse->window);
    XMoveWindow(evt->xbutton.display, mouse->window, vec.x, vec.y);
}

static
void resize_window(mouse_mov_t *mouse, XEvent *evt)
{
    static vec_t vec;
    static XPoint delta;
    vec_t move = DIRECTION_OFFSET[mouse->side];

    get_motion_delta(&delta, evt, mouse);
    vec.x = mouse->window_attr.x + move.x * delta.x,
    vec.y = mouse->window_attr.y + move.y * delta.y,
    vec.width = mouse->window_attr.width + move.width * delta.x;
    vec.height = mouse->window_attr.height + move.height * delta.y;
    set_window_on_top(evt->xbutton.display, mouse->window);
    if (vec.width < 40 || vec.height < 40)
        return;
    DEBUG("width=%d, height=%d", vec.width, vec.height);
    XMoveResizeWindow(
        evt->xbutton.display, mouse->window,
        vec.x, vec.y, vec.width, vec.height
    );
}

void handle_mouse_motion(wm_state_t *wm_state)
{
    XEvent evt = wm_state->event;
    mouse_mov_t *mouse = wm_state->mouse;

    DEBUG_CALL(debug_mouse_motion, wm_state, true);
    if (mouse->button == None)
        return;
    mouse->dragging = true;
    if (mouse->button == 1)
        move_window(mouse, &evt);
    else if (mouse->button == 3)
        resize_window(mouse, &evt);
    DEBUG_CALL(debug_win_rect, evt.xbutton.display, mouse->window, true);
}
