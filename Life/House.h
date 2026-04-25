#ifndef HOUSE_H
#define HOUSE_H

#include "Card.h"

class House : public Card {
    private:
    char name[32];
    int cost, sell1, sell2;
    public:

    House(const char* n, int c, int s1, int s2) : Card() {
        if (n) {
            std::strncpy(name, n, sizeof(name) - 1);
            name[sizeof(name) - 1] = '\0';
        } else {
            std::strcpy(name, "Null");
        }
        cost = c, sell1 = s1, sell2 = s2;}

    House() : Card() {
        std::strcpy(name, "Null");
        cost = 0;
        sell1 = 0;
        sell2 = 0;
    }
        
    // Copy Constructor
    House(const House& other) : Card(other) {
    std::strcpy(name, other.name);
    cost = other.cost;
    sell1 = other.sell1;
    sell2 = other.sell2;
    }

    // Polymorphism for making printing arrays easier (to check for errors)
    void display(std::ostream& COUT) const override {
        COUT << "House Name: " << name << std::endl;
        COUT << "Cost: " << cost << std::endl;
        COUT << "Low Sell: " << sell1 << std::endl;
        COUT << "High Sell: " << sell2 << std::endl;
    }

    // Manual set up for secondary file (Setter functions)
    void setName(const char* n) {
        std::strncpy(name, n, sizeof(name) - 1);
        name[sizeof(name) - 1] = '\0';
    }
    void setCost(int c) {cost = c;}
    void setS1(int s1) {sell1 = s1;}
    void setS2(int s2) {sell2 = s2;}

    // Getter functions
    const char *getName() const {return name;}
    int getCost() const {return cost;}
    int getS1() const {return sell1;}
    int getS2() const {return sell2;}

    /// @brief Read in House objects from binary files
    /// @param in binary file being read form 
    void hRead(std::ifstream &in) {
        in.read(name, sizeof(name));
        in.read(reinterpret_cast<char*>(&cost), sizeof(cost));
        in.read(reinterpret_cast<char*>(&sell1), sizeof(sell1));
        in.read(reinterpret_cast<char*>(&sell2), sizeof(sell2));
    }

    /// @brief Write House objects to binary files
    /// @param out binary file being written to
    void hWrite(std::ofstream &out) {
        out.write(name, sizeof(name));
        out.write(reinterpret_cast<char*>(&cost), sizeof(cost));
        out.write(reinterpret_cast<char*>(&sell1), sizeof(sell1));
        out.write(reinterpret_cast<char*>(&sell2), sizeof(sell2));
    }

    /// @brief Overload << operator to make it so that house info can be directly outputted via COUT
    /// @param COUT object to be outputted to
    /// @param house House Object
    /// @return COUT object that is being outputted to 
    friend std::ostream& operator<<(std::ostream& COUT, const House& house) {
        COUT << "Name: " << house.getName() << std::endl;
        COUT << "Cost: " << house.getCost() << std::endl;
        COUT << "Low Sell: " << house.getS1() << std::endl;
        COUT << "High Sell: " << house.getS2() << std::endl;
        return COUT;
    }
};

#endif