#include "Player.h"
Player::Player() {
	pos = Vector2Int(0, 0);
	speed = 1.0f;
	moveDir = Vector2Int(0, 1);
}
Player::Player(Vector2Int _pos, float _speed) {
	pos = _pos;
	speed = _speed;
	moveDir = Vector2Int(0, 1);
}
Player::~Player() {

}
void Player::ChangeMoveDir(float xRotationInput) {
	//if xRotationInput is 1, then the moveDir should be rotated 90 degrees clockwise
	//if xRotationInput is -1, then the moveDir should be rotated 90 degrees counter-clockwise
	//if xRotationInput is 0, then the moveDir should not change
	Vector2Int temp = moveDir;
	if (xRotationInput == 1) {
		moveDir.x = -temp.y;
		moveDir.y = temp.x;
	}
	else if (xRotationInput == -1) {
		moveDir.x = temp.y;
		moveDir.y = -temp.x;
	}
}
void Player::Move() {
	pos.x += moveDir.x * speed;
	pos.y += moveDir.y * speed;
}
void Player::Draw() {
	if(moveDir.x == 1 || moveDir.x == -1) DrawRectangle(pos.x-20, pos.y+10, 20, 10, BROWN);
	else {
		DrawRectangle(pos.x-10, pos.y+10, 10, 20, BROWN);
	}
	
}
void Player::Update() {
	Draw();
	Move();
	if(IsKeyPressed(KEY_RIGHT)) ChangeMoveDir(1);
	if(IsKeyPressed(KEY_LEFT)) ChangeMoveDir(-1);
}

