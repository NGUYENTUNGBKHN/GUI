#pragma once

class JListener
{
public:
	JWindow*	m_pOwner;

public:
	JListener()
	{
		m_pOwner = NULL;
	}

	JListener(JWindow* pOwner)
	{	
		m_pOwner = pOwner;
	}
public:
	virtual bool on(JWindow* pOwner, const String& str, int num=0, void* ptr=NULL) = 0;
};

