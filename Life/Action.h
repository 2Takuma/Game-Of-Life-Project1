#ifndef ACTION_H
#define ACTION_H

#include <cstring>
#include <fstream>
#include "Card.h"

class Action : public Card {
    private:
    char eDesc[256];
    int affect;
    public:
    Action(const char* e, int a) : Card() {
        if (e) {
            std::strncpy(eDesc, e, sizeof(eDesc) - 1);
            eDesc[sizeof(eDesc) - 1] = '\0';
        } else {
            std::strcpy(eDesc, "Null");
        }
        affect = a;
    }
    Action() : Card() { std::strcpy(eDesc, "Null"); affect = 0; }
    // Copy constructor Type 1
    Action(const Action& other) : Card(other) {
        std::strncpy(eDesc, other.eDesc, sizeof(eDesc) - 1);
        eDesc[sizeof(eDesc) - 1] = '\0';
        affect = other.affect;
    }

    // Getter functions
    const char* getE() const { return eDesc; }
    int getAff() const { return affect; }

    // Polymorphism for making printing arrays easier (to check for errors)
    void display(std::ostream& COUT) const override {
        COUT << "Action Card: " << eDesc  << std::endl;
        COUT << "Effect: " << affect << std::endl;
    }

    // Manual set up for secondary file (Setter functions)
    void setE(const char* e){
        std::strncpy(eDesc, e, sizeof(eDesc) - 1);
    }
    void setA(int a){affect = a;}

    /// @brief Read in Action Card Object from binary file
    /// @param in Binary file being read from
    void aRead(std::ifstream &in) {
        in.read(eDesc, sizeof(eDesc));
        in.read(reinterpret_cast<char*>(&affect), sizeof(affect));
    }

    /// @brief Write Action Card Objects to binary files
    /// @param out binary file being written to
    void aWrite(std::ofstream &out) {
        out.write(eDesc, sizeof(eDesc));
        out.write(reinterpret_cast<char*>(&affect), sizeof(affect));
    }
};

#endif