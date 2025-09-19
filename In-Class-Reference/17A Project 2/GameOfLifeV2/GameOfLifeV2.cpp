/* 
 * File:   GameOfLifeV2.cpp
 * Author: Keanu Arao
 * Created on May 12 2025 12:01 AM
 * Purpose:  Recreate the board game "Life" in C++ with an emphasis on classes
 *          Implement "Would you like to see your cards" function 
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
#include "Pet.h"

using namespace std;

// File Function Prototypes
void fRead(string, Action *, int);
void fRead(string, House *, int);
void fRead(string, Career *, int);
void fRead(string, Pet *, int);
void fRead(string, char *, int);

//Board Game Function Prototypes
int spin();
void pay(Player &);
void getMarr(Player &, Player* , int);
void getKids(Player &);
void stopChk(Player &, StpType);
void payChck(Player &, PayPos);
void action(Player &, Action *, int);
void buyH(Player &, House *, int);
void sellH(Player &);
void job(Player &, Career *, Career *, int);
void jobSlct(Player &, Career *, int);
void nightSc(Player &, Career *, Career *, int);
void getPets(Player &, Pet *, int &);
void retire(Player *, House *, int);

// Program Game Function Prototypes
void winner(Player *, int);
Player *pCreate(int &, Career *, Career *, int);
void results(Player *, int);
void seeCards(Player &);
// void record -> record winner to file
// Template Function Prototypes
template <typename T>
void dispArr(const T*, int);

/// Class Implementation
void Player::setName(std::string n) {
    name = n;
}

int Player::getMny() const {
    return money;
}


// Main Program
int main(){
    /// Setting the Random seed for rand functions
    srand(static_cast<unsigned int>(time(0)));

    int jSize = 20, hSize = 20, bSize = 100, pCount, aSize = 70, petList = 10;
    Career cc[jSize], careers[jSize];
    Action actions[aSize];
    House houses[hSize];
    Pet pets[petList];
    StpType stpType;
    PayPos payPos;
    char board[bSize];
    
    try {
        fRead("CollegeCareers.dat", cc, jSize);
        fRead("Careers.dat", careers, jSize);
        fRead("Houses.dat", houses, hSize);
        fRead("board.dat", board, bSize);
        fRead("Pets.dat", pets, petList);
        fRead("ActionCards.dat", actions, aSize);
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }

    cout << "Enter the amount of players you want playing (Max of 4 players):";
    
    while (true) {
        cin >> pCount;
        if (pCount > 4 || pCount <= 0) {
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            cout << "Invalid input. Enter a number (1-4)"<< endl;
            cout << ">";
        } else {
            break; // Exit loop if input is valid
        }
    }

    cin.ignore();
    Player *players = new Player[pCount];
    players = pCreate(pCount, cc, careers, jSize); // Create players (name, money, etc...)

    cout << "=========================================" << endl;
    cout << "Game starting..." << endl;
    int roll, eCount; // counter for end game check
    do {
        eCount = 0; 
        for (int i = 0; i < pCount; i++){
            if (players[i].getPos() == bSize - 2){ 
                eCount++;
                continue; // Skip to next player
            }
            cout << "=========================================" << endl;
            cout << players[i].getName() << "'s turn!" << endl;
            cout << "Tap enter to spin: ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            roll = spin();
            cout << "You rolled a(n) " << roll << endl;
            players[i].move(roll, bSize - 2); // Move Player
            stopChk(players[i], stpType); // Check to see if player passes/lands on stop
            payChck(players[i], payPos); // Call pay function (if they pass a pay position)
            cout << board[players[i].getPos()] << endl; 
            
            // -----ROUTE FUNCTION RE-IMPLEMENTED INTO MAIN GAME LOOP-----
            if (board[players[i].getPos()] == 'A'){ // Action space
                action(players[i], actions, 40); // Call action function
            }
            else if (board[players[i].getPos()] == 'P') {
                continue;
            }
            else if (board[players[i].getPos()] == 'V') {
                cout << "You have landed on a vacation spot! Get a 100K bonus!" << endl;
                players[i].addV();
            }
            else if (board[players[i].getPos()] == 'H'){ // House space
                buyH(players[i], houses, hSize); // Call house function
            }
            else if (board[players[i].getPos()] == 'M'){ // Marriage space
                getMarr(players[i], players, pCount); // Call marriage function
            }
            else if (board[players[i].getPos()] == 'C') {
                getPets(players[i], pets, petList);
            }
            else if (board[players[i].getPos()] == 'K'){ // Kids space
                getKids(players[i]); // Call kids function
            }
            else if (board[players[i].getPos()] == 'N') { // Night School
                nightSc(players[i],careers, cc, jSize);
            }
            else if (board[players[i].getPos()] == 'R'){
                cout << "You are retired." << endl;
                continue;
            }
            else {cout << "Error" << endl;}
                    
        } // End of for loop
    }while(eCount != pCount); // If all players are at the end of the board, end game

    retire(players, houses, pCount);
    results(players, pCount);

    delete[] players;
    return 0;
}

//===================================================================------FILE FUNCTIONS------=====================================================================================

void fRead(string name, char *arr, int size){ // Board reader 
    ifstream input(name, ios::binary);
    if (!input){
        throw std::runtime_error("Error opening file: " + name);
    }
    input.read(arr, size * sizeof(char));
    input.close();
}

void fRead(string name, Action *arr, int size){ // Action Card Reader
    ifstream input(name, ios::binary);
    if (!input){
        throw std::runtime_error("Error opening file: " + name);
    }
    for (int i = 0; i < size; i++) {
        arr[i].aRead(input);
    }
    input.close();
}

void fRead(string name, Career *arr, int size){ // Career Card Reader
    ifstream input(name, ios::binary);
    if (!input){
        throw std::runtime_error("Error opening file: " + name);
    }
    for (int i = 0; i < size; i++) {
        arr[i].cRead(input);
    }
    input.close();
}

void fRead(string name, House *arr, int size){  // House Card Reader
    ifstream input(name, ios::binary);
    if (!input){
       throw std::runtime_error("Error opening file: " + name);
    }
    for (int i = 0; i < size; i++) {
        arr[i].hRead(input);
    }
    input.close();
}

void fRead(string name, Pet *arr, int size){  // Pet Card Reader
    ifstream input(name, ios::binary);
    if (!input){
        throw std::runtime_error("Error opening file: " + name);
    }
    for (int i = 0; i < size; i++) {
        arr[i].pRead(input);
    }
    input.close();
}

//===============================================================------PROGAM GAME FUNCTIONS------=====================================================================================

/**
 * @brief   Create a player array and assigning them names and careers
 * 
 * @param pCount    Player count
 * @param cc        College Career Array
 * @param careers   Regular Career array
 * @param jSize     Size of career array(s)
 * @return arr* Return Array of Players
 */
