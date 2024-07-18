#include "raylib.h"
#include <cstdlib>
#include <ctime>
#include <malloc.h>
#include <chrono>
#include "MapGenerator.hpp"
#include "Player.h"
#include "MapRenderer.hpp"
#include "Inventory.h"
#include "TreasureChest.h"
#include <iostream>
#include "Pathfinder.h"
#include "astar.hpp"

bool isAnimatingForward = true;
float timer = 0;
float maxTime = 0.3f;
float delayTime = 1.0f;
float delayTimer = 0;
float currentPhase = 0;
float maxPhase = 6;

const int MARGIN_SIZE = 0;
const int CHUNK_SIZE = 32;
const int GRID_SIZE = 2;
const int TILE_SIZE = 15;

Vector2 offset;
Player* player;
Chunk*** chunks;

Chunk*** initialChunks;
std::vector<Chunk***> chunkPhases; // Speichert alle Phasen
Dynamic2DMapArray fullMap = Dynamic2DMapArray({ 0,0 });
Vector2Int totalGridSize;

std::pair<Vector2Int, Vector2Int> validStartEnd;
std::vector<Region> regions;

std::vector<Vector2Int> islandTilePos;
std::vector<Rectangle> islandRects;
std::vector<Rectangle> chestRects;

MapRenderer mapRenderer;
MapGenerator mapGenerator;

AStar::AStar<uint32_t, true> pathfinder;

Inventory* inventory;
std::vector<AStar::Vec2i> path;

bool shouldClose = false;

Vector2Int pixelToGrid(Vector2Int pixelPos) {
	return Vector2Int(pixelPos.x / TILE_SIZE, pixelPos.y / TILE_SIZE);
}

Vector2Int gridToPixel(Vector2Int gridPos) {
	return Vector2Int(gridPos.x * TILE_SIZE, gridPos.y * TILE_SIZE);
}


void SetupInventory()
{
	inventory = new Inventory(11);
}

void AnimatePhases()
{

	for (int phase = 0; phase < maxPhase; phase++) {
		Chunk*** animatedChunks = mapGenerator.AnimateChunks(initialChunks, GRID_SIZE, CHUNK_SIZE, phase);
		initialChunks = animatedChunks;
		chunkPhases.push_back(animatedChunks);
	}
}

void GenerateMap()
{
	chunkPhases.clear();
	islandRects.clear();
	islandTilePos.clear();
	// Generieren und Speichern der Phasen
	auto result = mapGenerator.GenerateChunks(GRID_SIZE, CHUNK_SIZE, MARGIN_SIZE);
	initialChunks = result.first;
	validStartEnd = result.second;
	chunkPhases.push_back(initialChunks); // Speichert die ursprüngliche Konfiguration
}

void GetRegions()
{
	std::vector<Region> regions = mapGenerator.regions;
	for (int i = 0; i < regions.size(); i++) {
		for (int j = 0; j < regions[i].tiles.size();j++) {
			islandTilePos.push_back(regions[i].tiles[j]);
		}
	}
}

void Start() {
	pathfinder = AStar::AStar<uint32_t, true>();
	SetTargetFPS(120);
	GenerateMap();
	AnimatePhases();
	GetRegions();
	totalGridSize = mapRenderer.GetTotalGridSize(CHUNK_SIZE, GRID_SIZE, TILE_SIZE);
	offset = { 0,0 };
	islandRects = mapGenerator.GetRegionRectangles(islandTilePos, totalGridSize.x, offset);
	fullMap = mapGenerator.ConcatenateChunks(initialChunks, GRID_SIZE, CHUNK_SIZE);

	chestRects = mapGenerator.SpawnChestsRandomChestOnWater(fullMap, { totalGridSize.x / TILE_SIZE,totalGridSize.y / TILE_SIZE }, 20);


	Vector2Int startTile = validStartEnd.first; // Startkachel aus ValidateMap
	Vector2Int startPixel = { startTile.x * TILE_SIZE, startTile.y * TILE_SIZE }; // Umrechnung in Pixelkoordinaten
	player = new Player(startPixel, 1); // Erstellung des Spielers mit Pixelkoordinaten
	islandRects.push_back({ (float)validStartEnd.second.x * 15,(float)validStartEnd.second.y * 15,15,15 });

	SetupInventory();
	pathfinder.setWorldSize(AStar::Vec2i(totalGridSize.x, totalGridSize.y));
	pathfinder.setDiagonalMovement(true);
	pathfinder.setHeuristic(AStar::Heuristic::manhattan);
	for (int i = 0; i < islandTilePos.size(); i++)
	{
		auto obsPos = AStar::Vec2i((islandTilePos[i].x * TILE_SIZE), (islandTilePos[i].y * TILE_SIZE));
		for(int x = 0; x < 15;x+=5)
		{
			for(int y = 0; y < 15; y+=5)
			{
				pathfinder.addObstacle({ obsPos.x + x, obsPos.y + y });
			}
		}
		
	}

	Vector2Int startPosPixel = { validStartEnd.first.x * TILE_SIZE, validStartEnd.first.y * TILE_SIZE };
	Vector2Int endPosPixel = { validStartEnd.second.x * TILE_SIZE, validStartEnd.second.y * TILE_SIZE };
	path = pathfinder.findPath(AStar::Vec2i(startPosPixel.x, startPosPixel.y), AStar::Vec2i(endPosPixel.x, endPosPixel.y));

}

