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
extern String1 Edit1(const char* pFormat, ...);
extern String1 EditComma1(int n);
extern String1 EditCommaU1(unsigned int n);


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
extern String2 Edit2(const wchar_t* pFormat, ...);
extern String2 EditComma2(int n);
extern String2 EditCommaU2(unsigned int n);


typedef	String1			String;
typedef	String1Array	StringArray;
typedef	String1Map		StringMap;
#define Edit			Edit1
#define EditComma		EditComma1
#define EditCommaU		EditCommaU1

#endif // __MY_STRING_H__