Player *pCreate(int &pCount, Career *cc, Career *careers, int jSize){
    string name;
    char in;
    Player *arr = new Player[pCount]; 
    for (int i = 0; i < pCount; i++){
        cout << "Enter player " << i + 1 << "'s name: ";
        getline(cin, name);
        arr[i].setName(name);
        cout << "Would you like to go to college? (Y/N): "; cin >> in;
        if (in == 'Y' || in == 'y'){
            arr[i].setCol();
        }
        job(arr[i], careers, cc, jSize);
        cin.ignore();
    }
    return arr;
}
/**
 * @brief Determines the winner of the game by how much money they have
 * 
 * @param players Player array
 * @param pSize size of Player array
 */
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
/**
 * @brief prints out the results of the game
 * 
 * @param players Player array
 * @param pSize Player array size
 */
void results(Player *players, int pSize){
    char pick;
    cout << "===========================================" << endl;
    cout << "Would you like to see the results? (Y/N): "; cin >> pick;
    if (pick == 'Y' || pick == 'y'){
        cout << "-------------------------------------------" << endl;
        dispArr(players, pSize);

        cout << "-------------------------------------------" << endl;
        winner(players, pSize);
        cout << "-------------------------------------------" << endl;
        cout << "Amount of turns taken in the game: " << players[0].getTurns() << endl;
        Player temp = players[0];
        for (int i = 1; i < pSize; i++) {
            temp = temp + players[i];
        }
        cout << "Total Money gained in game: " << temp.getMny() << endl;
        cout << "Total Action Cards gained in Game: " << temp.getAC() << endl;
        cout << "Total Vacation Cards gained in Game: " << temp.getVC() << endl;
        cout << "Total Children acquired (or made you could say) in game: " << temp.getKids() << endl;
        cout << "===========================================" << endl;
    }
    else {cout << "You have chosen to not see the results." << endl;}
    cin.ignore();
}

