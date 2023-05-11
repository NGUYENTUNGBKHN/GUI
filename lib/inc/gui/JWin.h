#pragma once

#include "libcpp.h"

#include "JColor.h"
#include "JPoint.h"
#include "JRect.h"

#include "FreeType.h"
#include "JDraw.h"
#include "JImage.h"

class JWindow;
class JRootWindow;



#include "JListener.h"
#include "JDockInfo.h"
#include "JTimer.h"

extern JImage* JWIN_NewJImage();
extern JImage* JWIN_NewJImage(int w, int h);
extern JImage* LoadImage(const TCHAR* pPath);
extern bool JKeypad_Init(JWindow* pParent);
#include "JWindow.h"
#include "JToastInfo.h"
#include "JScreen.h"
#include "JLabel.h"
#include "JButton.h"
#include "JKeypad.h"

#include "JBusyWindow.h"
#include "JRootWindow.h"
extern void JWIN_WorkRootWindow_10ms(JRootWindow* pRoot, void* pArg1, void* pArg2, void* pArg3);
extern bool JWIN_Init(PFN_MAINLOOPENTRY pfn);

#include "depend_impl/JImage_mem_ARGB8888.h"
#include "depend_impl/JDraw_mem_ARGB8888.h"

