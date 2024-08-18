#include "adds.h"

void checkData(std::string &token, std::vector<std::string>& result)
{
    if (!token.empty()) {
        result.push_back(token);
        token.clear();
    }
}

std::vector<std::string> separateText(std::string text, char seperator)
{
    std::vector<std::string> result;
    std::string token;
    bool inTag = false;
    for (char c : text) {
        if (c == '<') {
            checkData(token, result);
            inTag = true;
        } else if (c == '>') {
            checkData(token, result);
            inTag = false;
        } else if (c == ' ' && !inTag) {
            checkData(token, result);
        } else token += c;
    }
    checkData(token, result);
    return result;
}
