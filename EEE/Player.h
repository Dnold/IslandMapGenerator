#pragma once
#include "TileDefinitions.hpp"
#include "raylib.h"
class Player
{
	public:
	Vector2Int pos;
	float speed;
	Vector2Int moveDir;
	Player();
	Player(Vector2Int _pos, float _speed);
	~Player();
	void ChangeMoveDir(float xRotationInput);
	void Move();
	void Update();
	void Draw();
};

