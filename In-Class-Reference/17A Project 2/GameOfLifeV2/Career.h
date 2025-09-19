#ifndef CAREER_H
#define CAREER_H

#include <cstring>
#include "Card.h"

class Career : public Card {
    private:
    char title[32];
    int income;

    public:
    Career(const char t[32], int i) : Card() {std::strcpy(title, t), income = i;}
    Career() : Card() {strcpy(title, "Null"), income = 0;}

    // Copy Constructor Type 2
    Career(const Career& other) {
        strcpy(title, other.getTi());
        income = other.getInc();
    }

    // Polymorphism for making printing arrays easier (to check for errors)
    void display(std::ostream& COUT) const override {
        COUT << "Title: " << title  << std::endl;
        COUT << "Income: " << income << std::endl;
    }

    // Manual set up for secondary file (Setter functions)
    void setTi(const char t[32]) {std::strcpy(title, t);}
    void setInc(int i) {income = i;}
    
    // Getter functions
    const char* getTi() const {return title;}
    int getInc() const {return income;}

    /// @brief Read Career Objects from binary files
    /// @param in Binary file being read from
    void cRead(std::ifstream &in) {
        in.read(title, sizeof(title));
        in.read(reinterpret_cast<char*>(&income), sizeof(income));
    }

    /// @brief Write Career Objects to binary files
    /// @param out The binary files being written to
    void cWrite(std::ofstream &out) {
        out.write(title, sizeof(title));
        out.write(reinterpret_cast<char*>(&income), sizeof(income));
    }

    /// @brief Overload << so that Career Object can be directly used for cout to display its title and income
    /// @param COUT object that is being outputted to
    /// @param career Career Object
    /// @return COUT object that was being outputted to
    friend std::ostream& operator<<(std::ostream& COUT, const Career& career) {
        COUT << "Title: " << career.getTi() << std::endl;
        COUT << "Income: " << career.getInc() << std::endl;
        return COUT;
    }
};


#endif