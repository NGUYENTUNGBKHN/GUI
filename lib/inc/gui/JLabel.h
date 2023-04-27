#pragma once

class JLabel : public JWindow
{
    typedef JWindow super;
protected:
    bool m_bNoBg;
private:
    /* data */
public:
    JLabel(/* args */)
    {
        m_cFg = BLACK;
		m_cBg = TRANS;
		m_bNoBg = true;
		m_nTextAlign = JDraw::taLEFT | JDraw::taMIDDLE;
    }

    JLabel(bool bNoBg)
    {
        m_cFg = BLACK;
		m_cBg = TRANS;
		m_bNoBg = bNoBg;
		m_nTextAlign = JDraw::taLEFT | JDraw::taMIDDLE;
    }

    virtual ~JLabel()
    {
        
    }

    virtual bool Create(const TCHAR* pId, int x, int y, int w, int h, JWindow* pParent, const TCHAR* pText=_T(""), JImage* pImg=NULL)
    {
        if( !super::Create(pId, x, y, w, h, pParent) )
			return false;

		// text(pText);

		// if( pImg != NULL )
		// 	SetResource(pImg);

		return true;
    }

    virtual bool Create(int x, int y, int w, int h, JWindow* pParent, const TCHAR* pText=_T(""))
	{
		return Create(_T(""), x, y, w, h, pParent, pText);
	}

	virtual void OnPaint(JDraw* pDraw, int w, int h)
	{
		if( !m_bNoBg )
			pDraw->FillRect(0, 0, w, h, m_cBg);

		JColor fg;
		{
			fg = m_cFg;

			// if( text() != defaultText() )
			// {
			// 	if( m_cChangedFg != INVALID_COLOR )
			// 	{
			// 		fg = m_cChangedFg;
			// 	}
			// }

			// if( !enable() )
			// 	fg = COLOR_DISABLE_FG;
		}
		// printf("%s %d %ls \n",__func__,__LINE__,text().c_str());
		// pDraw->Text(m_rPad.x, m_rPad.y, w - (m_rPad.x + m_rPad.w), h - (m_rPad.y + m_rPad.h), text(), fg, m_nTextAlign, TRANS, m_nTextPosX, m_nTextPosY);
	}

	// virtual bool OnLButtonDown(int x, int y)
	// {
	// 	if( m_sId.GetLength() > 0 )
	// 	{
	// 		if( isEdit() )
	// 		{
	// 			notify(this, _T("VLavel,Down"), 0, NULL);
	// 		}
	// 		else
	// 		{
	// 			DoCommand(m_sId);
	// 		}
	// 	}
	// 	return super::OnLButtonDown(x, y);
	// }
};





