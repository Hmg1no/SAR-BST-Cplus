# SAR-BST-Cplus
This class project includes a design and implementation of a storage mechanism meant to hold information of all robots during the operations. The data structure used to manage the information of each and every robot in this design is a Binary Search Tree, AVL, or Splay tree. This implementation was developed utilizing C++.


Objective: 

-Implementing a balanced binary search tree (BST) data structure.
-Practice writing rebalancing routines for an AVL tree.
-Practice writing re-structuring routines for a Splay tree.
-Practice use of recursion in programs.
-Practice writing unit tests.

Assignment:
  For this project, you are provided with the skeleton .h and .cpp files and a 
 sample driver:

  -swarm.h – Interface for the Robot and Swarm classes.
  -swarm.cpp – A skeleton for the implementation of the class Swarm.
  -driver.cpp – A sample driver program. (Note: this file is provided to show typical usage. Please note, the randomly generated values are different on different platforms.)

Specifications:
  -This project has three classes: Random, Robot, and Swarm. The Random class is       provided as a supplementary class to facilitate the testing. The Robot class         defines the nodes in the binary tree. The Swarm class is the one that implements     the balanced binary search tree. This class allows for creating three different      types of BST. The three types are a regular BST with no balancing, an AVL tree,      and a Splay tree

Class Swarm:
  -The Swarm class implements a binary search tree. The Swarm::m_root member           variable points to a node of type Robot. Every node in Swarm is a Robot object.      The nodes are organized as a binary search tree. The Swarm class supports the        insertion and deletion operations. After insertion or deletion operations the        class checks for any required property such as an imbalance and it performs the      restructuring operations.


Swarm::Swarm():

The default constructor performs the required initializations. It creates an empty object. It also sets the type of the tree to NONE.

Swarm::Swarm(TREETYPE type):

The constructor performs the required initializations. It creates an empty object. It also specifies the type of the tree. The tree can be a regular BST which does not perform any re-structuring. It can be an AVL tree which re-balances the tree after every insertion or removal. The third type is a Splay tree which splays the accessed node to the tree root.

Swarm::~Swarm():

The destructor performs the required cleanup including memory deallocations and re-initializing.


void Swarm::insert(const Robot& robot):

This function inserts a Robot object into the tree in the proper position. The Robot::m_id should be used as the key to traverse the Swarm tree and abide by BST traversal rules. The comparison operators (>, <, ==, !=) work with the int type in C++. A Robot id is a unique number in the range MINID - MAXID. We do not allow a duplicate id or an object with invalid id in the tree.
Note:
In the Swarm tree data structure every node is a Robot object which is represented as a pointer to the Robot object. Therefore, for every insertion we need to allocate memory and use the information of robot to initialize the new node. Memory allocation takes place in the Swarm class.
If the tree type is BST, after an insertion, we should update the height for all nodes in the insertion path.
If the tree type is AVL, after an insertion, we should update the height of each node in the insertion path as well as check for an imbalance at each node in this path.
If the tree type is SPLAY, after and insertion, we need to splay the inserted node and bring it to the root of the tree while the tree preserves the BST property as well as updating the node heights.

void Swarm::clear():

The clear function deallocates all memory in the tree and makes it an empty tree.

void Swarm::remove(int id):

The remove function traverses the tree to find a node with the id and removes it from the tree. If the tree type is SPLAY, the remove function does not remove the node. In the case of BST or AVL tree the remove function should also update the heights for all nodes in the removal path.

void Swarm::setType(TREETYPE type):

This function sets the type of an existing Swarm object. Once the type is changed, the function should re-structure the tree according to the following rules:
If the type is changed from BST or SPLAY to AVL, the function should reconstruct the tree as an AVL tree. In the case of reconstruction the nodes are transferred from the old tree to the new tree. There should not be any reallocation of memory.
If the type is changed from AVL to BST or Splay, there is no need for reconstruction. After change the tree operations will perform according to the new type.
Any changes between BST and SPLAY types will not trigger a reconstruction. After change the tree operations will perform according to the new type.
Any changes to NONE will clear the tree and removes all the nodes.

TREETYPE Swarm::getType() const:

This function returns the current type of the tree.

const Swarm & Swarm::operator=(const Swarm & rhs):

This function overloads the assignment operator for the class Swarm. It creates an exact deep copy of the rhs.
  
