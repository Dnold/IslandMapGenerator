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
bool isAnimatingForward = true;
float timer = 0;
float maxTime = 0.25f;
float delayTime = 0.7f; 
float delayTimer = 0; 
float currentPhase = 0;
float maxPhase = 4;
const int CHUNK_SIZE = 48;
const int GRID_SIZE = 2;
const int TILE_SIZE = 10;
Player* player;
Chunk*** chunks;
Chunk*** originalChunks;
MapRenderer mapRenderer;
MapGenerator mapGenerator;
std::vector<Vector2Int> islandTilePos;
//Things before the Update loop
std::vector<Chunk***> chunkPhases; // Speichert alle Phasen

void Start() {
    player = new Player();
    
    SetTargetFPS(120);
    chunkPhases.clear();
    // Generieren und Speichern der Phasen
    Chunk*** initialChunks = mapGenerator.GenerateChunks(GRID_SIZE, CHUNK_SIZE, MARGIN_SIZE);
    chunkPhases.push_back(initialChunks); // Speichert die ursprüngliche Konfiguration
    for (int phase = 0; phase < maxPhase; phase++) {
        Chunk*** animatedChunks = mapGenerator.AnimateChunks(initialChunks, GRID_SIZE, CHUNK_SIZE, phase);
        initialChunks = animatedChunks;
        chunkPhases.push_back(animatedChunks);
    }
}
//Is called once per frame
void Update() {
    BeginDrawing();
    ClearBackground(BLACK);
    if (IsKeyPressed(KEY_SPACE)) {
		//New Map
        Start();
	}
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

    Vector2Int totalGridSize = mapRenderer.GetTotalGridSize(CHUNK_SIZE, GRID_SIZE, TILE_SIZE);
    Vector2Int offset = mapRenderer.CalculateOffset({ GetScreenWidth(), GetScreenHeight() }, totalGridSize);
    mapRenderer.DrawChunkGrid(chunks, offset.x, offset.y, CHUNK_SIZE, GRID_SIZE, TILE_SIZE);
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
    srand(static_cast<unsigned int>(time(nullptr)));

    InitWindow(1000, 1000, "Hello World");
	Start();
	
	while (!WindowShouldClose()) {
		Update();
	}
	End();
	return 0;
}
	
