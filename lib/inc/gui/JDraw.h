#pragma once 

#define ABS(N)		(((N)>=0) ? (N) : (-(N)))

class JImage;

class JDraw
{
public:
    static Map<String,int>	s_mFontName2Id;

    enum ta
	{
		taLEFT		= 1,
		taCENTER	= 2,
		taRIGHT		= 4,

		taTOP		= 8,
		taMIDDLE	= 16,
		taBOTTOM	= 32
	};

    JImage*		m_pDst;
	JRect		m_rClip;
	JPoint		m_ptOffset;
	JColor		m_cFillColor;
	JColor		m_cStrokeColor;
	int			m_nStrokeWidth;

	int			m_nFontId;
	int			m_nFontSize;

private:

public:
    JDraw()
	{
		m_pDst = NULL;

		m_rClip.x = 0;
		m_rClip.y = 0;
		m_rClip.w = 99999;
		m_rClip.h = 99999;

		m_ptOffset.x = 0;
		m_ptOffset.y = 0;

		m_nStrokeWidth = 1;

		m_nFontId = 0;
		m_nFontSize = 16;
	}
	~JDraw()
    {

    }

	/*****************************************************************************/
	/**
	 * highest drawing layer
	 */
	/*****************************************************************************/
	virtual bool FillTriangle(int x, int y, int w, int h, JColor c, int dir)
	{
		if( dir == 2 )
		{
			JPoint points[] =
			{
				JPoint(x, y),
				JPoint(x + w, y),
				JPoint(x + (w / 2), y + h),
			};
			FillPolygon(points, 3, c);
			return true;
		}
		return false;
	}

	virtual bool FrameSerrate(int x, int y, int w, int h, int nAngleStart, int nAngleStep, int nRadius, JColor cStroke, int nStrokeWidth)
	{
		return FrameSerrate2(x, y, w, h, nAngleStart, nAngleStep, nRadius, nRadius, cStroke, nStrokeWidth);
	}
	virtual bool FillSerrate(int x, int y, int w, int h, int nAngleStart, int nAngleStep, int nRadius, JColor cFill)
	{
		return FillSerrate2(x, y, w, h, nAngleStart, nAngleStep, nRadius, nRadius, cFill);
	}
	virtual bool Serrate(int x, int y, int w, int h, int nAngleStart, int nAngleStep, int nRadius, JColor cStroke, int nStrokeWidth, JColor cFill)
	{
		return Serrate2(x, y, w, h, nAngleStart, nAngleStep, nRadius, nRadius, cStroke, nStrokeWidth, cFill);
	}

	virtual bool FrameSerrate2(int x, int y, int w, int h, int nAngleStart, int nAngleStep, int nRadius1, int nRadius2, JColor cStroke, int nStrokeWidth)
	{
		return Serrate2(x, y, w, h, nAngleStart, nAngleStep, nRadius1, nRadius2, cStroke, nStrokeWidth, TRANS);
	}
	virtual bool FillSerrate2(int x, int y, int w, int h, int nAngleStart, int nAngleStep, int nRadius1, int nRadius2, JColor cFill)
	{
		return Serrate2(x, y, w, h, nAngleStart, nAngleStep, nRadius1, nRadius2, TRANS, 1, cFill);
	}
	virtual bool Serrate2(int x, int y, int w, int h, int nAngleStart, int nAngleStep, int nRadius1, int nRadius2, JColor cStroke, int nStrokeWidth, JColor cFill)
	{
		if( nAngleStep <= 5 )
			return false;

		JPoint points[64];
		int nPoints = 0;

		int r[2] = {nRadius1, nRadius2};
		int ir = 0;

		for( int i=0 ; i<360 ; i+=nAngleStep )
		{
			double t = (i + nAngleStart) * 3.14159265359 / 180;
			points[nPoints].x = (int)(r[ir] * cos(t));
			points[nPoints].y = (int)(r[ir] * sin(t));
			nPoints++;
			ir = (ir + 1) % 2;
		}

		if( cFill != TRANS )
			FillPolygon(points, nPoints, cFill, x + (w / 2), y + (h / 2));
		if( cStroke != TRANS )
			FramePolygon(points, nPoints, cStroke, nStrokeWidth, x + (w / 2), y + (h / 2));
		return true;
	}

