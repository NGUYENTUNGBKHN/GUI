#pragma once

class JToastInfo
{
public:
	String		m_sText;
	int			m_nWait;

	int			m_nState;	// 0.unprepare  1.ready  2.show  3.wait  4.hide  9.remove
	JImage*		m_pImage;
	int			m_nAlpha;

public:
	JToastInfo()
	{
		m_nState = 9;
	}

	JToastInfo(const TCHAR* pText, int nDuration)
	{
		m_nState = 0;
		m_pImage = NULL;
		m_nWait = nDuration;
		m_sText = pText;
		m_nAlpha = 0;
	}
};

class JToastInfoArray : public Array<JToastInfo>
{
};
