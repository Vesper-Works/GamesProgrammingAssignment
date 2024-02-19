#include "UWU_RectUtil.h"

namespace UWU_RectUtil
{

	bool valueInRange(int value, int min, int max)
	{
		return (value >= min) && (value <= max);
	}

	bool RectOverlap(SDL_Rect A, const SDL_Rect* B)
	{
		bool xOverlap = valueInRange(A.x, B->x, B->x + B->w) ||
			valueInRange(B->x, A.x, A.x + A.w);

		bool yOverlap = valueInRange(A.y, B->y, B->y + B->h) ||
			valueInRange(B->y, A.y, A.y + A.h);

		return xOverlap && yOverlap;
	}
};