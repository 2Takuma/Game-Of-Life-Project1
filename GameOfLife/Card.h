#ifndef CARD_H
#define CARD_H

#include <string>
#include <cstring>
#include <fstream>

class Card {
    protected:
    bool owned;
    static int count; // Track the amount of cards created in the game (used for testing, not actual game)
    public:
    Card() {owned = false; ++count;}
    ~Card() {--count;}

    void setTrue() {owned = true;}
    void setFlse() {owned = false;}

    bool getBool() const {return owned;}
    static int getCt() {return count;} 

    // Virtual function to make this class abstract -> makes displaying arrays easier
    virtual void display(std::ostream& COUT) const = 0;
};

int Card::count = 0;

#endif