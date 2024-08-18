#include "playlist.h"

Playlist::Playlist(std::string _name)
{
    name = _name;
}

std::string Playlist::getName() { return name; }

int Playlist::getNumOfSongs() { return numOfTracks; }

void Playlist::addTrack(Track track)
{
    tracks.push_back(track);
    numOfTracks = tracks.size();
    std::cout << OK << std::endl;
}

std::string Playlist::getDuration()
{
    int totalDurationInSeconds = 0;
    for (Track& track : tracks)
    {
        std::string duration = track.getDuration();
        int hours = std::stoi(duration.substr(0, 2));
        int minutes = std::stoi(duration.substr(3, 2));
        int seconds = std::stoi(duration.substr(6, 2));
        int trackDurationInSeconds = hours * HOUR_TO_SEC + minutes * MIN_TO_SEC + seconds;
        totalDurationInSeconds += trackDurationInSeconds;
    }
    int totalHours = totalDurationInSeconds / HOUR_TO_SEC;
    int totalMinutes = (totalDurationInSeconds % HOUR_TO_SEC) / MIN_TO_SEC;
    int totalSeconds = totalDurationInSeconds % MIN_TO_SEC;
    std::string totalDurationStr = "";
    totalDurationStr += (totalHours < TIME_THRESHOLD ? ZERO : "") + std::to_string(totalHours);
    totalDurationStr += TIME_SEP;
    totalDurationStr += (totalMinutes < TIME_THRESHOLD ? ZERO : "") + std::to_string(totalMinutes);
    totalDurationStr += TIME_SEP;
    totalDurationStr += (totalSeconds < TIME_THRESHOLD ? ZERO : "") + std::to_string(totalSeconds);
    return totalDurationStr;
}

void Playlist::removeTrack(int id)
{
    auto it = std::find_if(tracks.begin(), tracks.end(), [id](Track& track) {
        return track.getId() == id;
    });

    if (it != tracks.end())
        tracks.erase(it);
}

void Playlist::printTracks()
{
    std:: cout << USER_TABLE << std::endl;
    for (Track & trk : tracks)
    {
        trk.printInfoAbs();
    }
}