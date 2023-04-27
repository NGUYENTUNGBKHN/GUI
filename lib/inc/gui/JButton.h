#pragma once

class JButton : public JLabel
{
typedef JLabel super;

public:
	enum ButtonStyle
	{
		bsText = 1,
		bsIcon = 2,
		bsImage = 3,
		bsTopIcon = 4,
	};

protected:
	ButtonStyle		m_nButtonStyle;
	bool			m_bPress;
	int				m_nForceImageWidth;
	int				m_nForceImageHeight;
	int				m_nColor;
	int				m_nImageCount;	// 1.multipexed image  2.separated image

private:
    /* data */
public:
    JButton()
	{
		m_bPress = false;
		m_nForceImageWidth = 0;
		m_nForceImageHeight = 0;
		m_nColor = 7;
		m_nImageCount = 1;
	}

	virtual ~JButton()
	{
	}

	virtual bool Create(const TCHAR* pId, ButtonStyle bs, int x, int y, int w, int h, JWindow* pParent, const TCHAR* pText, JImage* pResource=NULL)
	{
		m_nButtonStyle = bs;

		if( m_nButtonStyle == bsText )
		{
			m_nTextAlign = JDraw::taCENTER | JDraw::taMIDDLE;
		}
		else if( m_nButtonStyle == bsIcon )
		{
			m_nTextAlign = JDraw::taLEFT | JDraw::taMIDDLE;
		}
		else if( m_nButtonStyle == bsImage )
		{
			m_nTextAlign = JDraw::taCENTER | JDraw::taMIDDLE;
		}
		else if( m_nButtonStyle == bsTopIcon )
		{
			m_nTextAlign = JDraw::taCENTER | JDraw::taBOTTOM;
		}

		// SetResource(pResource);
		return super::Create(pId, x, y, w, h, pParent, pText);
	}

	virtual bool wantDoubleClick()
	{
		return false;
	}

	virtual bool CanFocus()
	{
		return true;
	}

