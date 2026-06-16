// SnakeGame.cpp
#include "SnakeGame.h"

void SnakeGame::Start()
{
    Piece head1;

    head1.color = GREEN;
    head1.pos = Vector2{ borderGap.x / 2, borderGap.y / 2 };

    player.push_back(head1);

    Main();
}

void SnakeGame::Main()
{
    InitWindow(screenW, screenH, "Snake Game");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        Update();

        BeginDrawing();

        ClearBackground(DARKGRAY);

        Draw();

        EndDrawing();
    }

    CloseWindow();
}

void SnakeGame::Update()
{
    frameCount++;


    if (!aiMode)
    {
        Interaction();
    }
    else
    {
        UpdateAI();
    }

    if (frameCount % 20 == 0)//3movs x sec
    {
        for (int i = player.size() - 1; i > 0; i--)
        {
            player[i].pos = player[i - 1].pos;
        }

        player[0].pos += speed;

        canMove = true;
		for (int i = 1; i < player.size(); i++)//body collision
        {
            if (Vector2Equals(player[0].pos, player[i].pos))
            {
                gameOver = true;
            }
        }
    }

	if (player[0].pos.x > screenW - borderGap.x / 2 ||//wall collision
        player[0].pos.x < borderGap.x / 2 ||
        player[0].pos.y > screenH - borderGap.y / 2 ||
        player[0].pos.y < borderGap.y / 2)
    {
        gameOver = true;
    }

    if (!fruit.active)
    {
        fruit.active = true;

        fruit.NewRandomLocation(borderGap, screenW, screenH);
    }

	if (Vector2Equals(player[0].pos, fruit.pos))//fruit collision
    {
        fruit.active = false;

        Piece newPiece;
        newPiece.color = DARKGREEN;
        newPiece.pos = player.back().pos;

        player.push_back(newPiece);

        score++;
        normalFruitCount++;

		// Every 10 normal fruits, spawn a special fruit
        if (normalFruitCount % 10 == 0)
        {
            specialFruit.active = true;
            specialFruit.color = PURPLE;

            specialFruit.NewRandomLocation(borderGap, screenW, screenH);
        }
    }
    if (specialFruit.active &&
        Vector2Equals(player[0].pos, specialFruit.pos))
    {
        specialFruit.active = false;

        aiMode = true;
        aiTimer = 10.0f;
    }
    if (aiMode)
    {
        aiTimer -= GetFrameTime();

        if (aiTimer <= 0)
        {
            aiMode = false;
        }
    }
}

void SnakeGame::Draw()
{
    if (gameOver)
    {
        int txtSize = MeasureText("GAME OVER!", 50);

        DrawText("GAME OVER!",
            screenW / 2 - txtSize / 2,
            screenH / 2,
            50,
            RED
        );

        DrawText("PRESS R TO RESTART",
            screenW / 2 - 120,
            screenH / 2 + 60,
            20,
            WHITE);
    }

    // Vertical lines
    for (int i = 0; i < screenW / TILE_SIZE + 1; i++)
    {
        int yTop = borderGap.y / 2;
        int yBottom = screenH - borderGap.y / 2;

        int x = borderGap.x / 2 + i * TILE_SIZE;

        Vector2 start = Vector2{ (float)x, (float)yTop };
        Vector2 end = Vector2{ (float)x, (float)yBottom };

        DrawLineV(start, end, BLACK);
    }

    // Horizontal lines
    for (int i = 0; i < screenH / TILE_SIZE + 1; i++)
    {
        int xLeft = borderGap.x / 2;
        int xRight = screenW - borderGap.x / 2;

        int y = borderGap.y / 2 + i * TILE_SIZE;

        Vector2 start = Vector2{ (float)xLeft, (float)y };
        Vector2 end = Vector2{ (float)xRight, (float)y };

        DrawLineV(start, end, BLACK);
    }

    // Draw player
    for (Piece head : player)
    {
        head.Draw();
    }

    // Draw fruit
    fruit.Draw();
    if (specialFruit.active)
    {
        specialFruit.Draw();
    }
    DrawText(
        TextFormat("Score: %i", score),
        10,
        10,
        30,
        WHITE
    );

    DrawText(
        "R: Restart",
        10,
        45,
        20,
        WHITE
    );
    if (aiMode)
    {
        DrawText(
            TextFormat("AI MODE: %.0f", ceil(aiTimer)),
            10,
            70,
            20,
            PURPLE
        );
    }
}

void SnakeGame::Interaction()
{
    if (IsKeyPressed(KEY_P))
    {
        gamePause = !gamePause;
    }
    if (gameOver && IsKeyPressed(KEY_R))
    {
        player.clear();

        Piece head;

        head.color = GREEN;
        head.pos = Vector2{ borderGap.x / 2, borderGap.y / 2 };

        player.push_back(head);

        speed = Vector2{ TILE_SIZE, 0 };

        frameCount = 0;

        canMove = false;
        gamePause = false;
        gameOver = false;

        score = 0;
        normalFruitCount = 0;

        fruit.active = false;
        specialFruit.active = false;

        aiMode = false;
        aiTimer = 0.0f;
    }
    if (gamePause || gameOver)
    {
        return;
    }

    if (canMove)
    {
        if (IsKeyPressed(KEY_DOWN) && speed.y == 0)
        {
            speed = Vector2{ 0, TILE_SIZE };
            canMove = false;
        }

        if (IsKeyPressed(KEY_UP) && speed.y == 0)
        {
            speed = Vector2{ 0, -TILE_SIZE };
            canMove = false;
        }

        if (IsKeyPressed(KEY_LEFT) && speed.x == 0)
        {
            speed = Vector2{ -TILE_SIZE, 0 };
            canMove = false;
        }

        if (IsKeyPressed(KEY_RIGHT) && speed.x == 0)
        {
            speed = Vector2{ TILE_SIZE, 0 };
            canMove = false;
        }
    }
}
void SnakeGame::UpdateAI()
{
    Vector2 directions[4] =
    {
        { TILE_SIZE, 0 },
        { -TILE_SIZE, 0 },
        { 0, TILE_SIZE },
        { 0, -TILE_SIZE }
    };

    float bestDistance = FLT_MAX;
    Vector2 bestMove = speed;
    //Check neighbours
    for (int i = 0; i < 4; i++)
    {
        Vector2 nextPos = player[0].pos + directions[i];

        bool valid = true;

		// Walls
        if (nextPos.x > screenW - borderGap.x / 2 ||
            nextPos.x < borderGap.x / 2 ||
            nextPos.y > screenH - borderGap.y / 2 ||
            nextPos.y < borderGap.y / 2)
        {
            valid = false;
        }

        // Body
        for (int j = 1; j < player.size(); j++)
        {
            if (Vector2Equals(nextPos, player[j].pos))
            {
                valid = false;
                break;
            }
        }

        if (!valid)
            continue;

        float dist =
            fabs(nextPos.x - fruit.pos.x) +
            fabs(nextPos.y - fruit.pos.y);

        if (dist < bestDistance)
        {
            bestDistance = dist;
            bestMove = directions[i];
        }
    }

    speed = bestMove;
}