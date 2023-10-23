#include "TileDefinitions.h"
#include <list>
#include <stdexcept>
#include <vector>
struct Vector2Int
{
	int x;
	int y;
	Vector2Int(int _x, int _y) {
		x = _x;
		y = _y;
	}
	Vector2Int() : x(0), y(0) {}
};
class Dynamic2DMapArray
{
	int** map;
	Vector2Int size;
public:Dynamic2DMapArray(Vector2Int _size) : size(_size) {
	map = new int* [size.x];
	for (int i = 0; i < size.x; i++) {
		map[i] = new int[size.y];
	}
}

	  Vector2Int GetSize() const {
		  return size;
	  }
	  void SetValue(int x, int y, int value) {
		  if (x >= 0 && x < size.x && y >= 0 && y < size.y) {
			  map[x][y] = value;
		  }
	  }

	  int GetValue(int x, int y) const {
		  if (x >= 0 && x < size.x && y >= 0 && y < size.y) {
			  return map[x][y];
		  }
		  throw std::out_of_range("Indices out of range");
	  }
};
class Region
{
public:std::vector<Vector2Int> tiles;
public:Vector2Int center;
public:Vector2Int size;
public:Region(Vector2Int _center, Vector2Int _size, std::vector<Vector2Int> _tiles) {
	center = center;
	size = _size;
	tiles = _tiles;
}
};
class Chunk
{
	int id;
public:Vector2Int size;
public:Dynamic2DMapArray map;
public:Vector2Int pos;
public:std::vector<Region> regions;

public: Chunk(Vector2Int _size, int _id, Dynamic2DMapArray _map)
	: size(_size), id(_id), map(_size)
{
	map = _map;
}

};


enum class TileType { Water = 0, Island = 1, Sand = 2, Border = 3 };