#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "libcpp.h"
#include "JWin.h"

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

JRootWindow m_wndRoot;
DWORD m_dwNext = 0;
static Display *display;
static Window win;
static GC gc;
static int screen;
static Atom wm_delete_window;
static JImage *p_JImage = NULL;
static bool m_bExiting = false;


WORD myrand()
{
	static DWORD holdrand = 0;
	holdrand = (holdrand * 214013L) + 2531011L;
	return (WORD)(holdrand >> 16);
}

void Paint()
{
#if 0
	static const int sx = 0;
	static const int sy = 0;
	static const int dx = 0;
	static const int dy = 0;
	static const int w = 1280;
	static const int h = 768;
	static const int pw = 4;
	static const int lw = w * pw;

	static BYTE data[1280 * 768 * 4];
	for( int y=0 ; y<h ; y++ )
	{
		for( int x=0 ; x<w ; x++ )
		{
			data[(y * lw) + (x * pw) + 0] = (BYTE)myrand();
			data[(y * lw) + (x * pw) + 1] = (BYTE)myrand();
			data[(y * lw) + (x * pw) + 2] = (BYTE)myrand();
			data[(y * lw) + (x * pw) + 3] = (BYTE)255;
		}
	}

	XImage* pImage = XCreateImage(display, DefaultVisual(display, screen), 24, ZPixmap, 0, (char*)data, w, h, 32, lw);
	XPutImage(display, win, gc, pImage, 0, 0, 0, 0, DISPLAY_W, DISPLAY_H);
#else
    // printf("%s %d \n",__func__, __LINE__);
    
	XImage* pImage = XCreateImage(display, DefaultVisual(display, screen), 24, ZPixmap, 0, (char*)p_JImage->GetPixelAddress(0, 0), DISPLAY_W, DISPLAY_H, 32, DISPLAY_W * 4);
	XPutImage(display, win, gc, pImage, 0, 0, 0, 0, DISPLAY_W, DISPLAY_H);
#endif
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
                        m_bExiting = true;
                        XDestroyWindow(display, win);
                    }
                    break;
                case DestroyNotify:
                    m_bExiting = true;
                    XSetCloseDownMode(display, DestroyAll);
    				XCloseDisplay(display);
				    display = NULL;
                    break;
                default:
                    break;
            }
        }
    }

    if (!m_bExiting && (display != NULL))
    {
        DWORD dwNow = m_wndRoot.getTime();
		if( dwNow >= m_dwNext )
		{
			m_dwNext = dwNow + 10;
			//
			static int div = 0;
			div = (div + 1) % 3;

			if( div == 0 )
			{
				// timer with render per 30ms
				JWIN_WorkRootWindow_10ms(&m_wndRoot, p_JImage->GetPixelAddress(0, 0), NULL, NULL);
				Paint();
			}
			else
			{
				// timer per 10ms
				JWIN_WorkRootWindow_10ms(&m_wndRoot, NULL, NULL, NULL);
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
	XFlush(display);

	//
	m_wndRoot.Create(0, 0, DISPLAY_W, DISPLAY_H);

	//
	JWIN_Init(WorkMainLoop);
	p_JImage = JWIN_NewJImage(DISPLAY_W, DISPLAY_H);
	//
	extern void APP_Init(JRootWindow* pRoot);
	APP_Init(&m_wndRoot);
    printf("%s %d \n",__func__, __LINE__);
    
	// if( (argc >= 2) && !strcmp(argv[1], "-nomainloop") )
	// {
	// 	return 0;
	// }

    while(display != NULL)
    {
        XFlush(display);
        WorkMainLoop();
    }
    return 0;
}

