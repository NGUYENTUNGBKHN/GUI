#include "libcpp.h"
#include "JWin.h"

JRootWindow*	g_pRoot = NULL;

void APP_Init(JRootWindow* pRoot)
{
    g_pRoot = pRoot;
    JImage* pBusyImg = LoadImage(_T("./images/Busy.png"));
	g_pRoot->SetResource(pBusyImg);
}

