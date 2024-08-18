#include "person.h"

int Person::cnter = 0;

Person::Person(std::string _name, std::string _password, std::string _mode)
{
    mode = _mode;
    name = _name;
    pass = _password;
    cnter++;
    id = cnter;
}

int Person::getId() { return id; }

std::string Person::getMode() { return mode; }; 

std::string Person::getName() { return name; }; 

std::string Person::getPass() { return pass; }; 

