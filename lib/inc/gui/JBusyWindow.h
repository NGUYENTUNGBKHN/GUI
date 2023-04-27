#pragma once

class JBusyWindow : public JWindow
{
    typedef JWindow super;
protected:
    String		m_sMessage1;
	String		m_sMessage2;

	int			m_nFrames;
	int			m_nFrameDuration;

	int			m_nRender;
	bool		m_bHideReq;

    JButton*	m_pCancelButton;

protected:
    virtual bool OnCreate()
	{
		m_bHideReq = false;
		m_nFrames = 1;
		m_nFrameDuration = 1000;

		int bw = 250;
		m_pCancelButton = new JButton();
		m_pCancelButton->Create(_T("BUSY.CANCEL"), JButton::bsText, (w() - bw) / 2, h() * 60 / 100, bw, 60, this, _T("CANCEL"), NULL);
		m_pCancelButton->color(1);

		Setup(_T(""), _T(""), NULL);
		SetTimer(TIMER_AUTO_REDRAW, 10);
		return true;
	}

	virtual void OnPaint(JDraw* pDraw, int w, int h)
	{
		//
		{
			static const int ANIM_FRAMES = 5;
			static const int MAX_ALPHA = 192;

			if( m_bHideReq )
			{
				if( m_nRender > 0 )
					m_nRender--;
				if( m_nRender <= 0 )
					visible(false);
			}
			else
			{
				if( m_nRender < ANIM_FRAMES )
					m_nRender++;
			}

			int a = MAX_ALPHA * m_nRender / ANIM_FRAMES;
			pDraw->FillRect(0, 0, w, h, argb(a,0,0,0));
		}

		//
		if( m_sMessage1 != _T("") )
		{
			int y = h * 20 / 100;
			int size = 48;
			int sv = pDraw->SelectFontSize(size);
			pDraw->Text(0, y, w, size, m_sMessage1, WHITE, JDraw::taCENTER | JDraw::taMIDDLE);
			pDraw->SelectFontSize(sv);
		}

		if( m_sMessage2 != _T("") )
		{
			int y = h * 30 / 100;
			int size = 24;
			int sv = pDraw->SelectFontSize(size);
			pDraw->Text(0, y, w, size, m_sMessage2, WHITE, JDraw::taCENTER | JDraw::taMIDDLE);
			pDraw->SelectFontSize(sv);
		}

		if( (m_sMessage1 != _T("")) || (m_sMessage2 != _T("")) )
		{
			int cx = w / 2;
			int cy = h * 50 / 100;

			if( m_pResource == NULL )
			{
				int r = 80;
				int size = 30;
				int hsize = size / 2;
				int start = (getTime() / 8) % 360;

				for( int i=0 ; i<360 ; i+=60 )
				{
					double t = (start + i) * 3.1415927 / 180.0;
					int x = cx + (int)(r * cos(t));
					int y = cy + (int)(r * sin(t));

					JColor c = COLOR_ACTIVE_BG;
					c = rgb(
						GetR(c) * ((i / 2) + 180) / 360,
						GetG(c) * ((i / 2) + 180) / 360,
						GetB(c) * ((i / 2) + 180) / 360);

					pDraw->FillEllipse(x - hsize, y - hsize, size, size, c);
				}
			}
			else
			{
				int i = (getTime() / m_nFrameDuration) % m_nFrames;
				int sw = m_pResource->w() / m_nFrames;
				int sh = m_pResource->h();
				m_pResource->Draw(pDraw, cx - (sw / 2), cy - (sh / 2), sw, sh, i * sw, 0, sw, sh);
			}
		}
	}

public:
	virtual void Setup(const TCHAR* pMessage1, const TCHAR* pMessage2, JWindow* pCancelOwner)
	{
		m_bHideReq = false;
		m_sMessage1 = pMessage1;
		m_sMessage2 = pMessage2;
		m_nRender = 0;
		m_pCancelButton->owner(pCancelOwner);
		m_pCancelButton->visible(pCancelOwner != NULL);
		visible(true);
	}

	void UpdateMessage(const TCHAR* pMessage2)
	{
		m_sMessage2 = pMessage2;
	}

	void SetResource(JImage* pImages, int nFrames, int nFrameDuration)
	{
		super::SetResource(pImages);
		m_nFrames = nFrames;
		m_nFrameDuration = nFrameDuration;
	}

	void SetHideReq()
	{
		if( visible() )
			m_bHideReq = true;
	}

	JButton* CancelButton()
	{
		return m_pCancelButton;
	}

	bool IsBusy()
	{
		return visible() && !m_bHideReq;
	}
private:
    /* data */
public:
    JBusyWindow(/* args */);
    ~JBusyWindow();
};



