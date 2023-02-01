#include "JWin.h"
#include <png.h>


static const int minx = 0;
static const int miny = 0;
static const int maxx = 1280;
static const int maxy = 768;


bool G2D_Init() {return true;}


class ImageImpl : public JImage_mem_ARGB8888
{

};

