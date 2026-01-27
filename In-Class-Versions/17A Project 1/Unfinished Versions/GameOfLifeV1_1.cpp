/* File:   GameOfLifeV1.cpp
 * Author: Keanu Arao
 * Created on April 2nd 2025 11:01 PM
 * Purpose:  Unfinished version of the main project (This version was focused more on fixing bugs and organizing program)
 *           
 * Notes:
 *      Psuedo Code and Flowchart need to still be done + Write Up including both
 *      Current: Continue to work on project
 */

//System Libraries
#include <iostream>  //I/O Library
#include <string> // Strings
#include <ctime> // Time for rand
#include <cstdlib> // Srand to set seed
#include <fstream> // Binary Files
#include <iomanip> // setw, left/right --> later cuz i dont feel formatting yet
using namespace std;

// Structures -- Possibly put in header file
struct Player {
string name;
int money = 200, pos = 0, aCount = 0, stops = 0, kids = 0, paid = 0; // Player's Money and position on the "Board" + action card counter
int career, house; // Index in their respective arrays --> Can change to nested structures
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

//Function Prototypes
// File I/O Functions
void fRead(string name, House *arr, int size); // Read in .dat file to array
void fRead(string name, Career *arr, int size);
void fRead(string name, ActCard *arr, int size);
void fRead(string name, char *arr, int size); // Read in board file to array (for future version)

// Core Game Functions
int spin(); // Roll from 1-10 DONE
void move(Player &, int, int); // Move player to designated position (based on roll) DONE
void stopChk(Player &); // Check if player has landed on a stop (or passed it) and if they have, place them on the stop
void job(Player &, Career *, Career *); // When user picks out their career at start of game or possibly adding the night school spot
void house(Player &, House *, int); // When user lands on house stop
void jobSlct(Player &, Career *, int);
void action(Player &, ActCard *, int); // When the user lands on an action position -- Could also use for vacation and other spots maybe?
void pay(Player &, Career *); // When user passes on pay position
void payChck(Player &); // When user lands on pay position (or passes it)
void getMarr(Player &); // When user lands on marry stop WiP
void getKids(Player &); // When user lands on kids stop WiP

// Game setup and loop functions
Player *pCreate(int &); // DONE?
void route(Player &, char *);
void gmeLoop(Player *); // Game loop function (for the game itself) <-- could also be used for the action function
void display(Player *); // Show stats at the end of the game
// Possible Functions: sell (for end) or retirement function, gameloop function, loan function

//Execution of Code Begins Here
int main(int argc, char** argv) {
    srand(static_cast<unsigned int>(time(0))); //Random seed set (for spinner , etc...)
    //Declare all variables for this function
    const int jList = 20, aList = 40;
    int pCount;
    Career careers[jList] = {};
    Career cc[jList] = {};
    House houses[jList] = {};
    ActCard actCard[aList] = {};
    char board[100] = {};
    fRead("Careers.dat", careers, jList);
    fRead("CollegeCareers.dat", cc, jList);
    fRead("Houses.dat", houses, 20);
    fRead("ActionCards.dat", actCard, aList); // Read in action cards to array

    //fRead("Board.dat", board, 50); // Read in board file to array (for future version)
    
    cout << "Enter the amount of players you want playing (Max of 4 players):";
    
    cin >> pCount;
    if (!(pCount <= 4) || (pCount < 0)){
        cout << "Please enter a valid amount."; // Can change to where it gives the user the ability to re-enter a number for the playercount 
        return 1;
    }
    Player *players = new Player[pCount]; // Create player array (dynamic)
    players = pCreate(pCount); // Create players (name, money, etc...)

  
    //Clean up the code, close files, deallocate memory, etc....
    
    delete[] players; 
    //Exit stage right
    return 0;
}

//Function Implementations
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

void gmeLoop(Player *plyr, ActCard *actList, char *board, House *hArr, int pCount, int bSize){ // Game loop function (for the game itself)
    char input;
    int roll; // counter for endG game check
    int eCount = 0;
    do {
        eCount = 0; 
        for (int i = 0; i < pCount; i++){
            if (plyr[i].pos == bSize){ 
                continue; // Skip to next player
            }
        cout << plyr[i].name << "'s turn!" << endl;
        cout << "Tap enter to spin: "; cin >> input;
        roll = spin(); 
        cout << "You rolled a " << roll << endl;
        move(plyr[i], roll, bSize);
        // Call function to determine what to do at that position (maybe)
        payChck(plyr[i]); // Call pay function (if they pass a pay position)
        if (board[plyr[i].pos] == 'A'){ // Action space
            action(plyr[i], actList, 40); // Call action function
        }
        else if (board[plyr[i].pos] == 'M'){ // Marriage space
            getMarr(plyr[i]); // Call marriage function
        }
        else if (board[plyr[i].pos] == 'K'){ // Kids space
            getKids(plyr[i]); // Call kids function
        }
        else if (board[plyr[i].pos] == 'H'){ // House space
            house(plyr[i], hArr, 20); // Call house function
        }
        else {cout << "Error" << endl;}
        }

        for (int i = 0; i < pCount; i++){ // Check to end game
            if (plyr[i].pos == bSize){
                eCount++;
            }
        }

    }while(eCount != pCount); // If all players are at the end of the board, end game
    
}

Player *pCreate(int &pCount, Career *cc, Career *careers, int jSize){ // Create player function (for the game itself) 
    string name;
    Player *arr = new Player[pCount]; 
    for (int i = 0; i < pCount; i++){
        cout << "Enter player " << i + 1 << "'s name: "; cin >> name;
        arr[i].name = name;
        cout << "Would you like to go to college? (Y/N): "; cin >> name;
        if (name == "Y" || name == "y"){
            arr[i].college = true;
            job(arr[i], cc, cc);
        }
        else {
            arr[i].college = false;
            job(arr[i], careers, cc);
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

void move(Player *plyr, int roll, int bSize) {
    plyr->pos += roll;
    if (plyr->pos >= bSize) {
        plyr->pos = bSize -1;
    }
}

void stopChk(Player &plyr) { // Check if player has landed on a stop (or passed it) and if they have, place them on the stop
    const int stops[] = {13};
    const int size = sizeof(stops) / sizeof(stops[0]);

    if (plyr.pos < size && plyr.pos >= stops[plyr.stops]){
        plyr.stops++;
        plyr.pos = stops[plyr.stops]; // Place them on the stop
    }
}

// Action function (when the user lands on an action position -- Could also use for vacation and other spots maybe?) 
void action(Player &plyr, ActCard *actArr, int actSize){ 
    int act = (rand() % actSize) +1;
    cout << "You have landed on an action space!" << endl;
    cout << actArr[act].event << endl;
    plyr.money += actArr[act].affect; // Add or subtract money from player
    cout << "You now have " << plyr.money << "K" << endl;
}

// Player pay function
void pay(Player &plyr, Career *career){
    cout << "You have passed a pay position!" << endl;
    plyr.money += career[plyr.career].income; // Add money to player
    cout << "You now have " << plyr.money << "K" << endl;
}

// Player pay check function
void payChck(Player &player, Career *career){ 
    const int payPos[] = {1, 5, 10, 19, 30};
    const int size = sizeof(payPos) / sizeof(payPos[0]);
    if (player.paid < size && player.pos >= payPos[player.paid]) {
        pay(player, career);
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
    char temp;
    cout << "You have landed on the kids STOP! Spin for kids!" << endl;
    cout << ">"; cin >> temp;
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

// Job function
void job(Player &player, Career *carArr, Career *colArr) { 
     // Temporary max, future note: change every 20 if you change it
    if(player.college){
        jobSlct(player, colArr, 20);
    }
    else{
        jobSlct(player, carArr, 20);
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
        cout << "(Enter 0 or attempt to purchase if can't)" << endl;
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
            cout << "You bought the " << arr[id1].name << " House!"<< endl;
        }
        else if (pick == 2){
            if (plyr.money < arr[id2].cost) {
                cout << "You do not have enough money. Don't try to buy a house you can't afford loser" << endl;
                return;
            }
            plyr.house = id2;
            cout << "You bought the " << arr[id2].name << " House!"<< endl;
        }
        else if (pick == 0){
            cout << "You have chosen to not buy a house." << endl;
        }
        else {cout << "Please enter a valid response (0, 1 or 2)" << endl;} 
    } while ((pick != 1) && (pick != 2));
    cout << "=========================================" << endl;
}

