#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <list>
#include <queue>
#include <vector>

using namespace std;

#include "AssociativeArray.h"

//Structs
//=====================================
//Array of Players
struct Player {
  int number;
  string name;
  bool inGame;
  list <int> destroyers;
  string weapon;

  //Graph traversal members
  list<int> path;

};

//Typedefs
//=====================================
typedef AssociativeArray<string, AssociativeArray<string, string> > losersArray;

#include "loopSearch.cpp"

//Prototypes
//=====================================
int    readVerbs(losersArray &);
Player* findLoser(Player&, Player&, const losersArray&);
Player* findWinner(Player&, Player&, const losersArray&);

int    getPlayerNum();
void   getPlayerWeapons(vector<Player>&,  const losersArray&);
void   resolveResults(vector<Player>&, const losersArray&);
bool   isDraw(vector<Player>&);

void   copyContents(Player*, Player*);

void printResults(const vector<Player>& ,  const losersArray & );
void playRound(vector<Player>&, const losersArray&);




//Main routine
//=====================================
int main(void) {

  bool playing = true;
  vector <Player> winners;

  losersArray losesTo;
  if(readVerbs(losesTo) != 0) {
    return 1;
  }

  int numOfPlayers = getPlayerNum();

  vector<Player> playersArr (numOfPlayers);

  for(int i = 0; i < playersArr.size(); i++) {
    playersArr[i].name = "Player" + to_string(i);
  }

  int roundNumber = 1;

  //Playing rounds loop
  while(playersArr.size() > 1) {
    cout << endl << "Playing round " << roundNumber << endl;
    cout << "*************************************" << endl;
    playRound(playersArr, losesTo);
    roundNumber++;
  }

  //Final result output
  cout << "************* WINNER *****************" << endl;
  cout << "Winner is " << playersArr[0] << endl << endl;
}

//Secondary Functions
//=====================================

//Play round function
void playRound(vector<Player>& playersArr, const losersArray & losesTo) {
  getPlayerWeapons(playersArr, losesTo);

  resolveResults(playersArr, losesTo);

  printResults(playersArr, losesTo);

  findLongestLoopInGraph(playersArr, losesTo);

  //If is not draw, pop losers
  if(!isDraw(playersArr)) {
    for(int i = 0; i < playersArr.size(); i++)
      if(playersArr[i].inGame == false) {
        playersArr.erase(playersArr.begin() + i);
        i--;
      }
  }
}

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
Player* findLoser(Player*A, Player* B, const losersArray& LosesTo) {
  //Handle both players having same weapon:
  if(A->weapon == B->weapon)
    return nullptr;
  //Find loser:
  bool loses = LosesTo[A->weapon].containsKey(B->weapon);
  return loses ? A : B;
}

Player* findWinner(Player* A, Player* B, const losersArray& LosesTo) {
  //Handle both players having same weapon:
  if(A->weapon == B->weapon)
    return nullptr;
  //Find winner:
  bool loses = LosesTo[A->weapon].containsKey(B->weapon);
  return loses ? B : A;
}

//resolveResults marks all losers as such and adds their destroyers to their arrays
void resolveResults(vector<Player>& playersArr, const losersArray& losesTo) {
   //Compare every player to every other player except self
   for(int i = 0; i < playersArr.size() - 1; i++)
     for(int j = i + 1; j < playersArr.size(); j++) {
       Player* loser = findLoser(&playersArr[i], &playersArr[j], losesTo);
       Player* winner = findWinner(&playersArr[i], &playersArr[j], losesTo);

       //If both players have same weapon (indicated by nullptr), move on:
       if(loser == nullptr)
         continue;

       //Else, assign losers:
       loser->inGame = false;
       loser->destroyers.push_back(winner->number);

       //Debbuging segment
       /*cout << "Loser: " << loser->name << " with " << loser->weapon << endl;
       cout << "Winner: " << winner->name << " with " << winner->weapon << endl;

       for(int i = 0; i < numOfPlayers; i++) {
         cout << playersArr[i].name << " in game: " << (playersArr[i].inGame ? "true" : "false") << endl;
       }
       cout << endl;*/
     }
}

bool isDraw(vector<Player>& playersArr) {
  //Compare every player to the next. If at least one !=, is not draw.
  for(int i = 0; i < playersArr.size() - 1; i++) {
    if(playersArr[i].inGame != playersArr[i + 1].inGame){
      return false;
    }
  }
  return true;
}

void getPlayerWeapons(vector<Player>& A, const losersArray & losesTo ) {
    string input;
    bool correctInput = false;

    for (int x = 0; x < A.size(); x++) {
        //reinitalize array of players
        A[x].number = x;
        A[x].inGame = true;
        A[x].destroyers = list<int>();

        //get weapon input
        while (true){
          cout <<  A[x].name << " please enter your weapon: " ;
          getline(cin, input);
          if(losesTo.containsKey(input))
            break;
          else
            cout << "Please enter a correct weapon.." << endl;
        }
        A[x].weapon = input;
    }
    cout << endl;
}


void printResults(const vector<Player>& playersArr, const losersArray& losesTo) {
  for(int i = 0; i < playersArr.size(); i++) {
    cout << playersArr[i].name << (playersArr[i].inGame ? " has won\n" : " has lost: ");
     if(!playersArr[i].inGame) {
       for(list<int>::const_iterator it = playersArr[i].destroyers.begin(); it != playersArr[i].destroyers.end(); it++) {
       if(it != playersArr[i].destroyers.begin()) cout << "                  ";
       cout << playersArr[i].weapon
            << losesTo[playersArr[i].weapon][playersArr[*it].weapon]
            << playersArr[*it].weapon << endl;
        }
       cout << endl;
     }
   }
  cout << endl;
}
