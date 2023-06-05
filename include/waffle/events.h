#ifndef EVENTS_H_
    #define EVENTS_H_

    #include "wm.h"

void handle_key_press(wm_state_t *wm_state);
void handle_mouse_press(wm_state_t *wm_state);
void handle_mouse_release(wm_state_t *wm_state);
void handle_mouse_motion(wm_state_t *wm_state);

typedef void (*event_callback_t)(wm_state_t *);

static event_callback_t EVENT_TABLE[LASTEvent] = {
    [KeyPress] = &handle_key_press,
    [ButtonPress] = &handle_mouse_press,
    [ButtonRelease] = &handle_mouse_release,
    [MotionNotify] = &handle_mouse_motion,
};

#endif /* !EVENTS_H_ */
