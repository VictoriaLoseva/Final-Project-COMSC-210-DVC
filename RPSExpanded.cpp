#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <list>

using namespace std;

#include "AssociativeArray.h"

//Structs
//=====================================
//Array of Players
struct Player {
  string name;
  bool ingame;
  list <string> destroyers;
  string charachter;
};

//Typedefs
//=====================================
typedef AssociativeArray<string, AssociativeArray<string, string> > losersArray;


//Prototypes
//=====================================
int    readVerbs(losersArray &);
Player P1vsP2 (Player, Player, const losersArray&);
int    getPlayerNum();


//Main routine
//=====================================
int main(void) {

  losersArray losesTo;
  if(readVerbs(losesTo) != 0) {
    return 1;
  }

  int numOfPlayers = getPlayerNum();

  return 0;

}

//Secondary Functions
//=====================================

//Function to read info from file
int readVerbs(losersArray &LosesTo) {
  ifstream inputFile;
  inputFile.open("verbs.txt");

  if(!inputFile.good()) {
    cout << "Could not open file" << endl;
    return 1;
  }

  losersArray losesTo;

  while(inputFile.good()) {

    string loser, winner, verb;
    getline(inputFile, loser, ',');
    getline(inputFile, winner, ',');
    getline(inputFile, verb, '\n');

    losesTo[loser][winner] = verb;
  }

  return 0;

}

int getPlayerNum() {
  int playNum;
  cout << "Please enter number of players: ";
  cin >> playNum;
  return playNum;
}
//function that returns the winner of the match
Player P1vsP2 (Player A, Player B, const losersArray& LosesTo ) {

    bool loses = LosesTo[A.name].containsKey(B.name);


    return loses ? A : B;

}
