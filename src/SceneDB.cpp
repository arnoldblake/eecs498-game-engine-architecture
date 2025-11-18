#include "SceneDB.hpp"
#include "Engine.hpp"
#include <filesystem>
#include <vector>
#include "glm/glm.hpp"
#include "utils.hpp"

void SceneDB::loadScene(const std::string& sceneName, std::vector<Actor>& actors) {
    // Implementation for loading a scene
    std::string path = std::filesystem::current_path().string() + "/resources/scenes/" + sceneName;
    if (!std::filesystem::exists(path)) {
        std::cout << "error: scene file " << sceneName << " missing";
        std::exit(0);
    }
    ReadJsonFile(path, scene);


    for (auto& actor_object : scene["actors"].GetArray()) {
        Actor new_actor;
        if (actor_object.HasMember("template")) {
            std::string template_name = actor_object["template"].GetString();
            if (template_db->actor_templates.contains(template_name)) {
                new_actor = template_db->actor_templates[template_name];
            } else {
                std::cout << "error: template " << template_name << " is missing";
                std::exit(0);
            }
        }
        new_actor.id = next_actor_id++;
        new_actor.actor_name = actor_object.HasMember("name") ?
            actor_object["name"].GetString() : new_actor.actor_name;
        new_actor.view = actor_object.HasMember("view") ?
            actor_object["view"].GetString()[0] : new_actor.view;
        new_actor.blocking = actor_object.HasMember("blocking") ?
            actor_object["blocking"].GetBool() : new_actor.blocking;

        new_actor.nearby_dialogue = actor_object.HasMember("nearby_dialogue") ?
            actor_object["nearby_dialogue"].GetString() : new_actor.nearby_dialogue;
        new_actor.contact_dialogue = actor_object.HasMember("contact_dialogue") ?
            actor_object["contact_dialogue"].GetString() : new_actor.contact_dialogue;

        new_actor.position = glm::ivec2(
            actor_object.HasMember("x") ? actor_object["x"].GetInt() : new_actor.position.x,
            actor_object.HasMember("y") ? actor_object["y"].GetInt() : new_actor.position.y
        );
        if (actor_object.HasMember("velocity")) {
            new_actor.velocity = glm::ivec2(
                actor_object["velocity"].HasMember("x") ? actor_object["velocity"]["x"].GetInt() : new_actor.velocity.x,
                actor_object["velocity"].HasMember("y") ? actor_object["velocity"]["y"].GetInt() : new_actor.velocity.y
            );
        } else {
            new_actor.velocity = glm::ivec2(0, 0);
        }

        actors.push_back(new_actor);
        }
}

void SceneDB::setTemplateDB(TemplateDB* template_db_ptr) {
    template_db = template_db_ptr;
}
