#ifndef EVENTS_H_
    #define EVENTS_H_

    #include "wm.h"
    #define ATTR(key) __attribute__((key))

void handle_key_press(wm_state_t *wm_state);
void handle_mouse_press(wm_state_t *wm_state);
void handle_mouse_release(wm_state_t *wm_state);
void handle_mouse_motion(wm_state_t *wm_state);

void handle_enter(wm_state_t *wm_state);
void handle_leave(wm_state_t *wm_state);

void handle_map_notify(wm_state_t *wm_state);
void handle_map_request(wm_state_t *wm_state);

typedef void (*event_callback_t)(wm_state_t *);

static ATTR(used)
event_callback_t EVENT_TABLE[LASTEvent] = {
    [KeyPress] = &handle_key_press,
    [ButtonPress] = &handle_mouse_press,
    [ButtonRelease] = &handle_mouse_release,
    [MotionNotify] = &handle_mouse_motion,

    [EnterNotify] = &handle_enter,
    [LeaveNotify] = &handle_leave,

    [MapNotify] = &handle_map_notify,
    [MapRequest] = &handle_map_request,
};

static
void get_motion_delta(XPoint *delta, XEvent *evt, mouse_mov_t *mouse)
{
    delta->x = (short) (evt->xbutton.x_root - mouse->start.x);
    delta->y = (short) (evt->xbutton.y_root - mouse->start.y);
}

    #ifdef DEBUG_MODE
void debug_mouse_motion(wm_state_t *wm_state, bool mode);
void debug_win_rect(Display *display, Window win, bool mode);
    #endif

#endif /* !EVENTS_H_ */
