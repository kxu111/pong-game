#include "raylib.h"

#define RADIUS 18
#define MOVESPEED 2.6

// types
typedef enum GameScreen { GAMEPLAY = 0, ENDING } GameScreen;
typedef enum Direction { UP = 0, DOWN } Direction;

// globals
Rectangle ball, leftRacket, rightRacket;
int leftScore = 0, rightScore = 0, scoreWidth;

// blueprints
void InitializeElements(void);
void DrawElements(void);
void ServeBall(void);
void MoveBall(void);
void MoveRacket(Rectangle *racket, Direction direction);
void MoveEnemyCPU(Rectangle *racket);

void InitializeElements(void) {
    InitWindow(1280, 720, "pong");
    SetTargetFPS(60);

    int racketWidth = 30;
    int racketHeight = 150;
    int racketOffset = 30;
    float racketVertPos =
        (float)GetScreenHeight() / 2 - (float)racketHeight / 2;

    // initialize stuff
    ball = (Rectangle){(float)GetScreenWidth() / 2,
                       (float)GetScreenHeight() / 2, RADIUS};
    leftRacket =
        (Rectangle){racketOffset, racketVertPos, racketWidth, racketHeight};
    rightRacket = (Rectangle){GetScreenWidth() - racketOffset * 2,
                              racketVertPos, racketWidth, racketHeight};
    scoreWidth = MeasureText("00", 60);
}

void DrawElements(void) {
    DrawText(TextFormat("%02d", leftScore),
             (GetScreenWidth() / 2) - 50 - scoreWidth, 50, 60, GRAY);
    DrawText(TextFormat("%02d", rightScore), (GetScreenWidth() / 2) + 50, 50,
             60, GRAY);

    DrawCircle(ball.x, ball.y, RADIUS, WHITE);
    DrawLine((float)GetScreenWidth() / 2, 0, (float)GetScreenWidth() / 2,
             (float)GetScreenHeight(), WHITE);
    DrawRectangle(leftRacket.x, leftRacket.y, leftRacket.width,
                  leftRacket.height, WHITE);
    DrawRectangle(rightRacket.x, rightRacket.y, rightRacket.width,
                  rightRacket.height, WHITE);
}

void ServeBall(void) {
    ball.x = (float)GetScreenWidth() / 2;
    ball.y = (float)GetScreenHeight() / 2;
}

void MoveBall(void) {
    static int velocityX = RADIUS / 2;
    static int velocityY = RADIUS / 2;

    // check for racket collision & if ball is in front of racket
    if (CheckCollisionRecs(ball, leftRacket) && ball.x > leftRacket.x) {
        velocityX *= -1;
        ball.x += RADIUS;
    } else if (CheckCollisionRecs(ball, rightRacket) &&
               ball.x < rightRacket.x) {
        velocityX *= -1;
        ball.x -= RADIUS;
    }

    if (ball.y + RADIUS >= GetScreenHeight() || ball.y - RADIUS <= 0)
        velocityY *= -1;

    if (ball.x + RADIUS >= GetScreenWidth()) {
        // score points, flip the velocity, and reset the ball
        ++leftScore;
        ServeBall();
    } else if (ball.x - RADIUS <= 0) {
        ++rightScore;
        ServeBall();
    }

    ball.x += velocityX;
    ball.y += velocityY;
}

void MoveRacket(Rectangle *racket, Direction direction) {
    int step = (direction == UP) ? -RADIUS / MOVESPEED : RADIUS / MOVESPEED;

    if ((racket->y <= 0 && direction == UP) ||
        (racket->y >= GetScreenHeight() - racket->height && direction == DOWN))
        return;

    racket->y += step;
}

void MoveEnemyCPU(Rectangle *racket) {
    int step = RADIUS / MOVESPEED;
    if ((ball.y < racket->y) && (racket->y >= 0))
        racket->y -= step;
    else if ((ball.y > racket->y) &&
             (racket->y <= GetScreenHeight() - racket->height))
        racket->y += step;
}

int main(void) {
    GameScreen currentScreen = GAMEPLAY;
    InitializeElements();

    while (!WindowShouldClose()) {
        switch (currentScreen) {
        case GAMEPLAY:
            MoveBall();
            MoveEnemyCPU(&rightRacket);
            if (IsKeyDown(KEY_W))
                MoveRacket(&leftRacket, UP);
            else if (IsKeyDown(KEY_S))
                MoveRacket(&leftRacket, DOWN);
            break;
        case ENDING:
            break;
        }

        BeginDrawing();

        switch (currentScreen) {
        case GAMEPLAY:
            ClearBackground(BLACK);
            DrawElements();
            break;
        case ENDING:
            ClearBackground(BLACK);
            DrawText("The End! Press R to restart!", 190, 200, 20, WHITE);
            break;
        }

        EndDrawing();
    }
}
