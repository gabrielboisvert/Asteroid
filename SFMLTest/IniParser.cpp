#include "pch.h"
#include "IniParser.h"
#include <fstream>

static std::string& ltrim(std::string& str) {
    size_t startpos = str.find_first_not_of(" \t\r\n\v\f");
    if (std::string::npos != startpos)
        str = str.substr(startpos);
    return str;
}

static std::string& rtrim(std::string& str) {
    size_t endpos = str.find_last_not_of(" \t\r\n\v\f");
    if (std::string::npos != endpos)
        str = str.substr(0, endpos + 1);
    return str;
}

bool Config::parse(const std::string& filename)
{
    Section currentsection;
    std::ifstream ifi(filename.c_str());
    if (!ifi.is_open())
        return false;

    std::string line;
    while (std::getline(ifi, line))
    {
                            //Allow comment
        if (line.empty() || line[0] == '#' || line[0] == ';')
            continue;

        size_t pos = line.find_first_of("#");
        if (std::string::npos != pos)
            line = line.substr(0, pos);

        ltrim(rtrim(line));

        //Its a section
        if (line[0] == '[')
        {
            size_t end = line.find_first_of(']');
            if (end != std::string::npos)
            {
                if (!currentsection.name.empty())
                {
                    this->sections.push_back(currentsection);
                    currentsection.name.clear();
                    currentsection.keyvalues.clear();
                }

                currentsection.name = line.substr(1, end - 1);
            }
        }
        else
        {
            size_t split = line.find_first_of('=');
            if (split != std::string::npos)
            {
                std::string name = line.substr(0, split);
                ltrim(rtrim(name));

                std::string value = line.substr(split + 1);
                ltrim(rtrim(value));

                currentsection.keyvalues.insert({ name, value });
            }
        }
    }

    if (!currentsection.name.empty())
        sections.push_back(currentsection);

    ifi.close();
    return true;
}

Config::Config(const std::string& filename)
{
    this->parse(filename);
}

std::vector<Section>& Config::getSections()
{
	return this->sections;
}

Section* Config::getSection(const std::string& sectionname)
{
	for (unsigned int i = 0; i < this->sections.size(); i++)
		if (this->sections[i].name._Equal(sectionname))
			return &this->sections[i];
	return nullptr;
}

std::string* Config::getValue(const std::string& sectionname, const std::string& keyname)
{
	Section* section = this->getSection(sectionname);
	if (section == nullptr)
		return nullptr;

    if (section->keyvalues.find(keyname) == section->keyvalues.end())
        return nullptr;
	
	return &section->keyvalues.find(keyname)->second;
}