/**
 * @brief Template function for printing out an array of cards
 * 
 * @tparam T -> the template Type placeholder
 * @param arr The array of object type
 * @param size The array size 
 */
template <typename T>
void dispArr(const T* arr, int size){
    for (int i = 0; i < size; i++) {
        cout << arr[i] << endl;
        if constexpr (std::is_same<T, Player>::value) { // Check if object type of Player
            seeCards(const_cast<Player&>(arr[i]));
        }        
    } 
}

/**
 * @brief Displays all the cards a player got during the game
 * 
 * @param player The player to show cards to
 */
void seeCards(Player &player) {
    cin.ignore();
    char pick;
    cout << "Would you like to see the cards you recieved during the game? (y/n)" << endl;
    cin >> pick;
    if (pick == 'y' || pick == 'Y') {
        player.getCrds();
    }
    else{
        cout << "You have decided to not see the cards you recieved." << endl;
    }
    cin.ignore();
}
//==============================================================------CORE/BOARD GAME FUNCTIONS------=====================================================================================

int spin() {
    return (rand() % 10) + 1;
}

/// @brief Pays the player their income
/// @param player The Player
void pay(Player &player){
    cout << "You have passed a pay position!" << endl;
    player.addMny(player.getCar().getInc());
    cout << "You now have " << player.getMny() << "K" << endl;
}

/// @brief Makes sure to place the player in the correct spot when hypothetically passing stop indexes
/// @param plyr the player
/// @param types enum struct to hold array of stops (ints)
void stopChk(Player &plyr, StpType types) { // Check if player has landed on a stop (or passed it) and if they have, place them on the stop
    const int stops[] = {static_cast<int>(StpType::MARR),static_cast<int>(StpType::PETS),
                         static_cast<int>(StpType::KIDS), static_cast<int>(StpType::NIGHT)}; // Stops array (for now)
    const int size = sizeof(stops) / sizeof(stops[0]);

    if (plyr.getStps() < size && plyr.getPos() >= stops[plyr.getStps()]){
        plyr.setPos(stops[plyr.getStps()]); // Place them on the stop
        plyr.addStop();
    }
}

/// @brief Makes sure to not pay the player every single move by keeping count of how many pay positions the player has passed
/// @param player the player
/// @param places enum struct to hold pay position (ints)
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

/// @brief When a player lands on the marry stop, all other players pay that player 100K
/// @param plyr The Player getting paid
/// @param players The Player Array
/// @param pCount  The Player Array size 
void getMarr(Player &plyr, Player* players, int pCount){
    cout << "You have landed on the marriage STOP!" << endl;
    plyr.setMrr(); 
    for (int i = 0; i < pCount; i++) {
         if (players[i] != plyr) {
            players[i].addMny(-100);
            if (players[i].getMny() < 0) {
                players[i].setMny(0);   
            }
            plyr.addMny(100);
         }
    }
    cout << "You are now married! You receive 100K from all the other players!" << endl;
}

/// @brief When the player lands on the kids stop, they spin to get a certain amount of kids
/// @param plyr The player getting these kids
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

/// @brief When the player lands on the pets stop, they spin for a random pet
/// @param player The player gaining the pet
/// @param pets the array of pets
/// @param petList the pet array size
void getPets(Player &player, Pet *pets, int &petList){
    cout << "You have landed on the Pets STOP! Spin (Enter) for a pet!" << endl;
    cout << ">";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    int roll = (rand() % petList);
    while (pets[roll].getBool()){
        roll = (rand() % petList);
    }
    cout << "You got a " << pets[roll].getName() << "!" << endl;
    // Add pet card to Player's Card Vector then set ownership to true
    player.addCard(std::make_unique<Pet>(pets[roll]));
    pets[roll].setTrue();
}