void CheckMouseHoverAndClick() {
	Vector2 mousePosition = GetMousePosition();
	int startX = 800; // Startposition der Inventaritems auf dem Bildschirm
	int startY = 50;
	int lineHeight = 80; // Höhe jedes Gegenstandsslots
	int itemCount = inventory->items->GetSize();

	for (int i = 0; i < itemCount; ++i) {
		Rectangle itemSlotRect = { (float)startX, (float)(startY + i * lineHeight), 200, (float)lineHeight };
		if (CheckCollisionPointRec(mousePosition, itemSlotRect)) {
			if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
				// Gegenstand wurde angeklickt, versuche ihn in den Ausrüstungsslot zu laden
				auto item = inventory->items->GetItem<std::shared_ptr<BaseItem>>(i);
				if (inventory->TryEquipItem(item)) {
					// Gegenstand erfolgreich ausgerüstet, entferne ihn aus dem Inventar
					inventory->items->RemoveItem(i);
					break; // Verarbeitung beenden, da der Gegenstand entfernt wurde
				}
			}
		}
	}
	// Check Mouse Hover and Click for equiped Itmes
	for (int i = 0; i < 3; i++)
	{
		Rectangle itemSlotRect = { (float)(i * 100), 0, 100, 100 };
		if (CheckCollisionPointRec(mousePosition, itemSlotRect)) {
			if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
				// Gegenstand wurde angeklickt, versuche ihn aus dem Ausrüstungsslot zu entfernen
				Inventory* inventoryCopy = inventory; // Kopie des Zeigers für die Verwendung in der Lambda
				std::function<void(std::shared_ptr<BaseItem>)> returnToInventory = [inventoryCopy](std::shared_ptr<BaseItem> item) {
					inventoryCopy->AddItem<BaseItem>(item);
				};
				inventory->equipmentSlots[i].UnequipItem(returnToInventory); // Zugriff auf das spezifische EquipmentSlot-Objekt
			}
		}
	}
}
void HandleInventoryInput()
{
	if (IsKeyPressed(KEY_SPACE)) {
		//New Map
		Start();
	}
	if (IsKeyPressed(KEY_R))
	{
		inventory->SortInventoryWeight();
	}
	if (IsKeyPressed(KEY_W))
	{
		inventory->SortInventoryName();
	}
	if (IsKeyPressed(KEY_P))
	{
		inventory->SortInventoryPrice();
	}
}
void AnimateWater()
{
	if (timer < maxTime) {
		timer += GetFrameTime();
	}
	else {
		timer = 0;
		if (isAnimatingForward) {
			if (currentPhase < maxPhase) {
				currentPhase++;
			}
			else {
				isAnimatingForward = false; // Wechseln Sie die Richtung
				currentPhase--;
			}
		}
		else {
			if (currentPhase > 1) {
				currentPhase--;
			}
			else {
				isAnimatingForward = true; // Wechseln Sie die Richtung
				currentPhase++;
			}
		}
	}

	// Verwenden Sie die zwischengespeicherten Phasen
	chunks = chunkPhases[currentPhase];
}

void DrawMap()
{
	mapRenderer.DrawChunkGrid(chunkPhases[currentPhase], offset.x, offset.y, CHUNK_SIZE, GRID_SIZE, TILE_SIZE);
}

void DrawChests()
{
	for (int i = 0; i < chestRects.size(); i++)
	{
		DrawRectangleLinesEx(chestRects[i], 1, GOLD);
	}
}

bool CheckCollisionIslandRects(Rectangle playerRect)
{
	for (int i = 0; i < islandRects.size(); i++) {
		if (CheckCollisionRecs(playerRect, islandRects[i]))
		{
			return true;
		}
	}
	return false;
}

