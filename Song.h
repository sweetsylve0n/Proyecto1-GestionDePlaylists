#pragma once
#include <string>

// Estructura para representar una canción
struct Song {
    int id;
    std::string name;
    std::string artist;
    int durationSec; // duración en segundos, luego hacemos la conversion

    // Constructor 
    Song() : 
        id(0), 
        name(""), 
        artist(""), 
        durationSec(0) {}

    Song(int _id, const std::string& _name, const std::string& _artist, int _durationSec): 
        id(_id), 
        name(_name), 
        artist(_artist), 
        durationSec(_durationSec) {
    }
};