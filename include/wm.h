#ifndef WM_H_
    #define WM_H_

    #include <stdbool.h>

    #include <X11/Xlib.h>
    #include <X11/keysym.h>

    #include "utils.h"

typedef struct {
    bool dragging;
    unsigned int button;
    direction_t side;
    Window window;
    XWindowAttributes window_attr;
    XPoint start;
} mouse_mov_t;

typedef struct {
    mouse_mov_t *mouse;
    bool is_running;
    XEvent event;
    Window focused_window;
} wm_t;


void handle_key_press(wm_t *wm_state);
void handle_mouse_press(wm_t *wm_state);
void handle_mouse_release(wm_t *wm_state);
void handle_mouse_motion(wm_t *wm_state);

void handle_enter(wm_t *wm_state);
void handle_leave(wm_t *wm_state);

void handle_map_request(wm_t *wm_state);
void handle_configure_request(wm_t *wm_state);

typedef void (*event_callback_t)(wm_t *);

    #ifdef DEBUG_MODE
void debug_mouse_motion(wm_t *wm_state, bool mode);
void debug_win_rect(Display *display, Window win, bool mode);
    #endif

int setup_grab(Display *display, Window root_id);
void remove_grab(Display *display, Window root_id);

#endif /* !WM_H_ */
