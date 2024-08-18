#include "sputify.h"

void Sputify::run()
{
    getInput();
}

void Sputify::getInput()
{
    while (std::getline(std::cin, input))
    {
        try
        {
            if (!input.size())
                throw std::runtime_error(BAD_REQUEST);
            if (input.back() == '\r')
                input.pop_back();
            std::vector<std::string> data = separateText(input, SEPARATOR);
            decodeInput(data);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }
    }
}

void Sputify::decodeInput(std::vector<std::string> data)
{
    if ( ((data.size()<MIN_INPUT_ARG)||(data.size()>MAX_INPUT_ARG))||
         ((data[0] != POST_INSTR && data[0] != PUT_INSTR && data[0] != GET_INSTR && data[0] != DELETE_INSTR))||
           data[2] != INSTR_SIGN)
            throw std::runtime_error(BAD_REQUEST);
    else if (data[0] == POST_INSTR)
        PostInstr(data);
    else if (data[0] == GET_INSTR)
        GetInstr(data);
    else if (data[0] == DELETE_INSTR)
        DeleteInstr(data);
    else if (data[0] == PUT_INSTR)
        PutInstr(data);
}

void Sputify::PostInstr(std::vector<std::string> data)
{
    if (data[1]!=SIGNUP &&data[1]!=LOGIN&&data[1]!=LOGOUT&&data[1]!=PLAY_LIST&&data[1]!=MUSIC&&
        data[1]!=FOLLOW&&data[1]!=UNFOLLOW&&data[1]!=LIKE)
        throw std::runtime_error(NOT_FOUND);
    if (data[1]==SIGNUP&&data[3]==USERNAME&&data[5]==PASSWORD&&data[7]==MODE&&data.size()==SIGNUP_SIZE)
        signUp(data);
    else if (data[1]==LOGIN&&data[3]==USERNAME&&data[5]==PASSWORD&&data.size()==LOGIN_SIZE)
        logIn(data);
    else if (data[1] == LOGOUT && data.size() == LOGOUT_SIZE)
        logOut();
    else if (data[1] == PLAY_LIST && data[3] == NAME && data.size() == PLAYLIST_SIZE)
        newPlayList(data);
    else if (data[1]==MUSIC&&data[3]==TITLE&&data[5]==PATH&&data[7]==YEAR&&
             data[9]==ALBUM&&data[11]==TAGS&&data[13]==DURATION&&data.size()==MUSIC_SIZE)
        newTrack(data);
    else if (data[1]==FOLLOW && data[3]==ID&&data.size()==5)
        postFollow(data);
    else if (data[1]==UNFOLLOW && data[3]==ID&&data.size()==5)
        postUnfollow(data);
    else if (data[1]==LIKE && data[3]==ID&&data.size()==5)
        postLike(data);
    else throw std::runtime_error(BAD_REQUEST);
}

void Sputify::GetInstr(std::vector<std::string> data)
{
    if (data[1] != REG_MUSICS && data[1] != PLAY_LIST && data[1] != MUSICS &&
        data[1] != USERS && data[1] != SEARCH_MUSIC && data[1] != LIKES && data[1] != RECOMMENDATIONS)
        throw std::runtime_error(NOT_FOUND);
    if (data[1] == REG_MUSICS && data.size()==REG_MUSICS_SIZE)
        ArtistTracksInfo(data);
    else if (data[1] == PLAY_LIST && data[3] == ID && data.size()== PLAYLIST_SIZE)
        userPlaylistsInfo(data);
    else if (data[1] == PLAY_LIST && data[3] == NAME && data[5] == ID &&  data.size()== 7)
        userPlaylistDetail(data);
    else if (data[1] == MUSICS && data.size()==5 && data[3] == ID)
        printSpecialMusicInfo(data);
    else if (data[1] == MUSICS && data.size()==3)
        printAllMusics();
    else if (data[1] == USERS && data.size()==5 && data[3] == ID)
        printSpecialUserInfo(data);
    else if (data[1] == USERS && data.size()==3)
        printAllUsers();
    else if (data[1] == SEARCH_MUSIC && data[3]==NAME && data[5]==ARTIST && data.size()==SEARHC_MUSICS_SIZE)
        searchMusic(data);
    else if (data[1] == LIKES && data.size()==3)
        printUserFaves();
    else if (data[1] == RECOMMENDATIONS && data.size()==3)
        GetRecommendations();
    else throw std::runtime_error(BAD_REQUEST);
}

