#pragma once

typedef void (*PFN_MAINLOOPENTRY)(void);

class JRootWindow;

class JWindow;

typedef Array<JWindow*>		JWindowPtrArray;

class JWindow
{
private:
    /* data */
public:
    JImage*				m_pOffScreen;
	JImage*				m_pResource;
	JImage*				m_pResource2;
	int					m_nAlpha;

	String				m_sOrgXml;
	String				m_sTag;
	// CXmlNode*			m_pXmlNode;

	JColor				m_cFg;
	String				m_sFontName;
	String				m_sFontNameSv;
	int					m_nFontSize;
	int					m_nFontSizeSv;
	String				m_sText;

	JColor				m_cBg;
	String				m_sId;
	JRect				m_rWindow;
	JRect				m_rWindowSv;
	JRect				m_rPad;
	JRect				m_rKeypadHint;
	Array<JWindow*>		m_aChildren;
	void*				m_pNativeOwner;
	JWindow*			m_pParent;
	JListener*			m_pEventListener;
	bool				m_bVisible;
	bool				m_bEnable;
	bool				m_bInvalid;
	bool				m_bEnter;
	JDockInfoArray		m_aDockInfo;
	int					m_nTextAlign;
	int					m_nDoPaintLevel;
	int					m_nModalResult;
	bool				m_bAbsPos;

	JWindow*			m_pWinLastTouch;
	JPoint				m_ptTouchStart;
	DWORD				m_nTouchStartTime;
	JPoint				m_ptTouchEnd;
	DWORD				m_nTouchEndTime;
	JPoint				m_ptTouchMove;
	DWORD				m_nTouchMoveTime;
	int					m_nLastMoveX;
	int					m_nLastMoveY;
	int					m_nVelocityX;
	int					m_nVelocityY;
	int					m_nAfterSwipeX;
	int					m_nAfterSwipeY;
	bool				m_bCanClick;
	DWORD				m_nLastClickTime;
	int					m_nClick;
	bool				m_bSwipeDetected;
	bool				m_bCanScale;
	String				m_sScript;
	JWindow*			m_pOwner;
	String				m_sFocusNext;
	bool				m_bLButtonDownOperated;

	Map<String,String>	m_mAttributes;

	int					m_nTextPosX;
	int					m_nTextPosY;
	int					m_nCaretPosX;
	int					m_nCaretPosY;
	bool				m_bNoInput;
	bool				m_bPrivateWindow;
	bool				m_bHasFrame;

	JImage*				m_pResourceIcon;
	int					m_nIconSize;
	int					m_nIconIndex;

	TCHAR				m_cPassword;

	JColor				m_cChangedFg;
	String				m_sDefaultText;
public:
    static JWindow*				s_pFocus;
	static JWindow*				s_pCapture;
	// static Array<JModalInfo>	s_ModalStack;
	// static Array<JPopupInfo>	s_PopupStack;
	static Array<JTimer>		s_aTimer;
	static Array<JWindow*>		s_aRoot;
	static PFN_MAINLOOPENTRY	s_pfnMainLoopEntry;
	static Map<String,String>	s_mGlobalParam;
	static JRect				s_rCaret;
	static DWORD				s_dwCaretTimer;
public:
    static const int		TIMER_AUTO_REDRAW		= 0x10000001;
	static const int		TIMER_AUTO_UNLOAD		= 0x10000002;

	static const JColor		COLOR_BG				= rgb(255,255,255);
	static const JColor		COLOR_FG				= rgb(0,0,0);
	static const JColor		COLOR_ACTIVE_CURSOR		= rgb(160,140,230);
	static const JColor		COLOR_INACTIVE_CURSOR	= rgb(180,160,255);
	static const JColor		COLOR_ACTIVE_BG			= rgb(110,90,180);
	static const JColor		COLOR_ACTIVE_FG			= rgb(255,255,255);
	static const JColor		COLOR_TB_BG				= rgb(230,220,255);
	static const JColor		COLOR_DIALOG			= rgb(240,240,240);
	static const JColor		COLOR_CTL_N				= rgb(180,180,230);
	static const JColor		COLOR_CTL_A				= rgb(90,60,180);
	static const JColor		COLOR_DISABLE_FRAME		= rgb(210,210,210);
	static const JColor		COLOR_DISABLE_BG		= rgb(240,240,240);
	static const JColor		COLOR_DISABLE_FG		= rgb(100,100,100);
	static const JColor		COLOR_SCROLL_BG			= rgb(210,210,230);
	static const JColor		COLOR_SCROLL_FG			= rgb(160,150,220);
	static const JColor		COLOR_LOAD_ERROR		= rgb(255,0,0);

	static const int		SCROLLBAR_W				= 16;

	static const int		CLICK_IGNORE_LENGTH		= 20;
	static const int		DBLCLICK_MS				= 300;
	static const int		LONGPRESS_MS			= 1000;

