
#ifndef UWU_SPRITEFRAME
#define UWU_SPRITEFRAME

#include <SDL_rect.h>
#include <nlohmann/json.hpp>
#include "CustomJSONSerialisation.hpp"
using json = nlohmann::json;
struct UWU_SpriteFrame {
	SDL_Rect Rect;
	int Duration;
    UWU_SpriteFrame(const nlohmann::json& j) {
        Rect = j["frame"];
        Duration = j["duration"];
    }
};

#endif // !UWU_SPRITEFRAME