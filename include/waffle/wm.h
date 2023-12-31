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
} wm_state_t;

void setup_grab(Display *display, Window root_id);
void remove_grab(Display *display, Window root_id);

#endif /* !WM_H_ */
