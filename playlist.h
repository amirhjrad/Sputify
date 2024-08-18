#ifndef PLAYLIST_H
#define PLAYLIST_H
#include <iostream>
#include <string>
#include "track.h"
#include <iomanip>
#include <algorithm>
#include "consts.h"

class Playlist
{
private:
    std::string name;
    std::vector<Track> tracks;
    int numOfTracks = 0;
public:
    Playlist(std::string _name);
    void addTrack(Track track);
    std::string getName();
    int getNumOfSongs();
    std::string getDuration();
    void removeTrack(int id);
    void printTracks();
};

#endif