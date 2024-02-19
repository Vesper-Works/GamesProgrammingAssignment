#ifndef UWU_TEXTMANAGER
#define UWU_TEXTMANAGER
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#define FONT_SIZE 10
#define ATLAS_SIZE 256
class UWU_TextManager
{
public:

	static inline void InitialiseRenderer(SDL_Renderer* renderer) {

		_font = TTF_OpenFont("Assets/Fonts/ATTFShinGoProRegular.ttf", FONT_SIZE);

		SDL_Surface* atlasSurface = SDL_CreateRGBSurface(0, ATLAS_SIZE, ATLAS_SIZE, 32, 0, 0, 0, 0xff);
		SDL_SetColorKey(atlasSurface, SDL_TRUE, SDL_MapRGBA(atlasSurface->format, 0, 0, 0, 0));
		SDL_Rect dest{};
		for (int i = ' '; i <= 'z'; i++)
		{
			char c[2];
			c[0] = i;
			c[1] = 0;
			SDL_Surface* characterSurface = TTF_RenderUTF8_Solid(_font, c, SDL_Color{ 255,255,255 });

			TTF_SizeUTF8(_font, c, &dest.w, &dest.h);

			if (dest.x + dest.w >= ATLAS_SIZE)
			{
				dest.x = 0;

				dest.y += dest.h + 1;

				if (dest.y + dest.h >= ATLAS_SIZE)
				{
					UWU_Debug::Error("Font atlas is too small", DebugMask::High);
					exit(1);
				}
			}
			glyphs[i] = dest;
			SDL_BlitSurface(characterSurface, NULL, atlasSurface, &dest);
			SDL_FreeSurface(characterSurface);
			dest.x += dest.w;
		}
		_fontAtlases[renderer] = SDL_CreateTextureFromSurface(renderer, atlasSurface);
		SDL_FreeSurface(atlasSurface);
		UWU_Debug::Message("Initialised text for renderer: <%p>", DebugMask::High, renderer);
	}

	static inline void Draw(SDL_Renderer* renderer, const char* text, int x, int y, int r = 255, int g = 255, int b = 255) {
		Draw(renderer, std::string(text), x, y, r, g, b);
	}
	static inline void Draw(SDL_Renderer* renderer, std::string text, int x, int y, int r = 255, int g = 255, int b = 255) {
		SDL_SetTextureColorMod(_fontAtlases[renderer], r, g, b);
		int startX = x;
		for (auto& character : text) {
			if (character == '\n') {
				y += FONT_SIZE * RenderSizeMultiplier + 1;
				x = startX;
				continue;
			}
			SDL_Rect dest;
			dest.x = x;
			dest.y = y;
			dest.w = glyphs[character].w * RenderSizeMultiplier;
			dest.h = glyphs[character].h * RenderSizeMultiplier;
			SDL_RenderCopy(renderer, _fontAtlases[renderer], &glyphs[character], &dest);
			x += dest.w + RenderSizeMultiplier;
		}
		SDL_SetTextureColorMod(_fontAtlases[renderer], 255, 255, 255);
	}

private:
	static inline std::unordered_map<SDL_Renderer*, SDL_Texture*> _fontAtlases;
	static inline SDL_Rect glyphs[128];
	static inline TTF_Font* _font;
};

#endif // !UWU_TEXTMANAGER
