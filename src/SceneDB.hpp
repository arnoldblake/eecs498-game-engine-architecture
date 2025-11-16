#ifndef SCENEDB_HPP
#define SCENEDB_HPP
#include <string>
#include <vector>
#include "rapidjson/document.h"

struct Actor;

class SceneDB {
public:
    void loadScene(const std::string&, std::vector<Actor>& actors);

private:
    rapidjson::Document scene;
};

#endif
