#pragma once 

class JImage;

class JDraw
{
public:
    static Map<string, int> s_mFontName2Id;

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

};