void Sputify::DeleteInstr(std::vector<std::string> data)
{
    if (data[1] != MUSIC && data[1] != PLAY_LIST)
        throw std::runtime_error(NOT_FOUND);
    if (data[1] == MUSIC && data[3]== ID && data.size()==5)
        deleteTrack(data);
    else if (data[1] == PLAY_LIST && data[3]== NAME && data.size()==5)
        deletePL(data);
    else throw std::runtime_error(BAD_REQUEST);
}

void Sputify::PutInstr(std::vector<std::string> data)
{
    if (data[1] != ADD_PL)
        throw std::runtime_error(NOT_FOUND);
    if (data[1] == ADD_PL && data[3] == NAME && data[5] == ID && data.size() == ADD_PL_SIZE)
        addTrack(data);
    else throw std::runtime_error(BAD_REQUEST);
}

bool Sputify::compareLikes(Track* track1, Track* track2)
{
    if (track1->getLike() != track2->getLike()) {
        return track1->getLike() > track2->getLike();
    } else {
        return track1->getId() < track2->getId();
    }
}

void Sputify::fillMostLikedTracks(std::vector<Track*>& mostLikedTracks, User* tmpUser)
{
    std::vector<Track*> faveTracks = tmpUser->getFaves();

    for (Artist& artist : artists) {
        for (Track& track : artist.getTracks()) {
            if (mostLikedTracks.size() < MAX_RECOMMENDATION_TRACKS || compareLikes(&track, mostLikedTracks.back())) {
                if (track.getLike() != 0 && !tmpUser->isTrackLiked(track)) {
                    Track* newTrack = new Track(track);
                    mostLikedTracks.push_back(newTrack);
                    std::sort(mostLikedTracks.begin(), mostLikedTracks.end(), [this](Track* track1, Track* track2) {
                        return compareLikes(track1, track2);
                    });
                    if (mostLikedTracks.size() > MAX_RECOMMENDATION_TRACKS)
                        mostLikedTracks.pop_back();
                }
            }
        }
    }
}

void Sputify::GetRecommendations()
{
    if (loggedInId == 0)
        throw std::runtime_error(PERMISSION_DENIED);
    User* tmpUser = findUserById(loggedInId);
    if (tmpUser == nullptr)
        throw std::runtime_error(PERMISSION_DENIED);
    std::vector<Track*> mostLikedTracks;
    fillMostLikedTracks(mostLikedTracks, tmpUser);
    if (mostLikedTracks.empty())
        throw std::runtime_error(EMPTY);
    std::cout << RECOM_MUSICS_TABLE << std::endl;
    for (Track* track : mostLikedTracks)
        track->printRecom();
}

void Sputify::postFollow(std::vector<std::string> data)
{
    if (loggedInId == 0) throw std::runtime_error(PERMISSION_DENIED);
    int id;
    setSpecialUserParameter(id, data);
    User* currUser = findUserById(loggedInId);
    Artist* currArtist = findArtistById(loggedInId);
    User* tmpUser = findUserById(id);
    Artist* tmpArtist = findArtistById(id);
    if (tmpUser == nullptr && tmpArtist == nullptr)
        throw std::runtime_error(NOT_FOUND);
    if (loggedInId == id)
        throw std::runtime_error(BAD_REQUEST);
    if (currUser != nullptr){
        if (tmpUser != nullptr) {
            currUser->addFollowing(tmpUser);
            tmpUser->addFollower(currUser);
        }
        else if (tmpArtist != nullptr) {
            currUser->addFollowing(tmpArtist);
            tmpArtist->addFollower(currUser);
        }
    }
    else {
        if (tmpUser != nullptr) {
            currArtist->addFollowing(tmpUser);
            tmpUser->addFollower(currArtist);
        }
        else if (tmpArtist != nullptr) {
            currArtist->addFollowing(tmpArtist);
            tmpArtist->addFollower(currArtist);
        }
    }
}

