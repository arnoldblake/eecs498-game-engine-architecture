#include "TemplateDB.hpp"
#include <filesystem>
#include <iostream>
#include "utils.hpp"
#include "Engine.hpp"

TemplateDB::TemplateDB() {
    // Load all actor templates from resources/actor_templates/
    if (!std::filesystem::exists("resources/actor_templates")) {
            std::cout << "error: resources/actor_templates missing";
            std::exit(0);
        }
}

void TemplateDB::loadTemplates() {
    int id = 0;
    for (const auto& entry : std::filesystem::directory_iterator("resources/actor_templates")) {
        ReadJsonFile(entry.path().string(), actor_template);
        char view = actor_template["view"].GetString()[0];
        int x = actor_template["x"].GetInt();
        int y = actor_template["y"].GetInt();
        bool blocking = actor_template["blocking"].GetBool(); // Check if optional properties exist

        std::string nearby_dialogue = actor_template.HasMember("nearby_dialogue") ?
            actor_template["nearby_dialogue"].GetString() : "";
        std::string contact_dialogue = actor_template.HasMember("contact_dialogue") ?
            actor_template["contact_dialogue"].GetString() : "";
        std::string name = entry.path().stem().string(); // Use filename (without extension) as template name
        actor_templates[name] = Actor(id++, name, view, glm::ivec2(x, y), glm::ivec2(0, 0), blocking, nearby_dialogue, contact_dialogue);
    }
}
