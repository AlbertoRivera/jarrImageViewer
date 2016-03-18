#include "settings.h"
#include <fstream>
#include <iostream>

namespace jarr
{
    Settings::Settings()
    {

    }

    void Settings::load()
    {
        std::ifstream input;
        std::string line,sub;

        input.open(path.c_str());

        if (input.fail())
        {
            std::cout << "can't open file" << std::endl;
            return;
        }

        while(!input.eof())
        {
            getline(input,line);

            if(input.eof())
                break;

            Attribute matt = Attribute();

            for (unsigned int var = 0; var < line.size(); ++var) {
                if(line.at(var) == ' ' || line.at(var) == '=')
                {
                    sub = line.substr(0,var);
                    matt.name = sub;
                    line.replace(0,var,"");
                    break;
                }
            }

            for(unsigned int i = 0; i < line.size() ; i++)
            {
                if(line.at(i) != ' ' && line.at(i) != '=')
                    break;
                line.replace(i,1,"");
                i--;
            }

            matt.value = line;

            settings_map_att[matt.name] = matt;
        }

        input.close();
    }

    void Settings::save()
    {
        std::ofstream output;

        output.open(path.c_str());

        if (output.fail())
        {
            std::cout << "can't open file" << std::endl;
            return;
        }

        MAP_FOREACH
        {
            Attribute &tmp = MAP_FOREACH_ELEM;
            output << tmp.name << " = " << tmp.value << std::endl;
        }

        output.flush();
        output.close();
    }

    std::string Settings::retrive(std::string attribute_name)
    {
        return settings_map_att.at(attribute_name).value;
    }

    int Settings::retrive_as_int(std::string attribute_name)
    {
        return jarr::Attribute::to_number(
                    settings_map_att.at(attribute_name).value);
    }

    Attribute &Settings::retrive_att(std::string attribute_name)
    {
        return settings_map_att.at(attribute_name);
    }

    void Settings::modify(std::string attribute_name, std::string value)
    {
         settings_map_att[attribute_name].value = value;
    }

    void Settings::dump()
    {
        MAP_FOREACH
        {
            Attribute &tmp = MAP_FOREACH_ELEM;
            std::cout << tmp.name << " => " << tmp.value << std::endl;
        }
    }

}
