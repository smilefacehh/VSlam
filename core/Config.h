#pragma once

class Config
{
public:
    static Config& Instance();
    void LoadConfig();

    int maxNumFeatures;
private:
    Config();
};