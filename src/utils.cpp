#include <cctype>
#include <string>

bool isValidNickname(const std::string& nick) {
    if (nick.empty() || nick.length() > 9 || isdigit(nick[0]))
		return false;

    std::string special = "[]\\`_^{|}";
    for (size_t i = 0; i < nick.length(); ++i) {
        if (!isalnum(nick[i]) && special.find(nick[i]) == std::string::npos)
            return false;
    }
    return true;
}