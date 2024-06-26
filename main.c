#include <stdio.h>
#include <math.h>
#include "raylib.h"

#define WIN_WIDTH 1024
#define WIN_HEIGHT 512
#define MAP_SIZE 20
#define RAY_AMOUNT 128
#define FOV M_PI/4
#define CELL_SIZE (WIN_WIDTH / 2) / MAP_SIZE

int main() {
    int map[][MAP_SIZE] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
        {1, 0, 0, 1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 1, 0, 0, 0, 0, 3, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 1, 0, 0, 0, 2, 0, 0, 2, 0, 0, 1, 0, 0, 3, 0, 0, 1},
        {1, 0, 0, 1, 0, 0, 3, 0, 0, 3, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 1, 0, 2, 0, 0, 2, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 1, 0, 0, 0, 3, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1},
        {1, 0, 0, 1, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 2, 0, 3, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 0, 0, 0, 2, 0, 3, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    };

    float player_a = M_PI * 0.65;
    float player_x = 50.0f;
    float player_y = 35.0f;

    InitWindow(WIN_WIDTH, WIN_HEIGHT, "raycast");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();

            ClearBackground(LIGHTGRAY);

            DrawCircle(player_x, player_y, CELL_SIZE/4, RED);

            DrawRectangle(WIN_WIDTH/2, 0, WIN_WIDTH/2, WIN_HEIGHT/2, LIGHTGRAY);
            DrawRectangle(WIN_WIDTH/2, WIN_HEIGHT/2, WIN_WIDTH/2, WIN_HEIGHT/2, LIGHTGRAY);

            // Render grid
            for (int y = 0; y < MAP_SIZE; y++) {
                for (int x = 0; x < MAP_SIZE; x++) {
                    DrawLine((x+1) * CELL_SIZE, 0, (x+1)*CELL_SIZE, WIN_HEIGHT, BLACK); // vertical
                    DrawLine(0, y * CELL_SIZE, WIN_WIDTH / 2, y * CELL_SIZE, BLACK); // horizontal

                    if (map[x][y] > 0) {
                        DrawRectangle(y*CELL_SIZE, x*CELL_SIZE, CELL_SIZE, CELL_SIZE, DARKGRAY);
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
                if (map[(int)(moved_y/WIN_HEIGHT * MAP_SIZE)][(int)(moved_x/(WIN_WIDTH/2) * MAP_SIZE)] == 0) {
                    player_x = moved_x;
                    player_y = moved_y;
                }
            } else if (IsKeyDown(KEY_S) == true) {
                moved_x = -1.5f * cos(player_a) + player_x;
                moved_y = -1.5f * sin(player_a) + player_y;
                if (map[(int)(moved_y/WIN_HEIGHT * MAP_SIZE)][(int)(moved_x/(WIN_WIDTH/2) * MAP_SIZE)] == 0) {
                    player_x = moved_x;
                    player_y = moved_y;
                }
            }

            // Render rays and walls
            // TODO: snap rays to grid instead of extending them a fixed distance
            float acc = 0.0f;
            float column_height = 0.0f;
            int curr_cell = 0;
            Color wall_color = {};
            for (float a = player_a - (FOV/2); a < player_a + (FOV/2); a += FOV/RAY_AMOUNT) {
                float vx, vy;
                for (float c = 0.0f; c < sqrt(2)*WIN_HEIGHT; c+=0.1f) {
                    vx = c * cos(a) + player_x;
                    vy = c * sin(a) + player_y;
                    curr_cell = map[(int)(vy/WIN_HEIGHT * MAP_SIZE)][(int)(vx/(WIN_WIDTH/2) * MAP_SIZE)];
                    // TraceLog(LOG_INFO, "x:%d, y:%d", (int)(vx/(WIN_WIDTH/2) * MAP_SIZE), (int)(vy/WIN_HEIGHT * MAP_SIZE));
                    if (curr_cell > 0) {
                        // TraceLog(LOG_INFO, "factor: %f", 1/c);
                        column_height = WIN_HEIGHT/(c*cos(fabs(a - player_a))) * 65;
                        DrawLine(player_x, player_y, vx, vy, RED);
                        if (curr_cell == 1) {
                            wall_color = DARKGRAY;
                        } else if (curr_cell == 2) {
                            wall_color = MAROON;
                        } else if (curr_cell == 3) {
                            wall_color = BEIGE;
                        }
                        DrawLineEx((Vector2){(WIN_WIDTH/2) + acc,
                                           (WIN_HEIGHT - column_height)/2},
                                 (Vector2){(WIN_WIDTH/2) + acc,
                                         column_height + (WIN_HEIGHT - column_height)/2},
                                 (WIN_WIDTH/2) / RAY_AMOUNT,
                                 wall_color);
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
