#include <stdio.h>
#include <math.h>
#include "raylib.h"

#define WIN_WIDTH 1024
#define WIN_HEIGHT 512
#define MAP_SIZE 13
#define RAY_AMOUNT 128
#define FOV M_PI/4
#define CELL_SIZE (WIN_WIDTH / 2) / MAP_SIZE

int main() {
    int map[][MAP_SIZE] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 1},
        {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1},
        {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1},
        {1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1},
        {1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    };

    float player_a = M_PI * 0.65;
    float player_x = 331.5f;
    float player_y = 108.5f;

    InitWindow(WIN_WIDTH, WIN_HEIGHT, "raycast");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawCircle(player_x, player_y, CELL_SIZE/4, RED);

            // Render grid
            for (int y = 0; y < MAP_SIZE; y++) {
                for (int x = 0; x < MAP_SIZE; x++) {
                    DrawLine((x+1) * CELL_SIZE, 0, (x+1)*CELL_SIZE, WIN_HEIGHT, BLACK); // vertical
                    DrawLine(0, y * CELL_SIZE, WIN_WIDTH / 2, y * CELL_SIZE, BLACK); // horizontal

                    if (map[x][y] == 1) {
                        DrawRectangle(y*CELL_SIZE, x*CELL_SIZE, CELL_SIZE, CELL_SIZE, BLUE);
                    }
                }
            }

            // Process keyboard input
            float moved_x = 0.0f;
            float moved_y = 0.0f;
            if (IsKeyDown(KEY_D) == true) {
                player_a += 0.025;
            } else if (IsKeyDown(KEY_A) == true) {
                player_a -= 0.025;
            } else if (IsKeyDown(KEY_W) == true) {
                moved_x = 1.5f * cos(player_a) + player_x;
                moved_y = 1.5f * sin(player_a) + player_y;
                if (map[(int)(moved_y/WIN_HEIGHT * MAP_SIZE)][(int)(moved_x/(WIN_WIDTH/2) * MAP_SIZE)] != 1) {
                    player_x = moved_x;
                    player_y = moved_y;
                }
            } else if (IsKeyDown(KEY_S) == true) {
                moved_x = -1.5f * cos(player_a) + player_x;
                moved_y = -1.5f * sin(player_a) + player_y;
                if (map[(int)(moved_y/WIN_HEIGHT * MAP_SIZE)][(int)(moved_x/(WIN_WIDTH/2) * MAP_SIZE)] != 1) {
                    player_x = moved_x;
                    player_y = moved_y;
                }
            }

            // Render rays and walls
            float acc = 0.0f;
            float column_height = 0.0f;
            for (float a = player_a - (FOV/2); a < player_a + (FOV/2); a += FOV/RAY_AMOUNT) {
                float vx, vy;
                for (float c = 0.0f; c < sqrt(2)*WIN_HEIGHT; c+=0.1f) {
                    vx = c * cos(a) + player_x;
                    vy = c * sin(a) + player_y;
                    // TraceLog(LOG_INFO, "x:%d, y:%d", (int)(vx/(WIN_WIDTH/2) * MAP_SIZE), (int)(vy/WIN_HEIGHT * MAP_SIZE));
                    if (map[(int)(vy/WIN_HEIGHT * MAP_SIZE)][(int)(vx/(WIN_WIDTH/2) * MAP_SIZE)] == 1) {
                        // TraceLog(LOG_INFO, "factor: %f", 1/c);
                        column_height = WIN_HEIGHT/(c*cos(fabs(a - player_a))) * 65;
                        DrawLine(player_x, player_y, vx, vy, RED);
                        DrawLineEx((Vector2){(WIN_WIDTH/2) + acc,
                                           (WIN_HEIGHT - column_height)/2},
                                 (Vector2){(WIN_WIDTH/2) + acc,
                                         column_height + (WIN_HEIGHT - column_height)/2},
                                 (WIN_WIDTH/2) / RAY_AMOUNT,
                                 BLUE);
                        break;
                    }
                }
                acc += (WIN_WIDTH/2) / RAY_AMOUNT;
            }

        DrawFPS(WIN_WIDTH - 100, 10);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
