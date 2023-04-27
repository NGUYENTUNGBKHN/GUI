#include <libcpp.h>
#include "JWin.h"

Array<JWindow*>		JWindow::s_aRoot;


bool JKeypad_Init(JWindow* pParent)
{
	// return JKeypad::Init(pParent);
    return true;
}


bool JWindow::Init(PFN_MAINLOOPENTRY pfn)
{
	s_pfnMainLoopEntry = pfn;

	JWindow::s_mGlobalParam[_T("LANGUAGE")] = _T("JPN");

	JWindow::s_mGlobalParam[_T("MESSAGEBOX.BUTTON.TEXT.OK")]		= _T("OK");
	JWindow::s_mGlobalParam[_T("MESSAGEBOX.BUTTON.TEXT.CANCEL")]	= _T("Cancel");			//_T("ƒLƒƒƒ“ƒZƒ‹");
	JWindow::s_mGlobalParam[_T("MESSAGEBOX.BUTTON.TEXT.YES")]		= _T("Yes");			//_T("‚Í‚¢");
	JWindow::s_mGlobalParam[_T("MESSAGEBOX.BUTTON.TEXT.NO")]		= _T("No");				//_T("‚¢‚¢‚¦");
	JWindow::s_mGlobalParam[_T("MESSAGEBOX.BUTTON.TEXT.RETRY")]		= _T("Retry");			//_T("ÄŽŽs");

	return true;
}

bool JWIN_Init(PFN_MAINLOOPENTRY pfn)
{
	JWindow::Init(pfn);
	JDraw::Init();

#ifndef _WINDOWS
	extern bool G2D_Init();
	G2D_Init();
#endif

	return true;
}

JImage* VWIN_NewVImage(int w, int h)
{
	JImage* p = VWIN_NewVImage();
	if( !p->Create(w, h) )
	{
		delete p;
		return NULL;
	}
	return p;
}
