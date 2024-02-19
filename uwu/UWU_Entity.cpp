#include "UWU_Entity.h"
#include <typeinfo>
#include <algorithm>

namespace ECS {
	UWU_Entity::UWU_Entity(UWU_World* worldSpawnedIn, std::string tag)
	{
		world = worldSpawnedIn;
		_tag = tag;
		UWU_Debug::Message("Created entity <%p>", DebugMask::Low, this);
	}

	UWU_Entity::~UWU_Entity()
	{
		UWU_Debug::Message("Destroyed entity <%p>", DebugMask::Low, this);
		for (auto& component : _components) {
			UWU_Debug::Message("Destroyed component <%p>", DebugMask::Low, component.second);
			delete component.second;
		}
		_components.clear();
	}

	void UWU_Entity::Update(float deltaTime)
	{
		if (_components.empty()) { return; }
		for (auto& component : _components) {
			if (component.second->enabled) {
				component.second->Update(deltaTime);
			}
		}
	}

	void UWU_Entity::LateUpdate(float deltaTime)
	{
		if (_components.empty()) { return; }
		for (auto& component : _components) {
			if (component.second->enabled) {
				component.second->LateUpdate(deltaTime);
			}
		}
	}

	void UWU_Entity::Render(UWU_Window* window)
	{
		if (_components.empty()) { return; }
		for (auto& component : _components) {
			if (component.second->enabled) {
				component.second->Render(window);
			}
		}
	}

	void UWU_Entity::OnCollision(UWU_Entity* other)
	{
		if (_components.empty()) { return; }
		for (auto& component : _components) {
			if (component.second->enabled) {
				component.second->OnCollision(other);
			}
		}
	}

	std::string UWU_Entity::GetTag()
	{
		return _tag;
	}

	void UWU_Entity::ReadableTypeName(char* name)
	{
		// The mangled name starts with a '_' on some platforms
		if (name[0] == '_') {
			++name;
		}

		// The mangled name may contain some extra information after the real name, delimited by a '.'
		const char* dot = strchr(name, '.');
		if (dot) {
			dot = 0;
		}
		else {
			const char* star = std::strstr(name, " *");
			star = 0;
		}
	}
	void UWU_Entity::InitialiseComponent(UWU_Component* component)
	{
		component->entity = this;
		component->Start();
	}
}