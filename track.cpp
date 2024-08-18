#include "track.h"

int Track::cnter = 0;

Track::Track(std::string _title, std::string _path, int _years,
              std::string _album, std::string _tags, std::string _duration, std::string _Artist)
{
    title = _title;
    path = _path;
    year = _years;
    album = _album;
    tags = _tags;
    duration = _duration;
    cnter++;
    id = cnter;
    artist = _Artist;
}

int Track::getId() { return id; }

int Track::getLike() { return likes; }

std::string Track::getDuration() { return duration; }

std::string Track::getName() { return title; }

std::string Track::getTag() { return tags; }

void Track::printInfo()
{
    std::cout << id << USER_INFO_SEP << title << USER_INFO_SEP << artist 
              <<USER_INFO_SEP << year << USER_INFO_SEP << album << USER_INFO_SEP << tags 
              << USER_INFO_SEP << duration << std::endl;
}

void Track::printInfoAbs()
{
    std::cout << id << USER_INFO_SEP << title << USER_INFO_SEP << artist << std::endl;
}

void Track::like() { likes++; }

void Track::printRecom()
{
    std::cout << id << ARTIST_INFO_SEP << title << ARTIST_INFO_SEP << artist << ARTIST_INFO_SEP << likes << std::endl;
}