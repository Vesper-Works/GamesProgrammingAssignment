#ifndef UWU_TWEENING
#define UWU_TWEENING
#include <functional>
#include "UWU_Timer.hpp"

class UWU_Tweening {
private:
	struct Tween {
		int duration;
		UWU_Timer timer;
		std::shared_ptr <std::function<void()>> updateMethod;
		std::shared_ptr <std::function<void()>> endMethod;
		Tween(int duration, std::function<void()> updateMethod, std::function<void()> endMethod) : 
			duration(duration), 
			updateMethod(std::make_shared<std::function<void()>>(updateMethod)), 
			endMethod(std::make_shared<std::function<void()>>(endMethod)) {
		}

	};
	static inline std::vector<Tween> _tweens;
public:
	static inline void Update() {
		for (int i = _tweens.size() - 1; i >= 0; i--) {
			auto& tween = _tweens[i];
			if (tween.timer.GetDeltaTicks() >= tween.duration) {
				tween.endMethod->operator()();
				_tweens.erase(_tweens.begin() + i);
			}
			else {
				tween.updateMethod->operator()();
			}
		}

	}
	static inline void DoAfter(std::function<void()> method, int time) {
		DoFor([]() {}, method, time);
	}
	static inline void DoFor(std::function<void()> updateMethod, std::function<void()> endMethod, int time) {
		_tweens.emplace_back(Tween(time, updateMethod, endMethod));
	}
};

#endif //!UWU_TWEENING