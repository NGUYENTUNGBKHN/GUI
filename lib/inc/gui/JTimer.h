#pragma once

class JWindow;

class JTimer
{
public:
	JWindow*	pWindow;
	int			nId;
	int			nPeriod;
	int			nRemain;
	bool		bDisableWhenInvisible;
	bool		bCyclic;
};



