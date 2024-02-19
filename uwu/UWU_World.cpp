#include "UWU_World.h"
#include "UWU_TextureManager.h"
#include "UWU_RectUtil.h"
#include "LDtkLoader/Project.hpp"
#include "UWU_WindowManager.hpp"
#include <stack>
#include "UWU_Physics.h"
#include "UWU_ECS.h"
#include "UWU_InputManager.h"
#include "UWU_Exit.h"

int Area(int2 ll, int2 ur) {
	if (ll.x > ur.x || ll.y > ur.y) {
		return 0;
	}
	return (ur.x - ll.x + 1) * (ur.y - ll.y + 1);
};

void UpdateCache(int x, int gridHeight, std::vector<std::vector<bool>> grid, std::vector<int>& cache) {
	for (size_t y = 0; y < gridHeight; y++)
	{
		if (grid[x][y] != 0) {
			cache[y] = cache[y] + 1;
		}
		else {
			cache[y] = 0;
		}
	}
};

int2 Grow(int gridWidth, int gridHeight, std::vector<std::vector<bool>> grid, int2 ll, std::vector<int> cache) {
	int2 ur = int2(ll.x - 1, ll.y - 1);
	int x_max = gridWidth - 1;
	int y = ll.y - 1;
	while (y + 1 < gridHeight && grid[ll.x][y + 1] != 0) {
		y++;
		int x = min(ll.x + cache[y] - 1, x_max);
		x_max = x;
		if (Area(ll, int2(x, y)) > Area(ll, ur)) {
			ur = int2(x, y);
		}
	}
	return ur;
};
void UWU_World::CalculateCollisionField(const ldtk::Layer& layer, int2 levelOffset) {
	// Variables to keep track of the best rectangle so far: best_ll = (0, 0; best_ur = (-1, -1)
	SDL_Rect best_rect = { 0, 0, -1, -1 };
	int gridWidth = layer.getGridSize().x;
	int gridHeight = layer.getGridSize().y;
	int greatestWidth = 0;
	int y0 = 0;
	int w0 = 0;

	std::stack<std::pair<int, int>> stack;

	// Convert the tiles into a 2D array of 0s and 1s:
	std::vector<bool> column(gridHeight, 0);
	std::vector<std::vector<bool>> grid(gridWidth, column);
	for (const auto& tile : layer.allTiles()) {
		grid[tile.getGridPosition().x][tile.getGridPosition().y] = 1;
	}


	bool find = true;
	while (find) {
		UWU_Debug::Message("Grid:", DebugMask::High);
		for (int y = 0; y < gridHeight - 1; y++) {
			std::string row = "";
			for (int x = 0; x < gridWidth; x++) {
				row += std::to_string(grid[x][y]);
			}
			UWU_Debug::Message(row.c_str(), DebugMask::High);
		}
		std::vector<int> cache(gridHeight, 0);

		std::vector<SDL_Rect> rects;

		int2 ll = { 0, 0 };
		int2 ur = { 0, 0 };
		int2 best_ll = { 0, 0 };
		int2 best_ur = { -1, -1 };

		int count = 0;
		for (ll.x = gridWidth - 1; ll.x >= 0; ll.x--)
		{
			UpdateCache(ll.x, gridHeight, grid, cache);
			for (ll.y = 0; ll.y < gridHeight; ll.y++)
			{
				ur = Grow(gridWidth, gridHeight, grid, ll, cache);
				if (Area(ll, ur) > Area(best_ll, best_ur)) {
					best_ll = ll;
					best_ur = ur;
				}

			}
		}

		//Remove the rectangle from the grid:
		for (int x = best_ll.x; x <= best_ur.x; x++) {
			for (int y = best_ll.y; y <= best_ur.y; y++) {
				grid[x][y] = 0;
			}
		}

		best_rect = { best_ll.x, best_ll.y, best_ur.x - best_ll.x + 1, best_ur.y - best_ll.y + 1 };
		UWU_Debug::Message("Best rect: %d, %d, %d, %d", DebugMask::High, best_rect.x, best_rect.y, best_rect.w, best_rect.h);
		ECS::UWU_Entity* entity = new ECS::UWU_Entity(this);
		entity->AddComponent<ECS::UWU_Transform>(float2(best_rect.x * layer.getCellSize() + levelOffset.x, best_rect.y * layer.getCellSize() + levelOffset.y),
			float2(best_rect.w * layer.getCellSize(), best_rect.h * layer.getCellSize()));
		//entity->AddComponent<UWU_SpriteRenderer>();
		_physicsManager->AddBody(entity->AddComponent<ECS::UWU_Rigidbody>(float2(0, 0), 10, true));
		_entities.push_back(entity);

		//Stop looping if there are no more rectangles:
		find = false;
		for (int x = 0; x < gridWidth; x++) {
			for (int y = 0; y < gridHeight; y++) {
				if (grid[x][y] != 0) {
					find = true;
				}
			}
		}
	}
}

