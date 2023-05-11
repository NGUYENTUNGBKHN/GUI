#include "JWin.h"
#include <png.h>

static const int minx = 0;
static const int miny = 0;
static const int maxx = 1280;
static const int maxy = 768;

/******************************************************************************/
bool G2D_Init() { return true; } // omg...

/******************************************************************************/
class ImageImpl : public JImage_mem_ARGB8888
{
public:
    ImageImpl()
    {
        m_pData = NULL;
    }

    virtual ~ImageImpl()
    {
        Destroy();
    }

    virtual bool impl_Create(int w, int h)
    {
        PIXEL = 4;
        ALINE = ((w + 15) & 0xfff0) * PIXEL;
        LINE = ALINE;

        m_pData = (BYTE *)malloc(ALINE * h);
        if (m_pData == NULL)
            return false;

        return true;
    }

    virtual void Destroy()
    {
        if (m_pData != NULL)
        {
            free(m_pData);
            m_pData = NULL;
        }
    }
};

/******************************************************************************/
JImage *JWIN_NewJImage()
{
    return new ImageImpl();
}

/******************************************************************************/
JImage *_LoadImage(const TCHAR *pPath)
{
    // printf("LoadImage: %ls \n", pPath);

    JImage *pImage = NULL;

    {
        File f;
        if (!f.Open(pPath, _T("rb")))
        {
            printf("LoadImage: Open(%s) failed. \n", pPath);
            return NULL;
        }

        BYTE sig[4];
        if (!f.Read(sig, sizeof(sig)))
        {
            printf("LoadImage: Read() failed. \n");
            return NULL;
        }
        if (!png_check_sig(sig, sizeof(sig)))
        {
            printf("LoadImage: png_check_sig() failed. \n");
            return NULL;
        }

        png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (!png_ptr)
        {
            printf("LoadImage: () failed. \n");
            return NULL;
        }

        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr)
        {
            png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
            printf("LoadImage: png_create_info_struct() failed. \n");
            return NULL;
        }

        if (setjmp(png_jmpbuf(png_ptr)))
        {
            png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
            printf("LoadImage: setjmp() failed. \n");
            return NULL;
        }
        
        png_init_io(png_ptr, f);
        png_set_sig_bytes(png_ptr, sizeof(sig));
        png_read_info(png_ptr, info_ptr);

        int channels = (int)png_get_channels(png_ptr, info_ptr);
        if ((channels != 3) && (channels != 4))
        {
            png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
            printf("LoadImage: unsupported channels. \n");
            return NULL;
        }

        // int color_type = (int)png_get_filter_type(png_ptr, info_ptr);
        int width, height, depth, color_type;
        png_get_IHDR(png_ptr, info_ptr, (png_uint_32 *)&width, (png_uint_32 *)&height, &depth, &color_type, NULL, NULL, NULL);
        // printf("LoadImage: %S width=%d hwight=%d channels=%d color_type=%d \n", pPath, width, height, channels, color_type);

#if 0
// deprecate for trans_values matching
		double display_gamma = 2.2;
		double file_gamma;
		if( png_get_gAMA(png_ptr, info_ptr, &file_gamma) )
		{
			png_set_gamma(png_ptr, display_gamma, file_gamma);
		}
		else
		{
			png_set_gamma(png_ptr, display_gamma, 0.50);
		}
#endif

        png_read_update_info(png_ptr, info_ptr);

        //
        png_bytep trans;
        int num_trans = 0;
        png_color_16p trans_values = NULL;
        png_get_tRNS(png_ptr, info_ptr, &trans, &num_trans, &trans_values);
        // if( num_trans > 0 )
        //	printf("channels=%d num_trans=%d trans_values[0]=%04x:%04x:%04x \n", channels, num_trans, trans_values[0].red, trans_values[0].blue, trans_values[0].green);
        // else
        //	printf("channels=%d num_trans=0 \n", channels);

        //
        png_bytepp image;
        BYTE *buffer;
        {
            image = (png_bytepp)malloc(height * sizeof(png_bytep));
            // printf("***LoadImage: malloc-1 %u %p \n", (unsigned int)(height * sizeof(png_bytep)), image);
            if (image == NULL)
            {
                png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
                printf("LoadImage: malloc() 1 failed. \n");
                return NULL;
            }

            int lw = png_get_rowbytes(png_ptr, info_ptr);
            buffer = (BYTE *)malloc(lw * height);
            // printf("***LoadImage: malloc-2 %d %d %u %p \n", lw, height, (unsigned int)(lw * height), buffer);
            if (buffer == NULL)
            {
                png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
                printf("LoadImage: malloc() 2 failed. \n");
                return NULL;
            }

            for (int i = 0; i < height; i++)
                image[i] = buffer + (lw * i);
        }
        png_read_image(png_ptr, image);

        //
        // printf("%s %d w = %d h = %d \n",__func__, __LINE__, width, height);
        pImage = JWIN_NewJImage(width, height);
        for (int y = 0; y < height; y++)
        {
            const BYTE *s = image[y];

            DWORD *d = (DWORD *)pImage->GetPixelAddress(0, y);
            // printf("%s %d \n",__func__, __LINE__);
            if (channels == 4)
            {
                for (int x = 0; x < width; x++)
                {
                    BYTE r = *s++;
                    BYTE g = *s++;
                    BYTE b = *s++;
                    BYTE a = *s++;

                    *d++ = argb(a, r, g, b);
                }
            }
            else if (channels == 3)
            {
                for (int x = 0; x < width; x++)
                {
                    BYTE a = 255;
                    BYTE r = *s++;
                    BYTE g = *s++;
                    BYTE b = *s++;

                    if (num_trans == 1)
                    {
                        if ((r == trans_values[0].red) && (g == trans_values[0].green) && (b == trans_values[0].blue))
                        {
                            a = 0;
                        }
                    }

                    *d++ = argb(a, r, g, b);
                }
            }
        }

        //
        png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
        free(image);
        //		free(buffer);	// double free?
    }
    return pImage;
}

