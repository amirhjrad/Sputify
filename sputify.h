#ifndef SPUTIFY_H
#define SPUTIFY_H
#include <iostream>
#include <string>
#include <vector>
#include "adds.h"
#include "user.h"
#include "artist.h"
#include "track.h"
#include "consts.h"
#include <regex>
#include <algorithm>

class Sputify
{
private:
    int loggedInId = 0;
    std::string input;
    void PostInstr(std::vector<std::string> data);
    void GetInstr(std::vector<std::string> data);
    void DeleteInstr(std::vector<std::string> data);
    void PutInstr(std::vector<std::string> data);
    void signUp(std::vector<std::string> data);
    void logIn(std::vector<std::string> data);
    void logOut();
    void newPlayList(std::vector<std::string> data);
    void newTrack(std::vector<std::string> data);
    void deleteTrack(std::vector<std::string> data);
    void addTrack(std::vector<std::string> data);
    void ArtistTracksInfo(std::vector<std::string> data);
    void userPlaylistsInfo(std::vector<std::string> data);
    void userPlaylistDetail(std::vector<std::string> data);
    void printAllMusics();
    void printSpecialMusicInfo(std::vector<std::string> data);
    void printAllUsers();
    void printSpecialUserInfo(std::vector<std::string> data);
    void searchMusic(std::vector<std::string> data);
    void setSpecialUserParameter(int &id, std::vector<std::string> data);
    void setSearchMusicParameter(std::string &name, std::string &artist, 
         std::string &tag, std::vector<std::string> data);
    void setAddTrackParameter(std::string &name, int &id, std::vector<std::string> data);
    void setNewTrackParameter(std::string &title, std::string &path, std::string &album, 
         std::string &tags, std::string &duration, int &year, std::vector<std::string> data);
    void setSignUpParameter(std::string &userName, std::string &passWord, std::string &mode,
         std::vector<std::string> data);
    void setLogInParameter(std::string &userName, std::string &password, std::vector<std::string> data);
    std::vector<User> users;
    std::vector<Artist> artists;
    bool isUserExist(std::string userName);
    bool isArtistExist(std::string artistName);
    bool isSongExist(int id);
    bool isPLExist(std::string name);
    User* findUserByName(std::string name);
    User* findUserById(int id);
    Artist* findArtistById(int id);
    Artist* findArtistByName(std::string name);
    Track* findTrackByIdP(int id);
    Track findTrackById(int id);
    void postFollow(std::vector<std::string> data);
    void postUnfollow(std::vector<std::string> data);
    void deletePL(std::vector<std::string> data);
    void postLike(std::vector<std::string> data);
    void printUserFaves();
    bool compareLikes(Track* track1, Track* track2);
    void fillMostLikedTracks(std::vector<Track*>& mostLikedTracks, User* tmpUser);
    void GetRecommendations();
public:
     void run();
     void decodeInput(std::vector<std::string> data);
     void getInput();
};



#endif