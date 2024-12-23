/***********************************************************
 ** // UMBC - CMSC 341 - Fall 2024 - Proj2
 ** File:    mytest.cpp
 ** Project: Fall 2024 - Proj2
 ** Author:  Hazael Magino
 ** Date:    10/22/2022
 ** This file contains the proper implementations for swarm.cpp
 **********************************************************/
#include "swarm.h"
#include <algorithm>
#include <math.h>
#include <random>
#include <vector>

using namespace std;
enum RANDOM { UNIFORMINT, UNIFORMREAL, NORMAL, SHUFFLE };
class Random {
public:
  Random() {}
  Random(int min, int max, RANDOM type = UNIFORMINT, int mean = 50,
         int stdev = 20)
      : m_min(min), m_max(max), m_type(type) {
    if (type == NORMAL) {
      // the case of NORMAL to generate integer numbers with normal distribution
      m_generator = std::mt19937(m_device());
      // the data set will have the mean of 50 (default) and standard deviation
      // of 20 (default) the mean and standard deviation can change by passing
      // new values to constructor
      m_normdist = std::normal_distribution<>(mean, stdev);
    } else if (type == UNIFORMINT) {
      // the case of UNIFORMINT to generate integer numbers
      //  Using a fixed seed value generates always the same sequence
      //  of pseudorandom numbers, e.g. reproducing scientific experiments
      //  here it helps us with testing since the same sequence repeats
      m_generator = std::mt19937(10); // 10 is the fixed seed value
      m_unidist = std::uniform_int_distribution<>(min, max);
    } else if (type == UNIFORMREAL) { // the case of UNIFORMREAL to generate
                                      // real numbers
      m_generator = std::mt19937(10); // 10 is the fixed seed value
      m_uniReal =
          std::uniform_real_distribution<double>((double)min, (double)max);
    } else { // the case of SHUFFLE to generate every number only once
      m_generator = std::mt19937(m_device());
    }
  }
  void setSeed(int seedNum) {
    // we have set a default value for seed in constructor
    // we can change the seed by calling this function after constructor call
    // this gives us more randomness
    m_generator = std::mt19937(seedNum);
  }
  void init(int min, int max) {
    m_min = min;
    m_max = max;
    m_type = UNIFORMINT;
    m_generator = std::mt19937(10); // 10 is the fixed seed value
    m_unidist = std::uniform_int_distribution<>(min, max);
  }
  void getShuffle(vector<int> &array) {
    // this function provides a list of all values between min and max
    // in a random order, this function guarantees the uniqueness
    // of every value in the list
    // the user program creates the vector param and passes here
    // here we populate the vector using m_min and m_max
    for (int i = m_min; i <= m_max; i++) {
      array.push_back(i);
    }
    shuffle(array.begin(), array.end(), m_generator);
  }

  void getShuffle(int array[]) {
    // this function provides a list of all values between min and max
    // in a random order, this function guarantees the uniqueness
    // of every value in the list
    // the param array must be of the size (m_max-m_min+1)
    // the user program creates the array and pass it here
    vector<int> temp;
    for (int i = m_min; i <= m_max; i++) {
      temp.push_back(i);
    }
    std::shuffle(temp.begin(), temp.end(), m_generator);
    vector<int>::iterator it;
    int i = 0;
    for (it = temp.begin(); it != temp.end(); it++) {
      array[i] = *it;
      i++;
    }
  }

  int getRandNum() {
    // this function returns integer numbers
    // the object must have been initialized to generate integers
    int result = 0;
    if (m_type == NORMAL) {
      // returns a random number in a set with normal distribution
      // we limit random numbers by the min and max values
      result = m_min - 1;
      while (result < m_min || result > m_max)
        result = m_normdist(m_generator);
    } else if (m_type == UNIFORMINT) {
      // this will generate a random number between min and max values
      result = m_unidist(m_generator);
    }
    return result;
  }

  double getRealRandNum() {
    // this function returns real numbers
    // the object must have been initialized to generate real numbers
    double result = m_uniReal(m_generator);
    // a trick to return numbers only with two deciaml points
    // for example if result is 15.0378, function returns 15.03
    // to round up we can use ceil function instead of floor
    result = std::floor(result * 100.0) / 100.0;
    return result;
  }

  string getRandString(int size) {
    // the parameter size specifies the length of string we ask for
    // to use ASCII char the number range in constructor must be set to 97 - 122
    // and the Random type must be UNIFORMINT (it is default in constructor)
    string output = "";
    for (int i = 0; i < size; i++) {
      output = output + (char)getRandNum();
    }
    return output;
  }

  int getMin() { return m_min; }
  int getMax() { return m_max; }

private:
  int m_min;
  int m_max;
  RANDOM m_type;
  std::random_device m_device;
  std::mt19937 m_generator;
  std::normal_distribution<> m_normdist;     // normal distribution
  std::uniform_int_distribution<> m_unidist; // integer uniform distribution
  std::uniform_real_distribution<double> m_uniReal; // real uniform distribution
};
class Tester {
public:
  Swarm generateTree(int teamSize, TREETYPE type);
  bool
  isBst(Robot *root); // helper function to detirmine if AVL properties exist
  bool isSplay(
      int size,
      Robot *start); // helper function to detirmine if SPLAY properties exist
  bool testAVLInsertNormal(int size, TREETYPE type);
  bool testSplayInsertNormal(int size, TREETYPE type);
  int getLastInserted(); // helper function to detirmine validity of splay tree
  bool testBSTInsertNormal(int size, TREETYPE type);
  bool testAVLInsertEdge(int size, TREETYPE type);
  bool testSplayInsertEdge(int size, TREETYPE type);
  bool testBSTInsertError(int size, TREETYPE type);
  bool testSplayInsertError(int size, TREETYPE type);
  bool testAVLInsertError(int size, TREETYPE type);
  bool testBSTInsertEdge(int size, TREETYPE type);
  bool testClearNormal(int size, TREETYPE type);
  bool testClearEdge(int size, TREETYPE type);
  bool testClearError(int size, TREETYPE type);
  bool testRemoveNormal(int size, TREETYPE type);
  bool testRemoveEdge(int size, TREETYPE type);
  bool testRemoveError(int size, TREETYPE type, int rand);
  bool testSingleRemoveEdge(int size, TREETYPE type);
  bool testSetTypeNormal(int size, TREETYPE type, TREETYPE changeType);
  bool testRestructure(int size, TREETYPE type, TREETYPE changeType);
  bool testOverloadedNormal(const Swarm &orig);
  bool areIdentical(
      Robot const *orig,
      Robot const *copy); // helper tester for seeing if nodes are identical
  bool testOverloadedError(const Swarm &orig);
  bool testHeightsValues(int size, TREETYPE type);
  int calculateTestHeight(
      Robot *node); // helper function to test the heights of nodes
  bool testHeightsInBSTremoval(int size, TREETYPE type);
  bool testHeightsInAVLInserts(int size, TREETYPE type);
  bool testHeightsInSplayInserts(int size, TREETYPE type);
  bool testAVLMassiveInsertRemove(int size, TREETYPE type);
  bool testBSTMassiveInsertRemove(int size, TREETYPE type);
  bool testTreePerformanceHeavyLoad(int size, TREETYPE type);
  bool testSpecialNodeRemovals();

private:
  int m_lastInserted;
};

