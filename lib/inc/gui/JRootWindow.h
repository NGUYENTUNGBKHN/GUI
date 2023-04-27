#pragma once

class JScreen;

class DelayLoadInfo
{
public:
    JScreen* m_pInstance;
    String   m_sPath;
    String      m_sId;
    // JWindow    m_pParent;
    bool        m_bLoaded;
};

class JRootWindow : public JWindow
{

    typedef JWindow super;
protected:
    JBusyWindow					m_wndBusy;
private:
    /* data */

public:
    Map<String,JScreen*>		m_mId2Screen;
	// Array<VAnimationInfo>		m_aAnim;
	JWindow*					m_pLastScreen;
	JWindow*					m_pActiveScreen;
	JWindow*					m_pInteractiveInBusy;
	StringArray					m_aHistoryStack;
	bool						(*m_pfnPreJump)(const TCHAR* pId);
	String						m_sTopId;
	// VToastInfoArray				m_aToast;
	Map<JScreen*,DelayLoadInfo>	m_mId2DelayLoadInfo;
	Array<JScreen*>				m_aUnloadReq;
public:
    JRootWindow(/* args */)
    {
        m_pLastScreen = NULL;
		m_pActiveScreen = NULL;
		m_pInteractiveInBusy = NULL;
		m_pfnPreJump = NULL;
		m_sTopId = _T("");
    }

    virtual bool isRoot()
    {
        return true;
    }

    virtual bool OnCreate()
	{
		if( !m_wndBusy.Create(_T("BUSY"), 0, 0, m_rWindow.w, m_rWindow.h, this) )
			return false;
		m_wndBusy.visible(false);

		if( !JKeypad_Init(this) )
		{
			//return false;
		}

		return true;
	}

	virtual bool Create(int x, int y, int w, int h, const TCHAR* pId=_T(""))
	{
		return super::Create(pId, x, y, w, h, NULL);
	}

	
	#if 0
	void UpdateBusy(const TCHAR* pMessage1)
	{
		m_wndBusy.UpdateMessage(pMessage1);
	}
	void EnterBusy(const TCHAR* pMessage1, const TCHAR* pMessage2, VWindow* pCancelOwner)
	{
		bool bBusy = IsBusy();
		m_wndBusy.Setup(pMessage1, pMessage2, pCancelOwner);

		if( !bBusy )
			EnterPopup(&m_wndBusy, NULL);
	}

	bool IsBusy()
	{
		return m_wndBusy.IsBusy();
	}

	void LeaveBusy()
	{
		if( IsBusy() )
			m_wndBusy.LeavePopup(NULL, _T(""), 0, NULL);
	}

	void ShowBusy()
	{
		m_wndBusy.Setup(_T(""), _T(""), NULL);
		BringChildToTop(&m_wndBusy);
	}

	void HideBusy()
	{
		if( m_pInteractiveInBusy )
		{
			m_pInteractiveInBusy->visible(false);
			m_pInteractiveInBusy = NULL;
		}
		m_wndBusy.SetHideReq();
	}

	void SetBusyResource(VImage* pImages, int nFrames, int nFrameDuration)
	{
		m_wndBusy.SetResource(pImages, nFrames, nFrameDuration);
	}

	void ShowBusy(VWindow* pInteractive, VWindow* pOwner)
	{
		m_pInteractiveInBusy = pInteractive;

		ShowBusy();
		m_pInteractiveInBusy->owner(pOwner);
		m_pInteractiveInBusy->visible(true);
		BringChildToTop(m_pInteractiveInBusy);
	}

	bool HasAnimation(const TCHAR* pId)
	{
		for( int i=0 ; i<m_aAnim.GetSize() ; i++ )
		{
			if( m_aAnim[i].sId == pId )
			{
				return true;
			}
		}
		return false;
	}

	bool RemoveAnimation(const TCHAR* pId)
	{
		for( int i=0 ; i<m_aAnim.GetSize() ; i++ )
		{
			if( m_aAnim[i].sId == pId )
			{
				if( m_aAnim[i].bAutoDeleteImage && m_aAnim[i].pImage )
				{
					delete m_aAnim[i].pImage;
					m_aAnim[i].pImage = NULL;
				}
				m_aAnim.RemoveAt(i);
				return true;
			}
		}
		return false;
	}