	static const int		FLICK_MS				= 300;
	static const int		FLICK_LENGTH			= 10;
	static const int		FLICK_IGNORE_LENGTH		= 30;

	static const int		SWIPE_LENGTH			= 16;
	static const int		AFTER_SWIPE_VELOCITY	= 2;
public:

	static bool Init(PFN_MAINLOOPENTRY pfn);
    JWindow(/* args */)
    {
        m_bEnable = true;
        m_bEnter = false;
        m_sText = _T("");
    }

    virtual bool IsEnter()
	{
		return m_bEnter;
	}

	virtual bool IsHover()
	{
		return IsEnter();
	}

	virtual bool hovered()
	{
		return IsHover();
	}

	virtual String text()
	{
		return m_sText;
	}


    virtual bool enable()
	{
		return m_bEnable;
	}

	virtual bool disable()
	{
		return !enable();
	}

	virtual void enable(bool b)
	{
		m_bEnable = b;
		Invalidate();
	}

    virtual void SetTimer(int nId, int nElapse, bool bDisableWhenInvisible=true, bool bCyclic=true)
	{
		for( int i=0 ; i<s_aTimer.GetSize() ; i++ )
		{
			JTimer& t = s_aTimer[i];
			if( (t.pWindow == this) && (t.nId == nId) )
			{
				t.nPeriod = t.nRemain = nElapse;
				t.bCyclic = bCyclic;
				t.bDisableWhenInvisible = bDisableWhenInvisible;
				return;
			}
		}

		JTimer t;
		{
			t.pWindow = this;
			t.nId = nId;
			t.nPeriod = t.nRemain = nElapse;
			t.bCyclic = bCyclic;
			t.bDisableWhenInvisible = bDisableWhenInvisible;
		}
		s_aTimer.Add(t);
	}

    virtual bool KillTimer(int nId)
	{
		for( int i=0 ; i<s_aTimer.GetSize() ; i++ )
		{
			JTimer& t = s_aTimer[i];
			if( (t.pWindow == this) && (t.nId == nId) )
			{
				t.nPeriod = -1;
				return true;
			}
		}
		return false;
	}

	virtual void notify(JWindow* from, const TCHAR* pStr, int num, void* pPtr)
	{
		String str = pStr;

		if( m_pEventListener )
			m_pEventListener->on(m_pEventListener->m_pOwner, str, num, pPtr);
		else if( m_pOwner )
			m_pOwner->OnEvent(from->m_sId, str, num, pPtr);
		else
		{
			JWindow* pTo;
			{
				pTo = m_pParent;
				while( pTo->ThruEvent() )
				{
					if( pTo->m_pParent == NULL )
					{
						break;;
					}
					pTo = pTo->m_pParent;
				}
			}
			pTo->OnEvent(from->m_sId, str, num, pPtr);
		}
	}

	virtual bool PreCommand(const String& sId)
	{
		return true;
	}

	virtual bool OnCommand(const String& sId)
	{
		return false;
	}

	virtual bool DoCommand(const String& sId)
	{
		if( m_pOwner != NULL )
		{
			if( !m_pOwner->PreCommand(sId) )
				return false;
			return m_pOwner->OnCommand(sId);
		}
		if( m_pParent != NULL )
		{
			for( JWindow* pTarget=m_pParent ; pTarget ; )
			{
				if( pTarget->ThruEvent() )
				{
					pTarget = pTarget->m_pParent;
					continue;
				}
				if( !pTarget->PreCommand(sId) )
					return false;
				return pTarget->OnCommand(sId);
			}
		}
		return false;
	}

	virtual void OnEvent(const String sFromId, const String str, int num, void* pPtr)
	{
	}

	virtual bool ThruEvent()
	{
		return false;
	}

	virtual void ClearInvalidateFlag()
	{
		m_bInvalid = false;
	}

	virtual bool HasFocus()
	{
		return (s_pFocus == this);
	}

	virtual bool OnKey(bool bShift, bool bControl, int nKey)
	{
		return false;
	}

	virtual void OnTimer(int nId)
	{
		if( nId == TIMER_AUTO_REDRAW )
		{
			Invalidate();
			return;
		}
	}

    virtual JImage* CommonImage()
	{
		static JImage* p = NULL;
		if( p == NULL )
		{
			// p = LoadImage(_T("vwin.png"));
		}
		return p;
	}

	virtual bool visible() const
	{
		return m_bVisible;
	}

	virtual void visible(bool b, bool bNotify=true)
	{
		if( b == m_bVisible )
			return;
		m_bVisible = b;
		if( b )
			Invalidate();
		// else if( bNotify && m_pParent )
		// 	m_pParent->Invalidate();
	}

	virtual void SetResource(JImage* pResource)
	{
		printf("%s %d \n",__func__, __LINE__);
		m_pResource = pResource;
		Invalidate();
	}

	virtual void SetResource2(JImage* pResource2)
	{
		m_pResource2 = pResource2;
		Invalidate();
	}