Swarm Tester::generateTree(int numRobots, TREETYPE type) {
  Random idGen(MINID, MAXID);
  Random typeGen(0, 4);

  Swarm team(type);
  int teamSize = numRobots;
  ROBOTTYPE roboType = DEFAULT_TYPE;

  for (int i = 0; i < teamSize; i++) {
    int ID = idGen.getRandNum();
    roboType = static_cast<ROBOTTYPE>(typeGen.getRandNum());

    Robot robot(ID, roboType);
    team.insert(robot); // Insert the robot into the team

    m_lastInserted = ID; // Update the last inserted ID
  }
  return team; // Return the populated Swarm
}

int Tester::getLastInserted() {
  return m_lastInserted; // Return the last inserted ID
}

// Function: isBST(int size, TREETYPE type);
//  Function will verify if tree follows BST properties where
// Expected result: Where IDs are unique and left children are less than parents
//  and right children are greater than parents.
bool Tester::isBst(Robot *root) {

  // base case: empty tree
  if (root == nullptr) {
    return true;
  }

  // Check if the left child violates BST property
  if (root->getLeft() != nullptr &&
      (root->getLeft()->getID() >= root->getID())) {
    return false;
  }

  // Check if the rght child violates BST property
  if (root->getRight() != nullptr &&
      (root->getRight()->getID() <= root->getID())) {
    return false;
  }

  // Recursively check the left and right subtrees.
  return isBst(root->getLeft()) && isBst(root->getRight());
}

// Function: isSplay(int size, TREETYPE type);
//  Function will verify if tree follows Splay properties where m_root
//  is the most recently inserted node
//  Expected result: m_root is the last inserted node
bool Tester::isSplay(int id, Robot *start) {

  // Check if the tree is empty or root is nullptr
  if (start == nullptr) {
    return false; // No nodes in the tree, cannot be splayed correctly
  }

  // Check if the root of the Swarm is the last inserted node
  if (start->getID() != id) {

    return false; // If the root's ID does not match the last inserted ID, it's
                  // not splayed correctly
  }

  return true;
}

// Function: testAVLInsertNormal(int size, TREETYPE type);
// Case(Normal): Test normal avl insertions
// Expected result: after normal insertions AVL tree should be balanced
bool Tester::testAVLInsertNormal(int size, TREETYPE type) {
  // Generate a new tree with the given size and type
  Swarm newTeam = generateTree(size, type);

  // Check if the tree satisfies the following conditions:
  // 1. The number of nodes in the tree matches the expected size
  // 2. The tree's type matches the specified type
  // 3. The tree follows AVL properties (balanced)
  // 4. The tree maintains the Binary Search Tree (BST) property
  if (size == newTeam.countNodesinTree(newTeam.m_root) &&
      newTeam.m_type == type && newTeam.isAvl(newTeam.m_root) == true &&
      isBst(newTeam.m_root) == true) {

    // If all conditions are met, the test passes
    return true;
  }

  // If any condition fails, the test fails
  return false;
}

// Function: testSplayInsertNormal(int size, TREETYPE type);
// Case(Normal): Test normal splay insertions
// Expected result: after normal insertions splay tree should have the last
// inserted node as m_root
bool Tester::testSplayInsertNormal(int size, TREETYPE type) {

  Swarm newTeam = generateTree(size, type);

  // Verify that the tree satisfies the following conditions:
  // 1. The number of nodes in the tree matches the expected size.
  // 2. The type of the tree matches the specified type.
  // 3. The splay tree property holds: the most recently inserted node
  //    (tracked by m_lastInserted) is now the root of the tree.
  // 4. The tree maintains the Binary Search Tree (BST) property
  //    (elements are ordered correctly).
  if (size == newTeam.countNodesinTree(newTeam.m_root) &&
      newTeam.m_type == type &&
      isSplay(m_lastInserted, newTeam.m_root) == true &&
      isBst(newTeam.m_root)) {

    return true;
  }

  return false;
}
// Function: testBSTInsertNormal(int size, TREETYPE type);
// Case(Normal): Test normal BST insertions
// Expected result: after normal insertions BST tree should have BST properties
bool Tester::testBSTInsertNormal(int size, TREETYPE type) {

  Swarm newTeam = generateTree(size, type);

  // Check if the tree meets the following conditions:
  // 1. The number of nodes in the tree matches the expected size.
  // 2. The type of the tree matches the specified type.
  // 3. The tree maintains the Binary Search Tree (BST) property
  //    (i.e., all nodes are correctly ordered).
  if (size == newTeam.countNodesinTree(newTeam.m_root) &&
      newTeam.m_type == type && isBst(newTeam.m_root)) {

    return true;
  }

  return false;
}

// Function: testBSTInsertEdge(int size, TREETYPE type);
// Case(edge): Test edge AVL insertions such as inserting only 1 node
// Expected result: After normal insertions AVL tree should have AVL properties
bool Tester::testAVLInsertEdge(int size, TREETYPE type) {

  Swarm newTeam = generateTree(size, type);

  // Check if the generated tree meets the following conditions:
  // 1. The number of nodes in the tree matches the expected size.
  // 2. The tree's type matches the specified type.
  // 3. The tree satisfies AVL properties (i.e., it is balanced).
  // 4. The tree maintains the Binary Search Tree (BST) property (nodes are
  // ordered correctly).
  if (size == newTeam.countNodesinTree(newTeam.m_root) &&
      newTeam.m_type == type && newTeam.isAvl(newTeam.m_root) == true &&
      isBst(newTeam.m_root) == true) {

    return true;
  }

  return false;
}

// Function: testSplayInsertEdge(int size, TREETYPE type);
// Case(edge): Test edge Splay insertions such as inserting only 1 node
// Expected result: After normal insertions Splay tree should have Splay
// properties
bool Tester::testSplayInsertEdge(int size, TREETYPE type) {

  // Generate a new tree with the specified size and type.
  Swarm newTeam = generateTree(size, type);

  // Check if the generated tree meets the following conditions:
  // 1. The number of nodes in the tree matches the expected size.
  // 2. The tree's type matches the specified type.
  // 3. The tree follows splay tree properties, meaning the most recently
  // inserted node
  //    (tracked by `m_lastInserted`) is now the root.
  // 4. The tree maintains the Binary Search Tree (BST) property (i.e., nodes
  // are correctly ordered).
  if (size == newTeam.countNodesinTree(newTeam.m_root) &&
      newTeam.m_type == type &&
      isSplay(m_lastInserted, newTeam.m_root) == true &&
      isBst(newTeam.m_root) == true) {

    return true;
  }

  return false;
}

