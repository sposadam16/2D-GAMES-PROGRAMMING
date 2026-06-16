#include "SnakeGame.h"

void SnakeGame::Draw()
{
    if (gameOver)
    {
        int txtSize = MeasureText("GAME OVER", 50);
        DrawText("GAME OVER!", screenW / 2 - txtSize / 2, screenH / 2, 50, RED);
        return;
    }

    for (int i = 0; i < screenW / TILE_SIZE + 1; i++)
    {
        int yTop = borderGap.y / 2;
        int yBottom = screenH - borderGap.y / 2;

        int x = borderGap.x / 2 + i * TILE_SIZE;

        Vector2 start = Vector2{ (float)x,(float)yTop };
        Vector2 end = Vector2{ (float)x,(float)yBottom };



        DrawLineV(start, end, BLACK);
    }

    for (int i = 0; i < screenH / TILE_SIZE + 1; i++)
    {
        int xLeft = borderGap.x / 2;
        int xRight = screenW - borderGap.x / 2;

        int y = borderGap.y / 2 + i * TILE_SIZE;

        Vector2 start = Vector2{ (float)xLeft, (float)y };
        Vector2 end = Vector2{ (float)xRight, (float)y };

        DrawLineV(start, end, BLACK);
    }

    for (Piece head : player)
    {
        head.Draw();
    }

    fruit.Draw();

}

void SnakeGame::Interaction()
{
    if (IsKeyPressed(KEY_P))
    {
        gamePause = !gamePause;
    }

    if (gamePause || gameOver)
    {
        return;
    }

    if (canMove)
    {
        if (IsKeyPressed(KEY_DOWN) && speed.y == 0)
        {

            speed = Vector2{ 0,TILE_SIZE };
            canMove = false;
        }
        if (IsKeyPressed(KEY_UP) && speed.y == 0)
        {
            speed = Vector2{ 0,-TILE_SIZE };
            canMove = false;
        }
        if (IsKeyPressed(KEY_LEFT) && speed.x == 0)
        {
            speed = Vector2{ -TILE_SIZE,0 };
            canMove = false;
        }
        if (IsKeyPressed(KEY_RIGHT) && speed.x == 0)
        {
            speed = Vector2{ TILE_SIZE,0 };
            canMove = false;
        }
    }

}