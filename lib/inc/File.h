#ifndef __File_H
#define __File_H

class File
{
protected:
	FILE*	fp;
public:
	File()
	{
		fp = NULL;
	}

	virtual ~File()
	{
		Close();
	}

	operator FILE*()
	{
		return fp;
	}

	bool IsOpen()
	{
		return (fp != NULL);
	}

	bool Open(const TCHAR* pPath, const TCHAR* pMode)
	{
		if( fp )
			return false;
		fp = _tfopen(pPath, pMode);
		return (fp != NULL);
	}

	void Close()
	{
		if( !fp )
			return;
		fclose(fp);
		fp = NULL;
	}

	void Fflush()
	{
		if( !fp )
			return;
		fflush(fp);
	}

#ifdef _UNICODE
	bool ReadLine(String1& line)
	{
		if( !fp )
			return false;
		char buf[1024];
		if( !fgets(buf, sizeof(buf), fp) )
			return false;
		char* p = buf;
		if( !memcmp(p, "\xEF\xBB\xBF", 3) )
			p += 3;
		line = String1(p).split("\r\n")[0];
		return true;
	}
#endif

	bool ReadLine(String& line)
	{
		if( !fp )
			return false;
		char buf[1024];
		if( !fgets(buf, sizeof(buf), fp) )
			return false;
		char* p = buf;
		if( !memcmp(p, "\xEF\xBB\xBF", 3) )
			p += 3;
#ifdef _UNICODE
		line = String(U2W(p)).split(_T("\r\n"))[0];
#else
		line = String(p).split(_T("\r\n"))[0];
#endif
		return true;
	}

	bool WriteLine(const char* pLine)
	{
		if( !fp )
			return false;
		if( fputs(pLine, fp) == EOF )
			return false;
		if( fputs("\n", fp) == EOF )
			return false;
		return true;
	}

#ifdef _UNICODE
	bool WriteLine(const WCHAR* pLine)
	{

/* 漢字が含まれるとエラー
		if( !fp )
			return false;
		if( _fputts(pLine, fp) == EOF )
			return false;
		if( _fputts(_T("\n"), fp) == EOF )
			return false;
		return true;
*/
		if( ftell(fp) == 0 )
			Write("\xEF\xBB\xBF", 3);
		return WriteLine(W2U(pLine));
	}
#endif

	bool Read(void* pData, int nLength)
	{
		if( !fp )
			return false;
		return (fread(pData, nLength, 1, fp) == 1);
	}

	bool Write(const void* pData, int nLength)
	{
		if( !fp )
			return false;
		return (fwrite(pData, nLength, 1, fp) == 1);
	}

	QWORD GetSize()
	{
		if( !fp )
			return 0;
		fflush(fp);

#ifdef _WINDOWS
		ULARGE_INTEGER u;
		HANDLE hFile = (HANDLE)_get_osfhandle(_fileno(fp));
		u.LowPart = GetFileSize(hFile, &u.HighPart);
		return (QWORD)u.QuadPart;
#else
		struct stat st;
		if( fstat(fileno(fp), &st) == -1 )
			return 0;
		return st.st_size;
#endif
	}

	bool SetSize(QWORD n)
	{
		if( !fp )
			return false;
		fflush(fp);

#ifdef _WINDOWS
		ULARGE_INTEGER u;
		u.QuadPart = n;

		HANDLE hFile = (HANDLE)_get_osfhandle(_fileno(fp));
		SetFilePointer(hFile, (LONG)u.LowPart, (PLONG)&u.HighPart, FILE_BEGIN);
		SetEndOfFile(hFile);
		return true;
#else
		return false;
#endif
	}

