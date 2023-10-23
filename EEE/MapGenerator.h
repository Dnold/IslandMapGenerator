#include "MapGeneratorHelpers.h"
#include "raylib.h"
#include <cstdlib>
#include <ctime>
#include <malloc.h>
#include <chrono>
#include "TileDefinitions.h"
#include <queue>
#include <iostream>


class MapGenerator : public MapGeneratorHelpers
{
	int GetRandomTile() {
		// Generates a random number between 0 and 99 (inclusive).
		int value = rand() % 100;

		// If the value is less than 60, return 1, else return 0.
		return (value < 45) ? 1 : 0;
	}
	Dynamic2DMapArray GenerateRandomMap(int width, int height, int marginSize) {
		// Dynamically allocate memory for the 2D array
		Dynamic2DMapArray map = Dynamic2DMapArray(Vector2Int(width, height));


		// Fill the map with random tiles
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {

				if (x >= marginSize && x < width - marginSize && y >= marginSize && y < height - marginSize && !IsBorder(x, y, Vector2Int(width, height))) {

					map.SetValue(x, y, GetRandomTile());
				}
				else {
					map.SetValue(x, y, (int)TileType::Water);
				}
			}
		}
		return map;
	}
	Chunk CreateRandomChunk(int _id, Vector2Int _size, int marginSize) {

		Chunk chunk = Chunk(_size, _id, GenerateRandomMap(_size.x, _size.y, marginSize));
		return chunk;

	}
	Dynamic2DMapArray SmoothMap(Vector2Int size, Dynamic2DMapArray map) {
		for (int x = 0; x < size.x; x++) {
			for (int y = 0; y < size.y; y++) {
				int neighbourCount = GetNeighbourCount(x, y, map, size);

				if (IsBorder(x, y, size)) {
					continue;
				}
				if (neighbourCount > 4)
				{
					map.SetValue(x, y, (int)TileType::Island);  // Convert to the target tile type
				}
				else if (neighbourCount < 4)
				{
					map.SetValue(x, y, (int)TileType::Water);
				}
			}
		}
		return map;
	}
	std::vector<Vector2Int> GetRegionTiles(int x, int y, Dynamic2DMapArray map, int** mapFlags) {
		Vector2Int size = map.GetSize();
		std::vector<Vector2Int> tiles;

		mapFlags[x][y] = 1;
		TileType currentTileType = (TileType)map.GetValue(x, y);

		std::queue<Vector2Int> queue;
		queue.push(Vector2Int(x, y));

		int dirX[] = { 0, 0, 1, -1 };
		int dirY[] = { 1, -1, 0, 0 };

		while (queue.size() > 0) {
			Vector2Int tile = queue.front();
			queue.pop();
			tiles.push_back(tile);

			for (int i = 0; i < 4; i++) {
				int neighbourX = tile.x + dirX[i];
				int neighbourY = tile.y + dirY[i];
				if (IsInMapRange(neighbourX, neighbourY, size) && mapFlags[neighbourX][neighbourY] == 0 && map.GetValue(neighbourX, neighbourY) == (int)TileType::Island) {
					mapFlags[neighbourX][neighbourY] = 1;
					queue.push(Vector2Int(neighbourX, neighbourY));
				}
			}
		}

		return tiles;
	}

	std::vector<Region> GetRegions(Dynamic2DMapArray map) {
		std::vector<Region> regions;
		Vector2Int size = map.GetSize();
		int** mapFlags = new int* [size.x];
		for (int i = 0; i < size.x; i++) {
			mapFlags[i] = new int[size.y]();
		}

		int count = 0;
		for (int x = 0; x < size.x; x++) {
			for (int y = 0; y < size.y; y++) {
				if (mapFlags[x][y] == 0 && map.GetValue(x, y) == (int)TileType::Island) {
					std::vector<Vector2Int> newRegionTiles = GetRegionTiles(x, y, map, mapFlags);
					Region newRegion = Region(Vector2Int(x, y), Vector2Int(map.GetSize().x / 2, map.GetSize().y / 2), newRegionTiles);
					regions.push_back(newRegion);
					count++;
				}
			}
		}

		// Remember to delete the memory allocated for mapFlags
		for (int i = 0; i < size.x; i++) {
			delete[] mapFlags[i];
		}
		delete[] mapFlags;

		return regions;
	}

public:Dynamic2DMapArray ProccessMap(Dynamic2DMapArray map, int threshold, std::vector<Region> regions) {
	Dynamic2DMapArray newMap = map;
	for (int i = 0; i < regions.size(); i++) {
		Region region = regions[i];
		if (region.tiles.size() < threshold) {
			for (int j = 0; j < region.tiles.size(); j++) {
				Vector2Int tile = region.tiles[j];
				newMap.SetValue(tile.x, tile.y, (int)TileType::Water);
			}
		}
	}
	return newMap;
}
public:Dynamic2DMapArray SetSand(Dynamic2DMapArray map, Vector2Int size, std::vector<Region> regions) {

	Dynamic2DMapArray newMap = map;
	for (int i = 0; i < regions.size(); i++) {
		Region region = regions[i];
		for (int j = 0; j < region.tiles.size(); j++) {
			Vector2Int tile = region.tiles[j];
			if (GetNeighbourCount(tile.x, tile.y, map, size, TileType::Water) > 2) {
				newMap.SetValue(tile.x, tile.y, (int)TileType::Sand);
			}
		}
	}
	return newMap;
}
public:Chunk*** GenerateChunks(int gridSize, int chunkSize, int marginSize) {

	Chunk*** chunks = new Chunk * *[gridSize];
	for (int i = 0; i < gridSize; i++) {
		chunks[i] = new Chunk * [gridSize];
	}
	for (int chunkX = 0; chunkX < gridSize; chunkX++) {
		for (int chunkY = 0; chunkY < gridSize; chunkY++) {

			(chunks[chunkX][chunkY]) = new Chunk(CreateRandomChunk(1, Vector2Int(chunkSize, chunkSize), marginSize));

			for (int i = 0; i < 2; i++) {
				chunks[chunkX][chunkY]->map = SmoothMap(Vector2Int(chunkSize, chunkSize), chunks[chunkX][chunkY]->map);
			}
			chunks[chunkX][chunkY]->regions = GetRegions(chunks[chunkX][chunkY]->map);
			chunks[chunkX][chunkY]->map = ProccessMap(chunks[chunkX][chunkY]->map, 20, chunks[chunkX][chunkY]->regions);
			chunks[chunkX][chunkY]->regions = GetRegions(chunks[chunkX][chunkY]->map);
			chunks[chunkX][chunkY]->map = SetSand(chunks[chunkX][chunkY]->map, Vector2Int(chunkSize, chunkSize), chunks[chunkX][chunkY]->regions);
		}
	}
	return chunks;
}


};