	bool SetAnimationAutoHide(const TCHAR* pId, VWindow* p)
	{
		for( int i=0 ; i<m_aAnim.GetSize() ; i++ )
		{
			if( m_aAnim[i].sId == pId )
			{
				m_aAnim[i].pAutoHide = p;
				return true;
			}
		}
		return false;
	}

	void AddAnimation(VAnimationInfo& ai)
	{
		if( ai.sId != _T("") )
			RemoveAnimation(ai.sId);
		m_aAnim.Add(ai);
	}

	void AddAnimation_SlideOut(VWindow* pWin, bool bToRight, int nDuration=500)
	{
		VRect r;
		pWin->GetGlobalRect(&r);

		VAnimationInfo a;
		{
			a.pImage = VWIN_NewVImage(r.w, r.h);
			a.bAutoDeleteImage = true;
			pWin->PrepareDragImage(a.pImage, 0, 0, false);

			a.bVirtual = true;

			a.a0 = 255;
			a.a100 = 0;

			if( bToRight )
			{
				a.s0.set(0, 0, r.w, r.h);
				a.s100.set(0, 0, 0, r.h);
				a.d0.set(r.x, r.y, r.w, r.h);
				a.d100.set(r.r(), r.y, 0, r.h);
			}
			else
			{
				a.s0.set(0, 0, r.w, r.h);
				a.s100.set(r.w, 0, 0, r.h);
				a.d0.set(r.x, r.y, r.w, r.h);
				a.d100.set(r.x, r.y, 0, r.h);
			}

			a.nDuration = nDuration;
			a.dwStart = 0;
		}
		AddAnimation(a);
	}

	void AddAnimation_SlideOutV(VWindow* pWin, bool bToBottom, int nDuration=500)
	{
		VRect r;
		pWin->GetGlobalRect(&r);

		VAnimationInfo a;
		{
			a.pImage = VWIN_NewVImage(r.w, r.h);
			a.bAutoDeleteImage = true;
			pWin->PrepareDragImage(a.pImage, 0, 0, false);

			a.bVirtual = true;

			a.a0 = 255;
			a.a100 = 0;

			if( bToBottom )
			{
				a.s0.set(0, 0, r.w, r.h);
				a.s100.set(0, 0, r.w, 0);
				a.d0.set(r.x, r.y, r.w, r.h);
				a.d100.set(r.x, r.b(), r.w, 0);
			}
			else
			{
				a.s0.set(0, 0, r.w, r.h);
				a.s100.set(0, r.h, r.w, 0);
				a.d0.set(r.x, r.y, r.w, r.h);
				a.d100.set(r.x, r.y, r.w, 0);
			}

			a.nDuration = nDuration;
			a.dwStart = 0;
		}
		AddAnimation(a);
	}

	void AddAnimation_Pulldown(VWindow* pWin, VListener* pListener, int nDuration=200)
	{
		VRect r;
		pWin->GetGlobalRect(&r);

		{
			VAnimationInfo a;
			{
				a.pImage = VWIN_NewVImage(r.w, r.h);
				a.bAutoDeleteImage = true;
				pWin->PrepareDragImage(a.pImage, 0, 0, true);

				a.a0 = 128;
				a.a100 = 255;

				a.s0.set(0, r.h, r.w, 0);
				a.s100.set(0, 0, r.w, r.h);
				a.d0.set(r.x, r.y, r.w, 0);
				a.d100.set(r.x, r.y, r.w, r.h);

				a.nDuration = nDuration;
				a.dwStart = 0;

				a.bVirtual = true;
				a.pAutoShow = pWin;
				a.pAutoShowPopup = pListener;
			}
			AddAnimation(a);
		}
	}

