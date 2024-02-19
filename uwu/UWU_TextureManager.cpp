#include "UWU_TextureManager.h"
#include <SDL_image.h>
#include <filesystem>
#include <fstream>
#include "UWU_Debug.h"

bool UWU_TextureManager::LoadTexture(const std::string& fileName, const std::string& textureID, SDL_Renderer* renderer)
{
	//SDL_Surface* tempSurface = IMG_Load(fileName.c_str());
	//if (tempSurface == nullptr)
	//{
	//	UWU_Debug::Error("Error loading image: %s", SDL_GetError());
	//	return false;
	//}

	//SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
	//SDL_FreeSurface(tempSurface);

	SDL_Texture* texture = IMG_LoadTexture(renderer, fileName.c_str());

	if (texture == nullptr)
	{
		UWU_Debug::Error("Error creating texture: %s", DebugMask::High, SDL_GetError());
		return false;
	}

	UWU_Debug::Message("Loaded texture: %s", DebugMask::High, fileName.c_str());
	_textureMap[renderer][textureID] = texture;
	return true;
}

void UWU_TextureManager::DeleteInstance()
{
	delete& getInstance();
}

void UWU_TextureManager::Draw(SDL_Renderer* renderer, const std::string& textureID, const SDL_Rect* sourceRect, const SDL_Rect* destRect, double rotation, const SDL_Point* center, SDL_RendererFlip flip, SDL_Colour colour)
{
	//SDL_Rect newDest;
	//newDest.x = destRect->x * 2;
	//newDest.y = destRect->y * 2;
	//newDest.w = destRect->w * 2;
	//newDest.h = destRect->h * 2;
	SDL_SetTextureColorMod(_textureMap[renderer][textureID], colour.r, colour.g, colour.b);
	SDL_SetTextureAlphaMod(_textureMap[renderer][textureID], colour.a);
	SDL_RenderCopyEx(renderer, _textureMap[renderer][textureID], sourceRect, destRect, rotation, center, flip);
	SDL_SetTextureAlphaMod(_textureMap[renderer][textureID], 255);
	SDL_SetTextureColorMod(_textureMap[renderer][textureID], 255, 255, 255);
}

SDL_Texture* UWU_TextureManager::GetTexture(const std::string& textureID, SDL_Renderer* renderer)
{
	if (_textureMap[renderer].count(textureID) > 0)
	{
		return _textureMap[renderer][textureID];
	}
	else {
		UWU_Debug::Error("Tried to get texture <%s>, but it does not exist", DebugMask::High, textureID.c_str());
		return _textureMap[renderer]["default"];
	}
}

void UWU_TextureManager::FreeTexture(const std::string& textureID)
{
	for (auto& rendererPair : _textureMap)
	{
		SDL_DestroyTexture(rendererPair.second[textureID]);
		rendererPair.second.erase(textureID);
	}
}

UWU_TextureManager::~UWU_TextureManager()
{
	for (auto& rendererPair : _textureMap)
	{
		for (auto& texturePair : rendererPair.second)
		{
			SDL_Texture* texture = texturePair.second;
			int w, h;
			SDL_QueryTexture(texture, NULL, NULL, &w, &h); // check if the texture is still valid
			if (texture != nullptr && w > 0 && h > 0) // if it's still valid, destroy it
			{
				SDL_DestroyTexture(texture);
			}
			else {
				UWU_Debug::Error("Tried to destroy texture <%s>, but it does not exist", DebugMask::High, texturePair.first.c_str());
			}
		}
	}
}

void UWU_TextureManager::Initialise()
{
	UWU_Debug::DrawGap(DebugMask::High);
	UWU_Debug::DrawSeperator("Loading Animations...", DebugMask::High);
	//std::filesystem::current_path(std::filesystem::_path);
	for (const auto& file : std::filesystem::recursive_directory_iterator("Assets\\Textures")) {
		if (file.path().extension() == ".json") {
			LoadAnimationConfig(file.path().string());
		}
	}
}
void UWU_TextureManager::InitialiseRenderer(SDL_Renderer* renderer)
{
	UWU_Debug::DrawGap(DebugMask::High);
	UWU_Debug::DrawSeperator("Loading Textures For Renderer %p", DebugMask::High, renderer);
	for (const auto& file : std::filesystem::recursive_directory_iterator("Assets\\Textures")) {
		if (file.path().extension() == ".png") {
			LoadTexture(file.path().string(), file.path().filename().replace_extension().string(), renderer);
		}
	}
}

UWU_SpriteAnimation* UWU_TextureManager::GetAnimation(const std::string animationName)
{
	if (_animationMap.count(animationName) > 0)
	{
		return _animationMap[animationName];
	}
	else {
		UWU_Debug::Error("Tried to get animation <%s>, but it does not exist", DebugMask::High, animationName.c_str());
		return _animationMap["default"];
	}
}

void UWU_TextureManager::LoadAnimationConfig(const std::string& filePath) {

	nlohmann::json j;
	// Read JSON data from file
	try {
		std::ifstream input(filePath);
		input >> j;
	}
	catch (const std::exception& e) {
		UWU_Debug::Error("Failed to parse animation config file: %s", DebugMask::High, e.what());
	}

	std::vector<UWU_SpriteFrame*> frames;

	for (const auto& frame : j["frames"]) {
		frames.emplace_back(new UWU_SpriteFrame(frame));
	}
	std::vector<bool> usedFrames(frames.size(), false);
	std::filesystem::path fileNameWithExtension = j["meta"]["image"].get<std::string>();
	std::string fileName = fileNameWithExtension.replace_extension().string();
	// Parse JSON and create SpriteAnimation objects
	for (const auto& frameTag : j["meta"]["frameTags"]) {
		const std::string name = frameTag["name"].get<std::string>();

		std::vector<UWU_SpriteFrame*> animationFrames;
		const int startFrame = frameTag["from"].get<int>();
		const int endFrame = frameTag["to"].get<int>();
		for (int i = startFrame; i <= endFrame; i++)
		{
			int guy = animationFrames.size();
			animationFrames.push_back(frames[i]);
			usedFrames[i] = true;
		}
		_animationMap[name] = new UWU_SpriteAnimation(fileName, animationFrames);
		UWU_Debug::Message("Loaded animation: %s", DebugMask::High, name.c_str());
	}

	for (size_t i = 0; i < usedFrames.size(); i++)
	{
		if (!usedFrames[i]) {
			UWU_Debug::Warning("Unused animation frame found!", DebugMask::High);
			delete(frames[i]);
		}
	}
}
