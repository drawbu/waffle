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
    Display *display;
    Window root;
    Window focused_window;
    XEvent event;
    mouse_mov_t mouse;
    bool is_running;
} wm_t;

void handle_key_press(wm_t *wm);
void handle_mouse_press(wm_t *wm);
void handle_mouse_release(wm_t *wm);
void handle_mouse_motion(wm_t *wm);

void handle_enter(wm_t *wm);
void handle_leave(wm_t *wm);

void handle_map_request(wm_t *wm);
void handle_configure_request(wm_t *wm);

void set_window_on_top(Display *display, Window win);

typedef void (*event_callback_t)(wm_t *);

    #ifdef DEBUG_MODE
void debug_mouse_motion(wm_t *wm, bool mode);
void debug_win_rect(Display *display, Window win, bool mode);
    #endif

int wm_setup_grab(wm_t *wm);
void wm_map_windows(wm_t *wm);
void wm_remove_grab(wm_t *wm);

#endif /* !WM_H_ */
