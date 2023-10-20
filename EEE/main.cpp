#include "raylib.h"
#include <cstdlib>
#include <ctime>
#include <malloc.h>
#include <chrono>
/*there are 2 ways of doing it.*/
#include <iostream> // including the main thing needed
const int MARGIN_SIZE = 0;
struct Vector2Int {
	int x;
	int y;
	Vector2Int(int _x, int _y) {
		x = _x;
		y = _y;
	}
	Vector2Int() : x(0), y(0) {}
};
struct Chunk {
	int id;
public:Vector2Int size;
public:int** map;
public:Vector2Int pos;

	  Chunk(Vector2Int _size, int _id) {
		  size = _size;
		  id = _id;
	  }
};

bool IsBorder(int x, int y, Vector2Int size) {
	return x == 0 || x == size.x - 1 || y == 0 || y == size.y - 1;
}

int GetRandomTile() {
	// Generates a random number between 0 and 99 (inclusive).
	int value = rand() % 100;

	// If the value is less than 60, return 1, else return 0.
	return (value < 45) ? 1 : 0;
}
int** GenerateRandomMap(int width, int height) {
	// Dynamically allocate memory for the 2D array
	int** map = (int**)malloc(width * sizeof(int*));
	for (int i = 0; i < width; i++) {
		map[i] = (int*)malloc(height * sizeof(int));
	}

	// Fill the map with random tiles
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {

			if (x >= MARGIN_SIZE && x < width - MARGIN_SIZE && y >= MARGIN_SIZE && y < height - MARGIN_SIZE && !IsBorder(x, y, Vector2Int(width, height))) {

				map[x][y] = GetRandomTile();
			}
			else {
				map[x][y] = 0;
			}
		}
	}
	return map;
}

Chunk CreateRandomChunk(int _id, Vector2Int _size) {

	Chunk chunk = Chunk(_size, _id);
	chunk.map = GenerateRandomMap(_size.x, _size.y);
	return chunk;

}

void DrawRandomTile(Vector2 pos, int size, int value) {
	if (value == 1) {
		DrawRectangle(pos.x, pos.y, size, size, DARKGREEN);
	}
	else if (value == 3) {
		DrawRectangle(pos.x, pos.y, size, size, RED);
	}
	else {
		DrawRectangle(pos.x, pos.y, size, size, BLUE);
	}
}

bool IsInMapRange(int x, int y, const Vector2Int& size) {
	return x >= 0 && x < size.x && y >= 0 && y < size.y;
}

int GetNeighbourCount(int gridX, int gridY, int** map, const Vector2Int& size) {
	int count = 0;
	// Directions representing the 8 neighboring cells
	int dirX[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
	int dirY[8] = { 1, 1, 1, 0, 0, -1, -1, -1 };

	for (int i = 0; i < 8; i++) {
		int neighbourX = gridX + dirX[i];
		int neighbourY = gridY + dirY[i];

		// If the neighbor is within map boundaries, check its status
		if (IsInMapRange(neighbourX, neighbourY, size)) {
			count += map[neighbourX][neighbourY];
		}
		else {
			// Out-of-boundary neighbors are considered alive to create a solid boundary.
			count++;
		}
	}
	return count;
}
void DrawBorder(int** map, Vector2Int size, float xOffset, float yOffset,int tileSize) {
	for (int x = 0; x < size.x; x++) {
		for (int y = 0; y < size.y; y++) {
			if (IsBorder(x, y, size)) {
				DrawRectangle(xOffset + x * tileSize, yOffset + y * tileSize,tileSize,tileSize, RED);
			}
		}
	}
}
int** SmoothMap(Vector2Int size, int** map) {
	for (int x = 0; x < size.x; x++) {
		for (int y = 0; y < size.y; y++) {
			int neighbourCount = GetNeighbourCount(x, y, map, size);

			if (IsBorder(x, y, size)) {
				continue;
			}
			if (neighbourCount > 4)
			{
				map[x][y] = 1;  // Convert to the target tile type
			}
			else if (neighbourCount < 4)
			{
				map[x][y] = 0;
			}
		}
	}
	return map;
}
Chunk*** GenerateChunks(int gridSize, int chunkSize) {

	Chunk*** chunks = new Chunk * *[gridSize];
	for (int i = 0; i < gridSize; i++) {
		chunks[i] = new Chunk * [gridSize];
	}
	for (int chunkX = 0; chunkX < gridSize; chunkX++) {
		for (int chunkY = 0; chunkY < gridSize; chunkY++) {
			chunks[chunkX][chunkY] = new Chunk(Vector2Int(chunkSize, chunkSize), 0);
			*(chunks[chunkX][chunkY]) = CreateRandomChunk(1, Vector2Int(chunkSize, chunkSize));
			for (int i = 0; i < 2; i++) {
				chunks[chunkX][chunkY]->map = SmoothMap(Vector2Int(chunkSize, chunkSize), chunks[chunkX][chunkY]->map);
			}
		}
	}
	return chunks;
}
Vector2Int GetTotalGridSize(int chunkSize, int gridSize, int tileSize) {
	int total = chunkSize * gridSize * tileSize;
	return Vector2Int(total,total);
}
void DrawChunk(Chunk* chunk, Vector2Int offset, int chunkSize, int tileSize) {
	for (int x = 0; x < chunkSize; x++) {
		for (int y = 0; y < chunkSize; y++) {
			DrawRandomTile({ offset.x + (float)x * tileSize,offset.y + (float)y * tileSize }, tileSize, chunk->map[x][y]);
		}
	}
}
void DrawChunkGrid(Chunk*** chunks, float offsetX, float offsetY,int chunkSize,int gridSize,int tileSize) {
	for (int chunkX = 0; chunkX < gridSize; chunkX++) {
		for (int chunkY = 0; chunkY < gridSize; chunkY++) {
			Vector2Int offset = { (int)offsetX + (chunkX * chunkSize * tileSize),
					  (int)offsetY + (chunkY * chunkSize * tileSize) };
			DrawChunk(chunks[chunkX][chunkY],offset,chunkSize,tileSize);
		}
	}
}

int main(void) {

	srand(static_cast<unsigned int>(time(nullptr)));
	const int CHUNK_SIZE = 100;
	const int GRID_SIZE = 25;
	const int TILE_SIZE = 1;

	Chunk*** chunks = GenerateChunks(GRID_SIZE,CHUNK_SIZE);

	InitWindow(1000, 1000, "Hello World");
	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(BLACK);

		Vector2Int totalGridSize = GetTotalGridSize(CHUNK_SIZE,GRID_SIZE,TILE_SIZE);

		float xOffset = (GetScreenWidth() - totalGridSize.x) / 2.0f;
		float yOffset = (GetScreenHeight() - totalGridSize.y) / 2.0f;
		
		DrawChunkGrid(chunks, xOffset, yOffset, CHUNK_SIZE, GRID_SIZE, TILE_SIZE);
		
		DrawFPS(10, 10);
		EndDrawing();
	}
	// Deallocate memory at the end
	for (int i = 0; i < GRID_SIZE; i++) {
		for (int j = 0; j < GRID_SIZE; j++) {
			delete chunks[i][j];
		}
		delete[] chunks[i];
	}
	delete[] chunks;

	CloseWindow();
	return 0;
}
