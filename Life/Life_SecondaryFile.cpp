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

Action *aCreate(int);
House *hCreate(int);
Career *cCreate(int);
Pet *petCr(int);

void fWrite(string, Action *, int);
void fWrite(string, House *, int);
void fWrite(string, Career *, int);
void fWrite(string, Pet *, int);
void bWrite(string, char*, int);

int main(int argc, char** argv) {
    int aSize = 70, cSize = 20, hSize = 20, bSize = 100, petList = 10;

    char board[bSize] = 
    "APAAAPAAAAPAAM"
    "AAAAAPAAAHAC"
    "AAAAPAAAHAPAAAVAHPAK"
    "AAHAAPAAAVA"
    "AAAAAAHPAAAAVAAHAPAN"
    "AAAVAAPAAAAHPAAAVAAAAR";

    //bWrite("Board.dat", board, bSize);

    House *houses = new House[hSize];
    houses = hCreate(hSize);
    try {
        //fWrite("Houses.dat", houses, hSize);
    } catch (const std::exception& e) {
        cerr << "Exception :" << e.what() << endl;
        return 1;
    }

    Action *actions = new Action[aSize];
    actions = aCreate(aSize);
    try {
        //fWrite("ActionCards.dat", actions, aSize);
    } catch (const std::exception& e) {
        cerr << "Exception :" << e.what() << endl;
        return 1;
    }

    Career *cc = new Career[cSize];
    cc = cCreate(cSize);
    try {
        //fWrite("CollegeCareers.dat", cc, cSize);
    } catch (const std::exception& e) {
        cerr << "Exception :" << e.what() << endl;
        return 1;
    }
    
    Career *careers = new Career[cSize];
    careers = cCreate(cSize);
    try {
        //fWrite("Careers.dat", careers, cSize);
    } catch (const std::exception& e) {
        cerr << "Exception :" << e.what() << endl;
        return 1;
    }

    Pet *pets = new Pet[petList];
    
    // EXAMPLE OF MANUAL SET UP DUE TO ERROR I MADE WHILE INPUTTING
    ifstream input("Pets.dat", ios::binary);
    if (!input){
        cout << "Error opening file";
        return 1;
    }
    for (int i = 0; i < petList; i++) {
        pets[i].pRead(input);
    }
    input.close();
    pets[9].setPA(30);
    pets[9].setNA(-20);
    pets[9].setPStr("Your fox digs up a lost treasure! You earn 30k!");
    pets[9].setNStr("Your fox injures one of its paws. Lose 20k in vet fees."); 

    try {
        //fWrite("Pets.dat", pets, petList);
    } catch (const std::exception& e) {
        cerr << "Exception :" << e.what() << endl;
        return 1;
    }

    delete[] cc;
    delete[] careers;
    delete[] houses;
    delete[] pets;
    return 0;
}
void bWrite(string name, char* arr, int size){ // Board File Writer
    ofstream output(name, ios::binary);
    if (!output){
        throw runtime_error("Error opening file: " + name);
    }
    output.write(arr, size); 
    output.close();
}

void fWrite(string name, Action *arr, int size){ // Action Card Writer
    ofstream output(name, ios::binary);
    if (!output){
        throw runtime_error("Error opening file: " + name);
    }
    for (int i = 0; i < size; i++) {
        arr[i].aWrite(output);
    }
    output.close();
}

void fWrite(string name, House *arr, int size){ // House Card Writer
    ofstream output(name, ios::binary);
    if (!output){
        throw runtime_error("Error opening file: " + name);
    }
    for (int i = 0; i < size; i++) {
        arr[i].hWrite(output);
    }
    output.close();
}

void fWrite(string name, Career *arr, int size){ // Career Card Writer
    ofstream output(name, ios::binary);
    if (!output){
        throw runtime_error("Error opening file: " + name);
    }
    for (int i = 0; i < size; i++) {
        arr[i].cWrite(output);
    }
    output.close();
}

void fWrite(string name, Pet *arr, int size){ // Pet Card Writer
    ofstream output(name, ios::binary);
    if (!output){
        throw runtime_error("Error opening file: " + name);
    }
    for (int i = 0; i < size; i++) {
        arr[i].pWrite(output);
    }
    output.close();
}

// Array Creators
Action *aCreate(int size){
    string input;
    int affect;
    Action *arr = new Action[size];
    for (int i = 0; i < size; i++) {
        cout << "Enter Action Card Event: " << endl;
        getline(cin, input);
        cout << "Enter Affect (Positive or Negative int): " << endl;
        cin >> affect;
        cin.ignore();
        arr[i] = Action(input.c_str(), affect);
    }
    return arr;
}

House *hCreate(int size){
    string input;
    int c, s1, s2;
    House *arr = new House[size];
    for (int i = 0; i < size; i++) {
        cout << "Enter House Name: " << endl;
        getline(cin, input);
        cout << "Enter Cost (Int): " << endl;
        cin >> c;
        cout << "Enter Low Sell (Int): " << endl;
        cin >> s1;
        cout << "Enter High Sell (Int): " << endl;
        cin >> s2;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        arr[i] = House(input.c_str(), c, s1, s2);
    }
    return arr;
}

Career *cCreate(int size){
    string input;
    int inc;
    Career *arr = new Career[size];
    for (int i = 0; i < size; i++) {
        cout << "Enter Career Title: " << endl;
        getline(cin, input);
        cout << "Enter Career Income (int): " << endl;
        cin >> inc;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        arr[i] = Career(input.c_str(), inc);
    }
    return arr;
}

Pet *petCr(int size){
    string input, posS, negS;
    int posA, negA;

    Pet *arr = new Pet[size];
    for (int i = 0; i < size; i++) {
        cout << "Enter Pet Name: " << endl;
        getline(cin, input);
        cout << "Enter Positive Affect: " << endl;
        cin >> posA;
        cout << "Enter Negative Affect: " << endl;
        cin >> negA;
        cin.ignore();
        cout << "Enter Positive Event: " << endl;
        getline(cin, posS);
        cout << "Enter Negative Event: " << endl;
        getline(cin , negS);
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        arr[i] = Pet(input.c_str(), posA, negA, posS.c_str(), negS.c_str());
    }
    return arr;
}