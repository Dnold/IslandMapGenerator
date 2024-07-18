#pragma once
#include "MapGenerator.hpp"
#include <vector>
#include <queue>
#include <unordered_map>
#include <cmath>
#include <limits>
#include <memory>

class Pathfinder
{
    public:
    struct Node
    {
        Vector2Int position;
        float gCost; // Cost from start node
        float hCost; // Heuristic cost to target node
        float fCost() const { return gCost + hCost; } // Total cost
        std::shared_ptr<Node> parent; // Parent node for path back to start

        Node(Vector2Int pos, std::shared_ptr<Node> p = nullptr, float g = 0.0f, float h = 0.0f) : position(pos), parent(p), gCost(g), hCost(h) {}
    };

    struct NodeCompare
    {
        bool operator()(const std::shared_ptr<Node> a, const std::shared_ptr<Node> b) const {
            return a->fCost() > b->fCost();
        }
    };

    static std::vector<Vector2Int> FindPath(Dynamic2DMapArray& map, Vector2Int start, Vector2Int end) {
        std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, NodeCompare> openSet;
        std::unordered_map<int, std::shared_ptr<Node>> allNodes;

        auto toIndex = [&map](Vector2Int pos) { return pos.y * map.GetSize().x + pos.x; };
        auto startNode = std::make_shared<Node>(start);
        startNode->hCost = Heuristic(start, end);
        openSet.push(startNode);
        allNodes[toIndex(start)] = startNode;

        while (!openSet.empty()) {
            auto currentNode = openSet.top();
            openSet.pop();

            if (currentNode->position == end) {
                return ReconstructPath(currentNode);
            }

            std::vector<Vector2Int> neighbors = GetNeighbors(currentNode->position, map);
            for (auto& neighborPos : neighbors) {
                if (!IsWalkable(neighborPos, map)) continue; // Check if tile is walkable (water)

                float tentativeGCost = currentNode->gCost + Distance(currentNode->position, neighborPos);
                auto neighborIndex = toIndex(neighborPos);

                if (allNodes.find(neighborIndex) == allNodes.end() || tentativeGCost < allNodes[neighborIndex]->gCost) {
                    auto neighborNode = std::make_shared<Node>(neighborPos, currentNode, tentativeGCost, Heuristic(neighborPos, end));
                    openSet.push(neighborNode);
                    allNodes[neighborIndex] = neighborNode;
                }
            }
        }

        // No path found
        return std::vector<Vector2Int>();
    }

    static bool IsWalkable(Vector2Int pos, Dynamic2DMapArray& map) {
        int tileValue = map.GetValue(pos.x, pos.y);
        return tileValue == (int)TileType::Water || tileValue == (int)TileType::MediumWater || tileValue == (int)TileType::DeepWater || tileValue == (int)TileType::ShallowWater;
    }


    private:
    static float Heuristic(Vector2Int a, Vector2Int b) {
        return std::abs(a.x - b.x) + std::abs(a.y - b.y); // Manhattan distance
    }

    static float Distance(Vector2Int a, Vector2Int b) {
        return std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2)); // Euclidean distance
    }

    static bool IsInMapRange(int x, int y, Vector2Int size) {
        return x >= 0 && x < size.x && y >= 0 && y < size.y;
    }

    static std::vector<Vector2Int> GetNeighbors(Vector2Int pos, Dynamic2DMapArray& map) {
        std::vector<Vector2Int> neighbors;
        std::vector<Vector2Int> directions = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };

        for (const auto& dir : directions) {
            Vector2Int neighborPos = Vector2Int(dir.x + pos.x, dir.y + pos.y);
            if (IsInMapRange(neighborPos.x, neighborPos.y, map.GetSize())) {
                neighbors.push_back(neighborPos);
            }
        }
        return neighbors;
    }

    static std::vector<Vector2Int> ReconstructPath(std::shared_ptr<Node> currentNode) {
        std::vector<Vector2Int> path;
        while (currentNode != nullptr) {
            path.push_back(currentNode->position);
            currentNode = currentNode->parent;
        }
        std::reverse(path.begin(), path.end());
        return path;
    }
};
