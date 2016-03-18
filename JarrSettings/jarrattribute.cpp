#include "jarrattribute.h"
#include <iostream>
#include <sstream>

namespace jarr
{
    Attribute::Attribute()
    {
        this->type = 1;
        this->unique = false;
        next = NULL;
        prev = NULL;
    }

    Attribute::Attribute(std::string name, std::string value)
    {
        this->name  = name;
        this->value = value;
    }

    void Attribute::dump()
    {
        std::cout << "dump: " << this->name << " = " <<
                     this->value << std::endl ;

    }

    int Attribute::to_number(std::string str)
    {
        int number;
        std::istringstream is(str);
        is >> number;
        return number;
    }

    std::string Attribute::to_string(int number)
    {
        std::ostringstream os;
        os << number;
        return os.str();
    }
}
