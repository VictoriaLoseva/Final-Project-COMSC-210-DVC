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
  bool inGame;
  list <string> destroyers;
  string weapon;
};

//Typedefs
//=====================================
typedef AssociativeArray<string, AssociativeArray<string, string> > losersArray;


//Prototypes
//=====================================
int    readVerbs(losersArray &);
Player findLoser(Player, Player, const losersArray&);
Player findWinner(Player, Player, const losersArray&);

int    getPlayerNum();
void   getPlayerWeapons(Player*, int);
void   resolveResults(Player*, int, const losersArray&);

//Main routine
//=====================================
int main(void) {

  losersArray losesTo;
  if(readVerbs(losesTo) != 0) {
    return 1;
  }


  int numOfPlayers = getPlayerNum();

  Player* playersArr = new Player[numOfPlayers];

  getPlayerWeapons(playersArr, numOfPlayers);

  resolveResults(playersArr, numOfPlayers, losesTo);

  return 0;

}

//Secondary Functions
//=====================================

//Function to read info from file
int readVerbs(losersArray &losesTo) {
  ifstream inputFile;
  inputFile.open("verbs.txt");

  if(!inputFile.good()) {
    cout << "Could not open file" << endl;
    return 1;
  }

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

//function that returns the loser of the match
Player findLoser(Player A, Player B, const losersArray& LosesTo) {
    bool loses = LosesTo[A.name].containsKey(B.name);
    return loses ? A : B;
}

Player findWinner(Player A, Player B, const losersArray& LosesTo) {
    bool loses = LosesTo[A.name].containsKey(B.name);
    return !loses ? A : B;
}


//resolveResults marks all losers as such and adds their winners to thir arrays
void resolveResults(Player* playersArr, int numOfPlayers, const losersArray& losesTo) {
   //Compare every player to every other player except self
   for(int i = 0; i < numOfPlayers - 1; i++)
     for(int j = 0; j < numOfPlayers; j++) {
       Player loser = findLoser(playersArr[i], playersArr[j], losesTo);
       Player winner = findWinner(playersArr[i], playersArr[j], losesTo);
       loser.inGame = false;
       loser.destroyers.push_back(winner.name);
     }
}

void   getPlayerWeapons(Player* A, int size) {

    for (int x = 0; x < size; x++) {

        cout << "Player " << x << " please enter your weapon: " ;
        getline(cin, A[x].weapon);
    }


}
