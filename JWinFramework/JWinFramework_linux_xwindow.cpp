#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/utsname.h>

#define DISPLAY_W 1280
#define DISPLAY_H 768

#define DEBUG

#ifdef DEBUG
#define TRACE(fmt, arg...) printf("MAIN DEBUG (%d) : " fmt, __LINE__, ##arg)
#else
#define TRACE()
#endif // DEBUG
#define INFO(fmt, arg...) printf("MAIN (%d) : " fmt, __LINE__, ##arg)

static Display *display;
static Window win;
static GC gc;
static int screen;
static Atom wm_delete_window;

void Paint()
{

}


void WorkMainLoop(void)
{
    int x0, y0, x1, y1;

    {
        if (XEventsQueued(display, QueuedAlready) >= 1)
        {
            XEvent e;
            XNextEvent(display, &e);
            switch(e.type)
            {
                case ButtonPress:
                    if (e.xbutton.button == Button1)
                    {
                        // TRACE("x = %d y = %d \n",e.xbutton.x, e.xbutton.y);
                    }
                    if (e.xbutton.button == Button2)
                    {
                        // TRACE("x = %d y = %d \n",e.xbutton.x, e.xbutton.y);
                    }
                    break;
                case ButtonRelease:
                    if (e.xbutton.button == Button1)
                    {
                        // TRACE("x = %d y = %d \n",e.xbutton.x, e.xbutton.y);
                    }
                    if (e.xbutton.button == Button2)
                    {
                        // TRACE("x = %d y = %d \n",e.xbutton.x, e.xbutton.y);
                    }
                    break;
                case MotionNotify:
                    // TRACE("x = %d y = %d \n",e.xmotion.x, e.xmotion.y);
				    break;
                case Expose:
                    Paint();
                    break;
                case ClientMessage:
                    if (e.xclient.data.l[0] == wm_delete_window)
                    {
                        XDestroyWindow(display, win);
                    }
                    break;
                case DestroyNotify:
                    XSetCloseDownMode(display, DestroyAll);
    				XCloseDisplay(display);
				    display = NULL;
                    break;
                default:
                    break;
            }
        }
    }
}

int JWIN_Framework(int argc, char *argv[])
{

    display = XOpenDisplay(NULL);
	if( display == NULL )
	{
		puts("XOpenDisplay() failed.");
		return 1;
	}

	screen = DefaultScreen(display);
	win = XCreateSimpleWindow(display, RootWindow(display, screen),
		0, 0, DISPLAY_W, DISPLAY_H,
		1, BlackPixel(display, screen),
		WhitePixel(display, screen));

	XSetWindowAttributes attr;
	attr.backing_store = WhenMapped;
	XChangeWindowAttributes(display, win, CWBackingStore, &attr);

	wm_delete_window = XInternAtom(display, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(display, win, &wm_delete_window, 1);

	XStoreName(display, win, "GUI LINUX");
	XSetIconName(display, win, "GUI LINUX");

	gc = XCreateGC(display, RootWindow(display, screen), 0, 0);

	XSelectInput(display, win, ButtonPressMask | ButtonReleaseMask | PointerMotionMask | ExposureMask | StructureNotifyMask);

	XMapWindow(display, win);
	// XFlush(display);

	// if( (argc >= 2) && !strcmp(argv[1], "-nomainloop") )
	// {
	// 	return 0;
	// }

	// main loop
	// while( display != NULL )
	// {
	// 	XFlush(display);
	// }
    while(display != NULL)
    {
        XFlush(display);
        WorkMainLoop();
    }
    return 0;
}

