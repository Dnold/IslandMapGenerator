#include "TileDefinitions.h"
#include "MapGeneratorHelpers.h"
#include "raylib.h"
class MapRenderer
{
	MapGeneratorHelpers mapGenerator;
	public:Vector2Int CalculateOffset(Vector2Int screenSize, Vector2Int totalGridSize) {

		return Vector2Int((screenSize.x - totalGridSize.x) / 2, (screenSize.y - totalGridSize.y) / 2);
	}
	public:Vector2Int GetTotalGridSize(int chunkSize, int gridSize, int tileSize) {
		int total = chunkSize * gridSize * tileSize;
		return Vector2Int(total, total);
	}
		  void DrawTile(Vector2 pos, int size, int value) {
			  if (value == (int)TileType::Island) {
				  DrawRectangle(pos.x, pos.y, size, size, DARKGREEN);
			  }
			  else if (value == (int)TileType::Border) {
				  DrawRectangle(pos.x, pos.y, size, size, RED);
			  }
			  else if(value == (int)TileType::Water) {
				  DrawRectangle(pos.x, pos.y, size, size, BLUE);
			  }
			  else if (value == (int)TileType::Sand) {
				  DrawRectangle(pos.x, pos.y, size, size, YELLOW);
			  }
			  else {
				  DrawRectangle(pos.x, pos.y, size, size, BLACK);
			  }
		  }
		  void DrawChunk(Chunk* chunk, Vector2Int offset, int chunkSize, int tileSize) {
			  for (int x = 0; x < chunkSize; x++) {
				  for (int y = 0; y < chunkSize; y++) {
					  DrawTile({ offset.x + (float)x * tileSize,offset.y + (float)y * tileSize }, tileSize, chunk->map.GetValue(x,y));

				  }
			  }
			  //DrawBorder(chunk->map, Vector2Int(chunkSize,chunkSize), offset.x, offset.y, tileSize);
		  }

	public:void DrawChunkGrid(Chunk*** chunks, float offsetX, float offsetY, int chunkSize, int gridSize, int tileSize) {
		for (int chunkX = 0; chunkX < gridSize; chunkX++) {
			for (int chunkY = 0; chunkY < gridSize; chunkY++) {
				Vector2Int offset = { (int)offsetX + (chunkX * chunkSize * tileSize),
						  (int)offsetY + (chunkY * chunkSize * tileSize) };
				DrawChunk(chunks[chunkX][chunkY], offset, chunkSize, tileSize);

			}
		}
	}
		  void DrawBorder(int** map, Vector2Int size, float xOffset, float yOffset, int tileSize) {
			  for (int x = 0; x < size.x; x++) {
				  for (int y = 0; y < size.y; y++) {
					  if (mapGenerator.IsBorder(x, y, size)) {
						  DrawRectangle(xOffset + x * tileSize, yOffset + y * tileSize, tileSize, tileSize, RED);
					  }
				  }
			  }
		  }
};


