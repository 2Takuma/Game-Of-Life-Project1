/*
    This is the secondary file to the Game Of Life game. This file's main purpose is to create the files that will be used in the game + other functions.
*/

#include <iostream>
#include <string>
#include <fstream>
#include "GameStructs.h"
using namespace std;

void arr2Fi(string, char*, int);
void fi2Arr(string, char*, int);
void posLoc(char *, char); // Letter locator to find certain spots

int main(){
    Career careers[20] = 
    {"Actor", 70,
    "Fashion Designer", 50,
    "Race Car Driver", 80,
    "Athlete", 60,
    "Chef", 50,
    "Singer", 60,
    "Teacher", 60,
    "Firefighter", 100,
    "Police Officer", 70,
    "Musician", 70,
    "Chef", 70,
    "Pilot", 110,
    "Entrepreneur", 70,
    "Mechnanic", 90,
    "Plumber", 60,
    "Carpenter", 70,
    "Web Developer", 100,
    "Real Estate Agent", 90,
    "Construction Worker", 90,
    "Artist", 40,};

    Career cc[20] = 
    {"Software Engineer", 110,
    "Doctor", 120,
    "Lawyer", 90,
    "Accountant", 80,
    "Electrical Engineer", 110,
    "Scientist", 110,
    "Architect", 100,
    "Dentist", 90,
    "Pharmacist", 90,
    "Veterinarian", 80,
    "Nurse", 80,
    "Economist", 80,
    "Professor", 100,
    "Physicist", 110,
    "Financial Advisor", 80,
    "Civil Engineer", 90,
    "Biomedial Engineer", 110,
    "Biologist", 80,
    "Psychologist", 80,
    "Management Analyst", 90,};

    House houses[20] = 
    {
        {"Countryside Cottage", 120, 100, 160},
        {"Split-Level Home", 150, 130, 190},
        {"Ranch-Style Home", 180, 170, 210},
        {"Colonial House", 200, 170, 230},
        {"Cape Cod", 220, 190, 250},
        {"Tudor Mansion", 250, 200, 290},
        {"Modern Condo", 300, 290, 320},
        {"Beach House", 350, 320, 390},
        {"Log Cabin", 370, 330, 410},
        {"Penthouse Suite", 410, 390, 490},
        {"Farmhouse", 420, 400, 450},
        {"Victorian Mansion", 500, 460, 550},
        {"Studio Apartment", 100, 80, 110},
        {"Treehouse Retreat", 140, 120, 180},
        {"Lakefront Property", 350, 330, 380},
        {"Rooftop Garden Loft", 280, 270, 300},
        {"Eco-Friendly Tiny Home", 160, 140, 190},
        {"Suburban McMansion", 370, 340, 410},
        {"Haunted Mansion", 270, 250, 290},
        {"Family House", 200, 200, 200}
    }; // House name and cost + sell price (1st and second sell price)

    // Letters at the end are credits to friends for ideas // Add more potentially (for positive consequences)
    ActCard actCard[40] = 
    {"You go to a concert and lose your belongings. Pay 20k to for replacements. (L)", -20,
    "You win a small lottery! Collect 40k.", 40,
    "You sell candles as a side hustle. Collect 20k! (L)", 20,
    "You get scammed from a fake giveaway. Pay 30k. (L)", -30,
    "You place second in a hot dog eating contest! Collect 20k. (L)", 20,
    "You have a rough night of uno and break a friend's T.V. Pay 40k. (L)", -40,
    "You find old money laying around and invest in crypto. You gained 80k! (L)", 80,
    "You host a garage sale and accidentally sell your garage. Gain 50k, but lose 10k in emotional damage. (Ko)", 40,
    "You post a thirst trap online and Grandma likes it on accident. Pay 20k for therapy. (Ko)", -20,
    "You complain to your friend that they didn't help you out enough. Pay 30k for your own funeral. (Ko, Oddly specific)", -30,
    "You get tricked by a fake Squid Game recruiter. You get slapped a lot and lose 50k. (N)", -50,
    "You walk in an alleyway and someone tries to rob you. Batman appears and beats you up thinking you're the robber. Pay 40k for hospital fees. (N)", -40,
    "You walk up to a kid's lemonade stand and you steal his money. Make 30k... (N)" , 30,
    "A kid looking for someone who stole from his lemonade stand finds you and thinks you're the robber. He beats you up. Pay 20k for hospital fees. (N)", -20,
    "MrBeast asks you and a friend if you would want 40K or double it and give it to the next person. Your friend says to pass it on. Lose a friend and gain nothing.. (N)", 0,
    "You get awarded for your pottery skills. Gain 30k.", 30,
    "You win an large lottery! Collect 90k.", 90,
    "You break the neigbor's window in a rock throwing contest. Pay 40k for the window. (J, Oddly specfic)", -40,
    "You decide to ego-lift bench press. Your friend is doomscrolling and doesn't notice you struggle. You crush your chest and pay 50k for hospital fees. (I)", -50,
    "You decide to run Call of Duty Warzone on your broken down computer. You load up the game and your pc explodes. -30k (I)", -30,
    "You buy a fake scratch off ticket and give it to a co-worker you hate. Turns out it was real and they won 20k. (I, -20k)", -20,
    "An influencer asks if you would like a dollar or mystery gift. You choose mystery gift and receive 30k. (I)", 30,
    "You accidentally click on a spam ad that says you'll win 50k. The next day, a package arrives at your door full of 50k. (I)", 50,
    "You listen to r/WallStreetBets and invest for \"free tendies\". You lose 40k. (I)", -40,
    "A guy named Isami beats you up and takes your money. -20k (I)", -20,
    "You've realized that you had a schizo episode about a guy named Isami beating you up. You never lost your money. (I, +20k)", 20,
    "You go out with your friend and he has no money. You decide to buy food for him and his fat ahh buys the whole menu. Pay 30k. (I)", -30,
    "You play the game of Life and some asshole who wrote the cards steals your money. Pay 30k. (I)", -30,
    "You play the game of Life and someone really nice decided to give you money. Gain 30k. (I)", 30,
    "Your parents die in a plane crash... You get 60k from inheritance! (Ab)", 60,
    "Your pics on Feet Finder take off. You make 40k! (Ab)", 40,
    "You get assaulted by a naked homeless man. You lost 20k. (Ab)", -20,
    "You go fishing and accidentally catch the state fish. You get fined 30k. (Ab)", -30,
    "You kindly help an old lady cross the street, who then convinces you to invest in her \"Gain-Giver-Protein-Powder\". You lose 40k and hope in humanity. (Ab)", -40,
    "You get rick rolled in the big 25'. You lose 30k. (Oddly specfic)", -30,
    "You're suddenly suffering from a mid-life crisis. -40k (Ka)", -40,
    "Someone has crashed into your garage and mailbox. Before you can react, they speed off. Pay 30k for repairs. (Ka, Oddly Specific)", -30,
    "You made someone fall for a deez nuts joke in 2025. +40k (Ka)", 40,
    "You decided to become friends with a guy named Kevin. He turned out to be an awful person and now everyone blames you. You bribe them for forgiveness. Pay 20k. (Ka)", -20,
    "You happen to be on my good side. +70k!", 70,
    };

    char board[100] = 
    "APAAAPAAAAPAAM"
    "AAAAAPAAAHAC"
    "AAAAPAAAHAPAAAVAHPAK"
    "AAHAAPAAAVA"
    "AAAAAAHPAAAAVAAHAPAN"
    "AAAVAAPAAAAHPAAAVAAAAR"; // Adjusted to fit within 100 characters including null terminator
    
    // Position Locator
    posLoc(board, 'V');
    
    /* FUNCTIONS USED TO UPDATE STRUCT ARRAYS */
    arr2Fi("Houses.dat", reinterpret_cast<char*>(houses), sizeof(houses)); // Write to file
    arr2Fi("Careers.dat", reinterpret_cast<char*>(careers), sizeof(careers)); // Write to file
    arr2Fi("CollegeCareers.dat", reinterpret_cast<char*>(cc), sizeof(cc)); // Write to file
    arr2Fi("ActionCards.dat", reinterpret_cast<char*>(actCard), sizeof(actCard)); // Write to file
    arr2Fi("Board.dat", reinterpret_cast<char*>(board), sizeof(board)); // Write to file

}

void arr2Fi(string name, char* arr, int size){ // Testing
    ofstream output(name, ios::binary);

    if (!output){
        cout << "Error opening file";
        return;
    }
    output.write(arr, size); 
    output.close();
}

void fi2Arr(string name, char* arr, int size){ // Testing
    ifstream input(name, ios::binary);

    if (!input){
        cout << "Error opening file";
        return;
    }
    input.read(arr, size);
    input.close();
}

void posLoc(char *board, char letter){ // Used to find the values for stop/pay positions
for (int i = 0 ; i < 100; i++){ 
        if (board[i] == letter){
            cout << "Found " << letter << " at index: " << i << endl; 
        }
    }
}