	/*****************************************************************************/
	/**
	 * api type definition & geometry conversion layer
	 */
	/*****************************************************************************/
	virtual void SetPixel(int x, int y, JColor c, int nStrokeWidth)
	{
		x += m_ptOffset.x;
		y += m_ptOffset.y;
		m_cStrokeColor = c;
		m_nStrokeWidth = nStrokeWidth;
		impl_SetPixel(x, y);
	}

	virtual void LineH(int x, int y, int w, JColor c)
	{
		x += m_ptOffset.x;
		y += m_ptOffset.y;
		m_cFillColor = c;
		impl_LineH(x, y, w);
	}

	virtual void LineV(int x, int y, int h, JColor c)
	{
		x += m_ptOffset.x;
		y += m_ptOffset.y;
		m_cFillColor = c;
		impl_LineV(x, y, h);
	}

	virtual void Line(int x1, int y1, int x2, int y2, JColor c, int nStrokeWidth)
	{
		x1 += m_ptOffset.x;
		y1 += m_ptOffset.y;
		x2 += m_ptOffset.x;
		y2 += m_ptOffset.y;
		m_cStrokeColor = c;
		m_nStrokeWidth = nStrokeWidth;
		impl_Line(x1, y1, x2, y2);
	}

	/*****************************************************************************/
	virtual void FrameRect(int x, int y, int w, int h, JColor cStroke, int nStrokeWidth)
	{
		//x += m_ptOffset.x;
		//y += m_ptOffset.y;
		//m_cStrokeColor = cStroke;
		//m_nStrokeWidth = nStrokeWidth;
		//impl_FrameRect(x, y, w, h);
		int n = nStrokeWidth;
		FillRect(x, y, w, n, cStroke);
		FillRect(x, y, n, h, cStroke);
		FillRect(x + w - n, y, n, h, cStroke);
		FillRect(x, y + h - n, w, n, cStroke);
	}
	virtual void FrameRect(const JRect* r, JColor cStroke, int nStrokeWidth)
	{
		FrameRect(r->x, r->y, r->w, r->h, cStroke, nStrokeWidth);
	}

	//
	virtual void FillRect(int x, int y, int w, int h, JColor cFill)
	{
		x += m_ptOffset.x;
		y += m_ptOffset.y;
		m_cFillColor = cFill;
		impl_FillRect(x, y, w, h);
	}
	virtual void FillRect(const JRect* r, JColor cFill)
	{
		FillRect(r->x, r->y, r->w, r->h, cFill);
	}

	//
	virtual void Rectangle(int x, int y, int w, int h, JColor cStroke, int nStrokeWidth, JColor cFill)
	{
		FillRect(x, y, w, h, cFill);
		FrameRect(x, y, w, h, cStroke, nStrokeWidth);
	}
	virtual void Rectangle(const JRect* r, JColor cStroke, int nStrokeWidth, JColor cFill)
	{
		Rectangle(r->x, r->y, r->w, r->h, cStroke, nStrokeWidth, cFill);
	}

	/*****************************************************************************/
	virtual void FrameRoundRect(int x, int y, int w, int h, JColor cStroke, int nStrokeWidth, int nRoundRadius)
	{
		x += m_ptOffset.x;
		y += m_ptOffset.y;
		m_cStrokeColor = cStroke;
		m_nStrokeWidth = nStrokeWidth;
		impl_FrameRoundRect(x, y, w, h, nRoundRadius);
	}
	virtual void FrameRoundRect(const JRect* r, JColor c, int nStrokeWidth, int nRoundRadius)
	{
		FrameRoundRect(r->x, r->y, r->w, r->h, c, nStrokeWidth, nRoundRadius);
	}

	//
	virtual void FillRoundRect(int x, int y, int w, int h, JColor cFill, int nRoundRadius)
	{
		x += m_ptOffset.x;
		y += m_ptOffset.y;
		m_cFillColor = cFill;
		impl_FillRoundRect(x, y, w, h, nRoundRadius);
	}
	virtual void FillRoundRect(const JRect* r, JColor c, int nRoundRadius)
	{
		FillRoundRect(r->x, r->y, r->w, r->h, c, nRoundRadius);
	}

