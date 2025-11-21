#include "Engine.hpp"

#ifdef _WIN32
#include <windows.h>
#endif

#include <filesystem>
#include "SDL3/SDL.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600


SceneDB scene_db;

int main() {
    #ifdef _WIN32
    // Set working directory to executable directory
    char exe_path[MAX_PATH];
    GetModuleFileNameA(NULL, exe_path, MAX_PATH);
    std::filesystem::path exe_dir = std::filesystem::path(exe_path).parent_path();
    std::filesystem::current_path(exe_dir);
    #endif

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_CreateWindowAndRenderer("Hello SDL", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_FRect rect = {
        .w = 100,
        .h = 100
    };
    rect.x = (WINDOW_WIDTH - rect.w)/2.0f;
    rect.y = (WINDOW_HEIGHT - rect.h)/2.0f;
    SDL_RenderFillRect(renderer, &rect);
    SDL_RenderPresent(renderer);

    // Initialize game
    Engine engine;
    engine.GameLoop();

    return 0;
}
