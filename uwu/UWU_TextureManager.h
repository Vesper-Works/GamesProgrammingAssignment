#ifndef UWU_TEXTUREMANAGER
#define UWU_TEXTUREMANAGER
#include <unordered_map>
#include <string>
#include <SDL.h>
#include "UWU_SpriteFrame.hpp"
#include "UWU_SpriteAnimation.h"

class UWU_TextureManager
{
public:
	static UWU_TextureManager& getInstance()
	{
		static UWU_TextureManager instance;
		return instance;
	}
	static void DeleteInstance();
	void Draw(SDL_Renderer* renderer, const std::string& textureID, const SDL_Rect* sourceRect = NULL, const SDL_Rect* destRect = NULL, double rotation = 0, const SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_RendererFlip::SDL_FLIP_NONE, SDL_Colour colour = SDL_Colour{ 255,255,255 });
	SDL_Texture* GetTexture(const std::string& textureID, SDL_Renderer* renderer);
	void FreeTexture(const std::string& textureID);
	void Initialise();
	void InitialiseRenderer(SDL_Renderer* renderer);
	UWU_SpriteAnimation* GetAnimation(const std::string animationName);

	UWU_TextureManager(const UWU_TextureManager&) = delete;
	UWU_TextureManager& operator=(const UWU_TextureManager&) = delete;

private:
	UWU_TextureManager() = default;
	~UWU_TextureManager();
	bool LoadTexture(const std::string& fileName, const std::string& textureID, SDL_Renderer* renderer);
	void LoadAnimationConfig(const std::string& filePath);
	std::unordered_map<SDL_Renderer*, std::unordered_map<std::string, SDL_Texture*>> _textureMap;
	std::unordered_map<std::string, UWU_SpriteAnimation*> _animationMap;
};
#endif // !UWU_TEXTUREMANAGER