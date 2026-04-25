/* File:   GameOfLifeV1.cpp
 * Author: Keanu Arao
 * Created on April 2nd 2025 11:01 PM
 * Purpose:  Recreate the game of "Life" board game
 *           
 *      Main changes:
 *      worked on retirement function and fixing up code in general
 *      figuring out cin for having user enter to progress to add engagement
 *      putting the game together more so it can run
 *      figured out some reasons that could prevent code from running
 *      testing
 *      need to figure out file/array i/o fix
 *      finding more bugs to fix
 * 
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

// Structures -- Possibly put in header file
struct Player {
string name;
int money = 200, pos = 0, aCount = 0, stops = 0, kids = 0, paid = 0; // Player's Money and position on the "Board" + action card counter
int career, house = -1; // Index in their respective arrays --> Can change to nested structures
bool college = false, mrried = false;
};

struct Career{ 
    string title;
    int income;
};

struct House {
    string name;
    int cost, sell1, sell2;
};

struct ActCard {
    string event;
    int affect; // Positive or negative (add to players money or take away from it)
};

enum struct StpType { // 
    MARR = 13,
    KIDS = 45,
    NIGHT = 76,
    RET = 98
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
void job(Player &, Career *, Career *, int); // When user picks out their career at start of game or possibly adding the night school spot
void house(Player &, House *, int); // When user lands on house stop
void jobSlct(Player &, Career *, int);
void action(Player &, ActCard *, int); // When the user lands on an action position -- Could also use for vacation and other spots maybe?
void pay(Player &, Career *); // When user passes on pay position
void payChck(Player &, Career *, Career *, PayPos); // When user lands on pay position (or passes it)
void getMarr(Player &); // When user lands on marry stop WiP
void getKids(Player &); // When user lands on kids stop WiP
void retire(Player *, House *, int);
// Maybe Pets?

// Game setup and loop functions
Player *pCreate(int &, Career *, Career *, int); // DONE?
void route(Player &, char *);
void display(Player *); // Show stats at the end of the game
// Possible Functions: sell (for end) or retirement function, gameloop function, loan function

//Execution of Code Begins Here
int main(int argc, char** argv) {
    srand(static_cast<unsigned int>(time(0))); //Random seed set (for spinner , etc...)
    //Declare all variables for this function
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

    // Main game loop
    int roll, eCount; // counter for end game check

    do {
        eCount = 0; 
        for (int i = 0; i < pCount; i++){
            if (players[i].pos == bSize){ 
                continue; // Skip to next player
            }
        cout << players[i].name << "'s turn!" << endl;
        cout << "Tap enter to spin: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        roll = spin(); 
        cout << "You rolled a " << roll << endl;
        move(players[i], roll, bSize);
        stopChk(players[i], stpType);
        // Call function to determine what to do at that position
        payChck(players[i], careers, cc, payPos); // Call pay function (if they pass a pay position)
        if (board[players[i].pos] == 'A'){ // Action space
            action(players[i], actCard, 40); // Call action function
        }
        else if (board[players[i].pos] == 'H'){ // House space
            house(players[i], houses, 20); // Call house function
        }
        else if (board[players[i].pos] == 'M'){ // Marriage space
            getMarr(players[i]); // Call marriage function
        }
        else if (board[players[i].pos] == 'K'){ // Kids space
            getKids(players[i]); // Call kids function
        }
        else {cout << "Error" << endl;}
        }

        for (int i = 0; i < pCount; i++){ // Check to end game
            if (players[i].pos == bSize){
                eCount++;
            }
        }

    }while(eCount != pCount); // If all players are at the end of the board, end game

    cout << "=========================================" << endl;
    cout << "The game has finished!" << endl;
    cout << "=========================================" << endl;
    retire(players, houses, pCount);
    cout << "Would you like to see the results? (Y/N): "; // Possibly implement later if I feel like it 

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
// =========================GAME SETUP AND LOOP FUNCTIONS=========================
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

// ==============================================================================
// ===========================CORE GAME FUNCTIONS================================
// ==============================================================================

int spin() {
    return (rand() % 10) + 1; // ( rand() % range ) + 1 (because it goes from 0-9)
}

void move(Player &plyr, int roll, int bSize) {
    plyr.pos += roll;
    if (plyr.pos >= bSize) {
        plyr.pos = bSize -1;
    }
}

void stopChk(Player &plyr, StpType types) { // Check if player has landed on a stop (or passed it) and if they have, place them on the stop
    const int stops[] = {static_cast<int>(StpType::MARR), static_cast<int>(StpType::NIGHT), static_cast<int>(StpType::KIDS)}; // Stops array (for now)
    const int size = sizeof(stops) / sizeof(stops[0]);

    if (plyr.pos < size && plyr.pos >= stops[plyr.stops]){
        plyr.stops++;
        plyr.pos = stops[plyr.stops]; // Place them on the stop
    }
}

// Action function (when the user lands on an action position -- Could also use for vacation and other spots maybe?) 
void action(Player &plyr, ActCard *actArr, int actSize){ 
    plyr.aCount++;
    int act = rand() % actSize ;
    cout << "You have landed on an action space!" << endl;
    if (act >= 0 && act < actSize && !actArr[act].event.empty()) {
        cout << actArr[act].event << endl;
    } else {
        cout << "Invalid action card event." << endl;
    }
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
    cout << "You rolled a " << roll << endl;
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

void retire(Player *players, House *houses, int pSize) {
    for (int i = 0; i < pSize; i++){
        cout << "Retiring " << players[i].name << "..." << endl;
        if (players[i].mrried){ // Check if married
            players[i].money += 100;
        }
        if (players[i].kids > 0) { // Check for kids
            players[i].money += (players[i].kids * 100);
        }
        if (players[i].house != -1) { // House sell
            int roll = spin();
            cout << "Spin (Enter) to sell your house! (Odds = Low sell, Even = High Sell)" << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (roll % 2 == 0) {
                cout << "You rolled even! +" << houses[players[i].house].sell2 << "k!" << endl;
                players[i].money += houses[players[i].house].sell2;
            }
            else {
                cout << "You rolled odd! +" << houses[players[i].house].sell1 << "k!" << endl;
                players[i].money += houses[players[i].house].sell1;
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
    cout << "=======================================" << endl;
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
            cout << "=======================================" << endl;
        }
        else if (pick == 2) {
            player.career = id2;
            cout << "You have chosen: " << choice2.title << endl;
            cout << "=======================================" << endl;
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
        cout << "=========================================" << endl;
        if (pick == 1){
            if (plyr.money < arr[id1].cost) { // Check if player has enough money, kick em out if they don't
                cout << "You do not have enough money. Don't try to buy a house you can't afford loser" << endl;
                return;
            }
            plyr.house = id1;
            cout << "You bought the " << arr[id1].name << "!"<< endl;
        }
        else if (pick == 2){
            if (plyr.money < arr[id2].cost) {
                cout << "You do not have enough money. Don't try to buy a house you can't afford loser" << endl;
                return;
            }
            plyr.house = id2;
            cout << "You bought the " << arr[id2].name << "!"<< endl;
        }
        else if (pick == 0){
            cout << "You have chosen to not buy a house." << endl;
        }
        else {cout << "Please enter a valid response (0, 1 or 2)" << endl;} 
    } while ((pick != 1) && (pick != 2));
    cout << "=========================================" << endl;
}
