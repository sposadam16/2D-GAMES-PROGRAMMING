// SnakeGame.h
#pragma once
#include "raylib.h"
#include "raymath.h"
#include <cfloat>
#include <cmath>
#include <vector>

using namespace std;

class SnakeGame
{
public:

#define TILE_SIZE 73

    static const int screenW = 800;
    static const int screenH = 600;

    typedef struct Piece
    {
        Vector2 pos;
        Vector2 size = Vector2{ TILE_SIZE, TILE_SIZE };
        Color color;

        void Draw()
        {
            DrawRectangleV(pos, size, color);
        }

    } Piece;

    typedef struct Fruit
    {
        Vector2 pos;
        Color color = GOLD;
        Vector2 size = Vector2{ TILE_SIZE, TILE_SIZE };
        bool active = false;

        void NewRandomLocation(Vector2 borderGap, int screenW, int screenH)
        {
            int x = GetRandomValue(0, screenW / TILE_SIZE - 1) * TILE_SIZE + borderGap.x / 2;
            int y = GetRandomValue(0, screenH / TILE_SIZE - 1) * TILE_SIZE + borderGap.y / 2;

            pos = Vector2{ (float)x, (float)y };
        }

        void Draw()
        {
            DrawRectangleV(pos, size, color);
        }

    } Fruit;

    Vector2 speed = Vector2{ TILE_SIZE, 0 };

    vector<Piece> player;

    Fruit fruit = { 0 };
    Fruit specialFruit = { 0 };

    int score = 0;
    int normalFruitCount = 0;


    bool aiMode = false;
    float aiTimer = 0.0f;
    bool canMove = false;
    bool gamePause = false;
    bool gameOver = false;

    int frameCount = 0;

    Vector2 borderGap =
    {
        screenW % TILE_SIZE,
        screenH % TILE_SIZE
    };

    void Main();
    void Start();
    void Update();
    void Draw();
    void Interaction();
    void UpdateAI();
    
};