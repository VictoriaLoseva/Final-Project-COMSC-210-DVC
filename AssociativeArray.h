//Assignment 9 - Associative Array Template
//Victoria Loseva
//Student ID: 1674904

#ifndef AssociativeArray_h
#define AssociativeArray_h

#include <queue>

template <typename K, typename V>
class AssociativeArray {
  struct Node {
    K    key;
    V    value;
    bool inUse;
  };

  int capacity;
  int size;
  Node* values;
  Node  dummy;

  void setCapacity(int);            //change capacity

public:
  AssociativeArray(int=2);
  AssociativeArray(const AssociativeArray<K,V>&); //copy constructor

  //Getters
  V operator[ ](const K&) const;           //value getter
  bool containsKey(const K&) const;
  int  getSize() const {return size;} ;
  std::queue<K> keys() const;              //list of keys getter

  //Setters
  V& operator[ ](const K&);                //value setter
  void deleteKey(const K&);                //delete key
  void clear();                            //clear array

  //Assignment Operator
  AssociativeArray<K,V>& operator= (const AssociativeArray<K,V>&);

  //Destructor
//  ~AssociativeArray<K,V>();
};

//Constructor
//=====================================
template <typename K, typename V>
AssociativeArray<K,V>::AssociativeArray(int n) {
  values = new Node[n];
  capacity = n;
  size = 0;
}

//Copy constructor
//=====================================
template <typename K, typename V>
AssociativeArray<K,V>::AssociativeArray(const AssociativeArray<K,V>& other) {
  values = new Node[other.getSize()];
  capacity = other.getSize();
  int i = 0;
  for(std::queue<K> k = other.keys(); !k.empty(); k.pop(), i++) {
    values[i].key = k.front();
    values[i].value = other[k.front()];
    values[i].inUse = true;
  }
  size = other.getSize();
}

//Assignment operator
//=====================================
template<typename K, typename V>
AssociativeArray<K,V>& AssociativeArray<K,V>::operator= (const AssociativeArray<K,V>& other){
  if(this == &other)
    return *this;    //early exit

  Node* temp = values;

  values = new Node[other.getSize()];
  capacity = other.getSize();
  int i = 0;
  for(std::queue<K> k = other.keys(); !k.empty(); k.pop(), i++) {
    values[i].key = k.front();
    values[i].value = other[k.front()];
    values[i].inUse = true;
  }
  size = other.getSize();

  delete[] temp;

  return *this;
}
//Value getter
//=====================================
template <typename K, typename V>
V AssociativeArray<K,V>::operator[](const K& getKey) const {
  for(int i = 0; i < capacity; i++) {
    if(values[i].key == getKey && values[i].inUse == true)
      return values[i].value;
  }
  return dummy.value;
}

//Value setter
//=====================================
template <typename K, typename V>
V& AssociativeArray<K,V>::operator[](const K& setKey) {
  int firstUnused = capacity;
  //Search through existing nodes for key & first unused space
  for(int i = 0; i < capacity; i++)
    if(values[i].key == setKey && values[i].inUse == true)
      return values[i].value;
    else if(values[i].inUse == false && firstUnused == capacity)
      firstUnused = i;

  //No match found in existing nodes - add node to first unused
  if(firstUnused == capacity)
    setCapacity(capacity*2);
  values[firstUnused].key = setKey;
  values[firstUnused].inUse = true;
  size++;
  return values[firstUnused].value;

}

//containsKey
//=====================================
template<typename K, typename V>
bool AssociativeArray<K,V>::containsKey(const K& key) const {
  for(int i = 0; i < capacity; i++)
    if(values[i].key == key && values[i].inUse == true)
      return true;

  return false;
}
//deleteKey
//=====================================
template<typename K, typename V>
void AssociativeArray<K,V>::deleteKey(const K& key) {
  for(int i = 0; i < capacity; i++)
    if(values[i].key == key && values[i].inUse == true) {
      values[i].inUse = false;
      size--;
      return;
    }
}

//clear
//=====================================
template<typename K, typename V>
void AssociativeArray<K, V>::clear() {
  for(int i = 0; i < capacity; i++) {
    values[i].inUse = false;
  }
  size = 0;
}

//keys
//=====================================
template<typename K, typename V>
std::queue<K> AssociativeArray<K,V>::keys() const {
  std::queue<K> keysQueue;
  for(int i = 0; i < capacity; i++)
    if(values[i].inUse == true) {
      keysQueue.push(values[i].key);
    }
  return keysQueue;
}

//setCapacity
//=====================================
template<typename K, typename V>
void AssociativeArray<K,V>::setCapacity(int newCap) {
  //set limit
  int limit;
  if(newCap > capacity)
    limit = capacity;
  else
    limit = newCap;

  //create array with new capacity
  Node* newValues = new Node[newCap];

  //fill in new array with previous values
  for(int i = 0; i < limit; i++) {
    newValues[i].key = values[i].key;
    newValues[i].value = values[i].value;
    newValues[i].inUse = values[i].inUse;
  }

  //fill in the remainer of the array with "not in use"
  for(int i = limit; i < newCap; i++)
  {
    newValues[i].inUse = false;
  }

  //Handle reassigning and deleting old array
  Node* temp = values;
  values = newValues;
  delete[] temp;

  capacity = newCap;
}

//destructor
//=====================================
//template<typename K, typename V>
//AssociativeArray<K,V>::~AssociativeArray() {
//  delete[] values;
//}
#endif
