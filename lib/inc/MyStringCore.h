
class CLASSNAME;

class ARRAYNAME : public Array<CLASSNAME>
{
public:
    ARRAYNAME()
    {

    }

    ARRAYNAME(const ARRAYNAME& src)
    {
        Copy(src);
    }

    ARRAYNAME(const Array<CLASSNAME>& src)
    {
        for (int i = 0; i < src.GetSize(); i++)
        {
            Add(src[i]);
        }
    }

    void Sort(bool bAsc=true);
    CLASSNAME Join(const CHARTYPE* pSep, const CHARTYPE* pBracket=NULL);
    ~ARRAYNAME();
};

class CLASSNAME : public BASE
{
public:
    CLASSNAME()
    {

    }
    CLASSNAME(const CLASSNAME& src)
    {
        *this = src;
    }
    CLASSNAME& operator=(const CLASSNAME& src)
    {
        *(BASE*)this = src.c_str();
        return *this;
    }
    CLASSNAME(const CHARTYPE* src) : BASE(src)
	{
	}
    operator const CHARTYPE*() const
    {
        return c_str();
    }
    CLASSNAME operator+(const CHARTYPE* src) const
	{
		BASE buf = *this;
		buf += src;
		return CLASSNAME(buf.c_str());
	}
	CLASSNAME operator+(const CLASSNAME& src) const
	{
		BASE buf = *this;
		buf += src;
		return CLASSNAME(buf.c_str());
	}
	CHARTYPE operator[](int i) const
	{
		return at(i);
	}
	CHARTYPE& operator[](int i)
	{
		return at(i);
	}

	int GetLength() const
	{
		return (int)length();
	}

	CLASSNAME substr(int s, int n) const
	{
		if( s >= GetLength() )
		{
			CHARTYPE buf[1] = {0};
			return buf;
		}
		return CLASSNAME(BASE::substr(s, n).c_str());
	}
	CLASSNAME substr(int s) const
	{
		return substr(s, GetLength() - s);
	}
	CLASSNAME left(int n) const
	{
		if( n < 0 )
			n = 0;
		if( n > GetLength() )
			n = GetLength();
		return substr(0, n);
	}
	CLASSNAME right(int n) const
	{
		if( n > GetLength() )
			n = GetLength();
		return substr(GetLength() - n);
	}
	int find(char c, int nStart=0) const
	{
		size_t rc = BASE::find(c, nStart);
		if( rc == ((size_t)-1) )
			return -1;
		return (int)rc;
	}
	int find(const CHARTYPE* p, int nStart=0) const
	{
		size_t rc = BASE::find(p, nStart);
		if( rc == ((size_t)-1) )
			return -1;
		return (int)rc;
	}
	int rfind(char c) const
	{
		size_t rc = BASE::rfind(c);
		if( rc == ((size_t)-1) )
			return -1;
		return (int)rc;
	}
	int rfind(const CHARTYPE* p) const
	{
		size_t rc = BASE::rfind(p);
		if( rc == ((size_t)-1) )
			return -1;
		return (int)rc;
	}
	bool startWith(const CHARTYPE* p) const
	{
		return (find(p) == 0);
	}
	bool endWith(const CHARTYPE* p) const
	{
		int n = rfind(p);
		if( n < 0 )
			return false;
		if( n != (int)(GetLength() - CLASSNAME(p).GetLength()) )
			return false;
		return true;
	}
	bool IsWhiteSpace(CHARTYPE _c) const
	{
		unsigned long c = (unsigned long)_c;
		if( c == '\0' )
			return false;
		if( c <= 0x20 )
			return true;
		return false;
	}
	CLASSNAME trimLeft() const
	{
		const CHARTYPE* s = *this;
		const CHARTYPE* p = s;
		while( IsWhiteSpace(*p) )
			p++;
		int n = (int)(p - s);
		return substr(n);
	}
	CLASSNAME trimRight() const
	{
		const CHARTYPE* s = *this;
		const CHARTYPE* p = s + GetLength() - 1;
		while( (p >= s) && IsWhiteSpace(*p) )
			p--;
		int n = (int)((p + 1) - s);
		return left(n);
	}
	CLASSNAME trim() const
	{
		return trimLeft().trimRight();
	}

	void toUpper()
	{
		std::transform(BASE::begin(), BASE::end(), BASE::begin(), ::toupper);
	}

	void toLower()
	{
		std::transform(BASE::begin(), BASE::end(), BASE::begin(), ::tolower);
	}

	CLASSNAME upper() const
	{
		CLASSNAME s = *this;
		s.toUpper();
		return s;
	}

	CLASSNAME lower() const
	{
		CLASSNAME s = *this;
		s.toLower();
		return s;
	}

	bool IsNumeric(bool bPlus, bool bMinus, bool bDot)
	{
		if( GetLength() < 1 )
			return false;
		for( int i=0 ; i<GetLength() ; i++ )
		{
			CHARTYPE c = at(i);

			if( (c >= '0') && (c <= '9') )
				continue;
			if (bPlus && (c == '+'))
			{
				bPlus = false;
				continue;
			}
			if( bMinus && (c == '-') )
			{
				bMinus = false;
				continue;
			}
			if( bDot && (c == '.') )
			{
				bDot = false;
				continue;
			}

			return false;
		}
		return true;
	}

	bool IsNumeric(bool bPlusMinus, bool bDot)
	{
		return IsNumeric(bPlusMinus, bPlusMinus, bDot);
	}

	bool IsNumeric()
	{
		return IsNumeric(false, false, false);
	}

	ARRAYNAME split(const CLASSNAME& sep) const;
	ARRAYNAME splitExact(const CLASSNAME& sep) const;
	CLASSNAME replace(const CHARTYPE* pOld, const CHARTYPE* pNew) const;

};

typedef Map<CLASSNAME, CLASSNAME>    MAPNAME;

