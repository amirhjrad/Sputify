#include "artist.h"

Artist::Artist(std::string name, std::string password) : Person(name, password, ARTIST) {}

void Artist::newTrack(std::string title, std::string path, int years,
              std::string album, std::string tags, std::string duration)
{
    Track tmp(title, path, years, album, tags, duration, getName());
    tracks.push_back(tmp);
}

void Artist::deleteTrack(int trackId)
{
    auto it = std::find_if(tracks.begin(), tracks.end(), [trackId](Track& track) {
        return track.getId() == trackId;
    });

    if (it != tracks.end()) {
        tracks.erase(it);
        std::cout << OK <<std::endl;
    } else {
        throw std::runtime_error(PERMISSION_DENIED);
    }
}

Track* Artist::findTrackById(int id)
{
    for (Track& tmp : tracks)
        if (tmp.getId() == id)
            return &tmp;
    return NULL;
}

bool Artist::isTrackExist(int trackId)
{
    for (Track& tmp : tracks)
        if (tmp.getId() == trackId)
            return true;
    return false;
}

void Artist::searchMusic(std::string name, std::string tag)
{
    for (Track &tmpTrack : tracks)
    {
        std::string trackName = tmpTrack.getName();
        if (trackName.find(name) != std::string::npos)
        {
            if (tag == "-1"){
                tmpTrack.printInfoAbs();
                return;
            }
            else 
            {
                std::string trackTag = tmpTrack.getTag();
                if (trackTag.find(tag) != std::string::npos){
                    tmpTrack.printInfoAbs(); 
                    return;
                }
                else throw std::runtime_error(EMPTY);
            }
        }
        else throw std::runtime_error(EMPTY);
    }
}

bool Artist::isTrackExistByName(std::string name)
{
    for (Track &tmpTrack : tracks)
    {
        std::string trackName = tmpTrack.getName();
        if (trackName.find(name) != std::string::npos)
            return true;
    }
    return false;
}

void Artist::printTracks()
{
    std::cout << MUSIC_TABLE << std::endl;
    std::sort(tracks.begin(), tracks.end(), [](Track& track1, Track& track2) {
        return track1.getId() < track2.getId();
    });
    for (Track& tmpTrack : tracks) {
        tmpTrack.printInfo();
    }
}

void Artist::printTracksAbs()
{
    std::sort(tracks.begin(), tracks.end(), [](Track& track1, Track& track2) {
        return track1.getId() < track2.getId();
    });
    for (Track& tmpTrack : tracks) {
        tmpTrack.printInfoAbs();
    }
}

void Artist::printTrackById(int id)
{
    for (Track& tmp : tracks)
        if (tmp.getId() == id)
            tmp.printInfo();
}

void Artist::printInfo()
{
    std::cout << id << ARTIST_INFO_SEP << mode << ARTIST_INFO_SEP << name << ARTIST_INFO_SEP << tracks.size() << std::endl;
}

void Artist::printFollowings()
{
    int i = 0;
    for (Person* tmp : followings){
        std::cout << tmp->getName();
        if (i != static_cast<int>(followings.size())-1)
            std::cout <<ARTIST_INFO_SEP;
        i++;
    }
    std::cout << std::endl;
}

void Artist::printFollowers()
{
    int i = 0;
    for (Person* tmp : followers){
        std::cout << tmp->getName();
        if (i != static_cast<int>(followers.size())-1)
            std::cout <<ARTIST_INFO_SEP;
        i++;
    }
    std::cout << std::endl;
}

void Artist::printInfoMore()
{
    std::cout << ID_SIG << id << std::endl << MODE_SIG << mode << std::endl <<
                 USERNAME_SIG << name << std::endl << FOLLOWINGS_SIG;
    printFollowings();
    std::cout << FOLLOWERS_SIG;
    printFollowers();
    std:: cout << PL_SONGS_SIG;
    int i = 0;
    for (Track &tmpTrack : tracks)
    {
        std::cout << tmpTrack.getName();
        if (i != static_cast<int>(tracks.size())-1)
            std::cout <<ARTIST_INFO_SEP;
        i++;
    }
    std:: cout << std::endl;
}

std::vector<Track> Artist::getTracks() { return tracks; }

void Artist::addFollowing(Person* user)
{
    for (Person* following : followings)
        if (following == user)
            throw std::runtime_error(BAD_REQUEST);
    followings.push_back(user);
    std::cout << OK << std::endl;
}

void Artist::addFollower(Person* user)
{
    for (Person* follower : followers)
        if (follower == user)
            throw std::runtime_error(BAD_REQUEST);
    followers.push_back(user);
}

void Artist::removeFollowing(Person* user)
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

void Artist::removeFollower(Person* user)
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