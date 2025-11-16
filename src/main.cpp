#include "Engine.hpp"
#include <windows.h>
#include <filesystem>

SceneDB scene_db;

int main() {
    // Set working directory to executable directory
    char exe_path[MAX_PATH];
    GetModuleFileNameA(NULL, exe_path, MAX_PATH);
    std::filesystem::path exe_dir = std::filesystem::path(exe_path).parent_path();
    std::filesystem::current_path(exe_dir);

    // Initialize game
    Engine engine;
    engine.GameLoop();

    return 0;
}
