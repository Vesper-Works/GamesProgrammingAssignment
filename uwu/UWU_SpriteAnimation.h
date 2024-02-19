#include <SDL_rect.h>
#include <nlohmann/json.hpp>
#include "UWU_SpriteFrame.hpp"
using json = nlohmann::json;

#ifndef UWU_SPRITEANIMATION
#define UWU_SPRITEANIMATION

class UWU_SpriteAnimation
{

public:
	UWU_SpriteAnimation(const std::string& spriteFileName, const std::vector<UWU_SpriteFrame*>& frames) {
		SpriteFileName = spriteFileName;
		Frames = frames;
	}
	std::string SpriteFileName;
	std::vector<UWU_SpriteFrame*> Frames;
	UWU_SpriteFrame* GetNextFrame(int& frameNumber) {
		if (++frameNumber >= Frames.size()) {
			frameNumber = 0;
			return Frames[0];
		}
		return Frames[frameNumber];
	}
};

#endif // !UWU_SPRITEANIMATION


