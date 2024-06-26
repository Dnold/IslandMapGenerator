#include "raylib.h"
#include <cstdlib>
#include <ctime>
#include <malloc.h>
#include <chrono>
#include "MapGenerator.hpp"
#include "Player.h"
#include "MapRenderer.hpp"

#include <iostream>
const int MARGIN_SIZE = 0;


const int CHUNK_SIZE = 32;
const int GRID_SIZE = 3;
const int TILE_SIZE = 10;
Player* player;
Chunk*** chunks;
MapRenderer mapRenderer;
MapGenerator mapGenerator;
//Things before the Update loop
void Start() { 
	
	player = new Player();
	srand(static_cast<unsigned int>(time(nullptr)));
	
	chunks = mapGenerator.GenerateChunks(GRID_SIZE, CHUNK_SIZE,MARGIN_SIZE); // Initialize the map

	InitWindow(1000, 1000, "Hello World"); //Open Window
	SetTargetFPS(120);
}
//Is called once per frame
void Update(){
	BeginDrawing();
	ClearBackground(BLACK);
	if (IsKeyPressed(KEY_R)) {
		chunks = mapGenerator.GenerateChunks(GRID_SIZE, CHUNK_SIZE,MARGIN_SIZE); //Reload the map once if R is pressed
	}
	if (IsKeyDown(KEY_T)) {
		chunks = mapGenerator.GenerateChunks(GRID_SIZE, CHUNK_SIZE,MARGIN_SIZE); //Reload the map every frame if T is pressed
	}

	Vector2Int totalGridSize = mapRenderer.GetTotalGridSize(CHUNK_SIZE, GRID_SIZE, TILE_SIZE); // Calculate the total size of the map

	Vector2Int offset = mapRenderer.CalculateOffset({ GetScreenWidth(),GetScreenHeight() }, totalGridSize); // Calculate the offset to center the map

	mapRenderer.DrawChunkGrid(chunks, offset.x, offset.y, CHUNK_SIZE, GRID_SIZE, TILE_SIZE); // Draw the map
	player->Update();
	DrawFPS(10, 10);
	EndDrawing();
}
//Things for when the program ends
void End() {
	// Deallocate memory at the end
	

	CloseWindow();
	
}

int main(void) {
	Start();
	
	while (!WindowShouldClose()) {
		Update();
	}
	End();
	return 0;
}
	
