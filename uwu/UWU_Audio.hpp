#ifndef UWU_AUDIO
#define UWU_AUDIO
#include <unordered_map>
#include <SDL_mixer.h>
#include "UWU_Debug.h"
#include <filesystem>

class UWU_Audio
{
public:
	static inline void Initialise() {
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
			UWU_Debug::Message("SDL_mixer could not initialize! SDL_mixer Error: %s\n", DebugMask::High, Mix_GetError());
		}
		for (const auto& file : std::filesystem::recursive_directory_iterator("Assets\\Sounds")) {
			if (file.path().extension() == ".mp3") {
				LoadMusic(file.path().string(), file.path().filename().replace_extension().string());
			}
			else if (file.path().extension() == ".wav") {
				LoadSound(file.path().string(), file.path().filename().replace_extension().string());
			}
		}
	}
	static inline void PlaySoundEffect(const std::string& name, float volume = 1) {
		if (_sounds.count(name) > 0) {
			Mix_VolumeChunk(_sounds[name], std::clamp(volume, 0.f, 1.f) * MIX_MAX_VOLUME);
			Mix_PlayChannel(-1, _sounds[name], 0);
		}
		else {
			UWU_Debug::Error("Tried to play sound <%s>, but it does not exist", DebugMask::High, name.c_str());
		}
	}
	static inline void PlayMusic(const std::string& name, float volume = 1) {
		if (_music.count(name) > 0) {
			Mix_VolumeMusic(std::clamp(volume, 0.f, 1.f) * MIX_MAX_VOLUME);
			Mix_PlayMusic(_music[name], -1);
		}
		else {
			UWU_Debug::Error("Tried to play music <%s>, but it does not exist", DebugMask::High, name.c_str());
		}
	}
	static inline void StopMusic() {
		Mix_HaltMusic();
	}
	static inline void ToggleMute() {
		if (muted) {
			Mix_VolumeMusic(MIX_MAX_VOLUME);
			Mix_Volume(-1, MIX_MAX_VOLUME);
		}
		else {
			Mix_VolumeMusic(0);
			Mix_Volume(-1, 0);
		}
		muted = !muted;
	}
private:
	static inline std::unordered_map<std::string, Mix_Chunk*> _sounds;
	static inline std::unordered_map<std::string, Mix_Music*> _music;
	static inline bool muted = false;

	static inline void LoadSound(const std::string& path, const std::string& name) {
		Mix_Chunk* sound = Mix_LoadWAV(path.c_str());
		if (sound == nullptr) {
			UWU_Debug::Error("Failed to load sound <%s>! SDL_mixer Error: %s\n", DebugMask::High, name.c_str(), Mix_GetError());
		}
		else {
			_sounds[name] = sound;
			UWU_Debug::Message("Loaded sound <%s> from <%s>\n", DebugMask::High, name.c_str(), path.c_str());
		}
	}
	static inline void LoadMusic(const std::string& path, const std::string& name) {
		Mix_Music* music = Mix_LoadMUS(path.c_str());
		if (music == nullptr) {
			UWU_Debug::Error("Failed to load music <%s>! SDL_mixer Error: %s\n", DebugMask::High, name.c_str(), Mix_GetError());
		}
		else {
			_music[name] = music;
			UWU_Debug::Message("Loaded music <%s> from <%s>\n", DebugMask::High, name.c_str(), path.c_str());
		}
	}
};

#endif // !UWU_AUDIO
