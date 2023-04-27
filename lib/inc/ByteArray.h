#ifndef __BYTEARRAY_H
#define __BYTEARRAY_H

class ByteArray : public Array<BYTE>
{
public:
	ByteArray()
	{
	}

	ByteArray(int n, void** ppBuffer=NULL)
	{
		SetSize(n);
		if( ppBuffer )
			*ppBuffer = GetData();
	}

	ByteArray& operator=(const ByteArray& src)
	{
		Copy(src);
		return *this;
	}

	ByteArray operator+(const ByteArray& src) const
	{
		ByteArray tmp;
		tmp.Copy(*this);
		tmp.Append(src);
		return tmp;
	}

	ByteArray& operator+=(const ByteArray& src)
	{
		Append(src);
		return *this;
	}

	ByteArray sub(int s, int n) const
	{
		ByteArray tmp;
		for( int i=s ; i<(s + n) ; i++ )
			tmp.Add(at(i));
		return tmp;
	}

	ByteArray sub(int s) const
	{
		return sub(s, GetSize() - s);
	}

	void AddBin(const void* pData, int nLength)
	{
		const BYTE* p = (const BYTE*)pData;
		for( int i=0 ; i<nLength ; i++ )
			Add(*p++);
	}

	void AddString(const char* pStr)
	{
		for( ; *pStr ; pStr++ )
			Add(*(BYTE*)pStr);
	}

	void AddString(const WCHAR* pStr)
	{
		for( ; *pStr ; pStr++ )
		{
			WORD c = *(WORD*)pStr;
			Add((BYTE)c);
			Add((BYTE)(c >> 8));
		}
	}
};

#endif