	protected:
	INT64 GetTime_sub(int nType)
	{
		if( (nType < 0) || (nType > 2) )
			return -1;
		if( !fp )
			return -2;

#ifdef _WINDOWS
		HANDLE hFile = (HANDLE)_get_osfhandle(_fileno(fp));
		if( hFile == INVALID_HANDLE_VALUE )
			return -3;

		FILETIME ft[3];
		if( !GetFileTime(hFile, ft + 0, ft + 1, ft + 2) )
			return -4;

		return *(__int64*)(ft + nType);
#else
		struct stat buf;
		if (!fstat(fileno(fp), &buf))
		{
			if (nType == 0)
			{
				return (INT64)buf.st_ctime;		// 作成日時(ステータス変更)時間
			}
			else if (nType == 1)
			{
				return (INT64)buf.st_atime;		// アクセス時間
			}
			else if (nType == 2)
			{
				return (INT64)buf.st_mtime;		// 変更時間
			}
		}

		return -5;
#endif
	}
	public:

	INT64 GetTime_update()
	{
		return GetTime_sub(2);
	}

	void Dump(DWORD dwAddr, const void* pData, int nLength, const TCHAR* pHeader=NULL)
	{
		const BYTE* p = (const BYTE*)pData;

		pHeader = pHeader ? pHeader : _T("");

		String buf = pHeader;
		for( int i=0 ; i<nLength ; i++ )
		{
			if( (i & 15) == 0 )
			{
				if( i )
					buf += pHeader;
				buf += Edit(_T("%08x"), dwAddr + i);
			}
			buf += Edit(_T(" %02x"), p[i]);

			if( (i & 15) == 15 )
				buf += _T("\r\n");
		}
		WriteLine(buf);
	}

	void Dump(const void* pData, int nLength)
	{
		const BYTE* p = (const BYTE*)pData;

		String buf = _T("");
		for( int i=0 ; i<nLength ; i++ )
		{
			if( (i & 15) != 0 )
				buf += _T(" ");

			buf += Edit(_T("%02x"), p[i]);

			if( (i & 15) == 15 )
				buf += _T("\r\n");
		}
		WriteLine(buf);
	}

	static INT64 GetTime_update(const TCHAR* pPath)
	{
		File f;
		if( !f.Open(pPath, _T("r")) )
			return -1;
		return f.GetTime_update();
	}

#ifdef _WINDOWS
	static time_t Filetime2Clk(INT64 n)
	{
		if( n < 0 )
			return 0;

		FILETIME localfiletime;
		FileTimeToLocalFileTime((FILETIME*)&n, &localfiletime);

		SYSTEMTIME st;
		FileTimeToSystemTime(&localfiletime, &st);

		struct tm t;
		memset(&t, 0x00, sizeof(t));
		t.tm_year = st.wYear - 1900;
		t.tm_mon = st.wMonth - 1;
		t.tm_mday = st.wDay;
		t.tm_hour = st.wHour;
		t.tm_min = st.wMinute;
		t.tm_sec = st.wSecond;
		n = mktime(&t);
		return (time_t)n;
	}

	time_t GetTime_update_clk()
	{
		return File::Filetime2Clk(GetTime_update());
	}

	//static time_t GetTime_update_clk(const TCHAR* pPath)
	//{
	//	return Filetime2Clk(GetTime_update(pPath));
	//}
#endif