	JWindow* owner()
	{
		return m_pOwner;
	}

	JWindow* owner(JWindow* pWindow)
	{
		JWindow* rc = owner();
		m_pOwner = pWindow;
		return rc;
	}

	virtual DWORD getTime()
	{
		return GetTickCount();
	}

	int w() const
	{
		return m_rWindow.w;
	}

	int h() const
	{
		return m_rWindow.h;
	}

    virtual void TileBlit(JDraw* pDst, JImage* pSrc, int dx, int dy, int dw, int dh, int sx, int sy, int sw, int sh, int alpha=255, int horz=0, int vert=0)
	{
		if( horz < 1 )
			horz = sw * 25 / 100;
		if( vert < 1 )
			vert = sh * 25 / 100;

		int h2 = horz * 2;
		int v2 = vert * 2;

		pDst->DrawImage(pSrc,		dx, dy, horz, vert,								sx, sy, horz, vert,	alpha);
		pDst->DrawImage(pSrc,		dx + horz, dy, dw - h2, vert,					sx + horz, sy, sw - h2, vert,	alpha);
		pDst->DrawImage(pSrc,		dx + dw - horz, dy, horz, vert,					sx + sw - horz, sy, horz, vert,	alpha);

		pDst->DrawImage(pSrc,		dx, dy + vert, horz, dh - v2,					sx, sy + vert, horz, sh - v2,	alpha);
		pDst->DrawImage(pSrc,		dx + horz, dy + vert, dw - h2, dh - v2,			sx + horz, sy + vert, sw - h2, sh - v2,	alpha);
		pDst->DrawImage(pSrc,		dx + dw - horz, dy + vert, horz, dh - v2,		sx + sw - horz, sy + vert, horz, sh - v2,	alpha);

		pDst->DrawImage(pSrc,		dx, dy + dh - vert, horz, vert,					sx, sy + sh - vert, horz, vert,	alpha);
		pDst->DrawImage(pSrc,		dx + horz, dy + dh - vert, dw - h2, vert,		sx + horz, sy + sh - vert, sw - h2, vert,	alpha);
		pDst->DrawImage(pSrc,		dx + dw - horz, dy + dh - vert, horz, vert,		sx + sw - horz, sy + sh - vert, horz, vert,	alpha);
	}

	virtual void TileBlit3H(JDraw* pDst, JImage* pSrc, int dx, int dy, int dw, int dh, int sx, int sy, int sw, int sh, int alpha=255, int horz=0)
	{
		if( horz < 1 )
			horz = sw * 33 / 100;

		int h2 = horz * 2;

		pDst->DrawImage(pSrc,		dx, dy, horz, dh,								sx, sy,				horz,		sh, alpha);
		pDst->DrawImage(pSrc,		dx + horz, dy, dw - h2, dh,						sx + horz, sy,		sw - h2,	sh, alpha);
		pDst->DrawImage(pSrc,		dx + dw - horz, dy, horz, dh,					sx + sw - horz, sy,	horz,		sh, alpha);
	}

	virtual void TileBlit3V(JDraw* pDst, JImage* pSrc, int dx, int dy, int dw, int dh, int sx, int sy, int sw, int sh, int alpha=255, int vert=0)
	{
		if( vert < 1 )
			vert = sw * 33 / 100;

		int v2 = vert * 2;

		pDst->DrawImage(pSrc,		dx, dy, dw, vert,								sx, sy,				sw, vert,		alpha);
		pDst->DrawImage(pSrc,		dx, dy + vert, dw, dh - v2,						sx, sy + vert,		sw, sh - v2,	alpha);
		pDst->DrawImage(pSrc,		dx, dy + dh - vert, dw, vert,					sx, sy + sh - vert,	sw, vert,		alpha);
	}

	virtual void TileBlit3(JDraw* pDst, JImage* pSrc, int dx, int dy, int dw, int dh, int sx, int sy, int sw, int sh, int alpha=255, int horz=0)
	{
		TileBlit3H(pDst, pSrc, dx, dy, dw, dh, sx, sy, sw, sh, alpha, ((horz >= 0) ? horz : (sw * 25 / 100)));
	}

    virtual bool Create(const TCHAR* pId, int x, int y, int w, int h, JWindow* pParent=NULL)
	{
		if( (pId == NULL) || (w < 1) || (h < 1) )
			return false;

		m_sId = pId;
		m_rWindow.set(x, y, w, h);
		m_pParent = pParent;
		if( m_pParent )
			m_pParent->m_aChildren.Add(this);
		if( !OnCreate() )
			return false;
		if( pParent == NULL )
			s_aRoot.Add(this);

		Invalidate();

		return true;
	}

    virtual bool OnCreate()
	{
		return true;
	}

    virtual void Invalidate()
	{
		m_bInvalid = true;
		// printf("%s %d \n",__func__, __LINE__);
	}

    ~JWindow()
	{
		
	}
};