void Sputify::postUnfollow(std::vector<std::string> data)
{
    if (loggedInId == 0) throw std::runtime_error(PERMISSION_DENIED);
    int id;
    setSpecialUserParameter(id, data);
    User* currUser = findUserById(loggedInId);
    Artist* currArtist = findArtistById(loggedInId);
    User* tmpUser = findUserById(id);
    Artist* tmpArtist = findArtistById(id);
    if (tmpUser == nullptr && tmpArtist == nullptr)
        throw std::runtime_error(NOT_FOUND);
    if (loggedInId == id)
        throw std::runtime_error(BAD_REQUEST);
    if (currUser != nullptr){
        if (tmpUser != nullptr) {
            currUser->removeFollowing(tmpUser);
            tmpUser->removeFollower(currUser);
        }
        else if (tmpArtist != nullptr){
             currUser->removeFollowing(tmpArtist);
             tmpArtist->removeFollower(currUser);
        }
    }
    else {
        if (tmpUser != nullptr){
             currArtist->removeFollowing(tmpUser);
             tmpUser->removeFollower(currArtist);
        }
        else if (tmpArtist != nullptr) {
            currArtist->removeFollowing(tmpArtist);
            tmpArtist->removeFollower(currArtist);
        }
    }
}

void Sputify::searchMusic(std::vector<std::string> data)
{
    if (loggedInId == 0)
        throw std::runtime_error(PERMISSION_DENIED);
    User* tmpUser = findUserById(loggedInId);
    if (tmpUser == NULL)
        throw std::runtime_error(PERMISSION_DENIED);
    std::string name, artist, tag = "-1";
    setSearchMusicParameter(name, artist, tag, data);
    if (!artists.size())
        throw std::runtime_error(EMPTY);
    bool Exist = false;
    for (Artist& tmpArtist : artists)
    {
        std::string artistName = tmpArtist.getName();
        if (artistName.find(artist) != std::string::npos){
            if (tmpArtist.isTrackExistByName(name)){
                Exist = true;
                tmpArtist.searchMusic(name, tag);
            }
        }
    }
    if (!Exist) throw std::runtime_error(EMPTY);
}

void Sputify::printUserFaves()
{
    if (loggedInId == 0)
        throw std::runtime_error(PERMISSION_DENIED);
    User* tmpUser = findUserById(loggedInId);
    if (tmpUser == nullptr)
        throw std::runtime_error(PERMISSION_DENIED);
    if (!tmpUser->faveTracksSize())
        throw std::runtime_error(EMPTY);
    tmpUser->printFaves();
}

void Sputify::userPlaylistDetail(std::vector<std::string> data)
{
    if (loggedInId == 0)
        throw std::runtime_error(PERMISSION_DENIED);
    User* tmpUser = findUserById(loggedInId);
    if (tmpUser == nullptr)
        throw std::runtime_error(PERMISSION_DENIED);
    std::string name;
    int id;
    setAddTrackParameter(name, id, data);
    Artist* tmpArtist = findArtistById(id);
    if (tmpArtist != nullptr)
        throw std::runtime_error(BAD_REQUEST);
    tmpUser->printPlaylistDetail(name);
}

void Sputify::postLike(std::vector<std::string> data)
{
    if (loggedInId == 0)
        throw std::runtime_error(PERMISSION_DENIED);
    User* tmpUser = findUserById(loggedInId);
    if (tmpUser == nullptr)
        throw std::runtime_error(PERMISSION_DENIED);
    int id;
    setSpecialUserParameter(id, data);
    if (!isSongExist(id))
        throw std::runtime_error(NOT_FOUND);
    Track* tmpTrk = findTrackByIdP(id);
    tmpUser->addFavTracks(tmpTrk);
    tmpTrk->like();
    std:: cout << OK << std::endl;
}

void Sputify::deletePL(std::vector<std::string> data)
{
    if (loggedInId == 0)
        throw std::runtime_error(PERMISSION_DENIED);
    User* tmpUser = findUserById(loggedInId);
    if (tmpUser == nullptr)
        throw std::runtime_error(PERMISSION_DENIED);
    std::string plName = data[4];
    if (!isPLExist(plName))
        throw std::runtime_error(NOT_FOUND);
    tmpUser->removePL(plName);
}