	static bool EnumFiles(StringArray& aResult, const TCHAR* pDirPath, const TCHAR* pExt=NULL, bool bFileNameOnly=true, bool bIncludeDots=false, bool bRecurse=false)
	{
		String sDir = pDirPath;

#ifdef _WINDOWS
		if( sDir == _T("") )
		{
			DWORD drives = GetLogicalDrives();
			DWORD mask = 1;
			for( int i=0 ; i<26 ; i++,mask<<=1 )
			{
				if( (drives & mask) != 0 )
				{
					String buf = /*bFileNameOnly ? _T("") : _T("/")*/_T("");
					buf += (_T('A') + i);
					buf += _T(":");
					aResult.Add(buf);
				}
			}
			return true;
		}
		if( sDir.startWith(_T("/")) )
		{
			sDir = sDir.substr(1);

			if( (sDir.GetLength() == 2) && (sDir[1] == _T(':')) )
			{
				aResult.Add(_T(".."));
			}
		}

		CFileFind ent;
		if( !ent.FindFile(sDir + _T("\\*.*")) )
			return false;

		for( bool rc=true ; rc ; )
		{
			rc = (ent.FindNextFile() == TRUE);
			String name = ent.GetFileName();

			if( !bIncludeDots )
			{
				if( ent.IsDots() )
					continue;
			}
			if( name == _T(".") )
				continue;

			String buf;
			if( bFileNameOnly )
				buf = name;
			else
			{
				if( name == _T("..") )
					buf = name;
				else
					buf = ent.GetFilePath();
			}

			bool bFound = true;
			if( pExt )
			{
				//if( buf.right((int)s_tcslen(pExt)) != pExt )
				if( buf.right((int)_tcslen(pExt)) != pExt )
				{
					bFound = false;
				}
			}

			if( bFound )
			{
				aResult.Add(buf);
			}

			if( bRecurse )
			{
				if( ent.IsDirectory() && !ent.IsDots() )
				{
					if( !EnumFiles(aResult, ent.GetFilePath(), pExt, bFileNameOnly, bIncludeDots, bRecurse) )
					{
						return false;
					}
				}
			}
		}
#else
		DIR* pDir = opendir(T2U(pDirPath));
		if( !pDir )
			return false;

		for( struct dirent* p ; (p=readdir(pDir))!=NULL ; )
		{
			if( !strcmp(p->d_name, ".") )
				continue;

			if( !bIncludeDots )
			{
				if( !strcmp(p->d_name, "..") )
					continue;
			}

			String path = sDir + _T("/") + U2T(p->d_name);

			String buf;
			if( bFileNameOnly )
				buf = U2T(p->d_name);
			else
				buf = path;

			bool bFound = true;
			if( pExt )
			{
				if( buf.right(_tcslen(pExt)) != pExt )
				{
					bFound = false;
				}
			}
			if( bFound )
			{
				aResult.Add(buf);
			}

			if( bRecurse )
			{
				if( File::IsDir(path) && strcmp(p->d_name, ".") && strcmp(p->d_name, "..") )
				{
					if( !EnumFiles(aResult, path, pExt, bFileNameOnly, bIncludeDots, bRecurse) )
					{
						return false;
					}
				}
			}
		}
		closedir(pDir);
#endif
		return true;
	}

	static QWORD GetSize(const TCHAR* pPath)
	{
		QWORD rc = (QWORD)-1;

		File fp;
		if( fp.Open(pPath, _T("rb")) )
			rc = fp.GetSize();

		return rc;
	}

	static int Copy(const TCHAR* pDstPath, const TCHAR* pSrcPath)
	{
		File ifp;
		if( !ifp.Open(pSrcPath, _T("rb")) )
			return -1;

		File ofp;
		if( !ofp.Open(pDstPath, _T("wb")) )
			return -2;

		BYTE buffer[4096];
		while( true )
		{
			int n = (int)fread(buffer, 1, sizeof(buffer), ifp);
			if( n < 1 )
				break;
			if( !ofp.Write(buffer, n) )
				return -3;
		}
		return 0;
	}

	static String ReadContent(const TCHAR* pPath)
	{
		String buf = _T("");

		File f;
		if( !f.Open(pPath, _T("rb")) )
			return buf;

		for( String line ; f.ReadLine(line) ; )
		{
			buf += line;
			buf += _T("\r\n");
		}
		return buf;
	}

	static bool ReadContent(const TCHAR* pPath, ByteArray& aResult)
	{
		File f;
		if( !f.Open(pPath, _T("rb")) )
			return false;
		aResult.SetSize((int)f.GetSize());
		return f.Read(aResult.GetData(), aResult.GetSize());
	}

	static bool ReadContent(const TCHAR* pPath, void* pBuffer, int nLength, bool bCheckLength=true)
	{
		ByteArray a;
		if( !ReadContent(pPath, a) )
			return false;
		if( bCheckLength )
		{
			if( a.GetSize() != nLength )
				return false;
		}
		memcpy(pBuffer, a.GetData(), min(nLength, a.GetSize()));
		return true;
	}

