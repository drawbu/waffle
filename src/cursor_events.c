#include "debug.h"
#include "wm.h"

const vec_t DIRECTION_OFFSET[DIRECTION_COUNT] = {
    [ TOP_LEFT ] = { .x = 1, .y = 1, .width = -1, .height = -1 },
    [ TOP_RIGHT ] = { .x = 0, .y = 1, .width = 1, .height = -1 },
    [ BOTTOM_LEFT ] = { .x = 1, .y = 0, .width = -1, .height = 1 },
    [ BOTTOM_RIGHT ] = { .x = 0, .y = 0, .width = 1, .height = 1 },
};

void handle_enter(wm_t *wm)
{
    XCrossingEvent event = wm->event.xcrossing;

    wm->focused_window = event.window;
    DEBUG("focus:    %lu", wm->focused_window);

}

void handle_leave(wm_t *wm_state DEBUG_USED)
{
    DEBUG("unfocus: %lu", wm_state->focused_window);
}

static
void get_motion_delta(XPoint *delta, XEvent *evt, mouse_mov_t *mouse)
{
    delta->x = (short) (evt->xbutton.x_root - mouse->start.x);
    delta->y = (short) (evt->xbutton.y_root - mouse->start.y);
}

static
direction_t retrieve_mouse_side(mouse_mov_t *mouse, XEvent *event)
{
    int rel_x = event->xbutton.x_root - mouse->window_attr.x;
    int rel_y = event->xbutton.y_root - mouse->window_attr.y;
    bool right = rel_x > mouse->window_attr.width / 2;
    bool bottom = rel_y > mouse->window_attr.height / 2;
    return (direction_t)((bottom << 1) | right);
}

void handle_mouse_press(wm_t *wm)
{
    Window window = wm->event.xbutton.subwindow;

    if (window == None || window == wm->event.xbutton.root)
        return;
    wm->mouse.button = wm->event.xbutton.button;
    wm->mouse.window = window;
    wm->mouse.start.x = (short)wm->event.xbutton.x_root;
    wm->mouse.start.y = (short)wm->event.xbutton.y_root;
    XGetWindowAttributes(
        wm->event.xbutton.display,
        wm->mouse.window,
        &wm->mouse.window_attr
    );
    wm->mouse.side = retrieve_mouse_side(&wm->mouse, &wm->event);
}

void handle_mouse_release(wm_t *wm)
{
    DEBUG_CALL(debug_mouse_motion, wm, false);
    DEBUG_CALL(debug_win_rect, 0, 0, false);
    wm->mouse.button = None;
    wm->mouse.dragging = false;
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
        vec.x, vec.y, (unsigned int)vec.width, (unsigned int)vec.height
    );
}

void handle_mouse_motion(wm_t *wm)
{
    XEvent evt = wm->event;

    DEBUG_CALL(debug_mouse_motion, wm, true);
    if (wm->mouse.button == None)
        return;
    wm->mouse.dragging = true;
    if (wm->mouse.button == Button1)
        move_window(&wm->mouse, &evt);
    else if (wm->mouse.button == Button3)
        resize_window(&wm->mouse, &evt);
    DEBUG_CALL(debug_win_rect, evt.xbutton.display, wm->mouse.window, true);
}
