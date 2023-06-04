#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <stdbool.h>

typedef struct {
    bool dragging;
    Window window;
    XWindowAttributes window_attr;
    unsigned int start_x;
    unsigned int start_y;
} mouse_mov_t;

void handleKeyPress(XEvent event, bool *is_running) {
    KeySym key = XLookupKeysym(&event.xkey, 0);

    if (key == XK_a) {
        printf("Opening kitty\n");
        (void)system("kitty &");
    }
    if (key == XK_b) {
        printf("Quitting\n");
        *is_running = false;
    }
}

void handleMousePress(XEvent event, mouse_mov_t *mouse, Window root, Display *display) {
    Window window = event.xbutton.subwindow;

    if (window == 0 || window == root)
        return;
    mouse->dragging = true;
    mouse->window = window;
    XGetWindowAttributes(display, mouse->window, &mouse->window_attr);
    mouse->start_x = event.xbutton.x_root;
    mouse->start_y = event.xbutton.y_root;
}

void handleMouseRelease(XEvent event, mouse_mov_t *mouse) {
    mouse->dragging = false;
}

void handleMouseMotion(XEvent event, mouse_mov_t *mouse, Display *display) {
    if (!mouse->dragging)
        return;
    XMoveResizeWindow(
        display,
        mouse->window,
        mouse->window_attr.x + (event.xbutton.x_root - mouse->start_x),
        mouse->window_attr.y + (event.xbutton.y_root - mouse->start_y),
        mouse->window_attr.width,
        mouse->window_attr.height
    );
    printf(
        "Dragging: x=%d, y=%d\nWindow: x=%d, y=%d\n\n",
        (event.xbutton.x_root - mouse->start_x),
        (event.xbutton.y_root - mouse->start_y),
        mouse->window_attr.x + (event.xbutton.x_root - mouse->start_x),
        mouse->window_attr.x + (event.xbutton.y_root - mouse->start_y)
    );
}

void handleEvents(Display *display) {
    XEvent event;
    bool is_running = true;
    Window root = DefaultRootWindow(display);
    mouse_mov_t mouse = {
        .dragging = false,
        .start_x = 0,
        .start_y = 0,
    };

    XSelectInput(display, DefaultRootWindow(display), KeyPressMask);
    XGrabPointer(display, root, False, PointerMotionMask | ButtonPressMask | ButtonReleaseMask, GrabModeAsync, GrabModeAsync, None, None, CurrentTime);
    while (is_running) {
        XNextEvent(display, &event);
        switch (event.type) {
            case KeyPress:
                handleKeyPress(event, &is_running);
                break;
            case ButtonPress:
                handleMousePress(event, &mouse, root, display);
                break;
            case ButtonRelease:
                handleMouseRelease(event, &mouse);
                break;
            case MotionNotify:
                handleMouseMotion(event, &mouse, display);
                break;
            default:
                break;
        }
    }
    XUngrabPointer(display, CurrentTime);
}

int main() {
    Display *display;

    display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Unable to open display\n");
        return EXIT_FAILURE;
    }
    handleEvents(display);
    XCloseDisplay(display);
    return EXIT_SUCCESS;
}
