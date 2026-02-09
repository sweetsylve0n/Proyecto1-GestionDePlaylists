#pragma once
#include <string>

using namespace std;

struct Song {
    int id;
    string name;
    string artist;
    int durationSec; // duración en segundos, luego hacemos la conversion

    Song() : 
        id(0), 
        name(""), 
        artist(""), 
        durationSec(0) {}

    Song(int _id, const string& _name, const string& _artist, int _durationSec): 
        id(_id), 
        name(_name), 
        artist(_artist), 
        durationSec(_durationSec) {
    }
};