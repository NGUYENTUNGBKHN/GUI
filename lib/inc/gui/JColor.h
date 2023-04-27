#pragma once

typedef unsigned long	JColor;

#define argb(a,r,g,b)	(((a)<<24) | ((r)<<16) | ((g)<<8) | ((b)<<0))
#define rgb(r,g,b)		argb(255,r,g,b)

#define	GetA(c)		((c >> 24) & 255)
#define	GetR(c)		((c >> 16) & 255)
#define	GetG(c)		((c >>  8) & 255)
#define	GetB(c)		((c >>  0) & 255)

#define	TRANS		argb(0,0,0,0)
#define	INVALID_COLOR	argb(0,255,255,255)
#define	RANDOM_ARGB		argb((rand()&255),(rand()&255),(rand()&255),(rand()&255))
#define	RANDOM_RGB		argb(255,(rand()&255),(rand()&255),(rand()&255))

#define	BLACK		argb(255,   0,   0,   0)
#define	BLUE		argb(255,   0,   0, 255)
#define	RED			argb(255, 255,   0,   0)
#define	MAGENTA		argb(255, 255,   0, 255)
#define	GREEN		argb(255,   0, 255,   0)
#define	CYAN		argb(255,   0, 255, 255)
#define	YELLOW		argb(255, 255, 255,   0)
#define	WHITE		argb(255, 255, 255, 255)

#define	GRAY		argb(255, 128, 128, 128)

#define	GRAY100		argb(255, 255, 255, 255)
#define	GRAY75		argb(255, 192, 192, 192)
#define	GRAY50		argb(255, 128, 128, 128)
#define	GRAY25		argb(255,  64,  64,  64)
#define	GRAY0		argb(255,   0,   0,   0)


