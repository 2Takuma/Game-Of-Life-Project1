/* 
 * File:   GameOfLifeV2.cpp
 * Author: Keanu Arao
 * Created on May 12 2025 12:01 AM
 * 
 *              Main Focus:
 *                  transporting some of the functions over from original game and making them work with classes
 *                  working on all of the header files
 *                  brainstorming in header files
 */

//Include statements
#include <iostream>
#include <string> // Strings
#include <ctime> // Time for rand
#include <cstdlib> // Srand to set seed
#include <fstream> // Binary Files
#include <limits> // For numeric limits
// Header File inclusion
#include "Player.h"
#include "Career.h"
#include "Action.h"
#include "House.h"
#include "GameStructs.h"

//Function Prototypes
int spin();
void pay(Player &);
void getMarr(Player &, Player* , int);
void getKids(Player &);
void stopChk(Player &, StpType);
void payChck(Player &, PayPos);

void winner(Player *, int);
Player *pCreate(int &, Career *, Career *, int);
void results(Player *, int);

int main(){

   return 0;
}

// Function Implementations

// ----------------------------------------------------
//               Program Game Functions
// ----------------------------------------------------
Player *pCreate(int &pCount, Career *cc, Career *careers, int jSize){ // Player create function
    string name;
    char in;
    Player *arr = new Player[pCount]; 
    for (int i = 0; i < pCount; i++){
        cout << "Enter player " << i + 1 << "'s name: "; cin >> name;
        arr[i].setName(name);
        cout << "Would you like to go to college? (Y/N): "; cin >> in;
        if (in == 'Y' || in == 'y'){
            arr[i].setCol();
        }
        job(arr[i], careers, cc, jSize);
    }
    return arr;
}

void winner(Player *players, int pSize) {
    int most = -1, mostId;
    bool tie = false;
    for (int i = 0; i < pSize; i++) { 
        if (players[i].getMny() > most) {
            most = players[i].getMny();
            mostId = i;
            tie = false;
        }
        else if (players[i].getMny() == most) {
            tie = true;
        }
    }
    if (tie) {
        cout << "There was a tie!" << endl;
    }
    else {
        players[mostId].setWin();
        cout << "The Winner is " << players[mostId].getName() << "!" << endl;
    }
}

void results(Player *players, int pSize){
    char pick;
    cout << "===========================================" << endl;
    cout << "Would you like to see the results? (Y/N): "; cin >> pick;
    if (pick == 'Y' || pick == 'y'){
        cout << "-------------------------------------------" << endl;
        for (int i = 0; i < pSize; i++){ 
        cout << players[i] << endl;
        cout << "Action cards receieved: " << players[i].getAC() << endl;
        cout << "Vacations Taken: " << players[i].getVC() << endl;
        cout << "Amount of Kids: " << players[i].getKids() << endl;
        cout << "-------------------------------------------" << endl;
        } 
    }
    else {cout << "You have chosen to not see the results." << endl;}
    cin.ignore();
}

// ----------------------------------------------------
//               Core/Board Game Functions
// ----------------------------------------------------
int spin() {
    return (rand() % 10) + 1;
}

void pay(Player &player){
    cout << "You have passed a pay position!" << endl;
    player.addMny(player.career.getInc);
    cout << "You now have " << plyr.getMny() << "K" << endl;
}

void stopChk(Player &plyr, StpType types) { // Check if player has landed on a stop (or passed it) and if they have, place them on the stop
    const int stops[] = {static_cast<int>(StpType::MARR),static_cast<int>(StpType::PETS),
                         static_cast<int>(StpType::KIDS), static_cast<int>(StpType::NIGHT)}; // Stops array (for now)
    const int size = sizeof(stops) / sizeof(stops[0]);

    if (plyr.getStps() < size && plyr.getPos() >= stops[plyr.getStps()]){
        plyr.setPos(stops[plyr.getStps]); // Place them on the stop
        plyr.addStop;
    }
}

void payChck(Player &player, PayPos places){ 
    const int payPos[] = {static_cast<int>(PayPos::PAY1), static_cast<int>(PayPos::PAY2), static_cast<int>(PayPos::PAY3), 
                          static_cast<int>(PayPos::PAY4), static_cast<int>(PayPos::PAY5), static_cast<int>(PayPos::PAY6), 
                          static_cast<int>(PayPos::PAY7), static_cast<int>(PayPos::PAY8), static_cast<int>(PayPos::PAY9), 
                          static_cast<int>(PayPos::PAY10), static_cast<int>(PayPos::PAY11)};
    const int size = sizeof(payPos) / sizeof(payPos[0]);
    if (player.paidNum() < size && player.getPos() >= payPos[player.paidNum()]) {
        pay(player);
        player.addPaid();
    }
}

void getMarr(Player &plyr, Player* players, int pCount){
    cout << "You have landed on the marriage STOP!" << endl;
    plyr.setMrr(); 
    for (int i = 0; i < pCount; i++) {
         if (players[i].getName() != plyr.getName()) {
            players[i].addMny(-100);
            plyr.addMny(100);
         }
    }
    cout << "You are now married! You receive 100K from all the other players!" << endl;
}

void getKids(Player &plyr){
    cout << "You have landed on the kids STOP! Spin (Enter) for kids!" << endl;
    cout << ">";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    int roll = spin();
    cout << "You rolled a(n) " << roll << endl;
    if (roll <= 3) {
        cout << "You get no kids!" << endl;
    } else if (roll <= 6) {
        cout << "You have 1 kid!" << endl;
        plyr.setKids(1);
    } else if (roll <= 8) {
        cout << "You have twins!" << endl;
        plyr.setKids(2);
    } else {
        cout << "You have triplets!" << endl;
        plyr.setKids(3);
    }
}