/// @brief When a player lands on an action space, show the event and pay the player the affect
/// @param player The player
/// @param actArr The Action Card Array
/// @param actSize The Action Card Array size
void action(Player &player, Action *actArr, int actSize){
    cout << "You have landed on an action space!" << endl;
    player.addA();
    int act = rand() % actSize;

    // Loop until an unowned card is selected
    while (actArr[act].getBool()) {
        act = rand() % actSize;
    }
    // Add the card to the Player's vector, set the ownership to true, display card's affect
    player.addCard(std::make_unique<Action>(actArr[act]));
    actArr[act].setTrue();
    cout << actArr[act].getE() << endl;
    
    player.addMny(actArr[act].getAff()); // Add or subtract money from player
    if (player.getMny() < 0) {
        player.setMny(0); // Ensure money does not go below zero
    }
    cout << "You now have " << player.getMny() << "K" << endl;
}

/// @brief Run at the end of the game to give them their extra bits of money as a boost
/// @param players The player array
/// @param houses The house cards array
/// @param pSize Player array size
void retire(Player *players, House *houses, int pSize) {
    cout << "===========================================" << endl;
    for (int i = 0; i < pSize; i++){
        cout << "Retiring " << players[i].getName() << "..." << endl;
        if (players[i].getKids() > 0) { // Check for kids
            players[i].addMny(players[i].getKids() * 100);
        }
        if (players[i].getVC() > 0) { // Give player 100 per vacation counter
            players[i].addMny(players[i].getVC() * 100);
        }
        if (players[i].getAC() > 0) { // Give player 100 per action card
            players[i].addMny(players[i].getAC() * 100);
        } 
        sellH(players[i]);

    }
}

/// @brief Function used in retire function to help players sell their homes for extra money (like an investment)
/// @param player The player selling their homes
void sellH(Player &player) { // Sell House Function
    House *hcpy = player.getH();
    bool soldAny = false;
    for (int j = 0; j < 5; j++) {
        if (hcpy[j].getBool()) { // Check if the house is actually owned
            int roll = spin();
            cout << "Spin (Enter) to sell your house: " << hcpy[j].getName() << "! (Odds = Low sell, Even = High Sell)" << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (roll % 2 == 0) {
                cout << "You rolled even! +" << hcpy[j].getS2() << "k!" << endl;
                player.addMny(hcpy[j].getS2());
            }
            else {
                cout << "You rolled odd! +" << hcpy[j].getS1() << "k!" << endl;
                player.addMny(hcpy[j].getS1());
            }
            // Mark house as sold
            hcpy[j] = House(); // Reset to default/empty house
            soldAny = true;
        }
    }
}

/// @brief Prompt a player to buy 1 of 2 houses when landing on a house spot
/// @param player The player buying houses
/// @param arr The House Cards Array
/// @param size The size of the House Array
void buyH(Player &player, House *arr, int size){
    House* cpyArr = player.getH();
    int id1 = rand() % size, id2 = rand() % size;
    char pick;

    while (arr[id1].getBool()) {
        id1 = rand() % size;
    }

    while (arr[id2].getBool() || id2 == id1){
        id2 = rand() % size;
    }

    // Usage of Copy Constructor
    House choice1 = arr[id1];
    House choice2 = arr[id2];

    do {
        cin.clear();
        pick = '\0';
        cout << "=========================================" << endl;
        cout << "Please select the House you want to buy." << endl;
        cout << "(Enter 0 to cancel)" << endl;
        cout << "House 1: " << endl;
        cout << choice1;
        cout << "-----------------------------------------" << endl;
        cout << "House 2: " << endl;
        cout << choice2;
        cout << ">"; cin >> pick;
        if (pick == '1'){
            if (player.getMny() < arr[id1].getCost()) { // Check if player has enough money, kick em out if they don't
                cout << "You do not have enough money. Don't try to buy a house you can't afford loser" << endl;
                return;
            }
            // Set Card ownership to true -> Add House Card to player's vector of cards - > validate if player has enough room
            arr[id1].setTrue(); 
            player.addCard(std::make_unique<House>(choice1));
            bool added = false;
            for (int i = 0; i < 5; i++){
                if (!cpyArr[i].getBool()) { // Use ownership flag instead of name check
                    cpyArr[i] = choice1;
                    cpyArr[i].setTrue(); // Mark as owned
                    added = true;
                    break;
                }
            }
            if (!added) {
                cout << "You have the maximum amount of houses. House was not purchased." << endl;
                return;
            }
            cout << "You bought the " << arr[id1].getName() << "!"<< endl;
        }
        else if (pick == '2'){
            if (player.getMny() < arr[id2].getCost()) {
                cout << "You do not have enough money. Don't try to buy a house you can't afford loser" << endl;
                return;
            }
            // Set Card ownership to true -> Add House Card to player's vector of cards - > validate if player has enough room
            arr[id2].setTrue();
            player.addCard(std::make_unique<House>(choice2));
            bool added = false;
            for (int i = 0; i < 5; i++){
                if (!cpyArr[i].getBool()) { // Use ownership flag instead of name check
                    cpyArr[i] = choice2;
                    cpyArr[i].setTrue(); // Mark as owned
                    added = true;
                    break;
                }
            }
            if (!added) {
                cout << "You have the maximum amount of houses. House was not purchased." << endl;
                return;
            }
            cout << "You bought the " << arr[id2].getName() << "!"<< endl;
        }
        else if (pick == '0'){
            cout << "You have chosen to not buy a house." << endl;
            return;
        }
        else {cout << "Please enter a valid response (0, 1 or 2)" << endl;} 
    } while ((pick != '1') && (pick != '2') && (pick != '0'));

    cout << "=========================================" << endl;
    player.setH(cpyArr);
    cin.ignore();
}

