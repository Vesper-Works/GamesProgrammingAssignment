#ifndef UWU_WORLD
#define UWU_WORLD

#include "LDtkLoader/Project.hpp"
#include "UWU_Window.h"

enum ItemType {
	Normal,
	Long,
	Heal,
	NONE
};

static const std::string ITEM_NAMES[] = {
	"Normal",
	"Long",
	"Heal",
	"NONE"
};

namespace ECS {
	class UWU_Entity;
}
class UWU_Physics;
class UWU_World
{
public:
	UWU_World(const ldtk::World*);
	~UWU_World();
	void Update(float);
	void Render(UWU_Window*);
	void AddEntity(ECS::UWU_Entity*);
	void RemoveEntity(ECS::UWU_Entity*);
private:
	void CalculateCollisionField(const ldtk::Layer& layer, int2);
	const ldtk::World* _world;
	std::vector<ECS::UWU_Entity*> _entities;
	std::vector<ECS::UWU_Entity*> _deleteBuffer;
	UWU_Physics* _physicsManager;
	void CreatePlayerEntity(float2);
	void CreateBlockEntity(float2 position, float2 size, std::string);
	void CreateEnemyEntity(float2 position, std::vector<int2>, int);
	void CreateItemEntity(float2 position, ItemType);
	void CreateExitEntity(float2 position, float2 size);
	void UpdateEntities(float);
	void RenderEntities(UWU_Window*);
	void PrepareScene(UWU_Window*);
	void PresentScene(UWU_Window*);
	void RenderScene(UWU_Window*);
};

#endif // !UWU_WORLD