	static bool WriteContent(const TCHAR* pPath, const char* pContent)
	{
		File f;
		if( !f.Open(pPath, _T("wb")) )
			return false;
		fputs(pContent, f);
		return true;
	}

#ifdef _UNICODE
	static bool WriteContent(const TCHAR* pPath, const WCHAR* pContent)
	{
		String1 sContent;
		{
			sContent = "";
			if( pContent && *pContent )
				sContent += "\xEF\xBB\xBF";
			sContent += W2U(pContent);
		}
		return File::WriteContent(pPath, sContent);
	}
#endif

	static bool WriteContent(const TCHAR* pPath, const BYTE* pData, int nLength)
	{
		File f;
		if( !f.Open(pPath, _T("wb")) )
			return false;
		fwrite(pData, nLength, 1, f);
		return true;
	}

	static bool WriteContent(const TCHAR* pPath, const ByteArray& aContent)
	{
		return WriteContent(pPath, aContent.GetData(), aContent.GetSize());
	}

	static String GetDir(const TCHAR* pPath)
	{
		String buf = pPath;
		int n = buf.rfind(_T('\\'));
		if( n < 0 )
			n = buf.rfind(_T('/'));
		return buf.substr(0, n + 1);
	}

	static String GetName(const TCHAR* pPath)
	{
		String buf = pPath;
		if( buf == _T("..") )
			return buf;
		int n = buf.rfind(_T('\\'));
		if( n < 0 )
			n = buf.rfind(_T('/'));
		return buf.substr(n + 1);
	}

	static String GetTitle(const TCHAR* pPath)
	{
		String buf = GetName(pPath);
		if( buf == _T("..") )
			return buf;
		int n = buf.rfind(_T('.'));
		if( n >= 0 )
			buf = buf.substr(0, n);
		return buf;
	}

	static String GetExt(const TCHAR* pPath)
	{
		String buf = GetName(pPath);
		if( buf == _T("..") )
		{
			buf = _T("");
			return buf;
		}
		int n = buf.rfind(_T('.'));
		if( n < 0 )
			buf = _T("");
		else
			buf = buf.substr(n + 1);
		return buf;
	}

	static bool IsFile(const TCHAR* pPath)
	{
		String buf = pPath;
		if( buf.length() <= 3 )
		{
		}
		else
		{
			if( buf.right(1) == _T("\\") )
				buf = buf.substr(0, buf.GetLength() - 1);
			if( buf.right(1) == _T("/") )
				buf = buf.substr(0, buf.GetLength() - 1);
		}

		struct _stat st;
		if( !_tstat(buf, &st) )
		{
			if( st.st_mode & _S_IFREG )
				return true;
		}
		return false;
	}

	static bool Stat(const TCHAR* pPath, struct _stat* pStat)
	{
		String buf = pPath;
		if( buf.length() <= 3 )
		{
		}
		else
		{
			if( buf.right(1) == _T("\\") )
				buf = buf.substr(0, buf.GetLength() - 1);
			if( buf.right(1) == _T("/") )
				buf = buf.substr(0, buf.GetLength() - 1);
		}

		if( !_tstat(buf, pStat) )
		{
			return true;
		}
		return false;
	}

	static bool IsDir(const TCHAR* pPath)
	{
		struct _stat st;
		if( Stat(pPath, &st) )
		{
			if( st.st_mode & _S_IFDIR )
				return true;
		}
		return false;
	}

	static time_t GetTime_update_clk(const TCHAR* pPath)
	{
		struct _stat st;
		if( Stat(pPath, &st) )
		{
			return (time_t)st.st_mtime;
		}
		return 0;
	}

	static bool Touch(const TCHAR* pPath)
	{
		File f;
		if( !f.Open(pPath, _T("wb")) )
			return false;
		f.Close();
		return true;
	}

	static bool CanAccess(const TCHAR* pPath, int mode=0)
	{
		return (_taccess(pPath, mode) == 0);
	}
};

#endif
