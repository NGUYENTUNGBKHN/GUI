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
#include "JWindow.h"

#include "JScreen.h"
#include "JLabel.h"
#include "JButton.h"
#include "JKeypad.h"

extern JImage* JWIN_NewVImage();
extern JImage* JWIN_NewVImage(int w, int h);
extern bool JKeypad_Init(JWindow* pParent);

#include "JBusyWindow.h"
#include "JRootWindow.h"

extern bool JWIN_Init(PFN_MAINLOOPENTRY pfn);

#include "depend_impl/JImage_mem_ARGB8888.h"
#include "depend_impl/JDraw_mem_ARGB8888.h"

