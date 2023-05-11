#include <libcpp.h>
#include "JWin.h"

static const int minx = 0;
static const int miny = 0;
static const int maxx = 1280;
static const int maxy = 768;

Array<JWindow*>		JWindow::s_aRoot;
Map<String,String>	JWindow::s_mGlobalParam;
Array<JTimer>		JWindow::s_aTimer;
JWindow*			JWindow::s_pFocus = NULL;
PFN_MAINLOOPENTRY	JWindow::s_pfnMainLoopEntry = NULL;
JRect				JWindow::s_rCaret;
DWORD				JWindow::s_dwCaretTimer = 0;

static Map<String,JImage*> s_mPath2Image;
Map<String,int>		JDraw::s_mFontName2Id;

bool JKeypad_Init(JWindow* pParent)
{
	// return JKeypad::Init(pParent);
    return true;
}

JImage* LoadImage(const TCHAR* pPath)
{
	if( s_mPath2Image.has(pPath) )
		return s_mPath2Image[pPath];

	extern JImage* _LoadImage(const TCHAR* pPath);
	JImage* p = _LoadImage(pPath);
	if( p == NULL )
		return NULL;

	s_mPath2Image[pPath] = p;
	return p;
}


bool JWindow::Init(PFN_MAINLOOPENTRY pfn)
{
	s_pfnMainLoopEntry = pfn;

	JWindow::s_mGlobalParam[_T("LANGUAGE")] = _T("JPN");

	JWindow::s_mGlobalParam[_T("MESSAGEBOX.BUTTON.TEXT.OK")]		= _T("OK");
	JWindow::s_mGlobalParam[_T("MESSAGEBOX.BUTTON.TEXT.CANCEL")]	= _T("Cancel");			//_T("ƒLƒƒƒ“ƒZƒ‹");
	JWindow::s_mGlobalParam[_T("MESSAGEBOX.BUTTON.TEXT.YES")]		= _T("Yes");			//_T("‚Í‚¢");
	JWindow::s_mGlobalParam[_T("MESSAGEBOX.BUTTON.TEXT.NO")]		= _T("No");				//_T("‚¢‚¢‚¦");
	JWindow::s_mGlobalParam[_T("MESSAGEBOX.BUTTON.TEXT.RETRY")]		= _T("Retry");			//_T("ÄŽŽs");

	return true;
}

bool JWIN_Init(PFN_MAINLOOPENTRY pfn)
{
	JWindow::Init(pfn);
	JDraw::Init();

#ifndef _WINDOWS
	extern bool G2D_Init();
	G2D_Init();
#endif

	return true;
}

JImage* JWIN_NewJImage(int w, int h)
{
	JImage* p = JWIN_NewJImage();
	if( !p->Create(w, h) )
	{
		delete p;
		return NULL;
	}
	return p;
}

