#ifndef UWU_SPRITEANIMATOR
#define UWU_SPRITEANIMATOR

#include "UWU_SpriteRenderer.h"
#include "UWU_SpriteFrame.hpp"
#include "UWU_SpriteAnimation.h"
#include "UWU_Timer.hpp"
namespace ECS {
	class UWU_SpriteAnimator :
		public UWU_Component
	{
	public:
		UWU_SpriteAnimator(std::string animationName, bool loop = true);
		~UWU_SpriteAnimator();
		void SetAnimation(std::string animationName);
	protected:
		UWU_SpriteRenderer* _renderer;

		// Inherited via UWU_Component
		virtual void Update(float) override;
		virtual void LateUpdate(float) override;
		virtual void Render(UWU_Window*) override;
		virtual void Start() override;
		virtual void OnCollision(UWU_Entity*) override;

	private:
		int _currentFrameNumber;
		UWU_Timer _timer;
		UWU_SpriteFrame* _currentFrame;
		UWU_SpriteRenderer* _spriteRenderer;
		UWU_SpriteAnimation* _animation;
		bool _loop;
	};
}
#endif // !UWU_SPRITEANIMATOR
