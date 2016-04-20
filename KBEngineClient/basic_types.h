#ifndef BASIC_TYPES_H
#define BASIC_TYPES_H

#include <math.h>
#include <stdint.h>

namespace KBEngineClient
{
	typedef	int8_t		int8;
	typedef	uint8_t		uint8;
	typedef	int16_t		int16;
	typedef	uint16_t	uint16;
	typedef	int32_t		int32;
	typedef	uint32_t	uint32;
	typedef	int64_t		int64;
	typedef	uint64_t	uint64;

	class tPoint
	{
	public:
		int x;
		int y;
	public:
		tPoint()
		{
			x = 0;
			y = 0;
		}
		tPoint(int x, int y)
		{
			this->x = x;
			this->y = y;
		}
		bool operator== (const tPoint& Right)
		{
			if (Right.x == x && Right.y == y)
			{
				return true;
			}
			return false;
		}
		bool operator< (const tPoint& Right)
		{
			return (x != Right.x) ? (x < Right.x) : (y < Right.y);
		}
		float distance(const tPoint& target)
		{
			return sqrtf((float)((x - target.x)*(x - target.x) + (y - target.y)*(y - target.y)));
		}
	};

	inline bool operator< (const tPoint& Left, const tPoint& Right)
	{
		return (Left.x != Right.x) ? (Left.x < Right.x) : (Left.y < Right.y);
	}

}

#endif
