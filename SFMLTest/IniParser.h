#pragma once

#include <string>
#include <unordered_map>
#include <vector>

struct Section
{
    std::string name;
    std::unordered_multimap<std::string, std::string> keyvalues;
};

class Config
{
private:
    bool parse(const std::string& filename);

    std::vector<Section> sections;

public:
    Config() {};
    Config(const std::string& filename);

    std::vector<Section>& getSections();

    Section* getSection(const std::string& sectionname);

    std::string* getValue(const std::string& sectionname, const std::string& keyname);
};