	//
	virtual void RoundRect(int x, int y, int w, int h, JColor cStroke, int nStrokeWidth, JColor cFill, int nRoundRadius)
	{
		FillRoundRect(x, y, w, h, cFill, nRoundRadius);
		FrameRoundRect(x, y, w, h, cStroke, nStrokeWidth, nRoundRadius);
	}
	virtual void RoundRect(const JRect* r, JColor cStroke, int nStrokeWidth, JColor cFill, int nRoundRadius)
	{
		RoundRect(r->x, r->y, r->w, r->h, cStroke, nStrokeWidth, cFill, nRoundRadius);
	}

	/*****************************************************************************/
	virtual void FrameEllipse(int x, int y, int w, int h, JColor cStroke, int nStrokeWidth)
	{
		x += m_ptOffset.x;
		y += m_ptOffset.y;
		m_cStrokeColor = cStroke;
		m_nStrokeWidth = nStrokeWidth;
		impl_FrameEllipse(x, y, w, h);
	}
	virtual void FrameEllipse(const JRect* r, JColor cStroke, int nStrokeWidth)
	{
		FrameEllipse(r->x, r->y, r->w, r->h, cStroke, nStrokeWidth);
	}

	//
	virtual void FillEllipse(int x, int y, int w, int h, JColor cFill)
	{
		x += m_ptOffset.x;
		y += m_ptOffset.y;
		m_cFillColor = cFill;
		impl_FillEllipse(x, y, w, h);
	}
	virtual void FillEllipse(const JRect* r, JColor cFill)
	{
		FillEllipse(r->x, r->y, r->w, r->h, cFill);
	}

	//
	virtual void Ellipse(int x, int y, int w, int h, JColor cStroke, int nStrokeWidth, JColor cFill)
	{
		FillEllipse(x, y, w, h, cFill);
		FrameEllipse(x, y, w, h, cStroke, nStrokeWidth);
	}
	virtual void Ellipse(const JRect* r, JColor cStroke, int nStrokeWidth, JColor cFill)
	{
		Ellipse(r->x, r->y, r->w, r->h, cStroke, nStrokeWidth, cFill);
	}

	/*****************************************************************************/
	virtual void FramePolygon(const JPoint* pPoints, int nPoints, JColor c, int nStrokeWidth, int ox=0, int oy=0)
	{
		for( int i=0 ; i<nPoints ; i++ )
		{
			int j = ((i + 1) < nPoints) ? (i + 1) : 0;
			Line(pPoints[i].x + ox, pPoints[i].y + oy, pPoints[j].x + ox, pPoints[j].y + oy, c, nStrokeWidth);
		}
	}

	virtual void FillPolygon(const JPoint* pPoints, int nPoints, JColor cFill, int ox=0, int oy=0)
	{
		JPoint tmp[256];
		for( int i=0 ; i<nPoints ; i++ )
		{
			tmp[i].x = pPoints[i].x + m_ptOffset.x + ox;
			tmp[i].y = pPoints[i].y + m_ptOffset.y + oy;
		}
		m_cFillColor = cFill;
		impl_FillPolygon(tmp, nPoints);
	}

	virtual void Polygon(const JPoint* pPoints, int nPoints, JColor cStroke, int nStrokeWidth, JColor cFill, int ox=0, int oy=0)
	{
		FillPolygon(pPoints, nPoints, cFill, ox, oy);
		FramePolygon(pPoints, nPoints, cStroke, nStrokeWidth, ox, oy);
	}

