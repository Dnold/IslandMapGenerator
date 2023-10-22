#pragma once
#include "TileDefinitions.cpp"
class MapGeneratorHelpers
{
	public:bool IsBorder(int x, int y, Vector2Int size) {
		return x == 0 || x == size.x - 1 || y == 0 || y == size.y - 1;
	}

		  bool IsInMapRange(int x, int y, const Vector2Int& size) {
			  return x >= 0 && x < size.x && y >= 0 && y < size.y;
		  }
	protected:int GetNeighbourCount(int gridX, int gridY, Dynamic2DMapArray map, const Vector2Int& size) {
		int count = 0;
		// Directions representing the 8 neighboring cells
		int dirX[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
		int dirY[8] = { 1, 1, 1, 0, 0, -1, -1, -1 };

		for (int i = 0; i < 8; i++) {
			int neighbourX = gridX + dirX[i];
			int neighbourY = gridY + dirY[i];

			// If the neighbor is within map boundaries, check its status
			if (IsInMapRange(neighbourX, neighbourY, size)) {
				count += map.GetValue(neighbourX,neighbourY);
			}
			else {
				// Out-of-boundary neighbors are considered alive to create a solid boundary.
				count++;
			}
		}
		return count;
	}
};


