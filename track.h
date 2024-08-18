#ifndef TRACK_H
#define TRACK_H
#include <iostream>
#include <vector>
#include <string>
#include "consts.h"

class Track
{
private:
    std::string title;
    std::string path;
    int year;
    std::string album;
    std::string tags;
    std::string duration;
    int id;
    int likes = 0;
    static int cnter;
    std::string artist;
public:
    Track(std::string _title, std::string _path, int _years,
          std::string _album, std::string _tags, std::string _duration, std::string _Artist);
    int getId();
    int getLike();
    std::string getDuration();
    std::string getName();
    std::string getTag();
    void printInfo();
    void printInfoAbs();
    void like();
    void printRecom();
};


#endif