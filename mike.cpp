#define DEFAULT_WIDTH 800
#define DEFAULT_HEIGHT 450
#define DEFAULT_FPS 60
#include <raylib.h>
#include <vector>
#include "mike.hpp"

void InitGame();
void GameLoop(Level* level);

int main() {
	InitGame();
    Level level(1000, 500);
	GameLoop(&level);
    return 0;
}

void InitGame() {
	InitWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, "Mike! The Electrician");
	SetTargetFPS(DEFAULT_FPS);
}

void GameLoop(Level* level) {
    while (!WindowShouldClose()) {
        level->Update();
        BeginDrawing();
        level->Draw();
        EndDrawing();
    }
    CloseWindow();
}