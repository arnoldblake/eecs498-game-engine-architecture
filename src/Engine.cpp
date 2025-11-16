#include "Engine.hpp"
#include <iostream>
#include <filesystem>
#include "utils.hpp"

Engine::Engine() {
    for (int i = 0; i < HARDCODED_MAP_HEIGHT; i++) {
        memset(hardcoded_map[i], ' ', HARDCODED_MAP_WIDTH);
        hardcoded_map[i][HARDCODED_MAP_WIDTH] = '\0'; // Null-terminate each row
    }
}

void Engine::GameLoop() {
    if (!std::filesystem::exists("resources")) {
        std::cout << "error: resources/ missing";
        std::exit(0);
    }
    // Load game configuration
    std::string path = std::filesystem::current_path().string() + "/resources/game.config";
    ReadJsonFile(path, game_config);
    // Load rendering configuration
    if (!std::filesystem::exists("resources/game.config")) {
        std::cout << "error: resources/game.config missing";
        std::exit(0);
    }
    path = std::filesystem::current_path().string() + "/resources/rendering.config";
    ReadJsonFile(path, rendering_config);

    if (strcmp(game_config["initial_scene"].GetString(), "") == 0) {
        std::cout << "error: initial_scene unspecified";
        std::exit(0);
    }

    // Load initial scene
    scene_db.loadScene(game_config["initial_scene"].GetString(), actors);
    player_actor = &actors.back();  // Gets the last actor (player) - assign to member variable

    std::cout << game_config["game_start_message"].GetString() << std::endl;

    while (running) {
        Render();
        Input();
        Update();
    }
}

void Engine::Render() {
    int half_height = rendering_config["y_resolution"].GetInt() / 2;
    int half_width = rendering_config["x_resolution"].GetInt() / 2;
    for (int y = player_actor->position.y - half_height; y <= player_actor->position.y + half_height; ++y) {
        for (int x = player_actor->position.x - half_width; x <= player_actor->position.x + half_width; ++x) {
            // Check bounds
            if (x < 0 || x >= HARDCODED_MAP_WIDTH || y < 0 || y >= HARDCODED_MAP_HEIGHT) {
                std::cout << ' ';
            } else {
                // Check for actors at this position and always print the player last if it is in this position
                bool actor_found = false;
                for (const auto& actor : actors) {
                    if (player_actor ->position.x == x && player_actor->position.y == y) {
                        std::cout << player_actor->view;
                        actor_found = true;
                        break;
                    }
                    else if (actor.position.x == x && actor.position.y == y) {
                        std::cout << actor.view;
                        actor_found = true;
                        break;
                    }
                }
                if (!actor_found) {
                    std::cout << hardcoded_map[y][x];
                }
            }
        }
        std::cout << std::endl;
    }

    // Check surrounding actors for dialogue
    for (const auto& actor : actors) {
        if (actor.actor_name != "player") {
            int dx = std::abs(actor.position.x - player_actor->position.x);
            int dy = std::abs(actor.position.y - player_actor->position.y);
            if (dx <= 1 && dy <= 1) {
                if (dx == 0 && dy == 0) {
                    std::cout << actor.contact_dialogue << std::endl;
                    find_dialogue_command(actor.contact_dialogue);
                }
                else {
                    std::cout << actor.nearby_dialogue << std::endl;
                    find_dialogue_command(actor.nearby_dialogue);
                }
            }
        }
    }

    std::cout << "health : " << health << ", score : " << score << std::endl;
    std::cout << "Please make a decision..." << std::endl;
    std::cout << "Your options are \"n\", \"e\", \"s\", \"w\", \"quit\"" << std::endl;
}

void Engine::Input() {
    std::cin >> Engine::last_input;
}

void Engine::Update() {
    // Update player actor based on last input
    glm::ivec2 new_position = player_actor->position;

    if (last_input == "quit") {
        running = false;
        std::cout << game_config["game_over_bad_message"].GetString() << std::endl;
    }
    else if (last_input == "n") {
        new_position = glm::ivec2(player_actor->position.x, player_actor->position.y - 1);
    }
    else if (last_input == "s") {
        new_position = glm::ivec2(player_actor->position.x, player_actor->position.y + 1);
    }
    else if (last_input == "e") {
        new_position = glm::ivec2(player_actor->position.x + 1, player_actor->position.y);
    }
    else if (last_input == "w") {
        new_position = glm::ivec2(player_actor->position.x - 1, player_actor->position.y);
    }

    // Check for collisions with walls
    if (!movement_blocked(new_position)) {
        player_actor->position = new_position;
    }

    // Update other actors
    for (auto& actor : actors) {
       if (actor.actor_name != "player") {
           glm::ivec2 new_position = actor.position + actor.velocity;
           if(!movement_blocked(new_position)) {
               actor.position = new_position;
           }
           else {
               actor.velocity = -actor.velocity;
           }
       }
    }

}

bool Engine::movement_blocked(glm::ivec2 position) {
    if (hardcoded_map[position.y][position.x] == 'b') {
        return true;
    }
    for (const auto& actor : actors) {
        if (actor.blocking && (actor.position.x == position.x && actor.position.y == position.y)) {
            return true;
        }
    }
    return false;
}

void Engine::find_dialogue_command(std::string str) {
    if (str.find("health down") != std::string::npos) {
        health -= 1;
        if (health <= 0) {
            running = false;
            std::cout << game_config["game_over_bad_message"].GetString() << std::endl;
        }
    }
    else if (str.find("score up") != std::string::npos) {
        score += 1;
    }
    else if (str.find("you win") != std::string::npos) {
        running = false;
        std::cout << game_config["game_over_good_message"].GetString() << std::endl;
    }
    else if (str.find("game over") != std::string::npos) {
        running = false;
        std::cout << game_config["game_over_bad_message"].GetString() << std::endl;
    }
}