UWU_World::UWU_World(const ldtk::World* world)
{
	_world = world;
	_physicsManager = new UWU_Physics();
	for (auto& level : _world->allLevels()) {
		for (auto& layer : level.allLayers()) {
			if (!layer.isVisible()) {
				continue;
			}
			for (auto& entity : layer.allEntities()) {
				float2 position = float2(entity.getPosition().x + level.position.x, entity.getPosition().y + level.position.y);
				if (entity.getName() == "Player") {
					CreatePlayerEntity(position);
				}
				else if (entity.getName() == "Moveable_Block") {
					CreateBlockEntity(position, float2(entity.getSize().x, entity.getSize().y), entity.getField<ldtk::FieldType::String>("Texture").value());
				}
				else if (entity.getName() == "Enemy") {
					std::vector<int2> patrolPath;
					patrolPath.push_back((int2)position * RenderSizeMultiplier);
					auto& intPointPath = entity.getArrayField<ldtk::IntPoint>("patrol");

					for (auto& field : intPointPath) {
						patrolPath.push_back(int2(field.value().x * layer.getCellSize() + level.position.x * layer.getCellSize(), field.value().y + level.position.y) * RenderSizeMultiplier);
					}

					CreateEnemyEntity(position, patrolPath, entity.getField<int>("movement_speed").value());
				}
				else if (entity.getName() == "Item") {
					CreateItemEntity(position, ItemType::Normal);
				}
				else if (entity.getName() == "LongItem") {
					CreateItemEntity(position, ItemType::Long);
				}
				else if (entity.getName() == "HealItem") {
					CreateItemEntity(position, ItemType::Heal);
				}
				else if (entity.getName() == "Exit") {
					CreateExitEntity(position, float2(entity.getSize().x, entity.getSize().y));
				}
				else {
					UWU_Debug::Error("Trying to create unknown entity type: %s", DebugMask::High, entity.getName().c_str());
				}
			}
			if (layer.getName() == "Collisions") {
				CalculateCollisionField(layer, int2(level.position.x, level.position.y));
			}
		}
	}
}


UWU_World::~UWU_World()
{
	delete _physicsManager;
	for (auto& entity : _entities) {
		delete entity;
	}
}

void UWU_World::Update(float deltaTime)
{
	if (UWU_InputManager::getInstance().GetActionStateOnce(B_DebugAction1)) {
		float2 position = (float2)UWU_InputManager::getInstance().GetMousePosition() / RenderSizeMultiplier;
		ECS::UWU_Entity* entity = new ECS::UWU_Entity(this);
		entity->AddComponent<ECS::UWU_Transform>(position, float2(20, 20));
		_physicsManager->AddBody(entity->AddComponent<ECS::UWU_Rigidbody>(float2(0, 15), 5000));
		entity->AddComponent<ECS::UWU_SpriteRenderer>();
		entity->AddComponent<ECS::UWU_WindowOwner>(UWU_WindowManager::NewWindow((int2)position - int2(5, 5), int2(100, 100), 0, 0), float2(0.75f, 0.9f));
		_entities.push_back(entity);
	}
	if (UWU_InputManager::getInstance().GetActionStateOnce(B_DebugAction2)) {
		float2 position = (float2)UWU_InputManager::getInstance().GetMousePosition() / RenderSizeMultiplier;
		CreateItemEntity(position, Normal);
	}
	if (UWU_InputManager::getInstance().GetActionStateOnce(B_DebugAction3)) {
		float2 position = (float2)UWU_InputManager::getInstance().GetMousePosition() / RenderSizeMultiplier;
		CreateItemEntity(position, Long);
	}
	if (UWU_InputManager::getInstance().GetActionStateOnce(B_DebugAction4)) {
		float2 position = (float2)UWU_InputManager::getInstance().GetMousePosition() / RenderSizeMultiplier;
		CreateItemEntity(position, Heal);
	}
	for (auto& entity : _entities) {
		entity->Update(deltaTime);
	}
	for (auto& entity : _entities) {
		entity->LateUpdate(deltaTime);
	}

	_physicsManager->DoSteps(10);

	for (ECS::UWU_Entity* entity : _deleteBuffer) {
		if (entity == nullptr) {
			UWU_Debug::Warning("Trying to delete null entity", DebugMask::High);
			continue;
		}
		if (entity->HasComponent<ECS::UWU_Rigidbody>()) {
			_physicsManager->RemoveBody(entity->GetComponent<ECS::UWU_Rigidbody>());
		}

		_entities.erase(std::find(_entities.begin(), _entities.end(), entity));

		delete entity;
	}
	_deleteBuffer.clear();
}

