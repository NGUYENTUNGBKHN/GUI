/******************************************************************************/
#include <libcpp.h>

#define VWIN__NO_IMPL
#include "JWin.h"

/******************************************************************************/
FreeType			g_FreeType;

/******************************************************************************/
FT_Error FreeType::face_requester(FTC_FaceID face_id, FT_Library library, FT_Pointer request_data, FT_Face* pFace)
{
	if( !m_mPathes.IsContain(face_id) )
		return FT_Err_Ok;

	if( FT_New_Face(library, m_mPathes[face_id].c_str(), 0, pFace) != FT_Err_Ok )
	{
		return FT_Err_Ok;
	}
	if( FT_Select_Charmap(*pFace, FT_ENCODING_UNICODE) != FT_Err_Ok )
	{
		FT_Done_Face(*pFace);
		return FT_Err_Ok;
	}
	return FT_Err_Ok;
}

/******************************************************************************/
FT_Error FreeType::_face_requester(FTC_FaceID face_id, FT_Library library, FT_Pointer request_data, FT_Face* pFace)
{
	return ((FreeType*)request_data)->face_requester(face_id, library, 0, pFace);
}

/******************************************************************************/
FreeType::FreeType()
{
	m_bInitialized = false;
	m_nMaxX = 99999;
}

/******************************************************************************/
bool FreeType::Init()
{
	FT_Init_FreeType(&library);
	FTC_Manager_New(library, 0, 0, 0, _face_requester, this, &cache_man);
	FTC_CMapCache_New(cache_man, &cmap_cache);
	FTC_ImageCache_New(cache_man, &image_cache);
	m_bInitialized = true;
	return true;
}

/******************************************************************************/
void FreeType::Add(int nId, const TCHAR* pPath, int nFixedWidth, int nBaseOffset, int nZenkakuFontId)
{
	m_mPathes[(FTC_FaceID)nId]		= T2M(pPath);
	m_mFixedWidth[(FTC_FaceID)nId]	= nFixedWidth;
	m_mZenkakuId[(FTC_FaceID)nId]	= nZenkakuFontId;
	m_mBaseOffset[(FTC_FaceID)nId]	= nBaseOffset;
}

/******************************************************************************/
void FreeType::Select(int nFontId, int nSize)
{
	for( int i=0 ; i<2 ; i++ )
	{
		FreeTypeInfo& fti = m_Info[i];

		fti.face_id = (FTC_FaceID)nFontId;
		FTC_Manager_LookupFace(cache_man, fti.face_id, &fti.face);

		fti.scaler.face_id	= fti.face_id;
		fti.scaler.width	= 0;
		fti.scaler.height	= nSize << 6;
		fti.scaler.pixel	= 0;
		fti.scaler.x_res	= 72;
		fti.scaler.y_res	= 72;
		FTC_Manager_LookupSize(cache_man, &fti.scaler, &fti.font_size);

		fti.cmap_index = FT_Get_Charmap_Index(fti.face->charmap);

		fti.font_type.face_id	= fti.face_id;
		fti.font_type.width		= 0;
		fti.font_type.height	= nSize;
		fti.font_type.flags		= FT_LOAD_RENDER | FT_LOAD_TARGET_NORMAL;

		if( i == 0 )
		{
			int fw = m_mFixedWidth[fti.face_id];
			m_nFixedWidth = fw ? ((nSize / 2) * fw / 100) : 0;

			m_nBaseOffsetPixel = nSize * m_mBaseOffset[fti.face_id] / 100;

			m_nZenkakuId = m_mZenkakuId[fti.face_id];
			if( m_nZenkakuId < 0 )
				break;

			nFontId = m_nZenkakuId;
		}
	}
}

/******************************************************************************/
int FreeType::Render(JDraw* pDraw, int x, int y, JColor c, const TCHAR* pText, bool bNoRender)
{
	int x_sv = x;
	FreeTypeInfo& fti0 = m_Info[0];

	if( fti0.face == NULL )
	{
		// please call SelectFont() first.
		return 0;
	}

	y += ((fti0.face->height + fti0.face->descender) * fti0.face->size->metrics.y_ppem / fti0.face->units_per_EM);
	y += 1;

	FT_Bool bKerning = FT_HAS_KERNING(fti0.face);
	FT_UInt last_glyph_index = 0;

	int a1 = GetA(c);
	int r1 = GetR(c);
	int g1 = GetG(c);
	int b1 = GetB(c);

	for ( int n=0 ; pText[n] ; n++ )
	{
		//
		TCHAR chr = pText[n];
		FreeTypeInfo& fti = m_Info[((m_nZenkakuId >= 0) && (chr >= 0x100)) ? 1 : 0];

		// get bitmap
		FT_UInt glyph_index = FTC_CMapCache_Lookup(cmap_cache, fti.face_id, fti.cmap_index, chr);

		if( bKerning && (last_glyph_index != 0) && (glyph_index != 0) )
		{
			FT_Vector delta;
			FT_Get_Kerning(fti.face, last_glyph_index, glyph_index, ft_kerning_default, &delta);
			x += (delta.x >> 6);
		}
		last_glyph_index = glyph_index;

		FT_Glyph glyph;
		FTC_ImageCache_Lookup(image_cache, &fti.font_type, glyph_index, &glyph, NULL);

		FT_BitmapGlyph glyph_bitmap;
		if( glyph->format == FT_GLYPH_FORMAT_BITMAP )
		{
			glyph_bitmap = (FT_BitmapGlyph)glyph;
		}
		else
		{
			continue;
		}

		FT_Bitmap& bitmap = glyph_bitmap->bitmap;

		// render main
		if( !bNoRender )
		{
			int offset_x = (m_nFixedWidth > 0) ? ((m_nFixedWidth - bitmap.width + 1) / 2) : 0;
			for( int iy=0 ; iy<((int)bitmap.rows) ; iy++ )
			{
				int yy = y + iy - glyph_bitmap->top + m_nBaseOffsetPixel;

				for( int ix=0 ; ix<((int)bitmap.width) ; ix++ )
				{
					int n=0;
					if( bitmap.pixel_mode == FT_PIXEL_MODE_MONO )
					{
						n = bitmap.buffer[(iy * bitmap.pitch) + (ix / 8)];
						n = ((n & (0x80 >> (ix % 8))) != 0) ? 255 : 0;
					}
					else if( bitmap.pixel_mode == FT_PIXEL_MODE_GRAY )
					{
						n = bitmap.buffer[(iy * bitmap.pitch) + ix];
					}
					JColor ctmp = argb(n * a1 / 255, r1, g1, b1);

					int xx = x + ix + offset_x;
					if( xx >= m_nMaxX )
					{
						continue;
					}
					pDraw->SetPixel(xx, yy, ctmp, 1);
				}
			}
		}

		// next
		{
			if( m_nFixedWidth > 0 )
			{
				int w = m_nFixedWidth;
				/*
				w += (fti.font_type.height * 20 / 100);
				if( bitmap.width > 0 )
				{
					int tmp = bitmap.width + (fti.font_type.height * 5 / 100);
					if( tmp > w )
					{
						w = tmp;
					}
				}
				*/
				x += w;
			}
			else
			{
				if( bitmap.width == 0 )
				{
					//x += (glyph_bitmap->root.advance.x >> 16);
					x += (fti.font_type.height / 2);
				}
				else
				{
					x += bitmap.width;
					x += (fti.font_type.height * 5 / 100);
				}
			}
		}
	}

	//
	return x - x_sv;
}