void Sputify::setSearchMusicParameter(std::string &name, std::string &artist, std::string &tag,
                                      std::vector<std::string> data)
{
    for (int i = 0; i < static_cast<int>(data.size()); i++)
    {
        if(data[i] == NAME){
            name = data[i+1];
            i++;
        }
        else if(data[i] == ARTIST){
            artist = data[i+1];
            i++;
        }
        else if(data[i] == TAG){
            tag = data[i+1];
            i++;
        }
    }
}

void Sputify::printAllUsers()
{
    if (loggedInId == 0)
        throw std::runtime_error(PERMISSION_DENIED);
    if (users.size()==0 && artists.size()==0)
        throw std::runtime_error(EMPTY);
    std::cout << USERS_TABLE << std::endl;
    for (int id = 1; id <= (static_cast<int>(users.size()) + static_cast<int>(artists.size())); id++)
    {
        for (User& tmpUser : users)
            if(tmpUser.getId() == id)
                tmpUser.printInfo();
         for (Artist& tmpArtist : artists)
            if(tmpArtist.getId() == id)
                tmpArtist.printInfo();
    }
}

void Sputify::setSpecialUserParameter(int &id, std::vector<std::string> data)
{
    for (int i = 0; i < static_cast<int>(data.size()); i++)
        if(data[i] == ID){
            if (std::regex_match(data[i+1], std::regex("\\d+"))){
                id = std::stoi(data[i+1]);
                i++;
            }
            else throw std::runtime_error(BAD_REQUEST);
        }
}

void Sputify::printSpecialUserInfo(std::vector<std::string> data)
{
    if (loggedInId == 0)
        throw std::runtime_error(PERMISSION_DENIED);
    int id;
    bool userFound = false, artistFound = false;
    setSpecialUserParameter(id, data);
    for (Artist& tmpArtist : artists)
        if (tmpArtist.getId() == id)
        {
            userFound = true;
            tmpArtist.printInfoMore();
        }
    for (User& tmpUser : users)
        if (tmpUser.getId() == id)
        {
            artistFound = true;
            tmpUser.printInfoMore();
        }
    if (userFound == 0 && artistFound == 0)
        throw std::runtime_error(NOT_FOUND);
}

void Sputify::printSpecialMusicInfo(std::vector<std::string> data)
{
    if (loggedInId == 0)
        throw std::runtime_error(PERMISSION_DENIED);
    int id;
    for (int i = 0; i < static_cast<int>(data.size()); i++)
        setSpecialUserParameter(id ,data);
    bool isFound = false;
    for (Artist& tmpArtist : artists)
        if (tmpArtist.isTrackExist(id))
            isFound = true;
    if (!isFound)
        throw std::runtime_error(NOT_FOUND);
    else std::cout << MUSIC_TABLE << std::endl;
    for (Artist& tmpArtist : artists)
        tmpArtist.printTrackById(id);
}

void Sputify::printAllMusics()
{
    if (loggedInId == 0)
        throw std::runtime_error(PERMISSION_DENIED);
    int empties = 0;
    for (Artist& tmpArtist : artists)
        if (tmpArtist.getTracks().size() == 0)
            empties++;
    if (empties == static_cast<int>(artists.size()))
        throw std::runtime_error(EMPTY);
    else 
    {
        std::cout << MUSICS_TABLE << std::endl;
        for (Artist& tmpArtist : artists)
            tmpArtist.printTracksAbs();
    }
}

void Sputify::userPlaylistsInfo(std::vector<std::string> data)
{
    if (loggedInId == 0)
        throw std::runtime_error(PERMISSION_DENIED);
    User* foundedUser = findUserById(loggedInId);
    if (foundedUser == NULL)
        throw std::runtime_error(PERMISSION_DENIED);
    int userId = std::stoi(data[4]);
    User* tmpUser = findUserById(userId);
    Artist* tmpArtist = findArtistById(userId);
    if (tmpArtist != NULL)
        throw std::runtime_error(BAD_REQUEST);
    else if (tmpUser == NULL)
        throw std::runtime_error(NOT_FOUND);
    tmpUser->printPlaylistsInfo();
}

void Sputify::ArtistTracksInfo(std::vector<std::string> data)
{
    if (loggedInId == 0)
        throw std::runtime_error(PERMISSION_DENIED);
    Artist* tmpArtist = findArtistById(loggedInId);
    if (tmpArtist == NULL)
        throw std::runtime_error(PERMISSION_DENIED);
    tmpArtist->printTracks();
}

