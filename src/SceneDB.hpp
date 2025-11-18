#ifndef SCENEDB_HPP
#define SCENEDB_HPP
#include <string>
#include <vector>
#include "rapidjson/document.h"
#include "TemplateDB.hpp"

struct Actor;

class SceneDB {
public:
    void loadScene(const std::string&, std::vector<Actor>& actors);
    void setTemplateDB(TemplateDB* template_db);
private:
    rapidjson::Document scene;
    TemplateDB* template_db = nullptr;

    // static (class-level), inline (initialized)
    static inline int next_actor_id = 0;
};

#endif
