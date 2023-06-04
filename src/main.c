#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <stdbool.h>

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

void handleEvents(Display *display) {
    XEvent event;
    bool is_running = true;
    XSelectInput(display, DefaultRootWindow(display), KeyPressMask);

    while (is_running) {
        XNextEvent(display, &event);
        switch (event.type) {
            case KeyPress:
                handleKeyPress(event, &is_running);
                break;
            default:
                break;
        }
    }
}

int main() {
    Display *display;
    int screen;
    Window rootWindow;

    display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Unable to open display\n");
        return 1;
    }
    screen = DefaultScreen(display);
    rootWindow = RootWindow(display, screen);
    XSelectInput(display, rootWindow, KeyPressMask);
    handleEvents(display);
    XCloseDisplay(display);
    return 0;
}
