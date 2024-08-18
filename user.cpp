#include "user.h"

User::User(std::string name, std::string password) : Person(name, password, USER) {}

void User::newPlayList(std::string playlistName)
{
    Playlist tmp(playlistName);
    if (isPlaylistUniqe(playlistName))
        throw std::runtime_error(BAD_REQUEST);
    playlists.push_back(tmp);
    std::cout << OK << std::endl;
}

bool User::isPlaylistUniqe(std::string name)
{
    for (Playlist tmp : playlists)
        if (tmp.getName() == name)
            return true;
    return false;
}

bool User::isPlaylistExist(std::string name)
{
    for (Playlist tmp : playlists)
        if (tmp.getName() == name)
            return true;
    return false;
}

void User::addTrack(std::string playlistName, int id, Track track)
{
    if (!isPlaylistExist(playlistName))
        throw std::runtime_error(NOT_FOUND);
    for (Playlist& tmpPL : playlists)
    {
        if (tmpPL.getName() == playlistName)
            tmpPL.addTrack(track);
    }
}

void User::printPlaylistDetail(std::string plName)
{
    if (!isPlaylistExist(plName))
        throw std::runtime_error(NOT_FOUND);
    for (Playlist& tmpPL : playlists)
    {
        if (tmpPL.getName() == plName)
            tmpPL.printTracks();
    }
}

void User::printPlaylistsInfo()
{
    std::cout << PL_TABLE << std::endl;
    std::sort(playlists.begin(), playlists.end(), [](Playlist& pl1, Playlist& pl2) {
        return pl1.getName() < pl2.getName();
    });

    for (Playlist& tmpPL : playlists)
    {
        std::cout << tmpPL.getName() <<PL_INFO_SEP << tmpPL.getNumOfSongs() <<
                     PL_INFO_SEP << tmpPL.getDuration() << std::endl;
    }
}

void User::printInfo()
{
    std::cout << id <<PL_INFO_SEP << mode <<PL_INFO_SEP << 
                 name <<PL_INFO_SEP << playlists.size() << std::endl;
}

void User::printFollowings()
{
    int i = 0;
    for (Person* tmp : followings){
        std::cout << tmp->getName();
        if (i != static_cast<int>(followings.size())-1)
            std::cout <<USER_INFO_SEP;
        i++;
    }
    std::cout << std::endl;
}

void User::printFollowers()
{
    int i = 0;
    for (Person* tmp : followers){
        std::cout << tmp->getName();
        if (i != static_cast<int>(followers.size())-1)
            std::cout <<USER_INFO_SEP;
        i++;
    }
    std::cout << std::endl;
}

void User::printInfoMore()
{
    std::cout << ID_SIG << id << std::endl << MODE_SIG << mode << 
                 std::endl << USERNAME_SIG << name << std::endl << FOLLOWINGS_SIG;
    printFollowings();
    std::cout<< FOLLOWERS_SIG;
    printFollowers();
    std::cout << PL_SONGS_SIG;
    int i = 0;
    for (Playlist &tmpPL : playlists)
    {
        std::cout << tmpPL.getName();
        if (i != static_cast<int>(playlists.size())-1)
            std::cout <<PL_INFO_SEP;
        i++;
    }
    std:: cout << std::endl;
}

void User::removeTrack(int id)
{
    for (Playlist &tmpPL : playlists)
    {
        tmpPL.removeTrack(id);
    }
}

void User::removePL(std::string name)
{
    auto it = std::remove_if(playlists.begin(), playlists.end(), [name](Playlist& pl) {
    return pl.getName() == name;
    });

    playlists.erase(it, playlists.end());
    std::cout << OK << std::endl;
}

void User::addFollowing(Person* user)
{
    for (Person* following : followings)
        if (following == user)
            throw std::runtime_error(BAD_REQUEST);
    followings.push_back(user);
    std::cout << OK << std::endl;
}

void User::addFollower(Person* user)
{
    for (Person* follower : followers)
        if (follower == user)
            throw std::runtime_error(BAD_REQUEST);
    followers.push_back(user);
}

void User::removeFollowing(Person* user)
{
    bool isFound = false;
    for (Person* following : followings)
        if (following == user)
            isFound = true;
    if (isFound == false)
        throw std::runtime_error(BAD_REQUEST);
    auto newEnd = std::remove(followings.begin(), followings.end(), user);
    followings.erase(newEnd, followings.end());
    std::cout << OK << std::endl;
}

void User::removeFollower(Person* user)
{
    bool isFound = false;
    for (Person* follower : followers)
        if (follower == user)
            isFound = true;
    if (isFound == false)
        throw std::runtime_error(BAD_REQUEST);
    auto newEnd = std::remove(followers.begin(), followers.end(), user);
    followers.erase(newEnd, followers.end());
}

void User::addFavTracks(Track* trk) 
{
    for (Track* &tmp : faveTracks)
    {
        if (tmp->getId() == trk->getId())
            throw std::runtime_error(BAD_REQUEST);
    }
    faveTracks.push_back(trk); 
} 

int User::faveTracksSize() { return faveTracks.size(); }

void User::printFaves()
{
    std::cout << MUSICS_TABLE << std::endl;
    for (Track* trk : faveTracks)
        trk->printInfoAbs();
}

std::vector<Track*> User::getFaves() { return faveTracks; }

bool User::isTrackLiked(Track trk)
{
    for (Track* &tmp : faveTracks)
        if (tmp->getId() == trk.getId())
            return true;
    return false;
}