void Sputify::addTrack(std::vector<std::string> data)
{
    if (loggedInId == 0)
        throw std::runtime_error(PERMISSION_DENIED);
    User* tmpUser = findUserById(loggedInId);
    if (tmpUser == NULL)
        throw std::runtime_error(PERMISSION_DENIED);
    std::string name;
    int id;
    setAddTrackParameter(name, id, data);
    Track foundedTrack = findTrackById(id);
    tmpUser->addTrack(name, id, foundedTrack);
}

void Sputify::setAddTrackParameter(std::string &name, int &id, std::vector<std::string> data)
{
    for (int i = 0; i < static_cast<int>(data.size()); i++)
    {
        if(data[i] == NAME){
            name = data[i+1];
            i++;
        }
        else if(data[i] == ID){
            if (std::regex_match(data[i+1], std::regex("\\d+"))){
                id = std::stoi(data[i+1]);
                i++;
            }
            else throw std::runtime_error(BAD_REQUEST);
        }
    }
}

void Sputify::deleteTrack(std::vector<std::string> data)
{
    if (loggedInId == 0)
        throw std::runtime_error(PERMISSION_DENIED);
    Artist* tmpArtist = findArtistById(loggedInId);
    if (tmpArtist == nullptr)
        throw std::runtime_error(PERMISSION_DENIED);
    int trackId = std::stoi(data[4]);
    if (!isSongExist(trackId))
        throw std::runtime_error(NOT_FOUND);
    tmpArtist->deleteTrack(trackId);
    for (User &tmpUser : users)
        tmpUser.removeTrack(trackId);
}

void Sputify::newPlayList(std::vector<std::string> data)
{
    if (loggedInId == 0)
        throw std::runtime_error(PERMISSION_DENIED);
    User* tmpUser = findUserById(loggedInId);
    if (tmpUser == NULL)
        throw std::runtime_error(PERMISSION_DENIED);
    std::string name = data[4];
    tmpUser->newPlayList(name);
}

void Sputify::setNewTrackParameter(std::string &title, std::string &path, std::string &album, 
                                   std::string &tags, std::string &duration, int &year, std::vector<std::string> data)
{
    for (int i = 0; i < static_cast<int>(data.size()); i++)
    {
        if(data[i] == TITLE)
            title = data[i+1];
        else if(data[i] == PATH)
            path = data[i+1];
        else if(data[i] == YEAR){
            if (std::regex_match(data[i+1], std::regex("\\d+")))
                year = std::stoi(data[i+1]);
            else throw std::runtime_error(BAD_REQUEST);
        }
        else if(data[i] == ALBUM)
            album = data[i+1];
        else if(data[i] == TAGS)
            tags = data[i+1];
        else if(data[i] == DURATION)
            duration = data[i+1];
    }
}

void Sputify::newTrack(std::vector<std::string> data)
{
    if (loggedInId == 0)
        throw std::runtime_error(PERMISSION_DENIED);
    Artist* tmpArtist = findArtistById(loggedInId);
    if (tmpArtist == NULL)
        throw std::runtime_error(PERMISSION_DENIED);
    std::string title, path, album, tags, duration;
    int year;
    setNewTrackParameter(title, path, album, tags, duration, year, data);
    tmpArtist->newTrack(title, path, year, album, tags, duration);
    std::cout << OK << std::endl;
}

void Sputify::setSignUpParameter(std::string &userName, std::string &passWord, 
                                 std::string &mode, std::vector<std::string> data)
{
    for (int i = 0; i < static_cast<int>(data.size()); i++)
    {
        if(data[i] == USERNAME)
            userName = data[i+1];
        else if(data[i] == PASSWORD)
            passWord = data[i+1];
        else if(data[i] == MODE)
            mode = data[i+1];
    }
}

