/* File:   GameOfLifeV1.cpp
 * Author: Keanu Arao
 * Created on April 2nd 2025 11:01 PM
 * Purpose:  Recreate the game of "Life" board game
 *           
 *          Main Changes:
 *              added pets, winner, results functions
 *              implemented nested array for houses in player struct
 *              updated functions in accordance to nested structs
 *              general reworks
 *  
 */
//System Libraries
#include <iostream>  //I/O Library
#include <string> // Strings
#include <ctime> // Time for rand
#include <cstdlib> // Srand to set seed
#include <fstream> // Binary Files
#include <iomanip> // setw, left/right --> later cuz i dont feel formatting yet
#include <limits> // For numeric limits
using namespace std;

// Structures -- Header file later
struct House { // Possibly implement so that players can have multiple houses
    char name[32];
    int cost, sell1, sell2;
};

struct Player {
string name;
int money = 200, pos = 0, aCount = 0, stops = 0,
     kids = 0, paid = 0, pets = 0; // Player stats
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

//Function Prototypes
// File I/O Functions
void fRead(string name, House *arr, int size); // Read in .dat file to array
void fRead(string name, Career *arr, int size);
void fRead(string name, ActCard *arr, int size);
void fRead(string name, char *arr, int size); // Read in board file to array (for future version)

// Core Game Functions
int spin(); // Roll from 1-10 DONE
void move(Player &, int, int); // Move player to designated position (based on roll) DONE
void stopChk(Player &, StpType); // Check if player has landed on a stop (or passed it) and if they have, place them on the stop
void job(Player &, Career *, Career *, int); 
void jobSlct(Player &, Career *, int); // For Job function
void house(Player &, House *, int); // When user lands on house stop
void action(Player &, ActCard *, int); // When the user lands on an action position -- Could also use for vacation and other spots maybe?
void pay(Player &, Career *); // When user passes on pay position
void payChck(Player &, Career *, Career *, PayPos); // When user lands on pay position (or passes it)
void getMarr(Player &); // When user lands on marry stop WiP
void getKids(Player &); // When user lands on kids stop WiP
void retire(Player *, House *, int);
void night(Player &, Career *, Career *, int);
void vaca();
void getPets(Player &);
// Maybe Pets & Vacations

// Game/Program Functions
Player *pCreate(int &, Career *, Career *, int); // DONE?
void route(Player &, char *, Career *, Career *, House *, ActCard *);
void winner(Player *, int);
void results(Player *, int); // Show stats at the end of the game
void store(); // Store winner data --> search up appending data
void winList(); // Show winners
// Possible Functions: sell (for end) or retirement function, gameloop function, loan function

//Execution of Code Begins Here
int main(int argc, char** argv) {
    //Random seed set (for spinner , etc...)
    srand(static_cast<unsigned int>(time(0)));

    //Declarization & Initialization 
    const int jList = 20, aList = 40, bSize = 100;
    int pCount;
    char board[bSize] = {};
    Career careers[jList] = {};
    Career cc[jList] = {};
    House houses[jList] = {};
    ActCard actCard[aList] = {};
    StpType stpType;
    PayPos payPos;
    
    fRead("Careers.dat", careers, jList);
    fRead("CollegeCareers.dat", cc, jList);
    fRead("Houses.dat", houses, 20);
    fRead("ActionCards.dat", actCard, aList); // Read in action cards to array
    fRead("Board.dat", board, bSize); // Read in board file to array 
    
    cout << "Enter the amount of players you want playing (Max of 4 players):"; cin >> pCount;
    
    while ((pCount > 4) || (pCount <= 0)) {
        cout << "Please enter a valid amount of players (1-4): "; 
        cin >> pCount;
    }

    Player *players = pCreate(pCount, cc, careers, jList); // Create players (name, money, etc...)
    cout << "=========================================" << endl;
    cout << "Game starting..." << endl;
    cin.ignore();

    // Main game loop
    int roll, eCount; // counter for end game check

    do {
        eCount = 0; 
        for (int i = 0; i < pCount; i++){
            if (players[i].pos == bSize - 2){ 
                eCount++;
                continue; // Skip to next player
            }
            cout << "=========================================" << endl;
            if (players[i].pos == bSize - 2) {
                continue;
            } 
            cout << players[i].name << "'s turn!" << endl;
            cout << "Tap enter to spin: ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            roll = spin(); 
            cout << "You rolled a(n) " << roll << endl;
            move(players[i], roll, bSize); // Move player
            stopChk(players[i], stpType); // Check to see if player passes/lands on stop
            payChck(players[i], careers, cc, payPos); // Call pay function (if they pass a pay position)
            cout << board[players[i].pos] << endl; 
            route(players[i], board, cc, careers, houses, actCard); // Determine what to do when player lands on a spot
        }
    }while(eCount != pCount); // If all players are at the end of the board, end game

    cout << "=========================================" << endl;
    cout << "The game has finished!" << endl;
    cout << "=========================================" << endl;
    retire(players, houses, pCount);
    winner(players, pCount);
    results(players, pCount);
    
    delete[] players;  //Clean up 
    //Exit stage right
    return 0;
}

//  Function Implementations
//  ==============================================================================
//  ===========================FIlE I/O FUNCTIONS=================================
//  ==============================================================================

void fRead(string name, House *arr, int size){ // House Reader
    ifstream input(name, ios::binary);
    if (!input){
        cout << "Error opening file";
        return;
    }
    input.read(reinterpret_cast<char*>(arr), size * sizeof(House));
    input.close();
}

void fRead(string name, Career *arr, int size){ // Career Reader
    ifstream input(name, ios::binary);
    if (!input){
        cout << "Error opening file";
        return;
    }
    input.read(reinterpret_cast<char*>(arr), size * sizeof(Career));
    input.close();
}

void fRead(string name, ActCard *arr, int size){// Action Card Reader
    ifstream input(name, ios::binary);
    if (!input){
        cout << "Error opening file";
        return;
    }
    input.read(reinterpret_cast<char*>(arr), size * sizeof(ActCard));
    input.close();
}

void fRead(string name, char *arr, int size){ // Board reader function
    ifstream input(name, ios::binary);
    if (!input){
        cout << "Error opening file";
        return;
    }
    input.read(reinterpret_cast<char*>(arr), size * sizeof(char));
    input.close();
}

// ===============================================================================
// ========================= GAME / PROGRAM FUNCTIONS ============================
// ===============================================================================

Player *pCreate(int &pCount, Career *cc, Career *careers, int jSize){ // Create player function (for the game itself) 
    string name;
    Player *arr = new Player[pCount]; 
    for (int i = 0; i < pCount; i++){
        cout << "Enter player " << i + 1 << "'s name: "; cin >> name;
        arr[i].name = name;
        cout << "Would you like to go to college? (Y/N): "; cin >> name;
        if (name == "Y" || name == "y"){
            arr[i].college = true;
            job(arr[i], careers, cc, jSize);
        }
        else {
            arr[i].college = false;
            job(arr[i], careers, cc, jSize);
        }
    }
    return arr;
}

void route(Player &player, char *board, Career *cc, Career *careers, House *houses, ActCard *actCard) {
    if (board[player.pos] == 'A'){ // Action space
        action(player, actCard, 40); // Call action function
    }
    else if (board[player.pos] == 'P') {
        return;
    }
    else if (board[player.pos] == 'H'){ // House space
        house(player, houses, 20); // Call house function
    }
    else if (board[player.pos] == 'M'){ // Marriage space
        getMarr(player); // Call marriage function
    }
    else if (board[player.pos] == 'C') {
        getPets(player);
    }
    else if (board[player.pos] == 'K'){ // Kids space
        getKids(player); // Call kids function
    }
    else if (board[player.pos] == 'N') { // Night School
        char pick;
        cout << "-----------------------------------------" << endl;
        cout << "Would you like to go to night school? (Y/N): "; cin >> pick;
        if (pick == 'Y' || pick == 'y') {
            player.college = true;
            job(player, careers, cc, 20);
        }
        else {
            cout << "You have decided to not go to night school." << endl;
            return;
        }
    }
    else if (board[player.pos] == 'R'){
        cout << "You are retired." << endl;
        return;
    }
    else {cout << "Error" << endl;}
}

void winner(Player *players, int pSize) {
    int most = -1, mostId;
    bool tie = false;
    for (int i = 0; i < pSize; i++) { 
        if (players[i].money > most) {
            most = players[i].money;
            mostId = i;
            tie = false;
        }
        else if (players[i].money == most) {
            tie = true;
        }
    }
    if (tie) {
        cout << "There was a tie!" << endl;
    }
    else {
        players[mostId].winner = true;
    }
}

void results(Player *players, int pSize){
    char pick;
    cout << "===========================================" << endl;
    cout << "Would you like to see the results? (Y/N): "; cin >> pick;
    if (pick == 'Y' || pick == 'y'){
        cout << "-------------------------------------------" << endl;
        for (int i = 0; i < pSize; i++){ 
        cout << players[i].name << "'s Stats: " << endl;
        cout << "Money: " << players[i].money << endl;
        cout << "Kids: " << players[i].kids << endl;
        cout << "Pets: " << players[i].pets << endl;
        cout << "-------------------------------------------" << endl;
        } 
    }
    else {cout << "You have chosen to not see the results." << endl;}
}

// ==============================================================================
// ===========================CORE GAME FUNCTIONS================================
// ==============================================================================

int spin() {
    return (rand() % 10) + 1; // ( rand() % range ) + 1 (because it goes from 0-9)
}

void move(Player &plyr, int roll, int bSize) {
    plyr.pos += roll;
    if (plyr.pos >= bSize - 2) {
        plyr.pos = bSize - 2;
    }
}

void stopChk(Player &plyr, StpType types) { // Check if player has landed on a stop (or passed it) and if they have, place them on the stop
    const int stops[] = {static_cast<int>(StpType::MARR), static_cast<int>(StpType::KIDS), static_cast<int>(StpType::NIGHT)}; // Stops array (for now)
    const int size = sizeof(stops) / sizeof(stops[0]);

    if (plyr.stops < size && plyr.pos >= stops[plyr.stops]){
        plyr.pos = stops[plyr.stops]; // Place them on the stop
        plyr.stops++;
    }
}

// Action function (when the user lands on an action position -- Could also use for vacation and other spots maybe?) 
void action(Player &plyr, ActCard *actArr, int actSize){ 
    plyr.aCount++;
    int act = rand() % actSize ;
    cout << "You have landed on an action space!" << endl;
    cout << actArr[act].event << endl;
    plyr.money += actArr[act].affect; // Add or subtract money from player
    if (plyr.money < 0) {
        plyr.money = 0; // Ensure money does not go below zero
    }
    cout << "You now have " << plyr.money << "K" << endl;
}

// Player pay function
void pay(Player &plyr, Career *career){
    cout << "You have passed a pay position!" << endl;
    plyr.money += career[plyr.career].income; // Add money to player
    cout << "You now have " << plyr.money << "K" << endl;
}

// Player pay check function
void payChck(Player &player, Career *careers, Career * cc, PayPos places){ 
    const int payPos[] = {static_cast<int>(PayPos::PAY1), static_cast<int>(PayPos::PAY2), static_cast<int>(PayPos::PAY3), 
                          static_cast<int>(PayPos::PAY4), static_cast<int>(PayPos::PAY5), static_cast<int>(PayPos::PAY6), 
                          static_cast<int>(PayPos::PAY7), static_cast<int>(PayPos::PAY8), static_cast<int>(PayPos::PAY9), 
                          static_cast<int>(PayPos::PAY10), static_cast<int>(PayPos::PAY11)};
    const int size = sizeof(payPos) / sizeof(payPos[0]);
    if (player.paid < size && player.pos >= payPos[player.paid]) {
        if (player.college){
            pay(player, cc);
        }
        else {pay(player, careers);}
        player.paid++;
    }
}

void getMarr(Player &plyr){ // Possibly implement spin for cash from everyone (would involve adding player array to parameter list)
    cout << "You have landed on the marriage STOP!" << endl;
    plyr.mrried = true; 
    plyr.money += 100;
    cout << "You are now married! You recieve 100K as a reward! (Likely to change)" << endl;
}

void getKids(Player &plyr){ // Finish later
    cout << "You have landed on the kids STOP! Spin (Enter) for kids!" << endl;
    cout << ">";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    int roll = spin();
    cout << "You rolled a(n) " << roll << endl;
    if (roll <= 3) {
        cout << "You get no kids!" << endl;
    } else if (roll <= 6) {
        cout << "You have 1 kid!" << endl;
        plyr.kids = 1;
    } else if (roll <= 8) {
        cout << "You have twins!" << endl;
        plyr.kids = 2;
    } else {
        cout << "You have triplets!" << endl;
        plyr.kids = 3;
    }
}

void getPets(Player & plyr) {
    cout << "You have landed on the pets STOP! Spin (Enter) for Pets!" << endl;
    cout << ">";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    int roll = spin();
    cout << "You rolled a(n) " << roll << endl;
    if (roll <= 3) {
        cout << "You get no Pets!" << endl;
    } else if (roll <= 6) {
        cout << "You have 1 pet!" << endl;
        plyr.pets = 1;
    } else if (roll <= 8) {
        cout << "You have 2 pets!" << endl;
        plyr.pets = 2;
    } else {
        cout << "You have 3 pets!" << endl;
        plyr.pets = 3;
    }
}

void retire(Player *players, House *houses, int pSize) {
    for (int i = 0; i < pSize; i++){
        cout << "Retiring " << players[i].name << "..." << endl;
        if (players[i].mrried){ // Check if married
            players[i].money += 100;
        }
        if (players[i].kids > 0) { // Check for kids
            players[i].money += (players[i].kids * 100);
        }
        if (players[i].pets > 0) { // Check for kids
            players[i].money += (players[i].pets * 100);
        }
        if (players[i].house != -1) { // House sell
            for (int j = 0; j < 5; j++) {
                if (players[i].hIds[j] != -1){      // SELL HOUSE FUNCTION
                    int roll = spin();
                    cout << "Spin (Enter) to sell your house(s)! (Odds = Low sell, Even = High Sell)" << endl;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    if (roll % 2 == 0) {
                        cout << "You rolled even! +" << players[i].hOwned[j].sell2 << "k!" << endl;
                        players[i].money += players[i].hOwned[j].sell2;
                    }
                    else {
                        cout << "You rolled odd! +" << players[i].hOwned[j].sell1 << "k!" << endl;
                        players[i].money += players[i].hOwned[j].sell1;
                    }
                }
                else {break;}
            }
        }
        for (int j = 0; j < players[i].aCount; j++){ // Give 100 per action card
            players[i].money += 100;
        }
    }
}

// Job function
void job(Player &player, Career *carArr, Career *colArr, int jSize) { 
     // Temporary max, future note: change every 20 if you change it
    if(player.college){
        jobSlct(player, colArr, jSize);
    }
    else{
        jobSlct(player, carArr, jSize);
    }
}

// Job Select
void jobSlct(Player &player, Career *arr, int size){ // Change to where it takes in the career/collegecareer array as a parameter instead in version 2 (should be faster, but keep this for learning experience)
    Career choice1, choice2;
    int id1 = rand() % size, id2 = rand() % size;
    choice1 = arr[id1];
    choice2 = arr[id2];

    int pick;
    cout << "=========================================" << endl;
    cout << "Pick one of the two careers: " << endl;
    cout << "Job 1: " << choice1.title << endl;
    cout << "Salary: " << choice1.income << 'k' << endl;
    cout << "Job 2: " << choice2.title << endl;
    cout << "Salary: " << choice2.income << 'k' << endl;
    cout << "Enter 1 or 2 in accordance to the job:" << endl;
    cin >> pick;

    do {
        if (pick == 1) {
            player.career = id1;
            cout << "You have chosen: " << choice1.title << endl;
            cout << "=========================================" << endl;
        }
        else if (pick == 2) {
            player.career = id2;
            cout << "You have chosen: " << choice2.title << endl;
            cout << "=========================================" << endl;
        }   
        else {cout << "Please enter a valid response (1 or 2)" << endl;}  
    } while((pick != 1) && (pick != 2));
}

// House buy function
void house(Player &plyr, House *arr, int size){
    House choice1, choice2;
    int pick, id1 = rand() % size, id2 = rand() % size;
    while (id1 == id2){
        id2 = rand() % size;
    }
    choice1 = arr[id1];
    choice2 = arr[id2];

    do {
        cout << "=========================================" << endl;
        cout << "Please select the House you want to buy." << endl;
        cout << "(Enter 0 to cancel)" << endl;
        cout << "House 1: " << choice1.name << endl;
        cout << "Cost: " << choice1.cost << endl;
        cout << "Resell 1: " << choice1.sell1 << endl;
        cout << "Resell 2: " << choice1.sell2 <<endl;
        cout << "-----------------------------------------" << endl;
        cout << "House 2: " << choice2.name  << endl;
        cout << "Cost: " << choice2.cost << endl;
        cout << "Resell 1: " << choice2.sell1 << endl;
        cout << "Resell 2: " << choice2.sell2 << endl;
        cout << ">"; cin >> pick;
        if (pick == 1){
            if (plyr.money < arr[id1].cost) { // Check if player has enough money, kick em out if they don't
                cout << "You do not have enough money. Don't try to buy a house you can't afford loser" << endl;
                return;
            }
            for (int i = 0; i < 5; i++){
                if (plyr.hIds[i] == -1) {
                    plyr.hIds[i] = id1;
                    plyr.hOwned[i] = choice1;
                    break;
                }
            }
            cout << "You bought the " << arr[id1].name << "!"<< endl;
        }
        else if (pick == 2){
            if (plyr.money < arr[id2].cost) {
                cout << "You do not have enough money. Don't try to buy a house you can't afford loser" << endl;
                return;
            }
            for (int i = 0; i < 5; i++){
                if (plyr.hIds[i] == -1) {
                    plyr.hIds[i] = id2;
                    plyr.hOwned[i] = choice2;
                    break;
                }
            }
            cout << "You bought the " << arr[id2].name << "!"<< endl;
        }
        else if (pick == 0){
            cout << "You have chosen to not buy a house." << endl;
            return;
        }
        else {cout << "Please enter a valid response (0, 1 or 2)" << endl;} 
    } while ((pick != 1) && (pick != 2));
    cout << "=========================================" << endl;
}
