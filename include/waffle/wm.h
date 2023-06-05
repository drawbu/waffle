#ifndef WM_H_
    #define WM_H_

    #include <stdbool.h>
    #include <X11/Xlib.h>

typedef struct {
    bool dragging;
    unsigned int button;
    Window window;
    XWindowAttributes window_attr;
    XPoint start;
} mouse_mov_t;

typedef struct {
    mouse_mov_t *mouse;
    bool is_running;
    XEvent event;
} wm_state_t;

#endif /* !WM_H_ */
