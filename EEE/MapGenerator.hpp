#include "MapGeneratorHelpers.hpp"
#include "raylib.h"
#include <cstdlib>
#include <ctime>
#include <malloc.h>
#include <chrono>

#include <queue>
#include <iostream>


class MapGenerator : public MapGeneratorHelpers
{
	int GetRandomTile() {
		// Generates a random number between 0 and 99 (inclusive).
		int value = rand() % 100;

		// If the value is less than 60, return 1, else return 0.
		return (value <52) ? 1 : 0;
	}
	Dynamic2DMapArray GenerateRandomMap(int width, int height, int marginSize) {
		// Dynamically allocate memory for the 2D array
		Dynamic2DMapArray map = Dynamic2DMapArray(Vector2Int(width, height));
		Vector2Int size = map.GetSize();

		// Fill the map with random tiles
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {

				if (x >= marginSize && x < width - marginSize && y >= marginSize && y < height - marginSize && !IsBorder(x, y, size)) {

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
				int neighbourCount = MapGeneratorHelpers::GetNeighbourCount(x, y, map, size);

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
		TileType waterTiles[] = { TileType::Water,TileType::ShallowWater,TileType::MediumWater,TileType::DeepWater };

		Dynamic2DMapArray newMap = map;
		for (int i = 0; i < regions.size(); i++) {
			Region region = regions[i];
			for (int j = 0; j < region.tiles.size(); j++) {
				Vector2Int tile = region.tiles[j];
				for (TileType a : waterTiles) {
					if (GetNeighbourCount(tile.x, tile.y, map, size, a) > 0) {
						newMap.SetValue(tile.x, tile.y, (int)TileType::Sand);
					}
				}
				
			}
		}
		return newMap;
	}
		  //LEGACY
	public:Dynamic2DMapArray SetOceanDepth(Dynamic2DMapArray fullmap) {

		Vector2Int size = fullmap.GetSize();
		std::vector<Vector2Int> islandTiles;
		for (int x = 0; x < size.x; x++) {
			for (int y = 0; y < size.y; y++) {
				if (fullmap.GetValue(x, y) == (int)TileType::Island) {
					islandTiles.push_back(Vector2Int(x, y));
				}
			}
		}

		for (int x = 0; x < size.x; x++) {
			for (int y = 0; y < size.y; y++) {
				if (fullmap.GetValue(x, y) == (int)TileType::Water) {
					int minDistance = GetDistanceToNearestIsland(Vector2Int(x, y), islandTiles);

					if (minDistance < 4) {
						fullmap.SetValue(x, y, (int)TileType::ShallowWater);
					}
					else if (minDistance < 8) {
						fullmap.SetValue(x, y, (int)TileType::MediumWater);
					}
					else if (minDistance < 19) {
						fullmap.SetValue(x, y, (int)TileType::DeepWater);
					}
					else {
						fullmap.SetValue(x, y, (int)TileType::Water);
					}
				}
			}
		}
		return fullmap;
	}
	public:Dynamic2DMapArray FloodFillLayeredDepth(Dynamic2DMapArray map, int startX, int startY, int startDepth, int maxDepth, TileType targetType) {
		Dynamic2DMapArray newMap = map;
		struct TileInfo
		{
			Vector2Int pos;
			int depth;
		};
		std::queue<TileInfo> tilesToCheck;
		tilesToCheck.push(TileInfo{ Vector2Int(startX,startY),startDepth });

		int dirX[4] = { 0,0,1,-1 };
		int dirY[4] = { 1,-1,0,0 };
		while (!tilesToCheck.empty()) {
			TileInfo currentTile = tilesToCheck.front();
			tilesToCheck.pop();
			for (int i = 0; i < 4; i++) {
				int neighbourX = currentTile.pos.x + dirX[i];
				int neighbourY = currentTile.pos.y + dirY[i];
				if (IsInMapRange(neighbourX, neighbourY, map.GetSize())) {
					int newDepth = currentTile.depth + 1;
						if (newDepth < maxDepth && map.GetValue(neighbourX, neighbourY) == (int)TileType::Water) {
							newMap.SetValue(neighbourX, neighbourY, (int)targetType);
							tilesToCheck.push({ Vector2Int(neighbourX, neighbourY), newDepth });
						}
				}
			}
		}
		return newMap;
	}
			public:Dynamic2DMapArray SetOceanDepthInPhases(Dynamic2DMapArray map, std::vector<Region> regions) {
				Dynamic2DMapArray newMap = map;

				// Phase 1: Shallow Water
				for (Region& region : regions) {
					std::vector<Vector2Int> borderTiles = GetBorderTilesForRegion(region, map);
					for (Vector2Int& tile : borderTiles) {
						newMap = FloodFillLayeredDepth(map, tile.x, tile.y, 0, 4, TileType::ShallowWater);
					}
				}

				// Phase 2: Medium Water
				for (Region& region : regions) {
					std::vector<Vector2Int> borderTiles = GetBorderTilesForRegion(region, map);
					for (Vector2Int& tile : borderTiles) {
						std::vector<Vector2Int> extendedTiles = GetTilesWithinRadius(map, tile, 4);
						for (Vector2Int& extendedTile : extendedTiles) {
							newMap = FloodFillLayeredDepth(map, extendedTile.x, extendedTile.y, 4, 8, TileType::MediumWater);
						}
					}
				}

				// Phase 3: Deep Water
				for (Region& region : regions) {
					std::vector<Vector2Int> borderTiles = GetBorderTilesForRegion(region, map);
					for (Vector2Int& tile : borderTiles) {
						std::vector<Vector2Int> extendedTiles = GetTilesWithinRadius(map, tile, 8);
						for (Vector2Int& extendedTile : extendedTiles) {
							newMap = FloodFillLayeredDepth(map, extendedTile.x, extendedTile.y, 8, 14, TileType::DeepWater);
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
				chunks[chunkX][chunkY]->map = ProccessMap(chunks[chunkX][chunkY]->map, 75, chunks[chunkX][chunkY]->regions);
				chunks[chunkX][chunkY]->regions = GetRegions(chunks[chunkX][chunkY]->map);


			}

		}
		Dynamic2DMapArray fullmap = ConcatenateChunks(chunks, gridSize, chunkSize);
		std::vector<Region> regions = GetRegions(fullmap);
		fullmap = SetOceanDepthInPhases(fullmap, regions);
		//fullmap = SetSand(fullmap, Vector2Int(chunkSize * gridSize, chunkSize*gridSize), regions);
		chunks = DivideIntoChunks(fullmap, gridSize, chunkSize);

		return chunks;
	}


};

