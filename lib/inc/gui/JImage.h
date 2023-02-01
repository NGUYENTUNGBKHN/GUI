#pragma once

class JDraw;

class JImage
{
protected:
    int m_nWidth;
    int m_nHeight;
private:
    /* data */
public:
    JImage(/* args */)
    {

    }
    virtual ~JImage()
    {
        Destroy();
    }

    virtual bool Create(int w, int h);

    virtual int w() const 
    {
        return m_nWidth;
    }

    virtual int h() const
    {
        return m_nHeight;
    }

    virtual void Draw(JDraw *pDst, int dx, int dy, int dw, int dh, int sx, int sy, int sw, int sh, int alpha=255)
    {
        pDst->m_pDst->Draw(this, dx, dy, dw, dh, sx, sy, sw, sh, alpha);
    }

    virtual unsigned char* GetPixelAddress(int x, int y) const
    {
        //
        if ((x < 0) || (x >= m_nWidth) || (y < 0) || (y >= m_nHeight))
        {
            return 0;
        }
        return GetPixelAddressFast(x, y);
    }

    virtual bool impl_Create(int w, int h) = 0;
    virtual void Destroy();
    virtual unsigned char *GetPixelAddressFast(int x, int y) const = 0;
    virtual void Draw(JImage *pSrc, int dx, int dy, int dw, int dh, int sx, int sy, int sw, int sh, int alpha=255) = 0;
};




