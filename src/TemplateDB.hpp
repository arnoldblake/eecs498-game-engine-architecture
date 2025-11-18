#ifndef TEMPLATEDB_HPP
#define TEMPLATEDB_HPP
#include <string>
#include <unordered_map>
#include "rapidjson/document.h"

struct Actor;

class TemplateDB {
public:
    TemplateDB();
    void loadTemplates();
    std::unordered_map<std::string, Actor> actor_templates;

private:
    rapidjson::Document actor_template;
};

#endif
