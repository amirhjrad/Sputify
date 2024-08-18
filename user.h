#ifndef USER_H
#define USER_H
#include "person.h"
#include "playlist.h"
#include "track.h"
#include "artist.h"
#include <algorithm>

class User : public Person 
{
private:
    std::vector<Playlist> playlists;
    std::vector<Person*> followings;
    std::vector<Person*> followers;
    std::vector<Track*> faveTracks;
    bool isPlaylistUniqe(std::string name);
    void printFollowings();
    void printFollowers();
public:
    bool isPlaylistExist(std::string name);
    User(std::string name, std::string password);
    void newPlayList(std::string playlistName);
    void addTrack(std::string playlistName, int id, Track track);
    void printPlaylistsInfo();
    void printInfo();
    void printInfoMore();
    void removeTrack(int id);
    void removePL(std::string name);
    void addFollowing(Person* user);
    void addFollower(Person* user);
    void removeFollowing(Person* user);
    void removeFollower(Person* user);
    void printPlaylistDetail(std::string plName);
    void addFavTracks(Track *trk);
    int faveTracksSize();
    void printFaves();
    std::vector<Track*> getFaves();
    bool isTrackLiked(Track trk);
};

#endif