	virtual void OnPaint(JDraw* pDraw, int w, int h)
	{
		if( (m_nButtonStyle == bsText) || (m_nButtonStyle == bsIcon) )
		{
			if( CommonImage() )
			{
				pDraw->FillRect(0, 0, w, h, TRANS);

				if( disable() )
				{
					TileBlit(pDraw, CommonImage(), 0, 0, w, h, 48 * m_nColor, 48, 48, 48, 100);
				}
				else if( m_bPress && IsHover() )
				{
					TileBlit(pDraw, CommonImage(), 0, 0, w, h, 48 * m_nColor, 96, 48, 48);
				}
				else
				{
					TileBlit(pDraw, CommonImage(), 0, 0, w, h, 48 * m_nColor, 48, 48, 48);
				}
			}
			else
			{
				pDraw->FillRect(0, 0, w, h, enable() ? COLOR_ACTIVE_BG : COLOR_DISABLE_FRAME);
			}
		}
		else if( m_nButtonStyle == bsImage )
		{
			pDraw->FillRect(0, 0, w, h, TRANS);

			if( m_pResource != NULL )
			{
				if( m_pResource2 != NULL )
				{
					if( disable() )
					{
						m_pResource->Draw(pDraw, 0, 0, w, h, 0, 0, m_pResource->w(), m_pResource->h(), 64);
					}
					else if( m_bPress && IsHover() )
					{
						m_pResource2->Draw(pDraw, 0, 0, w, h, 0, 0, m_pResource2->w(), m_pResource2->h());
					}
					//else if( HasFocus() )
					//{
					//}
					//else if( IsHover() )
					//{
					//}
					else
					{
						m_pResource->Draw(pDraw, 0, 0, w, h, 0, 0, m_pResource->w(), m_pResource->h());
					}
				}
				else
				{
					int sh = m_pResource->h() / 5;
					int sy = 0;
					if( disable() )
						sy = 4;
					else if( m_bPress && IsHover() )
						sy = 3;
					else if( HasFocus() )
						sy = 2;
					else if( IsHover() )
					{
						sy = 1;
					}
					m_pResource->Draw(pDraw, 0, 0, w, h, 0, sy * sh, m_pResource->w(), sh);
				}
			}
			else
			{
				pDraw->FillRect(0, 0, w, h, enable() ? COLOR_LOAD_ERROR : COLOR_DISABLE_FRAME);
			}
		}
		else if( m_nButtonStyle == bsTopIcon )
		{
			pDraw->FillRect(0, 0, w, h, TRANS);

			if( m_pResource != NULL )
			{
				int max_h = h - pDraw->MeasureText(text()).h - 4;
				int iw;
				int ih;
				{
					float ah = ((float)m_pResource->w()) / ((float)w);
					float av = ((float)m_pResource->h()) / ((float)max_h);
					if( ah >= av )
					{
						iw = w;
						ih = m_pResource->h() * iw / m_pResource->w();
					}
					else
					{
						ih = max_h;
						iw = m_pResource->w() * ih / m_pResource->h();
					}
				}

				if( m_nForceImageWidth > 0 )
					iw = m_nForceImageWidth;
				if( m_nForceImageHeight > 0 )
					ih = m_nForceImageHeight;

				int hspace = (w - iw) / 2;
				int vspace = (max_h - ih) / 2;
				m_pResource->Draw(pDraw, hspace, vspace, iw, ih, 0, 0, m_pResource->w(), m_pResource->h());
			}
			else
			{
				pDraw->FillRect(0, 0, w, h, enable() ? COLOR_LOAD_ERROR : COLOR_DISABLE_FRAME);
			}

			if( m_bPress && IsHover() )
			{
				pDraw->FrameRoundRect(1, 1, w - 1, h - 1, COLOR_ACTIVE_BG, 2, 4);
			}
		}

		JColor cFgSv = m_cFg;

		if( disable() )
		{
			m_cFg = COLOR_DISABLE_FG;
		}
		else if( !IsEnter() || !m_bPress )
		{
			m_cFg = COLOR_ACTIVE_FG;
		}
		else
		{
			if( (m_nButtonStyle == bsText) || (m_nButtonStyle == bsIcon) )
			{
				if( !CommonImage() )
				{
					pDraw->FillRect(2, 2, w-4, h-4, COLOR_ACTIVE_FG);
					m_cFg = COLOR_ACTIVE_BG;
				}
			}
		}

		if( m_nButtonStyle == bsText )
		{
			if( CommonImage() )
			{
				if( enable() )
				{
					m_cFg = cFgSv;
				}
			}
		}
		else if( m_nButtonStyle == bsIcon )
		{
			int ih = m_nFontSize * 110 / 100;
			int iw = m_pResource->w() * ih / m_pResource->h();
			int space = (h - ih) / 2;
			m_pResource->Draw(pDraw, space, space, iw, ih, 0, 0, m_pResource->w(), m_pResource->h());

			m_rPad.x = h;
		}
		else if( m_nButtonStyle == bsImage )
		{
			if( enable() )
			{
				m_cFg = cFgSv;
			}
		}
		else if( m_nButtonStyle == bsTopIcon )
		{
			m_cFg = cFgSv;
		}

		super::OnPaint(pDraw, w, h);

		m_cFg = cFgSv;
	}

	virtual void OnMouseEnter()
	{
		Invalidate();
	}

	virtual void OnMouseLeave()
	{
		Invalidate();
	}

	virtual bool OnLButtonDown(int x, int y)
	{
		m_bPress = true;
		Invalidate();
		return true;
	}

	virtual void OnLButtonUp(int x, int y)
	{
		m_bPress = false;
		Invalidate();
	}

	virtual bool OnClick(int x, int y)
	{
		m_bPress = false;
		Invalidate();

		notify(this, _T("JButton,clicked"), 0, NULL);
		DoCommand(m_sId);
		return true;
	}

	virtual void ForceImageSize(int w, int h)
	{
		m_nForceImageWidth = w;
		m_nForceImageHeight = h;
		Invalidate();
	}

	virtual int color()
	{
		return m_nColor;
	}

	virtual void color(int n)
	{
		m_nColor = n;
	}

	virtual void image_count(int n)
	{
		m_nImageCount = n;
	}

	virtual String value()
	{
		return String(_T(""));
	}
};




