
struct Terminus {
    int index;
    list<int> path;
};

template <typename T>
bool contains(list<T>& searchL, const T& val)
{
	typename list<T>::iterator it = find(searchL.begin(), searchL.end(), val);
  if(it == searchL.end())
    return false;
	return true;
}

void findLongestLoopAtNode(vector<Player>& database, int start) {

  typedef list<int>::iterator iterator; // to iterate over neighbors
  bool first = true;

  //Clear out database:
  for(int i = 0; i < database.size(); i++) {
    database[i].path = list<int>();
  }

  //Initialize search
  list<Terminus> toDoList;
  first = true;
  list<int> startList; //startList.push_front(start);
  Terminus startTerminus = {start, startList};
  toDoList.push_front(startTerminus);

  while(!toDoList.empty()) {
    Terminus currentTop = toDoList.front();
    int currentNodeIndex = currentTop.index;
    Player currentNode = database[currentNodeIndex];


    //Check if we have arrived back at the start, and if yes, if is new longest
    if(currentNodeIndex == start && !first && currentTop.path.size() > database[start].path.size()) {
      database[start].path =  currentTop.path;
      //database[start].path.push_front(start);
      continue;
    }

    //Check that we are not closing a loop
    //If we are back at start, we have checked length of path in previous if statement
    if(contains(currentTop.path, currentNodeIndex) && !first) {
      toDoList.pop_front();
      continue;
    }
    toDoList.pop_front();
    //Add all neighbors to to-do list
    for(iterator it = currentNode.destroyers.begin(); it != currentNode.destroyers.end(); it++) {
      Player* currentNeighbor = &(database[*it]);
      int  currentNeighborIndex = (*it);


      list<int> temp = currentTop.path;
      temp.push_back(currentNodeIndex);
      Terminus newTerminus = {currentNeighborIndex, temp};
      toDoList.push_front(newTerminus);
      } //end of for loop over neighbors

    //cout << "done adding neighbors" << endl;
    //Pop-front assures depth-first search

    first = false;

  }//end of todo while loop

  //last in path must lose to first to close loop
  Player lastInPath = database[database[start].path.back()];
  if(!contains(lastInPath.destroyers, database[start].number)) {
    cout << "last element " << lastInPath.weapon << " does not lose to "
         << database[start].weapon << endl;
    database[start].path.clear();
  }
}

void findLongestLoopInGraph(vector<Player>& players, const losersArray& losesTo) {
  //For this specific type of graph, if a node belongs to one circuit,
  //then it belongs to no other circuits.
  vector<bool> nodeChecked(players.size());

  for(int i = 0; i < nodeChecked.size(); i++) {
    nodeChecked[i] = false;
  }

  int numOfChecked = players.size();
  int i = 0;
  list<int> longest;

  while(numOfChecked > 0 && i < players.size()) {
    if(nodeChecked[i] == false) {
      findLongestLoopAtNode(players, i);
    }
    else {
      continue;
      i++;
    }
    if(players[i].path.size() == 0) {
        nodeChecked[i] = true; numOfChecked--;
        i++;
        continue; //no loop
    }
    for(list<int>::iterator it = players[i].path.begin(); it != players[i].path.end(); it++) {
      nodeChecked[*it] = true; numOfChecked--;
    } //end of for loop
    if(players[i].path.size() > longest.size())
      longest = players[i].path;
    i++;
  } //end of while loop

  if(longest.size() == 0) {
    cout << "No loops!" << endl;
    return;
  }

   cout << "Longest loop: " << endl;
   list<int>::iterator it = longest.begin();
   list<int>::iterator ij = it;

   for(ij++; ij != longest.end(); it++, ij++) {
     string loserWeapon = players[*it].weapon;
     string winnerWeapon = players[*ij].weapon;
     cout << (it == longest.begin() ? "" : "   ...but ");
     cout << loserWeapon << losesTo[loserWeapon][winnerWeapon] << winnerWeapon << endl;
   }
   string winnerWeapon = players[players[longest.front()].number].weapon;
   cout << "   ...and " << players[*it].weapon << losesTo[players[*it].weapon][winnerWeapon]
       << winnerWeapon << "." << endl;

   cout << endl;
}
