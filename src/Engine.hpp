#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <string>
#include <vector>

#include "SceneDB.hpp"
#include "TemplateDB.hpp"

#include "rapidjson/document.h"
#include "glm/glm.hpp"

struct Actor {
public:
    int id;
	std::string actor_name;
	char view;
	glm::ivec2 position;
	glm::ivec2 velocity;
	bool blocking;
	std::string nearby_dialogue;
	std::string contact_dialogue;

	Actor(int id, std::string actor_name, char view, glm::ivec2 position, glm::ivec2 initial_velocity,
		bool blocking, std::string nearby_dialogue, std::string contact_dialogue)
		: id(id), actor_name(actor_name), view(view), position(position), velocity(initial_velocity), blocking(blocking), nearby_dialogue(nearby_dialogue), contact_dialogue(contact_dialogue) {}

	Actor() {}
};

class Engine {
public:
    Engine();
    void GameLoop();
    void Input();
    void Update();
    void Render();
    SceneDB scene_db;
    TemplateDB template_db;
private:
    bool running = true;
    bool changing_scene = false;
    int next_scene = 0;
    std::string last_input = "";

    std::vector<Actor> actors;
    Actor* player_actor = nullptr;

    bool movement_blocked(glm::ivec2);
    void find_dialogue_command(std::string);

    rapidjson::Document game_config;
    rapidjson::Document rendering_config;

    int health = 3;
    int score = 0;

    static constexpr int HARDCODED_MAP_WIDTH = 50;
    static constexpr int HARDCODED_MAP_HEIGHT = 26;
    char hardcoded_map[HARDCODED_MAP_HEIGHT][HARDCODED_MAP_WIDTH+1];
};


#endif