void UWU_World::Render(UWU_Window* window)
{
	if (SDL_GetWindowFlags(window->GetWindow()) & SDL_WINDOW_MINIMIZED) {
		return;
	}
	PrepareScene(window);
	RenderScene(window);
	RenderEntities(window);
	PresentScene(window);
}

void UWU_World::AddEntity(ECS::UWU_Entity* entity)
{
	_entities.push_back(entity);
	ECS::UWU_Rigidbody* body = nullptr;
	if (entity->TryGetComponent<ECS::UWU_Rigidbody>(body)) {
		_physicsManager->AddBody(body);
	}
}

void UWU_World::RemoveEntity(ECS::UWU_Entity* entity)
{
	if (std::find(_deleteBuffer.begin(), _deleteBuffer.end(), entity) == _deleteBuffer.end()) {
		_deleteBuffer.push_back(entity);
	}
}

void UWU_World::CreatePlayerEntity(float2 position)
{
	ECS::UWU_Entity* entity = new ECS::UWU_Entity(this, "Player");
	entity->AddComponent<ECS::UWU_Transform>(position, float2(11, 20));
	_physicsManager->AddBody(entity->AddComponent<ECS::UWU_Rigidbody>(float2(0, 40)));
	entity->AddComponent<ECS::UWU_PlayerController>(400);
	entity->AddComponent<ECS::UWU_WindowOwner>(UWU_WindowManager::NewWindow((int2)position, int2(200, 200), 0, 0), float2(0.75f, 0.9f));
	entity->AddComponent<ECS::UWU_SpriteRenderer>("default", false);
	entity->AddComponent<ECS::UWU_SpriteAnimator>("player_idle");
	entity->AddComponent<ECS::UWU_Health>(3);
	_entities.push_back(entity);
}

void UWU_World::CreateBlockEntity(float2 position, float2 size, std::string textureName)
{
	ECS::UWU_Entity* entity = new ECS::UWU_Entity(this);
	entity->AddComponent<ECS::UWU_Transform>(position, size);
	_physicsManager->AddBody(entity->AddComponent<ECS::UWU_Rigidbody>(float2(0, 15), 5000));
	entity->AddComponent<ECS::UWU_SpriteRenderer>(textureName);
	_entities.push_back(entity);
}


void UWU_World::CreateEnemyEntity(float2 position, std::vector<int2> path, int movementSpeed)
{
	ECS::UWU_Entity* entity = new ECS::UWU_Entity(this);
	entity->AddComponent<ECS::UWU_Transform>(position, float2(11, 20));
	_physicsManager->AddBody(entity->AddComponent<ECS::UWU_Rigidbody>(float2(0, 40)));
	entity->AddComponent<ECS::UWU_WindowOwner>(UWU_WindowManager::NewWindow((int2)position, int2(50, 50), 0, 0), float2(0.75f, 0.9f));
	entity->AddComponent<ECS::UWU_EnemyController>(path, movementSpeed);
	entity->AddComponent<ECS::UWU_SpriteRenderer>("default", false, SDL_Colour(255,0,0,255));
	entity->AddComponent<ECS::UWU_SpriteAnimator>("default");
	entity->AddComponent<ECS::UWU_Health>(2);
	_entities.push_back(entity);
}

