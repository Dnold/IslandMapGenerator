#include "TileDefinitions.hpp"
#include "MapGeneratorHelpers.hpp"
#include "raylib.h"
class MapRenderer
{
   // #011D43
	MapGeneratorHelpers mapGenerator;
	public:
	Vector2Int CalculateOffset(Vector2Int screenSize, Vector2Int totalGridSize) {

		return Vector2Int((screenSize.x - totalGridSize.x) / 2, (screenSize.y - totalGridSize.y) / 2);
	}
	public:Vector2Int GetTotalGridSize(int chunkSize, int gridSize, int tileSize) {
		int total = chunkSize * gridSize * tileSize;
		return Vector2Int(total, total);
	}
		  void DrawTile(Vector2 pos, int size, int value) {
			 
			  Color DEEP_WATER = { 4, 61, 143, 255 }; // #043D8F
			  Color MEDIUM_WATER = { 15, 157, 202, 255 }; // #0F9DCA
			  Color SHALLOW_WATER = { 151, 222, 223, 255 }; // #97DEDF
			  Color SAND = { 247, 237, 198, 255 };    // #F7EDC6
			  switch (value) {
			  case (int)TileType::Island:
				  DrawRectangle(pos.x, pos.y, size, size, DARKGREEN);
				  break;
			  case (int)TileType::Border:
				  DrawRectangle(pos.x, pos.y, size, size, RED);
				  break;
			  case (int)TileType::Water:
				  DrawRectangle(pos.x, pos.y, size, size, WATER);
				  break;
			  case (int)TileType::Sand:
				  DrawRectangle(pos.x, pos.y, size, size, SAND);
				  break;
			  case (int)TileType::DeepWater:
				  DrawRectangle(pos.x, pos.y, size, size, DEEP_WATER);
				  break;
			  case (int)TileType::MediumWater:
				  DrawRectangle(pos.x, pos.y, size, size, MEDIUM_WATER);
				  break;
			  case (int)TileType::ShallowWater:
				  DrawRectangle(pos.x, pos.y, size, size, SHALLOW_WATER);
				  break;
			  default:
				  DrawRectangle(pos.x, pos.y, size, size, BLACK);
				  break;
			  }

			  
		  }
		  void DrawChunk(Chunk* chunk, Vector2Int offset, int chunkSize, int tileSize) {
			  for (int x = 0; x < chunkSize; x++) {
				  for (int y = 0; y < chunkSize; y++) {
					  DrawTile({ offset.x + (float)x * tileSize,offset.y + (float)y * tileSize }, tileSize, chunk->map.GetValue(x, y));

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


