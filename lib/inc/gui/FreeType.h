#pragma once

/******************************************************************************/
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_CACHE_H
#include FT_TRUETYPE_TAGS_H
#include FT_TRUETYPE_TABLES_H

/******************************************************************************/
#ifdef _WINDOWS
	#pragma comment(lib, "freetype.lib")
#endif

/******************************************************************************/
class JDraw;

class FreeTypeInfo
{
public:
	FT_Face						face;
	FTC_ScalerRec				scaler;
	FT_Size						font_size;
	FT_Int						cmap_index;
	FTC_FaceID					face_id;
	FTC_ImageTypeRec			font_type;
};

class FreeType
{
protected:
	bool						m_bInitialized;
	Map<FTC_FaceID,String1>		m_mPathes;
	Map<FTC_FaceID,int>			m_mFixedWidth;
	Map<FTC_FaceID,int>			m_mZenkakuId;
	Map<FTC_FaceID,int>			m_mBaseOffset;
	FT_Error					rc;
	FT_Library					library;
	FTC_Manager					cache_man;
	FTC_CMapCache				cmap_cache;
	FTC_ImageCache				image_cache;
	int							m_nFixedWidth;
	int							m_nZenkakuId;
	int							m_nBaseOffsetPixel;
public:
	FreeTypeInfo				m_Info[2];
	int							m_nMaxX;

public:
	FT_Error face_requester(FTC_FaceID face_id, FT_Library library, FT_Pointer request_data, FT_Face* pFace);
	static FT_Error _face_requester(FTC_FaceID face_id, FT_Library library, FT_Pointer request_data, FT_Face* pFace);

	FreeType();
	bool Init();
	void Add(int nId, const TCHAR* pPath, int nFixedWidth, int nBaseOffset, int nZenkakuFontId=(-1));
	void Select(int nFontId, int nSize);
	int Render(JDraw* pDraw, int x, int y, JColor c, const TCHAR* pText, bool bNoRender=false);
};

/******************************************************************************/
extern FreeType g_FreeType;
