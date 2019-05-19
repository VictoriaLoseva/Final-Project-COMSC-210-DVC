#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <list>
#include <queue>


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
Player* findLoser(Player*, Player*, const losersArray&);
Player* findWinner(Player*, Player*, const losersArray&);

int    getPlayerNum();
void   getPlayerWeapons(Player*, int, const losersArray&);
void   resolveResults(Player*, int, const losersArray&);
bool   isDraw(Player*, int);

//Main routine
//=====================================
int main(void) {

  losersArray losesTo;
  if(readVerbs(losesTo) != 0) {
    return 1;
  }

  int numOfPlayers = getPlayerNum();

  Player* playersArr = new Player[numOfPlayers];

  getPlayerWeapons(playersArr, numOfPlayers, losesTo);

  resolveResults(playersArr, numOfPlayers, losesTo);

  if(isDraw(playersArr, numOfPlayers))
    cout << "Draw! " << endl;
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
    getline(inputFile, verb, ',');
    getline(inputFile, winner, '\n');

    if(loser == "") continue;
    losesTo[loser][winner] = verb;
  }

  //Debugging segment
  /*queue<string> losers = losesTo.keys();
  cout << "size of losesTo:" << losesTo.getSize() << endl;
  cout << "size of losers :" << losers.size() << endl;
  for(int i = 0; i < losers.size();){
    cout << losers.front() << " loses to:" << endl;
    queue<string> winners = losesTo[losers.front()].keys();
    for(int j = 0; j < winners.size();) {
       cout << "    " << winners.front() << endl;
       winners.pop();
     }
    losers.pop();
  }
  cout << endl;*/

  return 0;

}

int getPlayerNum() {
  int playNum;
  cout << "Please enter number of players: ";
  cin >> playNum;
  cin.ignore(1000, 10);
  return playNum;

}

//functions that return the loser/winner of the match
Player* findLoser(Player* A, Player* B, const losersArray& LosesTo) {
    bool loses = LosesTo[A->weapon].containsKey(B->weapon);
    return loses ? A : B;
}

Player* findWinner(Player* A, Player* B, const losersArray& LosesTo) {
    bool loses = LosesTo[A->weapon].containsKey(B->weapon);
    return loses ? B : A;
}

//resolveResults marks all losers as such and adds their destroyers to their arrays
void resolveResults(Player* playersArr, int numOfPlayers, const losersArray& losesTo) {
   //Compare every player to every other player except self
   for(int i = 0; i < numOfPlayers - 1; i++)
     for(int j = i + 1; j < numOfPlayers; j++) {
       Player* loser = findLoser(&playersArr[i], &playersArr[j], losesTo);
       Player* winner = findWinner(&playersArr[i], &playersArr[j], losesTo);
       loser->inGame = false;
       loser->destroyers.push_back(winner->name);

       //Debbuging segment
       /*cout << "Loser: " << loser->name << " with " << loser->weapon << endl;
       cout << "Winner: " << winner->name << " with " << winner->weapon << endl;

       for(int i = 0; i < numOfPlayers; i++) {
         cout << playersArr[i].name << " in game: " << (playersArr[i].inGame ? "true" : "false") << endl;
       }
       cout << endl;*/
     }
}

bool isDraw(Player* playersArr, int numOfPlayers) {
  for(int i = 0; i < numOfPlayers - 1; i++) {
    if(playersArr[i].inGame != playersArr[i + 1].inGame)
      return false;
  }
  return true;
}

void getPlayerWeapons(Player* A, int size, const losersArray & losesTo ) {
    string input;
    bool correctInput = false;

    for (int x = 0; x < size; x++) {
        A[x].inGame = true;
        while (true){
          cout << "Player " << x << " please enter your weapon: " ;
          getline(cin, input);
          if(losesTo.containsKey(input))
            break;
          else
            cout << "Please enter a correct weapon.." << endl;
        }
        A[x].weapon = input;
        A[x].name = "Player" + to_string(x);
    }

}
