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

    ~JRootWindow()
    {

    }
};


