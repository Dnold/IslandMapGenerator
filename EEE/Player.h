#pragma once
#include <iostream>
#include "astar.hpp"
#include "TileDefinitions.hpp"
#include "raylib.h"
class Player
{
	public:
	Vector2Int pos;
	float speed;
	float baseSpeed = 2.0f; // Basisgeschwindigkeit des Spielers
	float maxWeight = 100.0f;
	Vector2Int moveDir;
	Rectangle rect;
	Player();
	Player(Vector2Int _pos, float _speed);
	~Player();
	void ChangeMoveDir(float xRotationInput);
	void Move();
	void CheckOutOfBounds();
	Vector2Int PredictMove();
	void UpdateWeight(float weight);
	void Update();
	void Draw();
    Vector2Int gridToPixel(Vector2Int gridPos) {
        return Vector2Int(gridPos.x * 15, gridPos.y * 15);
    }
    void FollowPath(std::vector<AStar::Vec2i>& path) {
        if (path.empty()) return;

        // Der nächste Schritt ist das erste Element im Pfad
        AStar::Vec2i nextStep = path.front();

        // Berechnen Sie die Distanz zum nächsten Schritt
        Vector2Int distanceToNextStep = { nextStep.x - pos.x, nextStep.y - pos.y };

        // Wenn die Distanz 0 ist, hat der Spieler den Schritt erreicht
        if (distanceToNextStep.x == 0 && distanceToNextStep.y == 0) {
            // Entfernen Sie den erreichten Schritt aus dem Pfad
            path.erase(path.begin());

            // Wenn der Pfad jetzt leer ist, sind wir am Ziel angekommen
            if (path.empty()) return;

            // Aktualisieren Sie nextStep, da wir den aktuellen Schritt bereits erreicht haben
            nextStep = path.front();
            distanceToNextStep = { nextStep.x - pos.x, nextStep.y - pos.y };
        }

        // Bewegen Sie den Spieler in Richtung des nächsten Schritts
        // Diese Logik hängt von Ihrer spezifischen Bewegungslogik ab
        // Hier ist ein einfaches Beispiel, das den Spieler direkt zum nächsten Schritt bewegt
        pos.x += (distanceToNextStep.x != 0) ? (distanceToNextStep.x / std::abs(distanceToNextStep.x)) : 0;
        pos.y += (distanceToNextStep.y != 0) ? (distanceToNextStep.y / std::abs(distanceToNextStep.y)) : 0;
    }


};