void Sputify::signUp(std::vector<std::string> data)
{
    std::string userName, passWord, mode;
    setSignUpParameter(userName, passWord, mode, data);
    if ((mode != USER && mode != ARTIST))
        throw std::runtime_error(BAD_REQUEST);
    else if (loggedInId != 0)
        throw std::runtime_error(PERMISSION_DENIED);
    if (mode == USER){
        if(isUserExist(userName) || isArtistExist(userName))
            throw std::runtime_error(BAD_REQUEST);
        User tmpUser(userName, passWord);
        users.push_back(tmpUser);
        loggedInId = tmpUser.getId();
    } else if (mode == ARTIST){
        if(isUserExist(userName) || isArtistExist(userName))
            throw std::runtime_error(BAD_REQUEST);
        Artist tmpArtist(userName, passWord);
        artists.push_back(tmpArtist);
        loggedInId = tmpArtist.getId();
    }
    std::cout<< OK << std::endl;
}

void Sputify::setLogInParameter(std::string &userName, std::string &passWord, std::vector<std::string> data)
{
    for (int i = 0; i < static_cast<int>(data.size()); i++)
    {
        if(data[i] == USERNAME){
            userName = data[i+1];
            i++;}
        else if(data[i] == PASSWORD){
            passWord = data[i+1];
            i++;}
    }
}

void Sputify::logIn(std::vector<std::string> data) {
    std::string userName, passWord;
    setLogInParameter(userName, passWord, data);
    if (loggedInId != 0)
        throw std::runtime_error(PERMISSION_DENIED);
    if (!isUserExist(userName) && !isArtistExist(userName))
        throw std::runtime_error(NOT_FOUND);
    User* tmpUser = findUserByName(userName);
    Artist* tmpArtist = findArtistByName(userName);
    if (tmpUser != NULL) {
        if ((tmpUser->getName() == userName && tmpUser->getPass() != passWord) )
            throw std::runtime_error(PERMISSION_DENIED);
        if (tmpUser->getName() == userName && tmpUser->getPass() == passWord)
            loggedInId = tmpUser->getId();
    }
    else {
        if ((tmpArtist->getName() == userName && tmpArtist->getPass() != passWord))
            throw std::runtime_error(PERMISSION_DENIED);
        if (tmpArtist->getName() == userName && tmpArtist->getPass() == passWord)
            loggedInId = tmpArtist->getId();
    } std::cout << OK << std::endl;
}

void Sputify::logOut()
{
    if (!loggedInId)
        throw std::runtime_error(PERMISSION_DENIED);
    else loggedInId = 0;
    std:: cout << OK << std::endl;
}

bool Sputify::isUserExist(std::string userName)
{
    for (User &tmp : users)
        if (tmp.getName() == userName)
            return true;
    return false;
}

bool Sputify::isArtistExist(std::string artistName)
{
    for (Artist &tmp : artists)
        if (tmp.getName() == artistName)
            return true;
    return false;
}

bool Sputify::isSongExist(int id)
{
    bool exist = false;
    for (Artist& tmp : artists)
        if (tmp.isTrackExist(id))
            exist = true;
    return exist;
}

bool Sputify::isPLExist(std::string name)
{
    bool exist = false;
    for (User& tmp : users)
        if (tmp.isPlaylistExist(name))
            exist = true;
    return exist;
}

User* Sputify::findUserByName(std::string name)
{
    for (User &tmp : users)
        if (tmp.getName() == name)
            return &tmp;
    return NULL;
}

User* Sputify::findUserById(int id)
{
    for (User &tmp : users)
        if (tmp.getId() == id)
            return &tmp;
    return NULL;
}

Artist* Sputify::findArtistById(int id)
{
    for (Artist &tmp : artists)
        if (tmp.getId() == id)
            return &tmp;
    return NULL;
}

Artist* Sputify::findArtistByName(std::string name)
{
    for (Artist &tmp : artists)
        if (tmp.getName() == name)
            return &tmp;
    return NULL;
}

Track* Sputify::findTrackByIdP(int id)
{
     for (Artist& tmpArtist : artists)
    {
        Track* foundTrack = tmpArtist.findTrackById(id);
        if (foundTrack != nullptr)
            return foundTrack;
    }
    return nullptr;
}

Track Sputify::findTrackById(int id)
{
    for (Artist& tmpArtist : artists)
    {
        for (Track& tmpTrack : tmpArtist.getTracks())
        {
            if (tmpTrack.getId() == id)
                return tmpTrack;
        }
    }
    throw std::runtime_error(NOT_FOUND);
}