#pragma once

class JRect
{
public:
    int		x;
	int		y;
	int		w;
	int		h;
public:
    JRect(/* args */)
    {
        set(0, 0, 0, 0);
    }

    JRect(int x, int y, int w, int h)
	{
		set(x, y, w, h);
	}

    void set(int x, int y, int w, int h)
    {
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;
    }

    int r() const 
    {
        return x + w;
    }

    int b() const
    {
        return y + h;
    }

    bool PtInTRect(int ax, int ay) const 
    {
        if ((ax < x) || (ax > r()))
        {
            return false;
        } 
        if ((ay < y) || (ay > b()))
        {
            return false;
        }
    }

    bool PtInRect(const JPoint* p) const
    {
        return PtInTRect(p->x, p->y);
    }

    bool operator==(const JRect& b) const
	{
		const JRect& a = *this;

		if( a.x != b.x )	return false;
		if( a.y != b.y )	return false;
		if( a.w != b.w )	return false;
		if( a.h != b.h )	return false;
		return true;
	}

	bool operator!=(const JRect& b) const
	{
		return !(*this == b);
	}
    ~JRect()
    {
        
    }
private:
    /* data */
};


