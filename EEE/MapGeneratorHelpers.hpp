#pragma once
#include "TileDefinitions.hpp"
#include <cstdlib>


class MapGeneratorHelpers
{
	public:
	bool IsBorder(int x, int y, Vector2Int size) {
		return x == 0 || x == size.x - 1 || y == 0 || y == size.y - 1;
	}

	bool IsInMapRange(int x, int y, const Vector2Int& size) {
		return x >= 0 && x < size.x && y >= 0 && y < size.y;
	}
	std::vector<Vector2Int> GetTilesWithinRadius(Dynamic2DMapArray map, Vector2Int center, int radius) {
		std::vector<Vector2Int> result;
		for (int x = -radius; x <= radius; x++) {
			for (int y = -radius; y <= radius; y++) {
				if (x * x + y * y <= radius * radius) {  // Check if within circular radius
					Vector2Int newPoint(center.x + x, center.y + y);
					if (IsInMapRange(newPoint.x, newPoint.y, map.GetSize())) {
						result.push_back(newPoint);
					}
				}
			}
		}
		return result;
	}
	int GetNeighbourCount(int gridX, int gridY, Dynamic2DMapArray map, const Vector2Int& size) {
		int count = 0;
		// Directions representing the 8 neighboring cells
		int dirX[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
		int dirY[8] = { 1, 1, 1, 0, 0, -1, -1, -1 };

		for (int i = 0; i < 8; i++) {
			int neighbourX = gridX + dirX[i];
			int neighbourY = gridY + dirY[i];

			// If the neighbor is within map boundaries, check its status
			if (IsInMapRange(neighbourX, neighbourY, size)) {
				if (map.GetValue(neighbourX, neighbourY) == 1) {
					count++;
				}
			}
			else {
				// Out-of-boundary neighbors are considered alive to create a solid boundary.
				count++;
			}
		}
		return count;
	}
	int GetNeighbourCount(int gridX, int gridY, Dynamic2DMapArray map, const Vector2Int& size, TileType targetTileType) {
		int count = 0;
		// Directions representing the 8 neighboring cells
		int dirX[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
		int dirY[8] = { 1, 1, 1, 0, 0, -1, -1, -1 };

		for (int i = 0; i < 8; i++) {
			int neighbourX = gridX + dirX[i];
			int neighbourY = gridY + dirY[i];

			// If the neighbor is within map boundaries, check its status
			if (IsInMapRange(neighbourX, neighbourY, size)) {
				if (map.GetValue(neighbourX, neighbourY) == (int)targetTileType) {
					count++;
				}
			}

		}
		return count;
	}

	Dynamic2DMapArray ConcatenateChunks(Chunk*** chunks, int gridSize, int chunkSize) {
		int fullMapSize = gridSize * chunkSize;
		Dynamic2DMapArray fullmap = Dynamic2DMapArray(Vector2Int(fullMapSize, fullMapSize));

		for (int cx = 0; cx < gridSize; cx++) {
			for (int cy = 0; cy < gridSize; cy++) {
				for (int x = 0; x < chunkSize; x++) {
					for (int y = 0; y < chunkSize; y++) {
						Vector2Int tile = Vector2Int(cx * chunkSize + x, cy * chunkSize + y);
						fullmap.SetValue(tile.x, tile.y, chunks[cx][cy]->map.GetValue(x, y));
					}
				}
			}
		}
		return fullmap;
	}
	TileType GetWaterDepthByPhase(int phase) {
		switch (phase) {

		case 0:
			return TileType::ShallowWater;
		case 1:
			return TileType::ShallowWater;
		case 2:
			return TileType::MediumWater;
		case 3:
			return TileType::MediumWater;
		case 4:
			return TileType::DeepWater;
		case 5:
			return TileType::DeepWater;
		case 6:
			return TileType::Water;
		case 7:
			return TileType::Water;
		case 8:
			return TileType::Water;
		case 9:
			return TileType::Water;
		case 10:
			return TileType::Water;
		case 11:
			return TileType::Water;
		case 12:
			return TileType::Water;
		case 13:
			return TileType::Water;
		case 14:
			return TileType::Water;
		default:
			return TileType::Water;
		}
	}
	Vector2Int GetRangeDepths(int phase) {
		switch (phase) {

		case 0:
			return Vector2Int(0, 2);
		case 1:
			return Vector2Int(1, 3);
		case 2:
			return Vector2Int(4, 6);
		case 3:
			return Vector2Int(5, 7);
		case 4:
			return Vector2Int(8, 12);
		case 5:
			return Vector2Int(13, 16);
		case 6:
			return Vector2Int(17, 19);
	
		
		default:
			return Vector2Int(20, 100);
		}
	}
	Chunk*** DivideIntoChunks(Dynamic2DMapArray fullmap, int gridSize, int chunkSize) {


		Chunk*** chunks = new Chunk * *[gridSize];
		for (int i = 0; i < gridSize; i++) {
			chunks[i] = new Chunk * [gridSize];
		}
		for (int cx = 0; cx < gridSize; cx++) {
			for (int cy = 0; cy < gridSize; cy++) {
				Vector2Int chunkCenter = Vector2Int(cx * chunkSize + chunkSize / 2, cy * chunkSize + chunkSize / 2);
				Dynamic2DMapArray chunkMap = Dynamic2DMapArray(Vector2Int(chunkSize, chunkSize));
				for (int x = 0; x < chunkSize; x++) {
					for (int y = 0; y < chunkSize; y++) {

						int newX = cx * chunkSize + x;
						int newY = cy * chunkSize + y;
						if (newX >= 0 && newX < fullmap.GetSize().x && newY >= 0 && newY < fullmap.GetSize().y) {
							chunkMap.SetValue(x, y, fullmap.GetValue(newX, newY));
						}
						else {

						}
					}
				}
				chunks[cx][cy] = new Chunk(Vector2Int(chunkSize, chunkSize), 1, chunkMap);
			}
		}
		return chunks;
	}

	//LEGACY
	int GetDistanceToNearestIsland(Vector2Int pos, std::vector<Vector2Int> islandTilePos) {
		int minDistance = INT32_MAX;
		for (int i = 0; i < islandTilePos.size(); i++) {
			Vector2Int tile = islandTilePos[i];
			int distance = abs(tile.x - pos.x) + abs(tile.y - pos.y);
			if (distance < minDistance) {
				minDistance = distance;
			}

		}
		return minDistance;
	}

	std::vector<Vector2Int> GetBorderTilesForRegion(Region region, Dynamic2DMapArray map) {
		std::vector<Vector2Int> borderTiles;
		TileType waterTiles[] = { TileType::Water,TileType::ShallowWater,TileType::MediumWater,TileType::DeepWater,TileType::Sand };


		for (int i = 0; i < region.tiles.size(); i++) {
			Vector2Int tile = region.tiles[i];

			for (TileType a : waterTiles) {
				if (GetNeighbourCount(tile.x, tile.y, map, map.GetSize(), a) > 0) {
					borderTiles.push_back(tile);
					break;
				}
			}

		}

		return borderTiles;
	}

};


