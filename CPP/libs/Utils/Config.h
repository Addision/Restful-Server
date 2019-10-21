#pragma once

#include "../jsoncpp/json.h"
#include <memory>


class Config{
public:
    bool Load(const char *jsonFile = "./PB/serverconf.json");

public:
    Json::Value m_Root;
    Json::Value m_ServerConf;
};

extern std::unique_ptr<Config>  gPtr_Config;