// Function: testBSTInsertEdge(int size, TREETYPE type);
// Case(edge): Test edge BST insertions such as inserting only 1 node
// Expected result: After normal insertions BST tree should have BST properties
bool Tester::testBSTInsertEdge(int size, TREETYPE type) {

  Swarm newTeam = generateTree(size, type);

  // Check if the generated tree meets the following conditions:
  // 1. The number of nodes in the tree matches the expected size.
  // 2. The tree's type matches the specified type.
  // 3. The tree maintains the Binary Search Tree (BST) property,
  //    meaning all nodes follow the correct ordering rules.
  if (size == newTeam.countNodesinTree(newTeam.m_root) &&
      newTeam.m_type == type && isBst(newTeam.m_root)) {

    return true;
  }
  return false;
}

// Function: testClearNormal(int size, TREETYPE type);
// Case(Normal): Test normal case such as clearing a regular type tree
// Expected result: After removal no memory, or nodes should be pesent
bool Tester::testClearNormal(int size, TREETYPE type) {

  Swarm newTeam = generateTree(size, type);

  // The `clear` function deallocates all nodes in the tree and resets it to an
  // empty state.
  newTeam.clear();

  // Check if the tree is now empty by verifying if the root is set to
  // `nullptr`. If the root is nullptr, the tree has been successfully cleared.
  if (newTeam.m_root == nullptr) {
    return true;
  } else {

    return false;
  }
}

// Function: testClearEdge(int size, TREETYPE type);
// Case(edge): Test edge case such as clearing a regular type tree with 1 node
// Expected result: After removal no memory, or nodes should be pesent
bool Tester::testClearEdge(int size, TREETYPE type) {

  Swarm newTeam = generateTree(size, type);

  // Step 2: Clear the tree
  newTeam.clear();

  // Check if the tree is empty (root should be nullptr)
  if (newTeam.m_root == nullptr) {

    return true;
  } else {
    return false;
  }
}

// Function: testClearEdge(int size, TREETYPE type);
// Case(error): Test error case such as clearing a regular type tree with 0 node
// Expected result: Nothing should be clear as there is nothing to clear
bool Tester::testClearError(int size, TREETYPE type) {

  Swarm newTeam = generateTree(size, type);

  // attempt to clear the tree
  newTeam.clear();

  // Check if the tree is empty (root should be nullptr)
  if (newTeam.m_root == nullptr) {

    return true;
  } else {
    return false;
  }
}

// Function: testBSTInsertError(int size, TREETYPE type);
// Case(error): Test error insertions such as inserting into a BST type tree
// with 50 nodes
//  and type none
// Expected result: Nothing should be inserted as it is NONE type
bool Tester::testBSTInsertError(int size, TREETYPE type) {

  Swarm newTeam = generateTree(size, type);

  if (0 == newTeam.countNodesinTree(
               newTeam.m_root) &&  // size should be 0 since nothing is inserted
      newTeam.m_type == NONE &&    // type should be none
      newTeam.m_root == nullptr && // m_root should not be populated
      isBst(newTeam.m_root) ==
          true) { // Bst property should still hold (empty tree)
    return true;
  }

  return false;
}

// Function: testSplayInsertError(int size, TREETYPE type);
// Case(error): Test error insertions such as inserting into a regular splay
// tree with 50 nodes
//  and type none
// Expected result: Nothing should be inserted as it is NONE type
bool Tester::testSplayInsertError(int size, TREETYPE type) {

  Swarm newTeam = generateTree(size, type);

  if (0 == newTeam.countNodesinTree(
               newTeam.m_root) && // size should be 0 since nothing is inserted
      newTeam.m_type == NONE &&
      newTeam.m_root == nullptr &&
      isSplay(m_lastInserted, newTeam.m_root) ==
          false && // a tree with no nodes cannot be splayed
      isBst(newTeam.m_root) ==
          true) { // however an empty splay tree is still a BST
    return true;
  }
  return false;
}

// Function: testSplayInsertError(int size, TREETYPE type);
// Case(error): Test error insertions such as inserting into a regular AVL tree
// with 50 nodes
//  and type none
// Expected result: Nothing should be inserted as it is NONE type
bool Tester::testAVLInsertError(int size, TREETYPE type) {

  Swarm newTeam = generateTree(size, type);

  if (0 == newTeam.countNodesinTree(
               newTeam.m_root) &&  // size should be 0 since nothing is inserted
      newTeam.m_type == NONE &&    // type should be none
      newTeam.m_root == nullptr && // m_root should be null
      isBst(newTeam.m_root) == true) { // bst property should still hold
    return true;
  }
  return false;
}

// Function: testRemoveNormal(int size, TREETYPE type);
// Case(Normal): Test normal removals  into a regular type tree
// Expected result: Nodes should not be present, tree properties should still be
// present
bool Tester::testRemoveNormal(int size, TREETYPE type) {

  Swarm newTeam = generateTree(size, type);

  // Remove the last inserted node from the tree.

  newTeam.remove(m_lastInserted);

  if ((size - 1) ==
          newTeam.countNodesinTree(
              newTeam
                  .m_root) && // Check if the number of nodes is reduced by one.
      newTeam.m_type ==
          type && // Ensure the type of the tree remains unchanged.
      newTeam.m_root != nullptr && // Verify that the root of the tree is still
                                   // valid (not nullptr).
      isBst(newTeam.m_root) == true && // Check if the tree still adheres to
                                       // Binary Search Tree properties.
      newTeam.findNode(newTeam.m_root, m_lastInserted) ==
          false // Confirm that the last inserted node is no longer in the tree.
  ) {

    return true;
  }

  return false;
}

// Function: testRemoveEdge(int size, TREETYPE type);
// Case(Edge): Test normal removal of root node
// Expected result: Node should not be present, tree properties should still be
// present
bool Tester::testRemoveEdge(int size, TREETYPE type) {

  Swarm newTeam = generateTree(size, type);

  // Store the ID of the current root node.
  //  The root node's ID is stored in `oldroot` to use it later for removal and
  //  validation.
  int oldroot = newTeam.m_root->getID();

  // Remove the old root node from the tree.
  newTeam.remove(oldroot);

  if ((size - 1) ==
          newTeam.countNodesinTree(
              newTeam.m_root) && // Check if the tree now has one fewer node.
      newTeam.m_type == type &&  // Verify that the tree type remains the same.
      newTeam.m_root != nullptr && // Ensure the tree's root is not `nullptr`.
      newTeam.isAvl(newTeam.m_root) ==
          true &&              // Check if the tree maintains AVL balance.
      isBst(newTeam.m_root) && // Confirm that the tree still follows BST rules.
      newTeam.findNode(newTeam.m_root, oldroot) ==
          false // Ensure the old root node no longer exists in the tree.
  ) {

    return true;
  }
  return false;
}

