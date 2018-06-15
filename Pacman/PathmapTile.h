#ifndef PATHMAPTILE_H
#define PATHMAPTILE_H

struct PathmapTile
{
	PathmapTile(int x, int y, bool isBlockingFlag)
	: x(x)
	, y(y)
	, isBlockingFlag(isBlockingFlag)
	,isVisitedFlag(false)
	{
	}

	int x;
	int y;
	bool isBlockingFlag;
	bool isVisitedFlag;

	static bool less(const PathmapTile& lhs, const PathmapTile& rhs)
	{
		return lhs < rhs;
	}

	bool operator<(const PathmapTile &other) const
	{
		return x * y < other.x * other.y;
	}

	bool operator>(const PathmapTile &other) const
	{
		return x * y > other.x * other.y;
	}
};

#endif // PATHMAPTILE_H