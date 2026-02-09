#pragma once
#include <string>
#include <sstream>
#include <iomanip>
#include <cctype>

using namespace std;

inline int parseDurationToSeconds(const string& input) {
    string s;
    for (char c : input) if (!isspace((unsigned char)c)) s.push_back(c);
    if (s.empty()) return -1;

    size_t colonPos = s.find(':');
    if (colonPos == string::npos) {
        for (char c : s) if (!isdigit((unsigned char)c)) return -1;
        try {
            int sec = stoi(s);
            return sec >= 0 ? sec : -1;
        }
        catch (...) { return -1; }
    }
    else {
        string minStr = s.substr(0, colonPos);
        string secStr = s.substr(colonPos + 1);
        if (minStr.empty() || secStr.empty()) return -1;
        for (char c : minStr) if (!isdigit((unsigned char)c)) return -1;
        for (char c : secStr) if (!isdigit((unsigned char)c)) return -1;
        try {
            int minutes = stoi(minStr);
            int seconds = stoi(secStr);
            if (minutes < 0 || seconds < 0 || seconds >= 60) return -1;
            return minutes * 60 + seconds;
        }
        catch (...) { return -1; }
    }
}

// Convierte segundos a "mm:ss"
inline string formatSecondsToMMSS(int totalSeconds) {
    if (totalSeconds < 0) return "0:00";
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;
    std::ostringstream oss;
    oss << minutes << ':' << setw(2) << setfill('0') << seconds;
    return oss.str();
}