// Function: testRemoveError(int size, TREETYPE type);
// Case(Error): Test normal removal of root node not in tree
// Expected result: Removal Operation should not happen
bool Tester::testRemoveError(int size, TREETYPE type, int rand) {

  Swarm newTeam = generateTree(size, type);

  int randID = rand;

  // Remove a node not in tree.
  newTeam.remove(randID);

  // check if size is still the same and properties of splay tree still hold
  if (size == newTeam.countNodesinTree(newTeam.m_root) &&
      newTeam.m_type == type && newTeam.m_root != nullptr &&
      isSplay(m_lastInserted, newTeam.m_root) == true &&
      isBst(newTeam.m_root) == true &&
      newTeam.findNode(newTeam.m_root, randID) == false) {

    return true;
  }
  return false;
}

// Function: testSetTypeNormal(int size, TREETYPE type, TREETYPE changeType);
// Case(Normal): Test changing of tree type
// Expected result: Tree should resemble a new type of tree with new changetype
// properties
bool Tester::testSetTypeNormal(int size, TREETYPE type, TREETYPE changeType) {

  // Generate a new tree with given size and type
  Swarm newTeam = generateTree(size, type);

  bool stageOne = false; // Flag for the first stage check

  // Check initial properties of the tree
  if (size == newTeam.countNodesinTree(
                  newTeam.m_root) && // Size should match the number of nodes in
                                     // the tree
      newTeam.m_type == type &&      // Tree type should match the initial type
      newTeam.m_root != nullptr &&   // Tree root should be populated
      isBst(newTeam.m_root)          // Tree should maintain BST properties
  ) {
    stageOne = true; // First stage passed
  } else {
    return false; // If initial properties are incorrect, return false
  }

  bool stageTwo = false; // Flag for the second stage check

  newTeam.setType(changeType); // Change the tree type

  int insert = 10; // Number of new nodes to insert

  // Reseed and insert new 10 random nodes
  Random idGen(MINID, MAXID);
  Random typeGen(0, 4);
  idGen.setSeed(time(0));

  int teamSize = insert;
  ROBOTTYPE roboType = DEFAULT_TYPE;

  for (int i = 0; i < teamSize; i++) {
    int ID = idGen.getRandNum();
    roboType = static_cast<ROBOTTYPE>(typeGen.getRandNum());
    Robot robot(ID, roboType);
    newTeam.insert(robot); // Insert the robot into the team
    m_lastInserted = ID;   // Update the last inserted ID
  }

  // Check updated properties of the tree
  if ((size + insert) ==
          newTeam.countNodesinTree(
              newTeam
                  .m_root) && // Updated size should match the number of nodes
      newTeam.m_type == changeType && // Tree type should match the new type
      newTeam.m_root != nullptr &&    // Tree root should be populated
      isSplay(m_lastInserted, newTeam.m_root) ==
          true &&           // Check if the tree maintains Splay properties
      isBst(newTeam.m_root) // Tree should maintain BST properties
  ) {
    stageTwo = true; // Second stage passed
  } else {
    return false; // If updated properties are incorrect, return false
  }

  // Return true if both stages are passed
  if (stageOne && stageTwo) {
    return true;
  }

  return false;
}

// Function: testRestructure(int size, TREETYPE type, TREETYPE changeType);
// Case(Normal): Test changing of tree type
// Expected result: Tree should resemble a new type of tree with new changetype
// properties
bool Tester::testRestructure(int size, TREETYPE type, TREETYPE changeType) {

  if (type == SPLAY) {
    // Generate a new tree with given size and type
    Swarm newTeam = generateTree(size, type);

    bool stageOne = false; // Flag for the first stage check

    // Check initial properties of the tree
    if (size == newTeam.countNodesinTree(
                    newTeam.m_root) && // Size should match the number of nodes
                                       // in the tree
        newTeam.m_type == type &&    // Tree type should match the initial type
        newTeam.m_root != nullptr && // Tree root should be populated
        isBst(newTeam.m_root) &&
        isSplay(m_lastInserted,
                newTeam.m_root) // Tree should maintain BST properties
    ) {
      stageOne = true; // First stage passed
    } else {
      return false; // If initial properties are incorrect, return false
    }

    bool stageTwo = false; // Flag for the second stage check

    newTeam.setType(changeType); // Change the tree type

    int insert = ((size + size) / 2); // Number of new nodes to insert
    // Reseed and insert new 10 random nodes
    Random idGen(MINID, MAXID);
    Random typeGen(0, 4);
    idGen.setSeed(time(0));

    int teamSize = insert;
    ROBOTTYPE roboType = DEFAULT_TYPE;

    for (int i = 0; i < teamSize; i++) {
      int ID = idGen.getRandNum();
      roboType = static_cast<ROBOTTYPE>(typeGen.getRandNum());
      Robot robot(ID, roboType);
      newTeam.insert(robot); // Insert the robot into the team
      m_lastInserted = ID;   // Update the last inserted ID
    }

    // Check updated properties of the tree
    if ((size + insert) ==
            newTeam.countNodesinTree(
                newTeam.m_root) && // Updated size should be within threshold
                                   // with duplicates possible
        newTeam.m_type == changeType && // Tree type should match the new type
        newTeam.m_root != nullptr &&    // Tree root should be populated
        newTeam.isAvl(newTeam.m_root) ==
            true &&           // Check if the tree maintains Splay properties
        isBst(newTeam.m_root) // Tree should maintain BST properties
    ) {
      stageTwo = true; // Second stage passed
    } else {
      return false; // If updated properties are incorrect, return false
    }

    // Return true if both stages are passed
    if (stageOne && stageTwo) {
      return true;
    }
  }
  if (type == AVL) {

    // Generate a new tree with given size and type
    Swarm newTeam = generateTree(size, type);

    bool stageOne = false; // Flag for the first stage check

    // Check initial properties of the tree
    if (size == newTeam.countNodesinTree(
                    newTeam.m_root) && // Size should match the number of nodes
                                       // in the tree
        newTeam.m_type == type &&    // Tree type should match the initial type
        newTeam.m_root != nullptr && // Tree root should be populated
        isBst(newTeam.m_root) &&
        newTeam.isAvl(newTeam.m_root) // Tree should maintain BST properties
    ) {
      stageOne = true; // First stage passed
    } else {
      return false; // If initial properties are incorrect, return false
    }

    bool stageTwo = false; // Flag for the second stage check

    newTeam.setType(changeType); // Change the tree type

    int insert = (size + size) / 2; // Number of new nodes to insert

    // Reseed and insert new 10 random nodes
    Random idGen(MINID, MAXID);
    Random typeGen(0, 4);
    idGen.setSeed(time(0));

    int teamSize = insert;
    ROBOTTYPE roboType = DEFAULT_TYPE;

    for (int i = 0; i < teamSize; i++) {
      int ID = idGen.getRandNum();
      roboType = static_cast<ROBOTTYPE>(typeGen.getRandNum());
      Robot robot(ID, roboType);
      newTeam.insert(robot); // Insert the robot into the team
      m_lastInserted = ID;   // Update the last inserted ID
    }

    // Check updated properties of the tree
    if ((size + insert) ==
            newTeam.countNodesinTree(
                newTeam.m_root) && // Updated size should be within threshold of
                                   // duplicates possible
        newTeam.m_type == changeType && // Tree type should match the new type
        newTeam.m_root != nullptr &&    // Tree root should be populated
        isSplay(m_lastInserted, newTeam.m_root) ==
            true &&           // Check if the tree maintains Splay properties
        isBst(newTeam.m_root) // Tree should maintain BST properties
    ) {
      stageTwo = true; // Second stage passed
    } else {
      return false; // If updated properties are incorrect, return false
    }

    // Return true if both stages are passed
    if (stageOne && stageTwo) {
      return true;
    }
  }
  return false;
}