	void AddAnimation_Image2(VWindow* pSrc, VWindow* pDst, int nDuration, VListener* pPopupCancel=NULL, bool bNoSrcImage=false, bool bForceUpdateDstImage=false)
	{
		VRect rDst;
		pDst->GetGlobalRect(&rDst, true);

		VRect rSrc = rDst;
		//if( !bNoSrcImage )
			pSrc->GetGlobalRect(&rSrc, true);

		if( !bNoSrcImage )
		{
			VAnimationInfo a;
			{
				a.pImage = VWIN_NewVImage(rSrc.w, rSrc.h);
				a.bAutoDeleteImage = true;
				pSrc->PrepareDragImage(a.pImage, 0, 0);

				a.a0 = 255;
				a.a100 = 0;
				a.s0.set(0, 0, rSrc.w, rSrc.h);
				a.s100.set(0, 0, rSrc.w, rSrc.h);
				a.d0 = rSrc;
				a.d100 = rDst;

				a.nDuration = nDuration;
				a.dwStart = 0;
			}
			AddAnimation(a);
		}
		{
			VAnimationInfo a;
			{
				a.pImage = VWIN_NewVImage(rDst.w, rDst.h);
				a.bAutoDeleteImage = true;
				pDst->PrepareDragImage(a.pImage, 0, 0, bForceUpdateDstImage);

				a.a0 = 0;
				a.a100 = 255;
				a.s0.set(0, 0, rDst.w, rDst.h);
				a.s100.set(0, 0, rDst.w, rDst.h);
				a.d0 = rSrc;
				a.d100 = rDst;

				a.nDuration = nDuration;
				a.dwStart = 0;

				a.pAutoShow = pDst;
				a.pAutoShowPopup = pPopupCancel;
			}
			AddAnimation(a);
		}
	}

	void AddAnimation_FadeOut(VWindow* pSrc, int nDuration)
	{
		VRect rSrc;
		pSrc->GetGlobalRect(&rSrc);

		VAnimationInfo a;
		{
			a.pImage = VWIN_NewVImage(rSrc.w, rSrc.h);
			a.bAutoDeleteImage = true;
			pSrc->PrepareDragImage(a.pImage, 0, 0);

			a.a0 = 255;
			a.a100 = 0;
			a.s0.set(0, 0, rSrc.w, rSrc.h);
			a.s100.set(0, 0, rSrc.w, rSrc.h);
			a.d0 = rSrc;
			a.d100 = rSrc;

			a.nDuration = nDuration;
			a.dwStart = 0;
		}
		AddAnimation(a);
	}

	VScreen* GetScreen(const TCHAR* pId)
	{
		if( !m_mId2Screen.has(pId) )
			return NULL;
		return m_mId2Screen[pId];
	}

	virtual bool Back(int nEffect=0)
	{
		if( m_aHistoryStack.GetSize() >= 1 )
		{
			String sBackTo = m_aHistoryStack.Pop();
			return Jump(sBackTo, true, false, nEffect);
		}
		return Top(nEffect);
	}

	virtual bool Top(int nEffect=0)
	{
		bool rc = false;
		if( m_sTopId.GetLength() > 0 )
		{
			rc = Jump(m_sTopId, false, false, nEffect);
			ClearHistoryStack();
		}
		return rc;
	}

	virtual void ClearHistoryStack()
	{
		while( m_aHistoryStack.GetSize() >= 1 )
		{
			String sId = m_aHistoryStack.Pop();
			if( m_mId2Screen.has(sId) )
			{
				Unload(m_mId2Screen[sId]);
			}
		}
		m_aHistoryStack.clear();
	}

	virtual void ResetHistoryStack(const TCHAR* pTopId)
	{
		m_sTopId = (pTopId != NULL) ? pTopId : _T("");
		ClearHistoryStack();
	}

	virtual bool OnLButtonDown(int x, int y)
	{
		if( VKeypad_IsVisible() )
			VKeypad_Hide();
		return super::OnLButtonDown(x, y);
	}

	bool SendCommandToActiveScreen(const String& sId)
	{
		if( m_pActiveScreen != NULL )
		{
			if( m_pActiveScreen->m_aChildren.GetSize() > 0 )
			{
				m_pActiveScreen->m_aChildren[0]->DoCommand(sId);
				return true;
			}
		}
		return false;
	}

	VScreen* GetLastScreen()
	{
		return (VScreen*)m_pLastScreen;
	}

	virtual void OnTimer(int nId)
	{
		if( nId == TIMER_AUTO_UNLOAD )
		{
			DoUnload();
		}
	}

	String GetLastScreenId();

	/**************************************************************************/
	virtual bool Jump(VScreen* pNew, bool bForBack=false, bool bPushHistoryStack=true, int nEffect=0);
	virtual bool Jump(const TCHAR* pId, bool bForBack=false, bool bPushHistoryStack=true, int nEffect=0);
	virtual bool LoadNow(VScreen* pScreen);
	virtual void Unload(VScreen* pScreen);
	virtual void DoUnload();

	virtual void toast(const TCHAR* pText, int nDuration=700)
	{
		VToastInfo ti(pText, nDuration);
		m_aToast.Add(ti);
	}
	#endif 
    ~JRootWindow()
    {

    }
};