std::pair<int, bool> CheckCollisionChests(Rectangle playerRect)
{
	for (int i = 0; i < chestRects.size(); i++) {
		if (CheckCollisionRecs(playerRect, chestRects[i]))
		{
			return std::pair<int, bool>(i, true);
		}
	}
	return std::pair<int, bool>(-1, false);
}
void GetChestLoot(int toDelete)
{
	TreasureChest* chest = new TreasureChest(toDelete, chestRects[toDelete], 1);

	std::vector<std::shared_ptr<BaseItem>> items = chest->GetItems();
	for (int j = 0; j < items.size(); j++)
	{
		inventory->AddItem<BaseItem>(items[j]);
	}
	if (toDelete != -1) chestRects.erase(chestRects.begin() + toDelete);

}
void DrawLoop()
{
	ClearBackground(BLACK);
	DrawMap();
	AnimateWater();
	DrawChests();
	player->Draw();
	inventory->RenderItems(800, 50, 80);
	inventory->RenderEquipmentSlots(0, 0, 100);
	DrawFPS(10, 10);
	for (int i = 0; i < path.size(); i++)
	{

		DrawRectangle(path[i].x*TILE_SIZE, path[i].y*TILE_SIZE, 15, 15, RED);
	}


	auto obstacles = pathfinder.getObstacles();
	for (const auto& obstacle : obstacles)
	{
		DrawRectangle(obstacle.x, obstacle.y, 1, 1, BLUE);
	}

}
float CalculateInventoryWeight(Inventory* inventory)
{
	float weight = 0;
	for (int i = 0; i < inventory->items->GetSize(); i++)
	{
		auto item = inventory->items->GetItem<std::shared_ptr<BaseItem>>(i);
		weight += item->weight;
	}
	for (int i = 0; i <= 2; i++)
	{
		auto item = inventory->equipmentSlots[i];
		if (item.itemSlot != nullptr)
		{
			if (item.itemSlot > 0) {
				weight -= weight * item.itemSlot->strength;
				continue;
			}
			weight += item.itemSlot->weight;
		}
	}
	return weight;
}

//Is called once per frame
void Update() {
	//static std::vector<Vector2Int> path;
	//static size_t currentTargetIndex = 0;
	//static std::vector<Vector2Int> targets;

	//// Logic to set up targets if not initialized
	//if (targets.empty()) {
	//	// Example: Populate targets based on chestRects
	//	for (const auto& chestRect : chestRects) {
	//		Vector2Int targetPos = pixelToGrid({ (int)(chestRect.x + chestRect.width / 2.0f), (int)(chestRect.y + chestRect.height / 2.0f)
	//			});
	//		targets.push_back(targetPos);
	//	}
	//}

	//// Debug output to verify targets
	//std::cout << "Targets size: " << targets.size() << std::endl;
	//for (const auto& target : targets) {
	//	std::cout << "Target: " << target.x << ", " << target.y << std::endl;
	//}

	//// Calculate path if not already calculated and there are targets
	//if (path.empty() && currentTargetIndex < targets.size()) {
	//	// Calculate path to current target
	//	Vector2Int currentGridPos = pixelToGrid(player->pos);
	//	path = Pathfinder::FindPath(fullMap, currentGridPos, targets[currentTargetIndex]);
	//	currentTargetIndex++;
	//}

	//// Debug output to verify path
	//std::cout << "Path size: " << path.size() << std::endl;
	//for (const auto& pos : path) {
	//	std::cout << "Path point: " << pos.x << ", " << pos.y << std::endl;
	//}

	//// Follow the path with dynamic rotation
	//if (!path.empty()) {
	//	player->FollowPath(path);
	//}

	//// Check if player has reached the current path point
	//if (!path.empty() && player->pos == gridToPixel(path.front())) {
	//	path.erase(path.begin());
	//}

	//if (!path.empty()) {
	//	player->FollowPath(path);
	//}
	player->CheckOutOfBounds();
	player->Move();
	player->Update();
	player->UpdateWeight(CalculateInventoryWeight(inventory));

	Rectangle playerRect = player->rect;
	std::pair<int, bool> chestCollision = CheckCollisionChests(playerRect);
	if (chestCollision.second) {
		GetChestLoot(chestCollision.first);
		/*path.clear();*/
	}

	if (CheckCollisionIslandRects(playerRect)) {
		Start();
	}

	HandleInventoryInput();
	CheckMouseHoverAndClick();

	BeginDrawing();
	DrawLoop();
	EndDrawing();
}

//Things for when the program ends
void End() {
	// Deallocate memory at the end
	CloseWindow();
}

int main(void) {
	srand(static_cast<unsigned int>(time(nullptr)));

	InitWindow(960, 960, "Hello World");

	Start();

	while (!WindowShouldClose() && !shouldClose) {
		Update();
	}
	End();
	return 0;
}