/******************************************************************************/
void VWIN_NativeAlphaStretchBlt(void *pArg1, JImage *pNativeSrc, int dx, int dy, int dw, int dh, int sx, int sy, int sw, int sh, int alpha)
{
    if ((dw < 1) || (dh < 1) || (sw < 1) || (sh < 1))
        return;
    BYTE *pVram = (BYTE *)pArg1;
    JImage_mem_ARGB8888 *pImg = (JImage_mem_ARGB8888 *)pNativeSrc;

    int vsy = (sh << 16) / dh;
    int csy = 0;

    for (int y = 0; y < dh; y++)
    {
        if (((dy + y) < miny) || ((dy + y) >= maxy))
            continue;

        DWORD *s = (DWORD *)pImg->GetPixelAddress(sx, sy);
        DWORD *d = (DWORD *)(pVram + ((dy + y) * maxx * 4) + (dx * 4));
        //		WORD* d = (WORD*)(pVram + ((dy + y) * maxx * 2) + (dx * 2));

        int vsx = (sw << 16) / dw;
        int csx = 0;

        for (int x = 0; x < dw; x++)
        {
            DWORD pixel = 0xff000000;

            if ((sx >= 0) && (sx < pImg->w()))
                pixel = *s;

            if (((dx + x) >= minx) && ((dx + x) < maxx))
            {
                if (alpha < 0) // set alpha
                {
                    *d = pixel;
                }
                else
                {
                    BYTE a1 = GetA(pixel) * alpha / 255;
                    BYTE r1 = GetR(pixel);
                    BYTE g1 = GetG(pixel);
                    BYTE b1 = GetB(pixel);

                    if (a1 > 0)
                    {
                        DWORD dp = *d;

                        BYTE a2 = 255 - a1;
                        BYTE r2 = GetR(dp);
                        BYTE g2 = GetG(dp);
                        BYTE b2 = GetB(dp);

                        //*d = argb(255, (r1*a1/255 + r2*a2/255), (g1*a1/255 + g2*a2/255), (b1*a1/255 + b2*a2/255));
                        *d = argb(255, (r1 * a1 + r2 * a2) >> 8, (g1 * a1 + g2 * a2) >> 8, (b1 * a1 + b2 * a2) >> 8);
                        //						*d = (((r1*a1 + r2*a2) >> 11) << 11) | (((g1*a1 + g2*a2) >> 10) << 5) | ((b1*a1 + b2*a2) >> 11);
                    }
                }
            }

            d++;
            csx += vsx;
            if (csx >= 65536)
            {
                s += (csx >> 16);
                csx &= 0xffff;
            }
        }

        csy += vsy;
        if (csy >= 65536)
        {
            sy += (csy >> 16);
            csy &= 0xffff;
        }
    }
}
