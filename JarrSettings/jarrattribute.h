#ifndef JARR_JARRATTRIBUTE_H
#define JARR_JARRATTRIBUTE_H
#include <string>

namespace jarr
{
    class Attribute
    {
    public:
        Attribute ();
        Attribute (std::string,std::string);

        std::string name;
        std::string value;

        static const int VALUE_TYPE_NUMBER = 0;
        static const int VALUE_TYPE_STRING = 1;

        int type;
        bool unique;
        void dump();
        static int to_number(std::string);
        static std::string to_string(int);

        Attribute *next;
        Attribute *prev;
    };
}
#endif // JARR_JARRATTRIBUTE_H
