#ifndef PATHMAPTILE_H
#define PATHMAPTILE_H

struct PathmapTile
{
	PathmapTile(int x, int y, bool isBlockingFlag)
	: x(x)
	, y(y)
	, isBlockingFlag(isBlockingFlag)
	, isVisitedFlag(false)
	{
	}

	int x;
	int y;
	bool isBlockingFlag;
	bool isVisitedFlag;
};

inline bool operator==(const PathmapTile& lhs, const PathmapTile& rhs)
{
	return lhs.x == rhs.x && lhs.y == rhs.y;
}

#endif // PATHMAPTILE_H
