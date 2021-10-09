#include "Config.h"

Config& Config::Instance()
{
    static Config config;
    return config;
}

Config::Config()
{
    LoadConfig();
}

void Config::LoadConfig()
{
    maxNumFeatures = 300;
}