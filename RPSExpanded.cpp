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
  bool isVisited;
  int  prev;
  int  cost;

};

//Typedefs
//=====================================
typedef AssociativeArray<string, AssociativeArray<string, string> > losersArray;


//Prototypes
//=====================================
int    readVerbs(losersArray &);
Player* findLoser(Player&, Player&, const losersArray&);
Player* findWinner(Player&, Player&, const losersArray&);

int    getPlayerNum();
void   getPlayerWeapons(vector<Player>&,  const losersArray&);
void   resolveResults(vector<Player>&, const losersArray&);
bool   isDraw(vector<Player>&);

list<int>     findBiggestLoop(vector<Player> , int, int );


void copyContents(Player*, Player*);


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

  getPlayerWeapons(playersArr,  losesTo);

  resolveResults(playersArr, losesTo);

  if(isDraw(playersArr))
    cout << "Draw! " << endl;

  for(int i = 0; i < numOfPlayers; i++) {
    cout << playersArr[i].name << (playersArr[i].inGame ? " has won" : " has lost") << endl;
     if(!playersArr[i].inGame) {
       cout << "    "<<  playersArr[i].weapon ;
       int winner;
       for(int j = 0; j < numOfPlayers; j++) {
         if(playersArr[i].destroyers.front() == playersArr[j].number)
           winner = j;
           winners.push_back(playersArr[winner]);
       }
       cout << losesTo[playersArr[i].weapon][playersArr[winner].weapon] << playersArr[winner].weapon << endl;
     }
  }

     playing = ! winners.size() || (winners.size() == 1);





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
        A[x].inGame = true;
        while (true){
          cout << "Player " << x + 1 << " please enter your weapon: " ;
          getline(cin, input);
          if(losesTo.containsKey(input))
            break;
          else
            cout << "Please enter a correct weapon.." << endl;
        }
        A[x].weapon = input;
        A[x].number = x;
        A[x].name = "Player" + to_string(x);
    }
}



//  player   destroyer
list<int>     findBiggestLoop(vector<Player> database, int N, int start) {
  typedef list<int>::iterator iterator; // to iterate over neighbors
  bool first = true;
  //Clear out database:
  for(int i = 0; i < N; i++) {
    database[i].isVisited = false;
    database[i].prev = -1;
    database[i].cost = 0;
  }
  database[start].isVisited = true;
  queue<int> toDoList;
  toDoList.push(start);

  while(!toDoList.empty()) {
    int currentNodeIndex = toDoList.front();
    Player currentNode = database[currentNodeIndex];

    if(currentNodeIndex == start && !first) {
       break;
     }

    cout << "Checking neighbors..." << endl;
    for(iterator it = currentNode.destroyers.begin(); it != currentNode.destroyers.end(); it++) {
      Player* currentNeighbor = &(database[*it]);
      int  currentNeighborIndex = (*it);

      if(currentNeighbor->cost > currentNode.cost + 1 || currentNeighbor->isVisited == false) {
        currentNeighbor->cost = currentNode.cost + 1;
        currentNeighbor->prev = currentNodeIndex;
      }

      if(currentNeighbor->isVisited  == false)
        toDoList.push(currentNeighborIndex);


    } //end of for loop over neighbors

    database[currentNodeIndex].isVisited = true;
    toDoList.pop();
    first = false;

  }//end of todo while loop

  list<int> result;
  first = true;
  for(int i = start; i != start || first == true; i = database[i].prev) {
    result.push_back(i);
    first = false;
  }
  return result;

}
