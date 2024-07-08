#include <stdio.h>
#include <math.h>
#include "raylib.h"

#define WIN_WIDTH 1000
#define WIN_HEIGHT 700
#define MAP_SIZE 20
#define RAY_AMOUNT 20
#define FOV M_PI/4
#define MINIMAP_SIZE (WIN_WIDTH / 2)
#define CELL_SIZE (MINIMAP_SIZE / MAP_SIZE)

void move_player(float player_a, Vector2 *player_pos, int direction, int map[][MAP_SIZE]) {
    float moved_x = direction * cos(player_a) + player_pos->x;
    float moved_y = direction * sin(player_a) + player_pos->y;
    if (map[(int)(moved_y/MINIMAP_SIZE * MAP_SIZE)][(int)(moved_x/MINIMAP_SIZE * MAP_SIZE)] == 0) {
        player_pos->x = moved_x;
        player_pos->y = moved_y;
    }
}

int main() {
    int map[][MAP_SIZE] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0, 0, 3, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0, 2, 0, 0, 2, 0, 0, 1, 0, 0, 3, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 3, 0, 0, 3, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 0, 2, 0, 0, 2, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0, 3, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0},
        {0, 0, 0, 1, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 2, 0, 3, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0, 0, 2, 0, 3, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    };

    float player_a = M_PI * 1;
    Vector2 player_pos = {CELL_SIZE * 14.5f, CELL_SIZE * 12.5f};
    float ray_length = (sqrt(2)*MINIMAP_SIZE)/4;

    InitWindow(WIN_WIDTH, WIN_HEIGHT, "raycast");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();

            ClearBackground(LIGHTGRAY);

            DrawCircle(player_pos.x, player_pos.y, CELL_SIZE/4, RED);

            DrawRectangle(WIN_WIDTH/2, 0, WIN_WIDTH/2, WIN_HEIGHT/2, LIGHTGRAY);
            DrawRectangle(WIN_WIDTH/2, WIN_HEIGHT/2, WIN_WIDTH/2, WIN_HEIGHT/2, LIGHTGRAY);

            // Render minimap
            for (int y = 0; y < MAP_SIZE; y++) {
                for (int x = 0; x < MAP_SIZE; x++) {
                    DrawLine((x+1) * CELL_SIZE, 0, (x+1)*CELL_SIZE, MINIMAP_SIZE, BLACK); // vertical
                    DrawLine(0, (y+1) * CELL_SIZE, MINIMAP_SIZE, (y+1) * CELL_SIZE, BLACK); // horizontal

                    if (map[x][y] > 0) {
                        DrawRectangle(y*CELL_SIZE, x*CELL_SIZE, CELL_SIZE, CELL_SIZE, DARKGRAY);
                    }
                }
            }

            // Process keyboard input
            if (IsKeyDown(KEY_D) == true) {
                player_a += 0.025;
            } else if (IsKeyDown(KEY_A) == true) {
                player_a -= 0.025;
            } else if (IsKeyDown(KEY_W) == true) {
                move_player(player_a, &player_pos, 1, map);
            } else if (IsKeyDown(KEY_S) == true) {
                move_player(player_a, &player_pos, -1, map);
            }

            // Render rays
            for (float a = player_a - (FOV/2); a < player_a + (FOV/2); a += FOV/RAY_AMOUNT) {
                int x = player_pos.x / CELL_SIZE;
                int y = player_pos.y / CELL_SIZE;
                int x_step = 0;
                int y_step = 0;
                Vector2 start_pos2 = player_pos;
                Vector2 end_pos2 = {ray_length*cos(a) + player_pos.x, ray_length*sin(a) + player_pos.y};
                Vector2 coll_point = {};

                DrawLineV(start_pos2, end_pos2, RED);

                while ((x > 0 && x < MAP_SIZE) || (y > 0 && y < MAP_SIZE)) {
                    if (x > 0 && x < MAP_SIZE) { // Check vertical lines
                        if (fabs(cos(a)) < 0.001 || fabs(sin(a) - 1) < 0.001) {
                            x = MAP_SIZE;
                        } else {
                            if (cos(a) > 0) { // Viewing right
                                x_step = 1;
                            } else if (cos(a) < 0) { // Viewing left
                                x_step = -1;
                            }

                            Vector2 start_pos1 = {(x+1) * CELL_SIZE, 0};
                            Vector2 end_pos1 = {(x+1) * CELL_SIZE, MINIMAP_SIZE};

                            if (CheckCollisionLines(start_pos1, end_pos1, start_pos2, end_pos2, &coll_point)) {
                                DrawCircleV(coll_point, CELL_SIZE/6, RED);
                            }

                            x += x_step;
                        }
                    }
                    if (y > 0 && y < MAP_SIZE) { // Check horizontal lines
                        if (fabs(sin(a)) < 0.001 || fabs(cos(a) - 1) < 0.001) {
                            y = MAP_SIZE;
                        } else {
                            if (sin(a) > 0) {
                                y_step = 1; // Up
                            } else if (sin(a) < 0) {
                                y_step = -1; // Down
                            }

                            Vector2 start_pos1 = {0, (y+1) * CELL_SIZE};
                            Vector2 end_pos1 = {MINIMAP_SIZE, (y+1) * CELL_SIZE};

                            if (CheckCollisionLines(start_pos1, end_pos1, start_pos2, end_pos2, &coll_point)) {
                                DrawCircleV(coll_point, CELL_SIZE/6, RED);
                            }

                            y += y_step;
                        }
                    }
                }
            }

            // Render rays and walls
            // TODO: snap rays to grid instead of extending them a fixed distance
            /* float acc = 0.0f; */
            /* float column_height = 0.0f; */
            /* int curr_cell = 0; */
            /* Color wall_color = {}; */
            /* for (float a = player_a - (FOV/2); a < player_a + (FOV/2); a += FOV/RAY_AMOUNT) { */
            /*     float vx, vy; */
            /*     for (float c = 0.0f; c < sqrt(2)*WIN_HEIGHT; c+=0.1f) { */
            /*         vx = c * cos(a) + player_x; */
            /*         vy = c * sin(a) + player_y; */
            /*         curr_cell = map[(int)(vy/WIN_HEIGHT * MAP_SIZE)][(int)(vx/(WIN_WIDTH/2) * MAP_SIZE)]; */
            /*         // TraceLog(LOG_INFO, "x:%d, y:%d", (int)(vx/(WIN_WIDTH/2) * MAP_SIZE), (int)(vy/WIN_HEIGHT * MAP_SIZE)); */
            /*         if (curr_cell > 0) { */
            /*             // TraceLog(LOG_INFO, "factor: %f", 1/c); */
            /*             column_height = WIN_HEIGHT/(c*cos(fabs(a - player_a))) * 65; */
            /*             DrawLine(player_x, player_y, vx, vy, RED); */
            /*             if (curr_cell == 1) { */
            /*                 wall_color = DARKGRAY; */
            /*             } else if (curr_cell == 2) { */
            /*                 wall_color = MAROON; */
            /*             } else if (curr_cell == 3) { */
            /*                 wall_color = BEIGE; */
            /*             } */
            /*             DrawLineEx((Vector2){(WIN_WIDTH/2) + acc, */
            /*                                (WIN_HEIGHT - column_height)/2}, */
            /*                      (Vector2){(WIN_WIDTH/2) + acc, */
            /*                              column_height + (WIN_HEIGHT - column_height)/2}, */
            /*                      (WIN_WIDTH/2) / RAY_AMOUNT, */
            /*                      wall_color); */
            /*             break; */
            /*         } */
            /*     } */
            /*     acc += (WIN_WIDTH/2) / RAY_AMOUNT; */
            /* } */

            DrawFPS(WIN_WIDTH - 100, 10);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
