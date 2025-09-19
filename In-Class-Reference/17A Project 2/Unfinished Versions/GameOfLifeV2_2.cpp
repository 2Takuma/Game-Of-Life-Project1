/* 
 * File:   GameOfLifeV2_SecondaryFile.cpp
 * Author: Keanu Arao
 * Created on May 24th 2025 11:29 PM
 * 
 *                              Main Focus:
 *                                  File reading and writing for class objects
 *                                  Re-work functions from main game
 *                                  Worked on header files
 *           
 */

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

using namespace std;

Action *aCreate(int);
House *hCreate(int);
Career *cCreate(int);

void fRead(string, Action *, int);
void fRead(string, House *, int);
void fRead(string, Career *, int);

void fWrite(string, Action *, int);
void fWrite(string, House *, int);
void fWrite(string, Career *, int);


int main(int argc, char** argv) {
    int aSize = 3;

    Action *aCards = aCreate(3);

    fWrite("ActionCards.dat", aCards, aSize);
    
    cout << "Events in original Array" << endl;
    for (int i = 0; i < aSize; i++) {
        cout << aCards[i].getE() << endl;
    }

    Action aCards2[3];
    fRead("ActionCards.dat", aCards2, aSize);

    cout << "Events in copied Array read from file" << endl;
    for (int i = 0; i < aSize; i++) {
        cout << aCards2[i].getE() << endl;
    }

    delete[] aCards;

    return 0;
}

Action *aCreate(int size){
    string input;
    int affect;
    Action *arr = new Action[size];
    for (int i = 0; i < size; i++) {
        cout << "Enter Action Card Event: " << endl;
        getline(cin, input);
        cout << "Enter Affect (Positive or Negative int): " << endl;
        cin >> affect;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer
        arr[i] = Action(input.c_str(), affect); // Assuming Action has such a constructor
    }
    return arr;
}

void fRead(string name, Action *arr, int size){// Action Card Reader
    ifstream input(name, ios::binary);
    if (!input){
        cout << "Error opening file";
        return;
    }
    for (int i = 0; i < size; i++) {
        arr[i].aRead(input);
    }
    input.close();
}

void fWrite(string name, Action *arr, int size){// Action Card Writer
    ofstream output(name, ios::binary);
    if (!output){
        cout << "Error opening file";
        return;
    }
    for (int i = 0; i < size; i++) {
        arr[i].aWrite(output);
    }
}