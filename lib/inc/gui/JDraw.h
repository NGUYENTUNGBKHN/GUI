#pragma once 

#define ABS(N)		(((N)>=0) ? (N) : (-(N)))

class JImage;

class JDraw
{
public:
    static Map<string, int> s_mFontName2Id;

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
    /* data */
public:
    JDraw(/* args */)
    {
        m_pDst = NULL;
    }
    ~JDraw()
    {

    }

public:
    JImage* m_pDst;
    
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

};