	/*****************************************************************************/
	virtual void Text(int x, int y, int w, int h, const TCHAR* pText, JColor cFg, int nStyle, JColor cBg=TRANS, int nTextPosX=0, int nTextPosY=0)
	{
		if( cBg != TRANS )
			FillRect(x, y, w, h, cBg);

		int th = MeasureText(pText).h;

		int ty = y;
		if( nStyle & taMIDDLE )
			ty = y + (h - th) / 2;
		else if( nStyle & taBOTTOM )
			ty = y + (h - th);

		StringArray a = String(pText).split(_T("\n"));
		for( int i=0 ; i<a.GetSize() ; i++ )
		{
			JRect r = MeasureText(a[i]);

			int tx = x;
			if( nStyle & taCENTER )
				tx = x + (w - r.w) / 2;
			else if( nStyle & taRIGHT )
				tx = x + w - r.w;

			Text(tx, ty, a[i], cFg, nTextPosX, nTextPosY);

			//ty += (m_nFontSize * 110 / 100);
			ty += (r.h * 110 / 100);
		}
	}
	virtual void Text(JRect* pRect, const TCHAR* pText, JColor cFg, int nStyle, JColor cBg=TRANS, int nTextPosX=0, int nTextPosY=0)
	{
		Text(pRect->x, pRect->y, pRect->w, pRect->h, pText, cFg, nStyle, cBg, nTextPosX, nTextPosY);
	}

	virtual void Text(int x, int y, const TCHAR* pText, JColor c, int nTextPosX=0, int nTextPosY=0)
	{
		// if( c == TRANS )
		// 	return;

		// x += m_ptOffset.x;
		// y += m_ptOffset.y;

		// x += nTextPosX;
		// y += nTextPosY;

		// String2Array a = String(pText).splitExact(_T("\n"));
		// for( int i=0 ; i<a.GetSize() ; i++ )
		// {
		// 	impl_Text(x, y, c, a[i].c_str());

		// 	JRect r = MeasureText(a[i]);
		// 	y += (r.h * 110 / 100);
		// }
	}

	virtual JRect MeasureText(const TCHAR* pText)
	{
		return impl_MeasureText(pText);
	}

	virtual void DrawImage(JImage* pImg, int dx, int dy, int dw, int dh, int sx, int sy, int sw, int sh, int nAlpha);
	virtual void DrawImage(JImage* pImg, int dx, int dy, int dw, int dh, int nAlpha);

	/*****************************************************************************/
	/**
	 * simple implement layer
	 */
	virtual int GetWidth();

	virtual int GetHeight();

	virtual void impl_Line(int x1, int y1, int x2, int y2)
	{
		int w = x2 - x1;
		int h = y2 - y1;
		w += ((w >= 0) ? 1 : -1);
		h += ((h >= 0) ? 1 : -1);

		if( ABS(w) >= ABS(h) )
		{
			if( w == 0 )
			{
				impl_SetPixel(x1, y1);
				return;
			}

			if( w < 0 )
			{
				w = -w;
				h = -h;

				int t = x1;
				x1 = x2;
				x2 = t;

				t = y1;
				y1 = y2;
				y2 = t;
			}

			double a = (w == 0) ? 0 : (((double)h) / w);
			for( int i=0 ; i<w ; i++ )
			{
				impl_SetPixel(x1 + i, y1 + (int)(i * a));
			}
		}
		else
		{
			if( h < 0 )
			{
				w = -w;
				h = -h;

				int t = x1;
				x1 = x2;
				x2 = t;

				t = y1;
				y1 = y2;
				y2 = t;
			}

			double a = (h == 0) ? 0 : (((double)w) / h);
			for( int i=0 ; i<h ; i++ )
			{
				impl_SetPixel(x1 + (int)(i * a), y1 + i);
			}
		}
	}

	virtual void impl_FrameRect(int x, int y, int w, int h)
	{
		impl_Line(x, y, x + w - 1, y);
		impl_Line(x, y, x, y + h - 1);
		impl_Line(x + w - 1, y, x + w - 1, y + h - 1);
		impl_Line(x, y + h - 1, x + w, y + h - 1);
	}

	virtual void impl_FillRect(int x, int y, int w, int h)
	{
		for( int i=0 ; i<h ; i++ )
			impl_LineH(x, y + i, w);
	}

	virtual void impl_FrameEllipse(int x, int y, int w, int h)
	{
		w /= 2;
		h /= 2;
		if( (w < 1) || (h < 1) )
			return;

		int cx = x + w;
		int cy = y + h;

		int xx = w << 8;
		int yy = 0;
		int ix = 0;
		int iy = 0;

		while( xx >= 0 )
		{
			ix = xx >> 8;
			iy = yy >> 8;
			impl_SetPixel(cx+ix, cy+iy);
			impl_SetPixel(cx-ix, cy-iy);
			impl_SetPixel(cx-ix, cy+iy);
			impl_SetPixel(cx+ix, cy-iy);
			yy += ((xx * h / w) >> 8);
			xx -= ((yy * w / h) >> 8);
		}
	}