/******************************************************************************/
void JWIN_NativeAlphaStretchBlt(void* pArg1, JImage* pNativeSrc, int dx, int dy, int dw, int dh, int sx, int sy, int sw, int sh, int alpha)
{
	if( (dw < 1) || (dh < 1) || (sw < 1) || (sh < 1) )
		return;
	BYTE* pVram = (BYTE*)pArg1;
	JImage_mem_ARGB8888* pImg = (JImage_mem_ARGB8888*)pNativeSrc;

	int vsy = (sh << 16) / dh;
	int csy = 0;

	for( int y=0 ; y<dh ; y++ )
	{
		if( ((dy + y) < miny) || ((dy + y) >= maxy) )
			continue;

		DWORD* s = (DWORD*)pImg->GetPixelAddress(sx, sy);
		DWORD* d = (DWORD*)(pVram + ((dy + y) * maxx * 4) + (dx * 4));
//		WORD* d = (WORD*)(pVram + ((dy + y) * maxx * 2) + (dx * 2));

		int vsx = (sw << 16) / dw;
		int csx = 0;

		for( int x=0 ; x<dw ; x++ )
		{
			DWORD pixel = 0xff000000;

			if( (sx >= 0) && (sx < pImg->w()) ) 
				pixel = *s;

			if( ((dx + x) >= minx) && ((dx + x) < maxx) )
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

						BYTE a2 = 255 - a1;
						BYTE r2 = GetR(dp);
						BYTE g2 = GetG(dp);
						BYTE b2 = GetB(dp);

						//*d = argb(255, (r1*a1/255 + r2*a2/255), (g1*a1/255 + g2*a2/255), (b1*a1/255 + b2*a2/255));
						*d = argb(255, (r1*a1 + r2*a2) >> 8, (g1*a1 + g2*a2) >> 8, (b1*a1 + b2*a2) >> 8);
//						*d = (((r1*a1 + r2*a2) >> 11) << 11) | (((g1*a1 + g2*a2) >> 10) << 5) | ((b1*a1 + b2*a2) >> 11);
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

		csy += vsy;
		if( csy >= 65536 )
		{
			sy += (csy >> 16);
			csy &= 0xffff;
		}
	}
}

/******************************************************************************/
void JWIN_NativeAlphaBlt(void* pArg1, int cl, int ct, int cr, int cb, JImage* pSrc, int dx, int dy, int dw, int dh, int alpha)
{
	//
	int sx = 0;
	int sy = 0;
	int sw = dw;
	int sh = dh;

	// calc clip
	if( dx < cl )
	{
		int v = cl - dx;
		dx += v;
		dw -= v;
		sx += v;
		sw -= v;
	}
	if( (dx + dw) > cr )
	{
		int v = (dx + dw) - cr;
		dw -= v;
		sw -= v;
	}
	if( dy < ct )
	{
		int v = ct - dy;
		dy += v;
		dh -= v;
		sy += v;
		sh -= v;
	}
	if( (dy + dh) > cb )
	{
		int v = (dy + dh) - cb;
		dh -= v;
		sh -= v;
	}

	// render main
	if( (dw > 0) && (dh > 0) && (sw > 0) && (sh > 0) )
		JWIN_NativeAlphaStretchBlt(pArg1, pSrc, dx, dy, dw, dh, sx, sy, sw, sh, alpha);
}

/******************************************************************************/
void JWIN_WorkRootWindow_10ms(JRootWindow* pRoot, void* pArg1, void* pArg2, void* pArg3)
{
	DWORD dwNow = pRoot->getTime();

	//
	pRoot->DoTimer_10ms();
	if( pArg1 == NULL )
		return;

	// repaint invalid windows
	JDraw_mem_ARGB8888 vdraw;
	pRoot->DoPaint(&vdraw, 0, 0);

	// render main
	{
		Array<JWindow*> a;
		pRoot->EnumVisibleChildren(&a);

		for( int i=0 ; i<a.GetSize() ; i++ )
		{
			JWindow* pSrc = a[i];

			if( pSrc->isRoot() )
				continue;

			JRect rSrc;
			pSrc->GetGlobalRect(&rSrc);

			if( pSrc->abspos() )
			{
				rSrc.x -= pRoot->m_rWindow.x;
				rSrc.y -= pRoot->m_rWindow.y;
			}

			if( (pSrc->m_pOffScreen != NULL) && (rSrc.w == pSrc->m_pOffScreen->w()) && (rSrc.h == pSrc->m_pOffScreen->h()) )
			{
#if 0
				// virtual scroll all children
				{
					if( pSrc->m_pParent->isScrollWindow() && !(pSrc->isScrollBar()) )
					{
						JScrollWindow* sw = (JScrollWindow*)(pSrc->m_pParent);
						rSrc.x -= sw->m_ctlScrollBarH.GetPos();
						rSrc.y -= sw->m_ctlScrollBarV.GetPos();
					}
				}
#endif 
				// merge clip rect and alpha
				int cl = 0;
				int ct = 0;
				int cr = maxx;
				int cb = maxy;
				int a = pSrc->m_nAlpha;

				for( JWindow* p=pSrc ; p!=NULL ; p=p->m_pParent )
				{
					JRect r;
					p->GetGlobalRect(&r);

					if( p == pSrc )
						r = rSrc;

					cl = max(cl, r.x);
					ct = max(ct, r.y);
					cr = min(cr, r.r());
					cb = min(cb, r.b());
					a = a * p->m_nAlpha / 255;
				}

				//
				JWIN_NativeAlphaBlt(pArg1, cl, ct, cr, cb, pSrc->m_pOffScreen, rSrc.x, rSrc.y, rSrc.w, rSrc.h, a);
			}
		}
	}

	// render caret
	{
		static JImage* pCaret = NULL;
		if( pCaret == NULL )
		{
			pCaret = JWIN_NewJImage(32, 32);
			for( int y=0 ; y<32 ; y++ )
			{
				BYTE* d = pCaret->GetPixelAddress(0, y);
				for( int x=0 ; x<32 ; x++ )
				{
					*d++ = 0;
					*d++ = 0;
					*d++ = 0;
					*d++ = 255;
				}
			}
		}

		if( JWindow::s_pFocus != NULL )
		{
			JRect& r = JWindow::s_rCaret;
			if( (r.w > 0) && (r.h > 0) )
			{
				if( (((dwNow - JWindow::s_dwCaretTimer) / 500) & 1) == 0 )
				{
					JWIN_NativeAlphaBlt(pArg1, 0, 0, maxx, maxy, pCaret, r.x + pRoot->m_rWindow.x, r.y + pRoot->m_rWindow.y, r.w, r.h, 255);
				}
			}
		}
	}
#if 1
	// render toast
	{
		int dy = pRoot->h() * 95 / 100;

		for( int i=(pRoot->m_aToast.GetSize() - 1) ; i>=0 ; i-- )
		{
			JToastInfo& ti = pRoot->m_aToast[i];

			if( ti.m_nState == 0 )
			{
				{
					{
						vdraw.m_pDst = NULL;

						vdraw.m_rClip.x = 0;
						vdraw.m_rClip.y = 0;
						vdraw.m_rClip.w = 99999;
						vdraw.m_rClip.h = 99999;

						vdraw.m_ptOffset.x = 0;
						vdraw.m_ptOffset.y = 0;

						vdraw.m_nStrokeWidth = 1;

						vdraw.m_nFontId = 0;
						vdraw.m_nFontSize = 16;
					}

					int fs = 24;
					vdraw.SelectFontSize(fs);
					JRect rText = vdraw.MeasureText(ti.m_sText);

					int iw = rText.w + 64*2;
					int ih = rText.h + 52*2;

					if( ti.m_sText.find(_T("\n")) < 0 )
						ih -= fs;

					if( iw < 200 )
						iw = 200;
					if( ih < 110 )
						ih = 110;

					ti.m_pImage = JWIN_NewJImage(iw, ih);
					vdraw.m_pDst = ti.m_pImage;
					vdraw.FillRect(0, 0, iw, ih, TRANS);

					JImage* pRes = pRoot->CommonImage();
					if( pRes != NULL )
					{
						pRoot->TileBlit(&vdraw, pRes, 0, 0, iw, ih, 0, 480, 192, 144, 255, 64, 52);
					}
					vdraw.Text(64, 35, ti.m_sText, WHITE);

					vdraw.m_pDst = NULL;
				}
				ti.m_nState = 1;
			}
			if( ti.m_nState == 1 )
			{
				ti.m_nState = 2;
			}
			if( (ti.m_nState >= 2) && (ti.m_nState <= 4) )
			{
				if( ti.m_nState == 2 )
				{
					ti.m_nAlpha += 25;
					if( ti.m_nAlpha > 255 )
					{
						ti.m_nAlpha = 255;
						ti.m_nState = 3;
					}
				}
				else if( ti.m_nState == 3 )
				{
					ti.m_nWait -= 10;
					if( ti.m_nWait <= 0 )
					{
						ti.m_nState = 4;
					}
				}
				else if( ti.m_nState == 4 )
				{
					ti.m_nAlpha -= 5;
					if( ti.m_nAlpha < 0 )
					{
						ti.m_nAlpha = 0;
						ti.m_nState = 9;
					}
				}
				if( dy >= 0 )
				{
					JWIN_NativeAlphaBlt(pArg1, 0, 0, maxx, maxy, ti.m_pImage, (pRoot->w() - ti.m_pImage->w()) / 2, dy - ti.m_pImage->h(), ti.m_pImage->w(), ti.m_pImage->h(), ti.m_nAlpha);
				}

				dy -= ti.m_pImage->h();
				dy += 16;
			}
		}
		for( int i=(pRoot->m_aToast.GetSize() - 1) ; i>=0 ; i-- )
		{
			if( pRoot->m_aToast[i].m_nState == 9 )
			{
				delete pRoot->m_aToast[i].m_pImage;
				pRoot->m_aToast.RemoveAt(i);
			}
		}
	}
#endif 
#if 0
	// exec and render animation
	if( pRoot->m_aAnim.GetSize() > 0 )
	{
		for( int i=0 ; i<pRoot->m_aAnim.GetSize() ; i++ )
		{
			VAnimationInfo& a = pRoot->m_aAnim[i];
			if( !a.calc(dwNow) )
			{
				if( a.pAutoShow != NULL )
				{
					a.pAutoShow->visible(true);
					if( a.pAutoShowPopup != NULL )
					{
						a.pAutoShow->EnterPopup(a.pAutoShow, a.pAutoShowPopup);
					}
				}
				if( a.pAutoHide != NULL )
				{
					a.pAutoHide->visible(false);
				}
				if( a.pAutoInvalidate != NULL )
				{
					a.pAutoInvalidate->Invalidate();
				}
				if( a.pOnComplete != NULL )
				{
					a.pOnComplete->on(pRoot, _T("animation.completed"), 0, NULL);
				}
			}

			if( a.pImage != NULL )
			{
				VWIN_NativeAlphaStretchBlt(pArg1, a.pImage, a.d.x, a.d.y, a.d.w, a.d.h, a.s.x, a.s.y, a.s.w, a.s.h, a.a);

				if( a.bRemoveReq && a.bAutoDeleteImage )
				{
					delete a.pImage;
					a.pImage = NULL;
				}
			}
			else if( a.pWindow != NULL )
			{
				a.pWindow->m_rWindow = a.d;
				a.pWindow->m_nAlpha = a.a;
			}
		}
		for( int i=(pRoot->m_aAnim.GetSize() - 1) ; i>=0 ; i-- )
		{
			if( pRoot->m_aAnim[i].bRemoveReq )
			{
				pRoot->m_aAnim.RemoveAt(i);
			}
		}
	}
#endif
#if 0
	//
	if( (String2)s_sScreenShotPath != _T("") )
	{
		VImage* pTmpImage = VWIN_ReadNativeImage(pArg1);
		if( pTmpImage )
		{
			VWIN_SaveImage((String2)s_sScreenShotPath, pTmpImage);
			pTmpImage->Destroy();
		}
		s_sScreenShotPath = _T("");
	}
#endif 
}

/******************************************************************************/
void JDraw::DrawImage(JImage* pImg, int dx, int dy, int dw, int dh, int sx, int sy, int sw, int sh, int nAlpha)
{
	dx += m_ptOffset.x;
	dy += m_ptOffset.y;
	pImg->Draw(this, dx, dy, dw, dh, sx, sy, sw, sh, nAlpha);
}

/******************************************************************************/
void JDraw::DrawImage(JImage* pImg, int dx, int dy, int dw, int dh, int nAlpha)
{
	dx += m_ptOffset.x;
	dy += m_ptOffset.y;
	pImg->Draw(this, dx, dy, dw, dh, 0, 0, pImg->w(), pImg->h(), nAlpha);
}

/******************************************************************************/
int JDraw::GetWidth()
{
	return m_pDst->w();
}

/******************************************************************************/
int JDraw::GetHeight()
{
	return m_pDst->h();
}

/******************************************************************************/
void JWindow::GetGlobalRect(JRect* pRect, bool bFixScrollParent/*=false*/)
{
	pRect->x = 0;
	pRect->y = 0;
	pRect->w = m_rWindow.w;
	pRect->h = m_rWindow.h;

	for( JWindow* p=this ; p ; )
	{
		pRect->x += p->m_rWindow.x;
		pRect->y += p->m_rWindow.y;
		p = p->m_pParent;
	}

	if( bFixScrollParent )
	{
#if 0
		FixScrollParent(pRect, true);
#endif 	
	}
}
#if 0
/******************************************************************************/
void JWindow::FixScrollParent(JRect* pRect, bool bFixOverflow)
{
	if( m_pParent && m_pParent->isScrollWindow() && !isScrollBar() )
	{
		JScrollWindow* sw = (JScrollWindow*)m_pParent;
		VRect rParent;
		sw->GetGlobalRect(&rParent, false);

		pRect->x -= sw->m_ctlScrollBarH.GetPos();
		pRect->y -= sw->m_ctlScrollBarV.GetPos();

		if( bFixOverflow )
		{
			if( pRect->x < rParent.x )
			{
				int m = rParent.x - pRect->x;
				pRect->x = rParent.x;
				pRect->w -= m;
			}
			if( pRect->y < rParent.y )
			{
				int m = rParent.y - pRect->y;
				pRect->y = rParent.y;
				pRect->h -= m;
			}
			if( pRect->r() > rParent.r() )
			{
				int m = pRect->r() - rParent.r();
				pRect->w -= m;
			}
			if( pRect->b() > rParent.b() )
			{
				int m = pRect->b() - rParent.b();
				pRect->h -= m;
			}
		}
	}
}
#endif 

