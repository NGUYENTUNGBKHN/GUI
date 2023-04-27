#pragma once

class JImage_mem_ARGB8888 : public JImage
{
private:
    /* data */
public:
    int     PIXEL;
    int     ALINE;
    int     LINE;
    BYTE*   m_pData;
public:
    // JImage_mem_ARGB8888(/* args */)
    // {

    // }
    // ~JImage_mem_ARGB8888()
    // {

    // }
    virtual unsigned char* GetPixelAddressFast(int x, int y) const
	{
		printf("%s %d \n",__func__, __LINE__);
		return m_pData + (y * LINE) + (x * PIXEL);
	}

	/**
	 * with stretch
	 */
	virtual void Draw(JImage* _pSrc, int dx, int dy, int dw, int dh, int sx, int sy, int sw, int sh, int alpha)
	{
		if( (dw <= 0) || (dh <= 0) )
		{
			return;
		}
		if( (dw == sw) && (dh == sh) )
		{
			Draw(_pSrc, dx, dy, dw, dh, sx, sy, alpha);
			return;
		}

		JImage_mem_ARGB8888* pSrc = (JImage_mem_ARGB8888*)_pSrc;

		int vsy = (sh << 16) / dh;
		int csy = 0;

		for( int y=0 ; y<dh ; y++ )
		{
			if( ((dy + y) >= 0) && ((dy + y) < h()) )
			{
				DWORD* s = (DWORD*)pSrc->GetPixelAddressFast(sx, sy);
				DWORD* d = (DWORD*)GetPixelAddressFast(dx, dy + y);

				int vsx = (sw << 16) / dw;
				int csx = 0;

				for( int x=0 ; x<dw ; x++ )
				{
					DWORD pixel = 0xff000000;

					if( (sx >= 0) && (sx < pSrc->w()) ) 
						pixel = *s;

					if( ((dx + x) >= 0) && ((dx + x) < w()) )
					{
						if( alpha < 0 )	// set alpha
						{
							*d = pixel;
						}
						else
						{
							BYTE a1 = GetA(pixel) * alpha / 255;
							BYTE r1 = GetR(pixel);
							BYTE g1 = GetG(pixel);
							BYTE b1 = GetB(pixel);

							if( a1 > 0 )
							{
								DWORD dp = *d;

								BYTE a2 = GetA(dp);
								BYTE r2 = GetR(dp);
								BYTE g2 = GetG(dp);
								BYTE b2 = GetB(dp);

								BYTE a3 = min(255, a1 + a2);
								a2 = 255 - a1;
								*d = argb(a3, (r1*a1/255 + r2*a2/255), (g1*a1/255 + g2*a2/255), (b1*a1/255 + b2*a2/255));
							}
						}
					}

					d++;
					csx += vsx;
					if( csx >= 65536 )
					{
						s += (csx >> 16);
						csx &= 0xffff;
					}
				}
			}

			csy += vsy;
			if( csy >= 65536 )
			{
				sy += (csy >> 16);
				csy &= 0xffff;
			}
		}
	}

	/**
	 * w/o stretch
	 */
	virtual void Draw(JImage* _pSrc, int dx, int dy, int dw, int dh, int sx, int sy, int alpha)
	{
		if( (dw <= 0) || (dh <= 0) )
		{
			return;
		}
		JImage_mem_ARGB8888* pSrc = (JImage_mem_ARGB8888*)_pSrc;

		for( int y=0 ; y<dh ; y++ )
		{
			if( ((dy + y) >= 0) && ((dy + y) < h()) )
			{
				DWORD* s = (DWORD*)pSrc->GetPixelAddressFast(sx, sy);
				DWORD* d = (DWORD*)GetPixelAddressFast(dx, dy + y);

				for( int x=0 ; x<dw ; x++ )
				{
					DWORD pixel = 0xff000000;

					if( (sx >= 0) && (sx < pSrc->w()) ) 
						pixel = *s;

					if( ((dx + x) >= 0) && ((dx + x) < w()) )
					{
						if( alpha == 1024 )		// set alpha only
						{
							pixel = (*d & 0x00ffffff) | (pixel & 0xff000000);
							*d = pixel;
						}
						else if( alpha < 0 )	// set alpha
						{
							*d = pixel;
						}
						else
						{
							BYTE a1 = GetA(pixel) * alpha / 255;
							BYTE r1 = GetR(pixel);
							BYTE g1 = GetG(pixel);
							BYTE b1 = GetB(pixel);

							if( a1 > 0 )
							{
								DWORD dp = *d;

								BYTE a2 = GetA(dp);
								BYTE r2 = GetR(dp);
								BYTE g2 = GetG(dp);
								BYTE b2 = GetB(dp);

								BYTE a3 = min(255, a1 + a2);
								a2 = 255 - a1;
								*d = argb(a3, (r1*a1/255 + r2*a2/255), (g1*a1/255 + g2*a2/255), (b1*a1/255 + b2*a2/255));
							}
						}
					}
					s++;
					d++;
				}
			}
			sy++;
		}
	}
};





