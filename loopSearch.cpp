
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

void findBiggestLoop(vector<Player>& database, int start) {

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


    //Check if we have arrived back at the start
    if(currentNodeIndex == start && !first && currentTop.path.size() > database[start].path.size()) {
      cout << "Found new longest circuit: " << endl;
      database[start].path =  currentTop.path;
      for(iterator it = database[start].path.begin(); it != database[start].path.end(); it++) {
        cout << database[*it].weapon << "(" << database[*it].name << ")" << " -> ";
      }
      cout << "done printing";
      cout << endl;
      continue;
    }

    //Check that we are not closing a loop
    //If we are back at start, we have checked length of path in previous if statement
    if(contains(currentTop.path, currentNodeIndex) && !first) {
      toDoList.pop_front();
      cout << "found loop!" << endl;
      continue;
    }
    toDoList.pop_front();
    //Add all neighbors to to-do list
    cout << "adding new neighbors of " << currentNode.name << " " << currentNode.weapon << endl;
    for(iterator it = currentNode.destroyers.begin(); it != currentNode.destroyers.end(); it++) {
      Player* currentNeighbor = &(database[*it]);
      int  currentNeighborIndex = (*it);


      list<int> temp = currentTop.path;
      temp.push_back(currentNodeIndex);
      Terminus newTerminus = {currentNeighborIndex, temp};
      toDoList.push_front(newTerminus);
      cout << "    pushed neighbor, new top: " << database[toDoList.front().index].name << endl;
      } //end of for loop over neighbors

    //cout << "done adding neighbors" << endl;
    //Pop-front assures depth-first search

    first = false;

  }//end of todo while loop


}
