#ifndef ARTIST_H
#define ARTIST_H
#include <iostream>
#include <vector>
#include <string>
#include "person.h"
#include "user.h"
#include "track.h"
#include <algorithm>
#include "consts.h"

class Artist : public Person
{
private:
    std::vector<Track> tracks;
    std::vector<Person*> followings;
    std::vector<Person*> followers;
    void printFollowings();
    void printFollowers();
public:
    Track* findTrackById(int id);
    Artist(std::string name, std::string password);
    void newTrack(std::string title, std::string path, int years,
                  std::string album, std::string tags, std::string duration);
    void deleteTrack(int trackId);
    bool isTrackExist(int trackId);
    bool isTrackExistByName(std::string name);
    void printTracks();
    void printTracksAbs();
    void printTrackById(int id);
    std::vector<Track> getTracks();
    void printInfo();
    void printInfoMore();
    void searchMusic(std::string name, std::string tag);
    void addFollowing(Person* user);
    void addFollower(Person* user);
    void removeFollowing(Person* user);
    void removeFollower(Person* user);
};

#endif