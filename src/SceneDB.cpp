#include "SceneDB.hpp"
#include "Engine.hpp"
#include <filesystem>
#include <vector>
#include "glm/glm.hpp"
#include "utils.hpp"

void SceneDB::loadScene(const std::string& sceneName, std::vector<Actor>& actors) {
    // Implementation for loading a scene
    std::string path = std::filesystem::current_path().string() + "/resources/scenes/" + sceneName;
    ReadJsonFile(path, scene);

    int id = 0;
    for (auto& actor_object : scene["actors"].GetArray()) {
        std::string name = actor_object.HasMember("name") ?
            actor_object["name"].GetString() : "";
        char view = actor_object["view"].GetString()[0];
        int x = actor_object["x"].GetInt();
        int y = actor_object["y"].GetInt();
        bool blocking = actor_object["blocking"].GetBool(); // Check if optional properties exist

        std::string nearby_dialogue = actor_object.HasMember("nearby_dialogue") ?
            actor_object["nearby_dialogue"].GetString() : "";
        std::string contact_dialogue = actor_object.HasMember("contact_dialogue") ?
            actor_object["contact_dialogue"].GetString() : "";

               actors.push_back(Actor(id++, name, view, glm::ivec2(x, y), glm::ivec2(0, 0), blocking, nearby_dialogue, contact_dialogue));

    }
}
