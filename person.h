#ifndef PERSON_H
#define PERSON_H
#include <iostream>
#include <vector>
#include <string>
#include "consts.h"
#include <algorithm>

class User;
class Artist;
class Person
{
protected:
    std::string mode;
    std::string name;
    std::string pass;
    int id = 0;
    static int cnter;
    std::vector<User*> followingUsers;
    std::vector<Artist*> followingArtists;
    std::vector<User*> followerUsers;
    std::vector<Artist*> followerArtists;
public:
    Person(std::string _name, std::string _password, std::string _mode);
    std::string getMode();    
    std::string getName();
    std::string getPass();
    int getId();
};

#endif