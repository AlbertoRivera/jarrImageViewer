#ifndef JARR_SETTINGS_H
#define JARR_SETTINGS_H
#include <list>
#include <vector>
#include <string>
#include <map>
#include "jarrattribute.h"
namespace jarr
{
    class Settings
    {
    public:
        Settings();
        std::string path;
        std::map<std::string,Attribute> settings_map_att;
    #define MAP_ITERATOR std::map<std::string, Attribute>::iterator
    #define MAP_FOREACH for(MAP_ITERATOR it = settings_map_att.begin(); \
                        it != settings_map_att.end(); it++)
    #define MAP_FOREACH_ELEM  settings_map_att.at(it->first)

        // functions
        void load();
        void save();
        std::string retrive(std::string attribute_name);
        int retrive_as_int(std::string attribute_name);
        Attribute& retrive_att(std::string attribute_name);
        void modify(std::string attribute_name, std::string value);
        void dump();
    };
}
#endif // JARR_SETTINGS_H
