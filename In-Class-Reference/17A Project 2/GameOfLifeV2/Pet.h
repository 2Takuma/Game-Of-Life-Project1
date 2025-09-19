#ifndef PET_H
#define PET_H

#include "Card.h"
#include "Player.h"
#include <string>
#include <cstring>
#include <ctime>
#include <cstring>
#include <fstream>
#include <iostream>

class Pet : public Card {
    private:
        std::string name;
        int posAff, negAff;
        char posStr[256], negStr[256];
    public:
        Pet(std::string n, int pa, int na, const char* p, const char* neg) : Card(){
            name = n, posAff = pa, negAff = na;
            if (p) {
            std::strncpy(posStr, p, sizeof(posStr) - 1);
            posStr[sizeof(posStr) - 1] = '\0';
            }
            else {
            std::strcpy(posStr, "Null");
            }
            if (neg) {
            std::strncpy(negStr, neg, sizeof(negStr) - 1);
            negStr[sizeof(negStr) - 1] = '\0';
            }
            else {
            std::strcpy(negStr, "Null");
            }
        }
        Pet() : Card() {
            name = "Null", posAff = 0, negAff = 0;
            std::strcpy(posStr, "Null");
            std::strcpy(negStr, "Null");
        }
        // Copy constructor Type 1
        Pet(const Pet& other) : Card(other) {
            name = other.name;
            posAff = other.posAff;
            negAff = other.negAff;
            std::strncpy(posStr, other.posStr, sizeof(posStr) - 1);
            posStr[sizeof(posStr) - 1] = '\0';
            std::strncpy(negStr, other.negStr, sizeof(negStr) - 1);
            negStr[sizeof(negStr) - 1] = '\0';
        }

        /// @brief Read Pet Object from binary files
        /// @param in the binary file being read from
        void pRead(std::ifstream &in) {
            char nameBuf[256];
            in.read(nameBuf, sizeof(nameBuf));
            nameBuf[sizeof(nameBuf) - 1] = '\0';
            name = nameBuf;
            in.read(reinterpret_cast<char*>(&posAff), sizeof(posAff));
            in.read(reinterpret_cast<char*>(&negAff), sizeof(negAff));
            in.read(posStr, sizeof(posStr));
            posStr[sizeof(posStr) - 1] = '\0';
            in.read(negStr, sizeof(negStr));
            negStr[sizeof(negStr) - 1] = '\0';
        }

        /// @brief Write Pet Objects to binary files
        /// @param out the binary file being written to
        void pWrite(std::ofstream &out) {
            char nameBuf[256] = {0};
            std::strncpy(nameBuf, name.c_str(), sizeof(nameBuf) - 1);
            out.write(nameBuf, sizeof(nameBuf));
            out.write(reinterpret_cast<char*>(&posAff), sizeof(posAff));
            out.write(reinterpret_cast<char*>(&negAff), sizeof(negAff));
            out.write(posStr, sizeof(posStr));
            out.write(negStr, sizeof(negStr));
        }

        // Getter Functions
        std::string getName() const {return name;}
        int getPA() const {return posAff;}
        int getNA() const {return negAff;}
        const char* getPStr() const {return posStr;}
        const char* getNStr() const {return negStr;}

        // Polymorphism for making printing arrays easier (to check for errors)
        void display(std::ostream& COUT) const override {
            COUT << "Pet Name: " << name  << std::endl;
            COUT << "Positive Affect: " << posAff << std::endl;
            COUT << "Negative Affect: " << negAff  << std::endl;
            COUT << "Positive Event: " << posStr << std::endl;
            COUT << "Negative Event: " << negStr  << std::endl;
        }
        
        // Manual set up for secondary file (Setter function)
        void setName(std::string n) {name = n;}
        void setPA(int pa) {posAff = pa;}
        void setNA(int na) {negAff = na;}
        void setPStr(const char ps[256]) {std::strcpy(posStr, ps);}
        void setNStr(const char ns[256]) {std::strcpy(negStr, ns);}

        /// @brief (For each turn the player takes after obtaining a pet) Make the pet have a chance todo a positive or 
        ///        negative thing and make it affect the player's money 
        /// @return The positive/negative affect and event
        int randAff() {
            // Each affect/event has 1/10 chance
            int chance1 = (rand() % 10) + 1;
            int chance2 = (rand() % 10) + 1;

            if (chance1 == 1) {
                    std::cout << getPStr() << std::endl;
                    return getPA();
            }
            if (chance2 == 10) {
                    std::cout << getNStr() << std::endl;
                    return getNA();
            }
            return 0;
        }

        /// @brief Overload << Operator to be able to just cout pet name when cout << pet object (Im starting to realize I suck at these briefs I am sorry not sorry)
        /// @param COUT cout object to output into
        /// @param pet Pet Object
        /// @return COUT object that pet object has been outputted into
        friend std::ostream& operator<<(std::ostream& COUT, const Pet& pet) {
        COUT << "Name: " << pet.getName() << std::endl;
        return COUT;
        }
        
 };
#endif