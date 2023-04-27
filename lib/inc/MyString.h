#ifndef __MY_STRING_H__
#define __MY_STRING_H__


#define CLASSNAME String1
#define ARRAYNAME String1Array
#define MAPNAME String1Map
#define BASE std::string
#define CHARTYPE char
#define CHARSIZE 1
#include "MyStringCore.h"
#undef CLASSNAME
#undef ARRAYNAME
#undef MAPNAME
#undef BASE
#undef CHARTYPE
#undef CHARSIZE


#define CLASSNAME String2
#define ARRAYNAME String2Array
#define MAPNAME String2Map
#define BASE std::wstring
#define CHARTYPE wchar_t
#define CHARSIZE 2
#include "MyStringCore.h"
#undef CLASSNAME
#undef ARRAYNAME
#undef MAPNAME
#undef BASE
#undef CHARTYPE
#undef CHARSIZE


typedef	String1			String;
typedef	String1Array	StringArray;
typedef	String1Map		StringMap;

#endif // __MY_STRING_H__


