#include <SDL_rect.h>
#include <nlohmann/json.hpp>

#ifndef CUSTOMSERIALISERS
#define CUSTOMSERIALISERS
// Custom deserialiser for SDL_Rect

using json = nlohmann::json;

namespace nlohmann {
	template<>
	struct adl_serializer<SDL_Rect> {
		inline static void to_json(json& j, const SDL_Rect& rect) {
			j = json{ {"x", rect.x}, {"y", rect.y}, {"w", rect.w}, {"h", rect.h} };
		}

		inline static void from_json(const json& j, SDL_Rect& rect) {
			rect.x = j.at("x").get<int>();
			rect.y = j.at("y").get<int>();
			rect.w = j.at("w").get<int>();
			rect.h = j.at("h").get<int>();
		}
	};
}
#endif // ! CUSTOMSERIALISERS