// Function: testSingleRemoveEdge(int size, TREETYPE type, TREETYPE changeType);
// Case(Normal): Test remove of a single node that makes node a empty tree
// Expected result: Tree should be an empty tree
bool Tester::testSingleRemoveEdge(int size, TREETYPE type) {

  Swarm newTeam = generateTree(size, type);

  // Store the ID of the current root node.
  //  The root node's ID is stored in `oldroot` to use it later for removal and
  //  validation.
  int oldroot = newTeam.m_root->getID();

  // Remove the old root node from the tree.
  newTeam.remove(oldroot);

  if ((size - 1) ==
          newTeam.countNodesinTree(
              newTeam.m_root) && // Check if the tree now has one fewer node.
      newTeam.m_type == type &&  // Verify that the tree type remains the same.
      newTeam.m_root == nullptr && // Ensure the tree's root is not `nullptr`.
      isBst(newTeam.m_root) && // Confirm that the tree still follows BST rules.
      newTeam.findNode(newTeam.m_root, oldroot) ==
          false // Ensure the old root node no longer exists in the tree.
  ) {

    return true;
  }
  return false;
}
// Helper Function: areIdentical(int size, TREETYPE type, TREETYPE changeType);
// Case(Normal): Check if two node are the same
// Expected result: Two nodes should be the same
bool Tester::areIdentical(Robot const *orig, Robot const *copy) {

  // If both original and copy nodes are null, they are identical
  if (orig == nullptr && copy == nullptr) {
    return true;
  }

  // If one is null and the other is not, they are not identical
  if (orig == nullptr || copy == nullptr) {
    return false;
  }

  // If the IDs or heights of the nodes do not match, they are not identical
  if (orig->getID() != copy->getID() &&
      orig->getHeight() != copy->getHeight()) {
    return false;
  }

  // Recursively check the left and right subtrees for identical structure and
  // data
  return areIdentical(orig->getLeft(), copy->getLeft()) &&
         areIdentical(orig->getRight(), copy->getRight());
}

// Function: testOverloadedNormal(const Swarm &orig);
// Case(Normal): Create a new object with the exame data as original
// Expected result: new object should be the same as old object
bool Tester::testOverloadedNormal(const Swarm &orig) {

  // Create a new Swarm object
  Swarm newTeam;

  // Use the overloaded assignment operator to copy the original tree to the new
  // one
  newTeam = orig;

  // Count the number of nodes in both the original and the copied tree
  int sizeOrig = newTeam.countNodesinTree(orig.m_root);
  int sizeCopy = newTeam.countNodesinTree(newTeam.m_root);

  // Check if the copied tree has the same number of nodes as the original
  if (sizeCopy == sizeOrig &&             // Ensure sizes match
      newTeam.m_type == orig.getType() && // Ensure types match
      newTeam.m_root->getID() ==
          orig.m_root->getID() && // Ensure root IDs match
      isBst(
          newTeam.m_root) && // Ensure the copied tree maintains BST properties
      areIdentical(orig.m_root,
                   newTeam.m_root) // Ensure both trees are identical
  ) {
    return true;
  }

  return false;
}

// Function: testOverloadedError(const Swarm &orig);
// Case(Error): Attempt to check if empty object is copied
// Expected result: new object should be the same as old object
bool Tester::testOverloadedError(const Swarm &orig) {

  // Create a new Swarm object
  Swarm newTeam;

  // Use the overloaded assignment operator to copy the original tree to the new
  // one
  newTeam = orig;

  // Count the number of nodes in both the original and the copied tree
  int sizeCopy = newTeam.countNodesinTree(newTeam.m_root);
  int sizeOrig = newTeam.countNodesinTree(orig.m_root);
  // Check if the copied tree has the same number of nodes as the original
  if (sizeCopy == sizeOrig &&             // Ensure sizes match
      newTeam.m_type == orig.getType() && // Ensure types match
      newTeam.m_root == nullptr &&
      isBst(
          newTeam.m_root) && // Ensure the copied tree maintains BST properties
      areIdentical(orig.m_root,
                   newTeam.m_root) &&  // Ensure both trees are identical
      newTeam.isAvl(newTeam.m_root)) { //
    return true;
  }
  return false;
}

// Helper Function: calculateTestHeight(const Swarm &orig);
// Expected result: return max value height
int Tester::calculateTestHeight(Robot *node) {

  if (node == nullptr) {
    return -1;
  }

  // get heights of each subtree
  int leftHeight = calculateTestHeight(node->getLeft());
  int rightHeight = calculateTestHeight(node->getRight());

  // return height plus one
  return 1 + max(leftHeight, rightHeight);
}

// Function: testHeightsValues(int size, TREETYPE type);
// Expected result: return boolean of comparison of heights if valid or not
bool Tester::testHeightsValues(int size, TREETYPE type) {

  Swarm newTeam = generateTree(size, type);

  if (newTeam.m_root == nullptr) {
    return true; // Empty tree is always correct
  }

  // Get the height using the helper function
  int calculatedHeight = calculateTestHeight(newTeam.m_root);

  // Compare it with the height stored in the node
  if (newTeam.m_root->getHeight() != calculatedHeight) {
    return false; // Heights do not match
  }

  // Recursively test the left and right subtrees
  return calculateTestHeight(newTeam.m_root->getLeft()) &&
         calculateTestHeight(newTeam.m_root->getRight());
}

// Function: testHeightsInAVLInserts(int size, TREETYPE type);
// Expected result: check heights of avl tree should be valid
bool Tester::testHeightsInAVLInserts(int size, TREETYPE type) {

  Swarm newTeam = generateTree(size, type);

  if (newTeam.m_root == nullptr) {
    return true; // Empty tree is always correct
  }

  // Get the height using the helper function
  int calculatedHeight = calculateTestHeight(newTeam.m_root);

  // Compare it with the height stored in the node
  if (newTeam.m_root->getHeight() != calculatedHeight) {
    return false; // Heights do not match
  }

  // Recursively test the left and right subtrees
  bool leftState = calculateTestHeight(newTeam.m_root->getLeft());
  bool rightState = calculateTestHeight(newTeam.m_root->getRight());

  if (size == newTeam.countNodesinTree(newTeam.m_root) &&
      newTeam.m_type == type && newTeam.isAvl(newTeam.m_root) == true &&
      isBst(newTeam.m_root) == true && (leftState == rightState)) {

    // If all conditions are met, the test passes
    return true;
  }

  // If any condition fails, the test fails
  return false;
}

