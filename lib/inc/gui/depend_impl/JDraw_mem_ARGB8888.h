#pragma once

class VDraw_mem_ARGB8888 : public JDraw
{
public:
    virtual void impl_SetPixel(int x, int y)
    {
        if( (m_nStrokeWidth < 0) || (m_nStrokeWidth > 4) )
			return;

		x -= (m_nStrokeWidth / 2);
		y -= (m_nStrokeWidth / 2);

		static BYTE pat_0[1] = {0x80};
		static BYTE pat_1[1] = {0x80};
		static BYTE pat_2[2] = {0xc0,0xc0};
		static BYTE pat_3[3] = {0x40,0xe0,0x40};
		static BYTE pat_4[4] = {0x60,0xf0,0xf0,0x60};
		static BYTE* pattern_t[5] = {pat_0, pat_1, pat_2, pat_3, pat_4};

		BYTE* pPattern = pattern_t[m_nStrokeWidth];

		int a1 = GetA(m_cStrokeColor);
		int r1 = GetR(m_cStrokeColor);
		int g1 = GetG(m_cStrokeColor);
		int b1 = GetB(m_cStrokeColor);

		for( int iy=0 ; iy<m_nStrokeWidth ; iy++ )
		{
			BYTE mask = 0x80;
			for( int ix=0 ; ix<m_nStrokeWidth ; ix++ )
			{
				if( *pPattern & mask )
				{
					BYTE* p = m_pDst->GetPixelAddress(x + ix, y + iy);
					if( p )
					{
						if( a1 > 0 )
						{
							// modify here
							if( a1 == 255 )
							{
								*(DWORD*)p = m_cStrokeColor;
							}
							else
							{
								DWORD s = *(DWORD*)p;

								BYTE a2 = GetA(s);
								BYTE a3 = min(255, a1 + a2);
								a2 = 255 - a1;

								BYTE r2 = GetR(s);
								BYTE g2 = GetG(s);
								BYTE b2 = GetB(s);
								*(DWORD*)p = argb(a3, (r1*a1/255 + r2*a2/255), (g1*a1/255 + g2*a2/255), (b1*a1/255 + b2*a2/255));
							}
						}
					}
				}
				mask >>= 1;
			}
			pPattern++;
		}
    }
private:

};



