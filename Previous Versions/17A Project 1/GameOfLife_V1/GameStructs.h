#ifndef GAMESTRUCTS_H
#define GAMESTRUCTS_H
#include <string>
using namespace std;

struct House { // Possibly implement so that players can have multiple houses
    char name[32];
    int cost, sell1, sell2;
};

struct Player {
string name;
int money = 200, pos = 0, aCount = 0, stops = 0,
     kids = 0, paid = 0, pets = 0, vCount = 0; // Player stats
int career; // Index in their respective arrays --> Can change to nested structures
bool college = false, mrried = false, winner = false;
int hIds[5] = {-1, -1, -1, -1, -1}; // Indexes of houses in array
House hOwned[5]; // Houses Owned By user 
};

struct Career{ 
    char title[32];
    int income;
};

struct ActCard {
    char event[256];
    int affect; // Positive or negative (add to players money or take away from it)
};

enum struct StpType { // 
    MARR = 13,
    PETS = 25,
    KIDS = 45,
    NIGHT = 76,  // RET = 98, shouldn't be necessary for now
};

enum struct PayPos { // Pay positions (for now)
    PAY1 = 5,
    PAY2 = 10,
    PAY3 = 19,
    PAY4 = 30,
    PAY5 = 36,
    PAY6 = 43,
    PAY7 = 51,
    PAY8 = 64,
    PAY9 = 74,
    PAY10 = 83,
    PAY11 = 89,
};

#endif