	virtual void impl_FillEllipse(int x, int y, int w, int h)
	{
		w /= 2;
		h /= 2;
		if( (w < 1) || (h < 1) )
			return;

		int cx = x + w;
		int cy = y + h;

		int xx = w << 8;
		int yy = 0;
		int ix = 0;
		int iy = 0;

		while( xx >= 0 )
		{
			ix = xx >> 8;
			iy = yy >> 8;
			impl_LineH(cx-ix, cy-iy, ix*2);
			impl_LineH(cx-ix, cy+iy, ix*2);
			yy += ((xx * h / w) >> 8);
			xx -= ((yy * w / h) >> 8);
		}
	}

	virtual void impl_FrameRoundRect(int x, int y, int w, int h, int r)
	{
		if( r < 1 )
			return;
		if( w < (r * 2) )
			return;
		if( h < (r * 2) )
			return;

		int cx1 = x + r;
		int cx2 = x + w - r;

		int cy1 = y + r;
		int cy2 = y + h - r;

		int xx = r << 8;
		int yy = 0;
		int ix = 0;
		int iy = 0;

		while( xx >= 0 )
		{
			ix = xx >> 8;
			iy = yy >> 8;
			impl_SetPixel(cx1-ix-0, cy1-iy-0);
			impl_SetPixel(cx2+ix-1, cy1-iy-0);
			impl_SetPixel(cx1-ix-0, cy2+iy-1);
			impl_SetPixel(cx2+ix-1, cy2+iy-1);
			yy += (xx >> 8);
			xx -= (yy >> 8);
		}

		impl_Line(x, cy1, x, cy1 + h - (r * 2));
		impl_Line(x+w-1, cy1, x+w-1, cy1 + h - (r * 2));

		impl_Line(cx1, y, cx1 + w - (r * 2), y);
		impl_Line(cx1, y+h-1, cx1 + w - (r * 2), y+h-1);
	}

	virtual void impl_FillRoundRect(int x, int y, int w, int h, int r)
	{
		if( r < 1 )
			return;
		if( w < (r * 2) )
			return;
		if( h < (r * 2) )
			return;

		int cx1 = x + r;
		int cx2 = x + w - r;

		int cy1 = y + r;
		int cy2 = y + h - r;

		int xx = r << 8;
		int yy = 0;
		int ix = 0;
		int iy = 0;

		while( xx >= 0 )
		{
			ix = xx >> 8;
			iy = yy >> 8;
			impl_LineH(cx1-ix, cy1-iy, (cx2+ix) - (cx1-ix));
			impl_LineH(cx1-ix, cy2+iy-1, (cx2+ix) - (cx1-ix));
			yy += (xx >> 8);
			xx -= (yy >> 8);
		}

		impl_FillRect(x+1, y+r, w-2, h-(r*2));
	}

