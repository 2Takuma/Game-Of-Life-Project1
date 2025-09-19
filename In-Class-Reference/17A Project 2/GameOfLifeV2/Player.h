#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include "Career.h"
#include "House.h"
#include "Pet.h"
#include "Card.h"

class Player {
    private:// Player stats
        std::string name;
        static int turns; // Static data member
        int money = 200, pos = 0, aCount = 0, stops = 0,
            kids = 0, paid = 0, vCount = 0; 
        bool college = false, mrried = false, winner = false;
        Career career;
        House houses[5];
        Pet pet;
        // A vector of all the cards the player owns
        std::vector<std::unique_ptr<Card>> cards;
    public:
    Player(std::string n, bool c) {name = n; college = c;}
    Player() {name = "Null";}

    // Copy Constructor Type 1
    Player(const Player& other)
        : name(other.name),
          money(other.money),
          pos(other.pos),
          aCount(other.aCount),
          stops(other.stops),
          kids(other.kids),
          paid(other.paid),
          vCount(other.vCount),
          college(other.college),
          mrried(other.mrried),
          winner(other.winner),
          career(other.career),
          pet(other.pet)
    {
        for (int i = 0; i < 5; ++i) {
            houses[i] = other.houses[i];
        }
    }

    // Setter Functions
    void setName(std::string n); // Class Specification
    void setCar(Career c) {career = c;}
    void addMny(int m) {money += m;}
    void move(int r, int max) {
        pos += r;
        if (pos >= max) {
            pos = max;
        }
        ++turns;
    }
    void setPos(int p) {pos = p;}
    void addA() {aCount += 1;}
    void addV() {vCount += 1;}
    void setCol() {college = true;}
    void setMrr() {mrried = true;}
    void setWin() {winner = true;}
    void setKids(int k) {kids = k;}
    void addStop() {stops += 1;}
    void addPaid() {paid += 1;}
    void setMny(int m) {money = m;}
    void setH(const House *h) {
        for (int i = 0; i < 5; i++) {
            houses[i] = h[i];
        }
    }
    void setPet(Pet p) {pet = p;}
    // Add/Remove card to/from vector
    void addCard(std::unique_ptr<Card> card) {
        cards.push_back(std::move(card));
    }

    // Remove the first card of a card type (Use for Night School)
    template<typename T>
    void rmvCard() { 
    auto it = std::find_if(cards.begin(), cards.end(),
        [](const std::unique_ptr<Card>& card) {
            return dynamic_cast<T*>(card.get()) != nullptr;
        });

        if (it != cards.end()) {
            cards.erase(it);
        }
    }

    // Getter Functions
    std::string getName() const {return name;}
    int getMny() const; // Class Specification
    int getPos() const {return pos;}
    int getAC() const {return aCount;}
    int getVC() const {return vCount;}
    int getKids() const {return kids;}
    int getStps() const {return stops;}
    int paidNum() const {return paid;}
    bool getCol() const {return college;}
    bool getMrr() const {return mrried;}
    bool getWin() const {return winner;} 
    Career getCar() const {return career;}
    House *getH() {return houses;}
    Pet getPet() {return pet;}
    int getTurns() {return turns;}
    //Display all the Cards the player owns
    void getCrds() const {
        for (const auto& card : cards) {
            card->display(std::cout);
        }
    }

    /// @brief Overload << operator so that Players can just be outputted by cout
    /// @param COUT cout object
    /// @param player Player to output
    /// @return cout object that Player info was outputted into
    friend std::ostream& operator<<(std::ostream& COUT, const Player& player) {
        COUT << "Player name: " << player.getName() << std::endl;
        COUT << "Money: " << player.getMny() << std::endl;
        COUT << "Career: " << player.career.getTi() << std::endl;
        COUT << "Action cards receieved: " << player.getAC() << std::endl;
        COUT << "Vacations Taken: " << player.getVC() << std::endl;
        COUT << "Amount of Kids: " << player.getKids() << std::endl;
        return COUT;
    }

    /// @brief Overload + operator so that Player info can be added to show total results at end of game
    /// @param a Player object A
    /// @param b Player object B
    /// @return The result of adding the info from object A and B
    friend Player operator+(const Player& a, const Player& b){
        Player result = a;
        result.money += b.money;
        result.aCount += b.aCount;
        result.vCount += b.vCount;
        result.kids += b.kids;
        return result; 
    }

    /// @brief Overload == operator so that Player names can be compared easily
    /// @param a Player object A
    /// @param b Player object B
    /// @return The boolean determining whether or not the names are equal or not
    friend bool operator==(const Player& a, const Player& b) {
        return a.getName() == b.getName();
    }

    /// @brief Overload != operator so that Player names can be compared easily
    /// @param a Player object A
    /// @param b Player object B
    /// @return The boolean determining whether or not the names are NOT equal or not (equal)
    friend bool operator!=(const Player& a, const Player& b) {
        return a.getName() != b.getName();
    }

    /// @brief Overload = operator so that Players can be copied by using '='
    /// @param other Player object to be copied
    /// @return THIS player object (the one that is copying the other)
    Player& operator=(const Player& other) {
        if (this != &other) {
            name = other.name;
            money = other.money;
            pos = other.pos;
            aCount = other.aCount;
            stops = other.stops;
            kids = other.kids;
            paid = other.paid;
            vCount = other.vCount;
            college = other.college;
            mrried = other.mrried;
            winner = other.winner;
            career = other.career;
            pet = other.pet;
            for (int i = 0; i < 5; ++i) {
                houses[i] = other.houses[i];
            }
        }
        return *this;
    }
};

int Player::turns = 0;

#endif