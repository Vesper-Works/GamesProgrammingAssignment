#ifndef UWU_RECTUTIL
#define UWU_RECTUTIL
#include <SDL.h>
namespace UWU_RectUtil
{

	bool valueInRange(int value, int min, int max);

	bool RectOverlap(SDL_Rect A, const SDL_Rect* B);
};

#endif // !UWU_RECTUTIL

