#pragma once

class JWindow;

class JDockInfo
{
public:
	int			m_nBase;
	int			m_nSize;
	JWindow*	m_pPane;
};

class JDockInfoArray : public Array<JDockInfo>
{
};


