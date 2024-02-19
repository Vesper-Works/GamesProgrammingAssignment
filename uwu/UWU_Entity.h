#ifndef UWU_ENTITY
#define UWU_ENTITY 
#include <string>
#include "UWU_Debug.h"
#include <typeinfo>
#include "UWU_Component.h"
#include "UWU_Window.h"
#include <unordered_map>
#include "UWU_World.h"
class UWU_World;
namespace ECS {
	class UWU_Component;
	class UWU_Entity
	{
	public:
		UWU_Entity(UWU_World*, std::string = "");
		~UWU_Entity();
		void Update(float);
		void LateUpdate(float);
		void Render(UWU_Window*);
		void OnCollision(UWU_Entity*);
		std::string GetTag();
		template <typename T, typename... TArgs> T* AddComponent(TArgs&&... mArgs)
		{
			T* component = new T(std::forward<TArgs>(mArgs)...);
			InitialiseComponent(static_cast<UWU_Component*>(component));
			char* name = const_cast<char*>(typeid(T).name());
			ReadableTypeName(name);
			if (_components.contains(name)) {
				UWU_Debug::Error("Entity <%p> already has %s, deleting", DebugMask::Medium, this, name);
				delete component;
				return nullptr;
			}
			_components.emplace(name, component);

			UWU_Debug::Message("Created component: %s <%p>", DebugMask::Low, name, component);
			return component;
		}
		inline void AddExistingComponent(void* newComponent)
		{
			UWU_Component* component = static_cast<UWU_Component*>(newComponent);
			InitialiseComponent(component);
			char* name = const_cast<char*>(typeid(component).name());
			ReadableTypeName(name);
			if (_components.contains(name)) {
				UWU_Debug::Error("Entity <%p> already has %s, deleting", DebugMask::Medium, this, name);
				delete component;
			}
			_components.emplace(name, component);
			UWU_Debug::Message("Created component: %s <%p>", DebugMask::Low, name, component);
		}

		template <typename T> T* GetComponent() {
			char* name = const_cast<char*>(typeid(T).name());
			ReadableTypeName(name);
			UWU_Component* component = _components.at(name);
			if (component != nullptr)
			{
				return static_cast<T*>(component);
			}
			else {
				UWU_Debug::UWU_Debug::Error("Entity <%p> failed to retrieve component %s", DebugMask::Low, this, name);
			}
		}

		template <typename T> void RemoveComponent() {
			char* name = const_cast<char*>(typeid(T).name());
			ReadableTypeName(name);
			UWU_Component* component = _components.at(name);
			if (component != nullptr)
			{
				delete component;
				_components.erase(name);
			}
			else {
				UWU_Debug::UWU_Debug::Error("Entity <%p> failed to remove component %s", DebugMask::Low, this, name);
			}
		}

		template <typename T> bool HasComponent() {
			char* name = const_cast<char*>(typeid(T).name());
			ReadableTypeName(name);
			return _components.contains(name);
		}

		template <typename T> bool TryGetComponent(T*& component) {
			char* name = const_cast<char*>(typeid(T).name());
			ReadableTypeName(name);
			if (_components.contains(name)) {
				component = static_cast<T*>(_components.at(name));
				return true;
			}
			else {
				return false;
			}
		}

		UWU_World* world;

	private:
		std::string _tag;
		void ReadableTypeName(char*);
		std::unordered_map<char*, UWU_Component*> _components;
		void InitialiseComponent(UWU_Component*);
	};

}
#endif // !UWU_ENTITY

