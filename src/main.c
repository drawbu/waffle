#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <stdbool.h>

#include "colors.h"
#include "debug.h"
#include "waffle/wm.h"

typedef struct mouse_mov_s {
    bool dragging;
    Window window;
    XWindowAttributes window_attr;
    XPoint start;
} mouse_mov_t;

void handle_key_press(wm_state_t *wm_state)
{
    XKeyEvent key = wm_state->event.xkey;
    KeySym key_pressed = XLookupKeysym(&key, 0);
    Window focused_window = key.subwindow;
    unsigned int modifiers = key.state;

    if (modifiers & Mod4Mask && key_pressed == XK_Return) {
        printf("Opening kitty\n");
        (void)system("kitty &");
        return;
    }
    if (key_pressed == XK_b) {
        printf("Quitting\n");
        wm_state->is_running = false;
        return;
    }
    if (focused_window == None || focused_window == wm_state->event.xkey.window)
        return;
    key.window = focused_window;
    XSendEvent(key.display, focused_window, False, KeyPressMask, (XEvent *)&key);
    XFlush(key.display);
}

void handle_mouse_press(wm_state_t *wm_state)
{
    Window window = wm_state->event.xbutton.subwindow;

    if (window == None || window == wm_state->event.xbutton.root)
        return;
    wm_state->mouse->dragging = true;
    wm_state->mouse->window = window;
    XGetWindowAttributes(wm_state->event.xbutton.display, wm_state->mouse->window, &wm_state->mouse->window_attr);
    wm_state->mouse->start.x = (short)wm_state->event.xbutton.x_root;
    wm_state->mouse->start.y = (short)wm_state->event.xbutton.y_root;
}

void handle_mouse_release(wm_state_t *wm_state)
{
    wm_state->mouse->dragging = false;
}

void handleMouseMotion(wm_state_t *wm_state) {
    if (!wm_state->mouse->dragging)
        return;
    XMoveResizeWindow(
        wm_state->event.xbutton.display,
        wm_state->mouse->window,
        wm_state->mouse->window_attr.x +
        (wm_state->event.xbutton.x_root - wm_state->mouse->start.x),
        wm_state->mouse->window_attr.y +
        (wm_state->event.xbutton.y_root - wm_state->mouse->start.y),
        wm_state->mouse->window_attr.width,
        wm_state->mouse->window_attr.height
    );
}

void setup_grab(Display *display, Window root_id)
{
    static int grab_event_pointer = (
        PointerMotionMask | ButtonPressMask | ButtonReleaseMask
    );

    DEBUG("GEP: %d", grab_event_pointer);
    XGrabPointer(
        display, root_id, False,
        grab_event_pointer, GrabModeAsync, GrabModeAsync,
        None, None, CurrentTime
    );
    XGrabKeyboard(
        display, root_id, True,
        GrabModeAsync, GrabModeAsync, CurrentTime
    );
}

void remove_grab(Display *display)
{
    XUngrabPointer(display, CurrentTime);
    XUngrabKeyboard(display, CurrentTime);
}

void wm_run(Display *display)
{
    Window root = DefaultRootWindow(display);
    wm_state_t wm = {
        .is_running = true,
        .mouse = &(mouse_mov_t){ 0 }
    };

    printf("%sWaffle%s is running!\n", RED, RESET);
    setup_grab(display, root);
    while (wm.is_running) {
        XNextEvent(display, &wm.event);
        switch (wm.event.type) {
            case KeyPress:
                handle_key_press(&wm);
                break;
            case ButtonPress:
                handle_mouse_press(&wm);
                break;
            case ButtonRelease:
                handle_mouse_release(&wm);
                break;
            case MotionNotify:
                handleMouseMotion(&wm);
                break;
            default:
                break;
        }
    }
    remove_grab(display);
}

int main(void)
{
    Display *display = XOpenDisplay(NULL);

    if (display == NULL) {
        fprintf(stderr, "Unable to open display\n");
        return EXIT_FAILURE;
    }
    DEBUG("Display size: [%d, %d]",
        XDisplayWidth(display, 0), XDisplayHeight(display, 0));
    wm_run(display);
    XCloseDisplay(display);
    return EXIT_SUCCESS;
}
