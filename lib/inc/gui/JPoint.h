#pragma once

class JPoint
{
public:
	int		x;
	int		y;
public:
	JPoint()
	{
		set(0, 0);
	}

	JPoint(int x, int y)
	{
		set(x, y);
	}

	void set(int x, int y)
	{
		this->x = x;
		this->y = y;
	}
};