// Function: testAVLMassiveInsertRemove(int size, TREETYPE type);
// Expected result: check properties of avl tree which should be valid
bool Tester::testAVLMassiveInsertRemove(int size, TREETYPE type) {

  // Generate a tree with the given size and type
  Swarm newTeam = generateTree(size, type);

  // Remove 150 nodes
  int removedCount = 0;
  Random idGen(MINID, MAXID);

  do {
    int removeID = idGen.getRandNum();
    // Check if the node exists before attempting removal
    if (newTeam.findNode(newTeam.m_root, removeID)) {
      newTeam.remove(removeID);
      removedCount++;
    }
  } while (removedCount < 150);

  //  Validate tree properties

  // Recalculate heights for verification
  int calculatedHeight = calculateTestHeight(newTeam.m_root);
  // Compare the recalculated height with the height stored in the root node
  if (newTeam.m_root->getHeight() != calculatedHeight) {
    return false; // Heights do not match
  }

  // Recursively validate the heights of the left and right subtrees
  bool leftState = calculateTestHeight(newTeam.m_root->getLeft());
  bool rightState = calculateTestHeight(newTeam.m_root->getRight());

  // Ensure all conditions are met
  if ((size - removedCount) ==
          newTeam.countNodesinTree(newTeam.m_root) && // Node count is correct
      newTeam.m_type == type &&                       // Tree type is correct
      newTeam.isAvl(newTeam.m_root) && // Tree maintains AVL properties
      isBst(newTeam.m_root) &&         // Tree maintains BST properties
      leftState == rightState // Left and right subtree heights are correct
  ) {
    return true;
  }

  return false;
}

// Function: testBSTMassiveInsertRemove(int size, TREETYPE type);
// Expected result: check properties of BST tree which should be valid
bool Tester::testBSTMassiveInsertRemove(int size, TREETYPE type) {

  // Generate a tree with the given size and type
  Swarm newTeam = generateTree(size, type);

  // Remove 150 nodes
  int removedCount = 0;
  Random idGen(MINID, MAXID);

  do {
    int removeID = idGen.getRandNum();
    // Check if the node exists before attempting removal
    if (newTeam.findNode(newTeam.m_root, removeID)) {
      newTeam.remove(removeID);
      removedCount++;
    }
  } while (removedCount < 150);

  //  Validate tree properties

  // Recalculate heights for verification
  int calculatedHeight = calculateTestHeight(newTeam.m_root);
  // Compare the recalculated height with the height stored in the root node
  if (newTeam.m_root->getHeight() != calculatedHeight) {
    return false; // Heights do not match
  }

  // Recursively validate the heights of the left and right subtrees
  bool leftState = calculateTestHeight(newTeam.m_root->getLeft());
  bool rightState = calculateTestHeight(newTeam.m_root->getRight());

  // Ensure all conditions are met
  if ((size - removedCount) ==
          newTeam.countNodesinTree(newTeam.m_root) && // Node count is correct
      newTeam.m_type == type &&                       // Tree type is correct
      isBst(newTeam.m_root) && // Tree maintains BST properties
      leftState == rightState  // Left and right subtree heights are correct
  ) {
    return true;
  }

  return false;
}

// Function: testSpecialNodeRemovals(int size, TREETYPE type);
// Expected result: Check removals of different node case removal scenarios
//  should still result in AVL properties
bool Tester::testSpecialNodeRemovals() {

  Swarm tree(AVL);
  tree.insert(Robot(50));
  tree.insert(Robot(30)); // Node with one child
  tree.insert(Robot(70));
  tree.insert(Robot(20));
  tree.insert(Robot(40));
  tree.insert(Robot(60));
  tree.insert(Robot(80)); // Node with two children

  // Remove node with one child (Node 30)
  tree.remove(30);
  bool isValidTree = isBst(tree.m_root) && tree.isAvl(tree.m_root);
  if (!isValidTree) {
    return false;
  }

  // Remove node with two children (Node 70)
  tree.remove(70);
  isValidTree = isBst(tree.m_root) && tree.isAvl(tree.m_root);
  if (!isValidTree) {
    return false;
  }

  // Remove root node (Node 50)
  tree.remove(50);
  isValidTree = isBst(tree.m_root) && tree.isAvl(tree.m_root);
  if (!isValidTree) {
    return false;
  }
  return true;
}

// Function: testTreePerformanceHeavyLoad(int size, TREETYPE type);
// Expected result: Result should still maintain properties to specific type of
// tree
bool Tester::testTreePerformanceHeavyLoad(int size, TREETYPE type) {

  if (type == AVL) {
    Swarm newTeam = generateTree(size, type);

    //  Validate tree properties

    // Recalculate heights for verification
    int calculatedHeight = calculateTestHeight(newTeam.m_root);
    // Compare the recalculated height with the height stored in the root node
    if (newTeam.m_root->getHeight() != calculatedHeight) {
      return false; // Heights do not match
    }

    // Recursively validate the heights of the left and right subtrees
    bool leftState = calculateTestHeight(newTeam.m_root->getLeft());
    bool rightState = calculateTestHeight(newTeam.m_root->getRight());

    // Check if the tree satisfies the following conditions:
    // 1. The number of nodes in the tree matches the expected size
    // 2. The tree's type matches the specified type
    // 3. The tree follows AVL properties (balanced)
    // 4. The tree maintains the Binary Search Tree (BST) property
    // 5. Check if all heights are accurate
    // Ensure all conditions are met
    if (newTeam.m_type == type &&        // Tree type is correct
        newTeam.isAvl(newTeam.m_root) && // Tree maintains AVL properties
        isBst(newTeam.m_root) &&         // Tree maintains BST properties
        leftState == rightState // Left and right subtree heights are correct
    ) {
      return true;
    }
    return false;

  } else if (type == BST) {
    Swarm newTeam = generateTree(size, type);

    //  Validate tree properties

    // Recalculate heights for verification
    int calculatedHeight = calculateTestHeight(newTeam.m_root);
    // Compare the recalculated height with the height stored in the root node
    if (newTeam.m_root->getHeight() != calculatedHeight) {
      return false; // Heights do not match
    }

    // Recursively validate the heights of the left and right subtrees
    bool leftState = calculateTestHeight(newTeam.m_root->getLeft());
    bool rightState = calculateTestHeight(newTeam.m_root->getRight());

    // Check if the tree satisfies the following conditions:
    // 1. The number of nodes in the tree matches the expected size
    // 2. The tree's type matches the specified type
    // 3. The tree follows AVL properties (balanced)
    // 4. The tree maintains the Binary Search Tree (BST) property
    // 5. Check if all heights are accurate
    // Ensure all conditions are met
    if (newTeam.m_type == type && // Tree maintains AVL properties
        isBst(newTeam.m_root) &&  // Tree maintains BST properties
        leftState == rightState   // Left and right subtree heights are correct
    ) {
      return true;
    }
    return false;
  } else if (type == SPLAY) {
    Swarm newTeam = generateTree(size, type);

    //  Validate tree properties

    // Recalculate heights for verification
    int calculatedHeight = calculateTestHeight(newTeam.m_root);
    // Compare the recalculated height with the height stored in the root node
    if (newTeam.m_root->getHeight() != calculatedHeight) {
      return false; // Heights do not match
    }

    // Recursively validate the heights of the left and right subtrees
    bool leftState = calculateTestHeight(newTeam.m_root->getLeft());
    bool rightState = calculateTestHeight(newTeam.m_root->getRight());

    // Check if the tree satisfies the following conditions:
    // 1. The number of nodes in the tree matches the expected size
    // 2. The tree's type matches the specified type
    // 3. The tree follows AVL properties (balanced)
    // 4. The tree maintains the Binary Search Tree (BST) property
    // 5. Check if all heights are accurate
    // Ensure all conditions are met
    if (newTeam.m_type == type && // Tree type is correct
        isSplay(m_lastInserted,
                newTeam.m_root) && // Tree maintains AVL properties
        isBst(newTeam.m_root) &&   // Tree maintains BST properties
        leftState == rightState    // Left and right subtree heights are correct
    ) {
      return true;
    }

    return false;
  }

  return false;
}