/// @brief Prompt a player to pick a new career (if wanted) and change their career according to so
/// @param player The player being prompted
/// @param carArr The regular Career Cards Array
/// @param colArr The College Career Cards Array
/// @param jSize The size of both of the Career Arrays
void nightSc(Player &player, Career *carArr, Career *colArr, int jSize){
    char pick;
    cout << "Would you like to go to night school? (y/n)" << endl;
    cin >> pick;
    if (pick == 'y' || pick == 'Y') {
        if (!player.getCol()) {player.setCol();}
        player.rmvCard<Career>();
        player.getCar().setFlse();
        job(player, carArr, colArr, jSize);
    }
    else {
        cout << "You have decided to not go to night school." <<endl;
    }
    cin.ignore();
}

/// @brief Prompt the user a job select depending on whether they wanted to go to college or not
/// @param player The player being prompted
/// @param carArr The Regular Career Cards Array
/// @param colArr The College Career Cards Array
/// @param jSize The size of the Career Card Arrays
void job(Player &player, Career *carArr, Career *colArr, int jSize) { 
    if(player.getCol()){
        jobSlct(player, colArr, jSize);
    }
    else{
        jobSlct(player, carArr, jSize);
    }
}

/// @brief Prompt the user to pick between 1 of 2 career choices
/// @param player The player being prompted
/// @param arr The Career Card Array
/// @param size The size of the Career Array
void jobSlct(Player &player, Career *arr, int size){ 
    int id1 = rand() % size, id2 = rand() % size;

    while(arr[id1].getBool()){
        id1 = rand() % size;
    }

    while(arr[id2].getBool() || id2 == id1){
        id2= rand() % size;
    }

    // Usage of Copy Constructor
    Career choice1 = arr[id1];
    Career choice2 = arr[id2];

    char pick;
    cout << "=========================================" << endl;
    cout << "Pick one of the two careers: " << endl;
    cout << "Job 1: " << endl;
    cout << choice1;
    cout << "Job 2: " << endl;
    cout << choice2;
    cout << "Enter 1 or 2 in accordance to the job:" << endl;

    do {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        pick = '\0';
        cin >> pick;
        if (pick == '1') {
            player.setCar(choice1);
            player.addCard(std::make_unique<Career>(choice1));
            arr[id1].setTrue();
            cout << "You have chosen: " << choice1.getTi() << endl;
            cout << "=========================================" << endl;
        }
        else if (pick == '2') {
            player.setCar(choice2);
            player.addCard(std::make_unique<Career>(choice2));
            arr[id2].setTrue();
            cout << "You have chosen: " << choice2.getTi() << endl;
            cout << "=========================================" << endl;
        }   
        else {cout << "Please enter a valid response (1 or 2)" << endl;}  
    } while((pick != '1') && (pick != '2'));
}