void UWU_World::CreateItemEntity(float2 position, ItemType power)
{
	SDL_Colour colour;
	switch (power)
	{
	case Normal:
		colour = { 255, 255, 255, 255 };
		break;
	case Long:
		colour = { 0, 0, 255, 255 };
		break;
	case Heal:
		colour = { 0, 255, 0, 255 };
		break;
	case NONE:
		colour = { 0, 0, 0, 0 };
		break;
	default:
		colour = { 255, 255, 255, 255 };
		break;
	}
	ECS::UWU_Entity* entity = new ECS::UWU_Entity(this, ITEM_NAMES[power]);
	entity->AddComponent<ECS::UWU_Transform>(position, float2(20, 20));
	_physicsManager->AddBody(entity->AddComponent<ECS::UWU_Rigidbody>(float2(0, 0), 1, true, true));
	entity->AddComponent<ECS::UWU_SpriteRenderer>("default", true, colour);
	entity->AddComponent<ECS::UWU_SpriteAnimator>("item");
	_entities.push_back(entity);
}

void UWU_World::CreateExitEntity(float2 position, float2 size)
{
	ECS::UWU_Entity* entity = new ECS::UWU_Entity(this);
	entity->AddComponent<ECS::UWU_Transform>(position, size);
	_physicsManager->AddBody(entity->AddComponent<ECS::UWU_Rigidbody>(float2(0, 0), 1, true, true));
	entity->AddComponent<ECS::UWU_Exit>();
	_entities.push_back(entity);
}

//void UWU_World::CreateEnemyEntity(float2 position, float2 size, std::string textureName)
//{
//	ECS::UWU_Entity* entity = new ECS::UWU_Entity(this);
//	entity->AddComponent<ECS::UWU_Transform>(position, size);
//	_physicsManager->AddBody(entity->AddComponent<ECS::UWU_Rigidbody>(float2(0, 15), 5000));
//	entity->AddComponent<ECS::UWU_SpriteRenderer>(textureName);
//	_entities.push_back(entity);
//}

void UWU_World::UpdateEntities(float deltaTime)
{
	for (auto& entity : _entities) {
		entity->Update(deltaTime);
	}
	for (auto& entity : _entities) {
		entity->LateUpdate(deltaTime);
	}
	_physicsManager->DoSteps(10);
}

void UWU_World::RenderEntities(UWU_Window* window)
{
	for (auto entity : _entities) {
		if (window->IsOverlapping(*entity->GetComponent<ECS::UWU_Transform>()->GetRect())) {
			entity->Render(window);
		}//If the entity isn't within the window, there's no reason to render it.
	}
	//UWU_TextManager::DrawTextW(window->GetRenderer(), "hi!", 10, 10);
}

void UWU_World::PrepareScene(UWU_Window* window)
{
	window->PrepareForRender(_world->getBgColor().r, _world->getBgColor().g, _world->getBgColor().b);
}

void UWU_World::PresentScene(UWU_Window* window)
{
	window->PresentRender();
}

void UWU_World::RenderScene(UWU_Window* window)
{
	for (auto& level : _world->allLevels()) {

		const auto& layers = level.allLayers();

		for (int i = layers.size() - 1; i >= 0; i--) {

			const ldtk::Layer& layer = layers[i];
			if (layer.allTiles().empty()) { continue; }

			std::filesystem::path tilesetPath = layer.getTileset().path;
			std::string tilesetTextureName = tilesetPath.filename().replace_extension().string();
			int opacity = layer.getOpacity() * 255;

			const auto& tiles = layer.allTiles();
			for (const auto& tile : tiles) {
				auto tile_position = tile.getTrueWorldPosition();
				auto tile_texture_rect = tile.getTextureRect();

				// Get destination rect on the window
				float2 tilePos = float2(tile_position.x, tile_position.y) * RenderSizeMultiplier;
				float2 tileSize = float2(1, 1) * layer.getCellSize() * RenderSizeMultiplier;

				if (!window->IsOverlapping((int2)tilePos, (int2)tileSize)) {
					continue;
				}
				float2 inversePos = window->GetInversePosition(tilePos);
				SDL_Rect dest = {
					inversePos.x, inversePos.y,
					layer.getCellSize() * RenderSizeMultiplier, layer.getCellSize() * RenderSizeMultiplier
				};
				// Get source rect on the tileset texture
				SDL_Rect src = {
					tile_texture_rect.x, tile_texture_rect.y,
					tile_texture_rect.width, tile_texture_rect.height
				};
				// Get flip flags
				int flip = (tile.flipX ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE)
					| (tile.flipY ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE);

				// Render the tile
				SDL_Colour colour = { 255,255,255,opacity };
				UWU_TextureManager::getInstance().Draw(window->GetRenderer(), tilesetTextureName, &src, &dest, 0, 0, (SDL_RendererFlip)flip, colour);
			}

		}

	}
}