int main() {
  Tester aTester;
  cout << "\nTesting AVL Insert Normal" << endl;
  if (aTester.testAVLInsertNormal(50, AVL)) {
    cout << "Testing AVL Insert Normal case passed!: Tree correctly handles "
            "AVL insertions"
         << endl;
  } else {
    cout << "\nTesting AVL Insert Normal case failed!: " << endl;
    cout << "AVL properties not maintained after insertion " << endl;
  }

  cout << "\nTesting Splay Insert Normal" << endl;
  if (aTester.testSplayInsertNormal(20, SPLAY)) {
    cout << "Testing Splay Insert Normal case passed!: Tree correctly handles "
            "Splay insertions"
         << endl;
  } else {
    cout << "\nTesting Splay Insert Normal case failed!: " << endl;
    cout << "Splay properties not maintained after insertion " << endl;
  }

  cout << "\nTesting BST Insert Normal" << endl;
  if (aTester.testBSTInsertNormal(20, SPLAY)) {
    cout << "Testing BST Insert Normal case passed!: Tree correctly handles "
            "BST insertions"
         << endl;
  } else {
    cout << "\nTesting BST Insert Normal case failed!: " << endl;
    cout << "BST properties not maintained after insertion " << endl;
  }
  cout << "\nTesting AVL Insert Edge" << endl;
  if (aTester.testAVLInsertEdge(1, AVL)) {
    cout << "Testing AVL Insert Edge case passed!: Tree correctly handles AVL "
            "insertions with 1 insertion"
         << endl;
  } else {
    cout << "\nTesting AVL Insert Edge case failed!: " << endl;
    cout << "AVL properties not maintained after insertion " << endl;
  }
  cout << "\nTesting Splay Insert Edge" << endl;
  if (aTester.testSplayInsertEdge(1, SPLAY)) {
    cout << "Testing Splay Insert Edge case passed!: Tree correctly handles "
            "Splay insertions with 1 insertion"
         << endl;
  } else {
    cout << "\nTesting Splay Insert Edge case failed!: " << endl;
    cout << "Splay properties not maintained after insertion " << endl;
  }
  cout << "\nTesting BST Insert Edge" << endl;
  if (aTester.testBSTInsertEdge(1, BST)) {
    cout << "Testing BST Insert Edge case passed!: Tree correctly handles BST "
            "insertions with 1 insertion"
         << endl;
  } else {
    cout << "\nTesting BST Insert Edge case failed!: " << endl;
    cout << "BST properties not maintained after insertion " << endl;
  }
  cout << "\nTesting BST Insert Error Case(size 50, type None)" << endl;
  if (aTester.testBSTInsertError(50, NONE)) {
    cout << "Testing Clear Function Error case passed!: No memory inserts "
            "occured"
         << endl;
  } else {
    cout << "\nTesting BST Insert Error case failed!: " << endl;
    cout << "Memory was allocated" << endl;
  }
  cout << "\nTesting Splay Insert Error Case(size 50, type None)" << endl;
  if (aTester.testSplayInsertError(50, NONE)) {
    cout << "Testing Splay Insert Error case passed!: No memory inserts occured"
         << endl;
  } else {
    cout << "\nTesting Splay Insert Error case failed!: " << endl;
    cout << "Memory was allocated" << endl;
  }
  cout << "\nTesting AVL Insert Error Case(size 50, type None)" << endl;
  if (aTester.testAVLInsertError(50, NONE)) {
    cout << "Testing AVL Insert Error Case case passed!: No memory inserts "
            "occured"
         << endl;
  } else {
    cout << "\nTesting AVL Insert Error case failed!: " << endl;
    cout << "Memory was allocated" << endl;
  }
  cout << "\nTesting Clear Function Normal Case" << endl;
  if (aTester.testClearNormal(50, SPLAY)) {
    cout << "Testing Clear Function Normal case passed!: Tree memory/data has "
            "been deallocated"
         << endl;
  } else {
    cout << "\nTesting Clear Function Normal case failed!: " << endl;
    cout << "Memory not fully deleted " << endl;
  }
  cout << "\nTesting Clear Function Edge Case" << endl;
  if (aTester.testClearEdge(1, AVL)) {
    cout << "Testing Clear Function Edge case passed!: Tree memory/data has "
            "been deallocated after insert 1 node"
         << endl;
  } else {
    cout << "\nTesting Clear Function Edege case failed!: " << endl;
    cout << "Memory not fully deleted " << endl;
  }
  cout << "\nTesting Clear Function Error Case(O nodes)" << endl;
  if (aTester.testClearError(0, BST)) {
    cout << "Testing Clear Function Error case passed!: No memory to delete or "
            "Double free error"
         << endl;
  } else {
    cout << "\nTesting Clear Function Error case failed!: " << endl;
    cout << "Memory not fully deleted " << endl;
  }
  cout << "\nTesting Clear Function Edge Case(type-NONE)" << endl;
  if (aTester.testClearError(50, NONE)) {
    cout << "Testing Clear Function Edge case passed!: No memory to delete or "
            "Double free error"
         << endl;
  } else {
    cout << "\nTesting Clear Function Edge case failed!: " << endl;
    cout << "Memory not deleted " << endl;
  }
  cout << "\nTesting Remove Function Normal Case" << endl;
  if (aTester.testRemoveNormal(100, BST)) {
    cout << "Testing Remove Function Normal Case case passed!: Node "
            "succesfully removed"
         << endl;
  } else {
    cout << "\nTesting Remove Function Normal Case case failed!: " << endl;
    cout << "Node still present " << endl;
  }
  cout << "\nTesting Remove Function Edge Case- removing Root" << endl;
  if (aTester.testRemoveEdge(75, AVL)) {
    cout << "Testing Remove Function Edge Case case passed!: Root node "
            "succesfully removed"
         << endl;
  } else {
    cout << "\nTesting Remove Function Edge Case case failed!: " << endl;
    cout << "m_root still present or memory leaks " << endl;
  }
  cout << "\nTesting Remove Function Edge Case- removing Node results in empty "
          "tree"
       << endl;
  if (aTester.testSingleRemoveEdge(1, BST)) {
    cout << "Testing Remove Function Edge Case case passed!: Root node "
            "succesfully removed"
         << endl;
  } else {
    cout << "\nTesting Remove Function Edge Case case failed!: " << endl;
    cout << "m_root still present or memory leaks " << endl;
  }
  cout << "\nTesting Remove Function Error Case- removing node that does not "
          "exist"
       << endl;
  if (aTester.testRemoveError(500, SPLAY, 111111)) {
    cout << "Testing Remove Function Edge Case case passed!:Nothing was not "
            "removed"
         << endl;
  } else {
    cout << "\nTesting Remove Function Error Case case failed!: " << endl;
  }
  cout << "\nTesting SetType Normal Case- Change from BST to Splay" << endl;
  if (aTester.testSetTypeNormal(15, BST, SPLAY)) {
    cout << "Testing Remove SetType Normal Normal  case passed!: Tree "
            "successfuly changed"
         << endl;
  } else {
    cout << "\nTesting SetType Normal Normal Case case failed!: " << endl;
    cout << "\n Tree is not of changetype" << endl;
  }
  cout
      << "\nTesting Restructure in SetType (edge) case Change from SPLAY to AVL"
      << endl;
  if (aTester.testRestructure(100, SPLAY, AVL)) {
    cout << "Testing Restructure in SetType (edge) case passed!: Tree "
            "successfuly changed"
         << endl;
  } else {
    cout << "\nTesting Remove Function Error Case case failed!: " << endl;
    cout << "\n Tree is not of changetype" << endl;
  }
  cout
      << "\nTesting Restructure in SetType (edge) case Change from AVL to SPLAY"
      << endl;
  if (aTester.testRestructure(100, AVL, SPLAY)) {
    cout << "Testing Remove Function Edge Case case passed!: Tree successfuly "
            "changed"
         << endl;
  } else {
    cout << "\nTesting Remove Function Error Case case failed!: " << endl;
    cout << "\n Tree is not of changetype" << endl;
  }

  Swarm newCopyTeam = aTester.generateTree(50, BST);
  cout << "\nTesting Overloaded Normal case" << endl;
  if (aTester.testOverloadedNormal(newCopyTeam)) {
    cout << "Testing Overloaded Normal case passed!: Tree is an exact copy"
         << endl;
  } else {
    cout << "\nTesting Overloaded Normal case failed!: " << endl;
    cout << "\n Tree is not an exact copy" << endl;
  }

  Swarm newCopyTeam1;
  cout << "\nTesting Overloaded Error case" << endl;
  if (aTester.testOverloadedError(newCopyTeam1)) {
    cout << "Testing Overloaded Error case passed!: Tree did not copy empty "
            "object"
         << endl;
  } else {
    cout << "\nTesting Overloaded Normal Error Case case failed!: " << endl;
    cout << "\n Tree copied empty tree" << endl;
  }

  cout << "\nTesting calculation of Height Values of Splay" << endl;
  if (aTester.testHeightsValues(200, SPLAY)) {
    cout << "Testing calculation of height values in Splay case passed!: All "
            "heights are accurate"
         << endl;
  } else {
    cout << "\nTesting alculation of height values case failed!: " << endl;
    cout << "\n Hieghts are not valid" << endl;
  }

  cout << "\nTesting calculation of Height Values of AVL after numerous "
          "Insertions"
       << endl;
  if (aTester.testHeightsInAVLInserts(300, AVL)) {
    cout << "Testing calculation of height values case passed!: All heights "
            "are accurate"
         << endl;
  } else {
    cout << "\nTesting alculation of height values case failed!: " << endl;
    cout << "\n Hieghts are not valid" << endl;
  }

  cout << "\nTesting AVL after numerous Insertions & Removals" << endl;
  if (aTester.testAVLMassiveInsertRemove(300, AVL)) {
    cout << "Testing calculation of height values case passed!: All heights "
            "are accurate"
         << endl;
  } else {
    cout << "\nTesting alculation of height values case failed!: " << endl;
    cout << "\n Hieghts are not valid" << endl;
  }

  cout << "\nTestingBST after numerous Insertions & Removals" << endl;
  if (aTester.testBSTMassiveInsertRemove(300, BST)) {
    cout << "Testing AVL massive insert and Remove case passed!: Tree is "
            "accurate"
         << endl;
  } else {
    cout << "\nTesting AVL massive insert and Remove case failed!: " << endl;
    cout << "\n Tree is not valid" << endl;
  }

  cout << "\nTesting AVL with special node removals" << endl;
  if (aTester.testSpecialNodeRemovals()) {
    cout << "Testing AVL with special node removals case passed!: Tree "
            "maintains properties"
         << endl;
  } else {
    cout << "\nTesting AVL with special node removals case failed!: " << endl;
    cout << "\n Tree properties not maintained" << endl;
  }

  cout << "\nTesting AVL with massive load" << endl;
  if (aTester.testTreePerformanceHeavyLoad(5000, AVL)) {
    cout << "Testing AVL with massive load case passed!: Tree maintains "
            "properties"
         << endl;
  } else {
    cout << "\nTesting AVL with special node removals case failed!: " << endl;
    cout << "\n Tree properties not maintained" << endl;
  }

  cout << "\nTesting BST with massive load" << endl;
  if (aTester.testTreePerformanceHeavyLoad(5000, BST)) {
    cout << "Testing BST with massive load case passed!: Tree maintains "
            "properties"
         << endl;
  } else {
    cout << "\nTesting BST with special node removals case failed!: " << endl;
    cout << "\n Tree properties not maintained" << endl;
  }

  cout << "\nTesting SPLAY with massive load" << endl;
  if (aTester.testTreePerformanceHeavyLoad(5000, SPLAY)) {
    cout << "Testing SPLAY with massive load case passed!: Tree maintains "
            "properties"
         << endl;
  } else {
    cout << "\nTesting SPLAY with special node removals case failed!: " << endl;
    cout << "\n Tree properties not maintained" << endl;
  }

  return 0;
}