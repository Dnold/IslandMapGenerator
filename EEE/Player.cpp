#include "Player.h"

#include <filesystem>
#include <iostream>
#include <raymath.h>

Player::Player() {
    pos = Vector2Int(100, 100);
    speed = 1.0f;
    moveDir = Vector2Int(0, 1);
    rect = { (float)pos.x,(float)pos.y,8,8 };
}
Player::Player(Vector2Int _pos, float _speed) {
    pos = _pos;
    speed = _speed;
    moveDir = Vector2Int(0, 1);
    rect = { (float)_pos.x,(float)_pos.y,8,8 };
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
    std::cout << "MoveDir: " << moveDir.x << " " << moveDir.y << std::endl;
}
Vector2Int Player::PredictMove() {
    Vector2Int playerPos = pos;
    playerPos.x += static_cast<int>(moveDir.x * abs(speed));
    playerPos.y += static_cast<int>(moveDir.y * abs(speed));
    return playerPos;
}
void Player::Move() {
    pos.x += static_cast<int>(moveDir.x * speed);
    pos.y += static_cast<int>(moveDir.y * speed);
}
void Player::Draw() {

    if (moveDir.x == 1 || moveDir.x == -1) {
        rect = { (float)pos.x - 8, (float)pos.y + 8, 16, 8 };
    }
    else {
        rect = { (float)pos.x - 8, (float)pos.y + 8, 8, 16 };
    }
    DrawRectangleRec(rect, BROWN);
}
void Player::UpdateWeight(float weight)
{

    // Berechnen Sie das Verhältnis des aktuellen Gewichts zum maximalen Gewicht
    float weightRatio = weight / maxWeight;


    // Stellen Sie sicher, dass weightRatio zwischen 0 und 1 liegt
    weightRatio = weightRatio > 1.0f ? 1.0f : weightRatio;

    // Berechnen Sie die neue Geschwindigkeit als lineare Skalierung der Basisgeschwindigkeit
    speed = baseSpeed * (1.0f - (2.0f * weightRatio));
    if (speed < 1) speed = 1.0f;
    std::cout << "Speed: " << speed << std::endl;
}



void Player::Update() {
    if (IsKeyPressed(KEY_RIGHT)) ChangeMoveDir(1);
    if (IsKeyPressed(KEY_LEFT)) ChangeMoveDir(-1);


}
void Player::CheckOutOfBounds()
{
    // Check if player is out of bounds
    if (rect.x < 0) {
        rect.x = rect.width;
        pos.x = static_cast<int>(rect.width);
    }
    else if (rect.x > GetScreenWidth()) {
        rect.x = GetScreenWidth() - rect.width;
        pos.x = static_cast<int>(GetScreenWidth() - rect.width);
    }
    if (rect.y < 0) {
        rect.y = rect.height;
        pos.y = static_cast<int>(rect.height);
    }
    else if (rect.y > GetScreenHeight() + rect.height) {
        rect.y = GetScreenHeight() - rect.height;
        pos.y = static_cast<int>(GetScreenHeight() - rect.height);
    }
}