	virtual void impl_FillPolygon(const JPoint* pPoints, int nPoints)
	{
		//
		if( nPoints >= 256 )
		{
			return;
		}
		//int w = GetWidth();
		int h = GetHeight();

		//
		int nTop, nBottom;
		{
			nTop = 99999;
			nBottom = 0;

			for( int i=0 ; i<nPoints ; i++ )
			{
				nTop = min(nTop, pPoints[i].y);
				nBottom = max(nBottom, pPoints[i].y);
			}

			if( nTop < 0 )
				nTop = 0;
			if( nBottom >= h )
				nBottom = (h - 1);
		}

		// operate all lines
		for( int y=nTop ; y<=nBottom ; y++ )
		{
			// correct cross points
			int xa[256];
			int xn = 0;

			for( int i=0 ; i<nPoints ; i++ )
			{
				int i0 = i;
				int i1 = (i + 1) % nPoints;
				int i2 = (i - 1 + nPoints) % nPoints;

				int y0 = pPoints[i0].y;
				int y1 = pPoints[i1].y;
				int y2 = pPoints[i2].y;

				if( (((y >= y0) && (y <= y1)) || ((y >= y1) && (y <= y0))) && (y0 != y1) )
				{
					int x0 = pPoints[i0].x;
					int x1 = pPoints[i1].x;

					int vx = x1 - x0;
					int vy = y1 - y0;

					if( y0 == y1 )
					{
						xa[xn++] = x0;
						xa[xn++] = x1;
					}
					else
					{
						xa[xn++] = x0 + ((y - y0) * vx) / vy;
						if( xn >= 2 )
						{
							if( xa[xn - 1] == xa[xn - 2] )
							{
								if( ((y1 >= y0) && (y2 >= y0)) || ((y1 < y0) && (y2 < y0)) )
								{
									//nop;
								}
								else
								{
									xn--;
									impl_LineH(xa[xn - 1], y, 1);
								}
							}
						}
					}
				}
			}

			// skip no effect line
			if( xn < 2 )
				continue;

			// sort xa[xn]
			{
				static class { public: static int CompareFunc(const void* pa, const void* pb)
				{
					int a = *(int*)pa;
					int b = *(int*)pb;
					return a - b;
				}} _;
				qsort(xa, xn, sizeof(int), _.CompareFunc);
			}

			// fill 0to1, 2to3, 4to5, ...
			for( int i=0 ; i<(xn - 1) ; i+=2 )
			{
				impl_LineH(xa[i + 0], y, xa[i + 1] - xa[i + 0] + 1);
			}
		}
	}

	/*****************************************************************************/
	static void InitFont()
	{
		g_FreeType.Init();
	}

	/**
	 * @param nFixedWidth 0.proportional font  100.fixed font
	 */
	static void LoadFont(const TCHAR* pName, const TCHAR* pPath, int nFixedWidth, int nBaseOffset, const TCHAR* pZenkakuFontName=NULL)
	{
		int nId = s_mFontName2Id.Keys().GetSize();
		s_mFontName2Id[pName] = nId;

		int nZenkakuFontId = pZenkakuFontName ? (s_mFontName2Id.has(pZenkakuFontName) ? s_mFontName2Id[pZenkakuFontName] : -1) : -1;

		g_FreeType.Add(nId, pPath, nFixedWidth, nBaseOffset, nZenkakuFontId);
	}

	bool SelectFont(int nId, int nHeight)
	{
		m_nFontId = nId;
		m_nFontSize = nHeight;
		g_FreeType.Select(nId, nHeight);
		return true;
	}

	bool SelectFont(const TCHAR* pName, int nHeight)
	{
		if( !s_mFontName2Id.has(pName) )
			return false;
		return SelectFont(s_mFontName2Id[pName], nHeight);
	}

	String GetFontName()
	{
		Array<String> a = s_mFontName2Id.Keys();
		for( int i=0 ; i<a.GetSize() ; i++ )
		{
			String name = a[i];
			if( s_mFontName2Id[name] == m_nFontId )
			{
				return name;
			}
		}
		return String(_T(""));
	}

	bool SelectFontName(const TCHAR* pName)
	{
		return SelectFont(s_mFontName2Id[pName], m_nFontSize);
	}

	bool SelectFontSize(int nHeight)
	{
		return SelectFont(m_nFontId, nHeight);
	}

	virtual void impl_Text(int x, int y, JColor c, const TCHAR* pText)
	{
		g_FreeType.Render(this, x, y, c, pText, false);
	}

	virtual JRect impl_MeasureText(const TCHAR* pText)
	{
		int w = 0;
		int h = 0;
		{
			String1Array a = String(pText).splitExact(_T("\n"));
			for( int i=0 ; i<a.GetSize() ; i++ )
			{
				w = max(w, g_FreeType.Render(this, 0, 0, 0, a[i], true));
				h += (g_FreeType.m_Info[0].font_type.height * 110 / 100);
			}
		}
		return JRect(0, 0, w, h);
	}

	/*****************************************************************************/
	static void Init()
	{
		InitFont();
	}

	/*****************************************************************************/
	virtual void impl_SetPixel(int x, int y) = 0;
	virtual void impl_LineH(int x, int y, int w) = 0;
	virtual void impl_LineV(int x, int y, int h) = 0;
};

