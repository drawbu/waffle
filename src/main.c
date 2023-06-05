#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <stdbool.h>

#include "colors.h"
#include "debug.h"

typedef struct {
    bool dragging;
    Window window;
    XWindowAttributes window_attr;
    unsigned int start_x;
    unsigned int start_y;
} mouse_mov_t;

void handle_key_press(XEvent *event, bool *is_running) {
    XKeyEvent key = event->xkey;
    KeySym key_pressed = XLookupKeysym(&key, 0);
    unsigned int modifiers = key.state;
    Window focused_window = key.subwindow;

    if (modifiers & Mod4Mask && key_pressed == XK_Return) {
        printf("Opening kitty\n");
        (void)system("kitty &");
        return;
    }
    if (key_pressed == XK_b) {
        printf("Quitting\n");
        *is_running = false;
        return;
    }
    if (focused_window == None || focused_window == event->xkey.window)
        return;
    key.window = focused_window;
    XSendEvent(key.display, focused_window, False, KeyPressMask, (XEvent *)&key);
    XFlush(key.display);
}

void handle_mouse_press(XEvent *event, mouse_mov_t *mouse) {
    Window window = event->xbutton.subwindow;

    if (window == None || window == event->xbutton.root)
        return;
    mouse->dragging = true;
    mouse->window = window;
    XGetWindowAttributes(event->xbutton.display, mouse->window, &mouse->window_attr);
    mouse->start_x = event->xbutton.x_root;
    mouse->start_y = event->xbutton.y_root;
}

void handle_mouse_release(XEvent *event, mouse_mov_t *mouse) {
    mouse->dragging = false;
}

void handleMouseMotion(XEvent *event, mouse_mov_t *mouse) {
    if (!mouse->dragging)
        return;
    XMoveResizeWindow(
        event->xbutton.display,
        mouse->window,
        mouse->window_attr.x + (event->xbutton.x_root - mouse->start_x),
        mouse->window_attr.y + (event->xbutton.y_root - mouse->start_y),
        mouse->window_attr.width,
        mouse->window_attr.height
    );
}

void handle_events(Display *display) {
    XEvent event;
    bool is_running = true;
    Window root = DefaultRootWindow(display);
    mouse_mov_t mouse = {
        .dragging = false,
        .start_x = 0,
        .start_y = 0,
    };

    printf("%sWaffle%s is running!\n", RED, RESET);
    XSelectInput(display, DefaultRootWindow(display), KeyPressMask);
    XGrabPointer(display, root, False, PointerMotionMask | ButtonPressMask | ButtonReleaseMask, GrabModeAsync, GrabModeAsync, None, None, CurrentTime);
    XGrabKeyboard(display, DefaultRootWindow(display), True, GrabModeAsync, GrabModeAsync, CurrentTime);
    while (is_running) {
        XNextEvent(display, &event);
        switch (event.type) {
            case KeyPress:
                handle_key_press(&event, &is_running);
                break;
            case ButtonPress:
                handle_mouse_press(&event, &mouse);
                break;
            case ButtonRelease:
                handle_mouse_release(&event, &mouse);
                break;
            case MotionNotify:
                handleMouseMotion(&event, &mouse);
                break;
            default:
                break;
        }
    }
    XUngrabPointer(display, CurrentTime);
    XUngrabKeyboard(display, CurrentTime);
}

int main() {
    Display *display;

    display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Unable to open display\n");
        return EXIT_FAILURE;
    }
    handle_events(display);
    XCloseDisplay(display);
    return EXIT_SUCCESS;
}
