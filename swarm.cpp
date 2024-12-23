/***********************************************************
 ** // UMBC - CMSC 341 - Fall 2024 - Proj2
 ** File:    Swarm.cpp
 ** Project: Fall 2024 - Proj2
 ** Author:  Hazael Magino
 ** Date:    10/3/2022
 ** This file contains the proper implementations for swarm.cpp
 **********************************************************/
#include "swarm.h"

// Name: Swarm() - Default Constructor
// Desc: Used to build a new empty Swarm object (for invalid parameters)
// parameters: not assigned
// Preconditions: None
// Postconditions: Creates a new empty swarm object with a default values of
// nullptrs and type of tree to none
Swarm::Swarm() {
  m_root = nullptr;
  m_type = NONE;
}

// Name: Swarm() - Constructor
// Desc: Used to build a new empty Swarm object
// parameters: TREETYPE - Type of BST being built
// Preconditions: None
// Postconditions: Creates a new empty swarm obj5ect with a default values of
// nullptrs and the BST type
Swarm::Swarm(TREETYPE type) {
  m_type = type;
  m_root = nullptr;
}
// Name: Swarm() - Destructor
// Desc: The destructor performs the required cleanup including memory
// deallocations and re-initializing. Preconditions: Bobot BST must exist
// Postconditions: Dynamic memory is cleared, member variables to default values
Swarm::~Swarm() {
  clear();
  m_type = NONE;
  m_root = nullptr;
}
// Name: Clear()
// Desc: Deallocates all memory in the tree and makes it an empty tree.
// parameters: NONE
// Preconditions: Robot BST must exist
// Postconditions: Dynamic memory is cleared, BST is now an empty object
void Swarm::clear() {
  destruct(m_root);
  m_root = nullptr;
}

// Name: Destruct()
// Desc: Helper function that helps recursively delete robots in BST
// parameters: Robot pointer
// Preconditions: Robot BST must exist
// Postconditions: Dynamic memory is cleared, BST is now an empty object
void Swarm::destruct(Robot *robot) {

  // nullptr is base case
  if (robot != nullptr) {
    // post-order traversal deletion
    // must locate child robot
    destruct(robot->m_left);
    destruct(robot->m_right);
    delete robot;
    robot = nullptr;
  }
}

// Name: findNode()
// Desc: //helper function to make sure there the node is present in tree
// parameters: Robot pointer
// Preconditions: Robot BST must exist
// Postconditions: boolean value is returned if found or doesn't exist
bool Swarm::findNode(Robot *Node, int id) {

  if (Node == nullptr) {
    return false;
  } else if (Node->getID() == id) {
    return true;
  }

  // if id is greater go to the right else go left
  if (id > Node->getID()) {

    // recurse down the tree untill node is found or ifmit is nullptr
    return findNode(Node->getRight(), id);
  } else {
    return findNode(Node->getLeft(), id);
  }
}

// Name: findMinTrav()
// Desc: helper to find the min value of a subtree
// parameters: Robot pointer
// Preconditions: Robot BST must exist
// Postconditions: Pointer of the least value is returned
Robot *Swarm::findMinTrav(Robot *aNode) {

  if (aNode == nullptr || aNode->getLeft() == nullptr) {
    return aNode;
  } else {

    // Traverse to the least node
    return findMin(aNode->getLeft());
  }
}

// Name: findMin()
// Desc: helper to find the min value of a subtree
// parameters: Robot pointer
// Preconditions: Robot BST must exist
// Postconditions: Pointer of the least value is returned
Robot *Swarm::findMin(Robot *Node) { return findMinTrav(Node); }

// Name: remove()
// Desc: function that will remove node in a tree
// parameters: id of robot
// Preconditions: Robot BST must exist
// Postconditions: Id of node is removed from tree
void Swarm::remove(int id) {

  bool exist = findNode(m_root, id);

  if (exist == true) {

    m_root = removeNode(id, m_root);
  }
}

// Name: remove()
// Desc: helper function of remove to recuse and remove node from a tree
// parameters: id of robot, a pointer of where to go
// Preconditions: Robot BST must exist
// Postconditions: Id of node is removed from tree
Robot *Swarm::removeNode(int id, Robot *aNode) {
  if (aNode == nullptr) {
    return aNode; // Base case: Node not found
  } else if (id < aNode->getID()) {
    aNode->m_left = removeNode(id, aNode->m_left); // Go left
  } else if (id > aNode->getID()) {
    aNode->m_right = removeNode(id, aNode->m_right); // Go right
  } else if (aNode->m_left != nullptr &&
             aNode->m_right != nullptr) { // Found the node to delete
    // Case 2: Node with two children
    Robot *temp = findMin(
        aNode->m_right); // Find the smallest in node in the right subtree
    aNode->setID(temp->getID()); // Copy the data from the previous node
    aNode->m_right = removeNode(temp->getID(),
                                aNode->m_right); // remove temp (now child node)

  } else {
    // hold the node to delete
    Robot *temp = aNode;

    // move Node to not lost other nodes
    if (aNode->getLeft() == nullptr) {
      aNode = aNode->getRight();
    } else {
      aNode = aNode->getLeft();
    }

    // delete node that was held
    delete temp;
  }

  // Update heights after deletion
  reAdjustHeight(aNode);

  // If it's an AVL tree, check for imbalance and fix it
  if (m_type == AVL) {
    CheckBalance(aNode);
  }
  // after everything has been done return pointer
  return aNode;
}

// Name: insert()
// Desc: function that inserts nodes based on the type of tree
// parameters: Robot pointer
// Preconditions: m_type must be defined
// Postconditions: Node is inserted into specific data type
void Swarm::insert(const Robot &robot) {

  if (m_type != NONE) {
    // check if ID exists (must iterate tree to make sure it does not already
    // exist) false means it doesn't exist, true means the opposite
    bool doesExist = duplciateId(m_root, robot.getID());

    if (m_type == AVL) {

      if (doesExist != true) {

        // call recursive function for insertion
        addInAVL(m_root, robot);
      }

    } else if (m_type == SPLAY) {

      if (doesExist != true) {

        // call recursive function for insertion
        addInSplay(m_root, robot);
      }

    } else if (m_type == BST) {

      if (doesExist != true) {
        // call recursive function for insertion
        addInBST(m_root, robot);
      }
    } else {
      return;
    }
  }
}
// Name: duplciateId()
// Desc: helper function to identify if node already exists in tree
// parameters: Robot pointer, ID of node to find
// Preconditions: tree must exist
// Postconditions: Boolean value is returned: true - duplicate, false- unique
bool Swarm::duplciateId(Robot *Node, int id) const {

  if (Node == nullptr) {
    return false;
  } else if (Node->getID() == id) {
    return true;
  }

  // if id is greater go to the right else go left
  if (id > Node->getID()) {

    // recurse down the tree untill node is found or it is nullptr
    return duplciateId(Node->getRight(), id);
  } else {
    return duplciateId(Node->getLeft(), id);
  }
}

// Name: addInSplay()
// Desc: helper function to recurse and insert node in splay tree
// parameters: Robot pointer, and Node itself
// Preconditions: tree must exist
// Postconditions: Node is added to splay tree
void Swarm::addInSplay(Robot *&pointer, const Robot &aNode) {

  // first detirmine if the tree is empty
  if (pointer == nullptr) {

    // if we have reached the bottom of the tree allocate memory for new object
    pointer = new Robot(aNode.getID(), aNode.getType(), aNode.getState());
    pointer->setRight(nullptr);
    pointer->setLeft(nullptr);
    return;
  } else if (aNode.getID() < pointer->getID()) {

    // iterate the tree to find accurate positioning
    addInSplay(pointer->m_left, aNode);
  } else if (aNode.getID() > pointer->getID()) {

    // iterate the tree to find accurate positioning
    addInSplay(pointer->m_right, aNode);
  }

  pointer->setHeight(getHighestH(pointer));

  // splay recently added node
  splay(pointer, aNode.getID());
}

// Name: splay()
// Desc: helper function to recurse and splay tree as needed
// parameters: Robot pointer, and id of node
// Preconditions: tree must exist
// Postconditions: Node is splayed to m_root
void Swarm::splay(Robot *&Node, int id) {

  // Base case: if Node is null or the key is found, return
  if (Node == nullptr || Node->getID() == id) {
    return;
  }

  // If the key lies in the left subtree
  if (Node->getID() > id) {
    // If there is no left child, the key is not in the tree
    if (Node->getLeft() == nullptr) {
      return;
    }

    // Zig-Zig (Left Left) case
    if (Node->getLeft()->getID() > id) {
      // Recur for the left subtree of the left child
      splay(Node->m_left->m_left, id);
      // Perform a single right rotation
      Node = rotateRight(Node);

    }
    // Zig-Zag (Left Right) case
    else if (Node->getLeft()->getID() < id) {
      // Recur for the right subtree of the left child
      splay(Node->m_left->m_right, id);
      // If the left child's right child is not null, perform a left rotation
      if (Node->getLeft()->getRight() != nullptr) {
        Node = rotateLeft(Node->m_left);
      }
    }

    // Perform a second right rotation if the left child is not null
    if (Node->getLeft() != nullptr) {
      Node = rotateRight(Node);
    }
  }
  // If the key lies in the right subtree
  else {
    // If there is no right child, the key is not in the tree
    if (Node->getRight() == nullptr) {
      return;
    }

    // Zag-Zig (Right Left) case
    if (Node->getRight()->getID() > id) {
      // Recur for the left subtree of the right child
      splay(Node->m_right->m_left, id);
      // If the right child's left child is not null, perform a right rotation
      if (Node->getRight()->getLeft() != nullptr) {
        Node = rotateRight(Node->m_right);
      }
    }
    // Zag-Zag (Right Right) case
    else if (Node->getRight()->getID() < id) {
      // Recur for the right subtree of the right child
      splay(Node->m_right->m_right, id);
      // Perform a single left rotation
      Node = rotateLeft(Node);
    }

    // Perform a second left rotation if the right child is not null
    if (Node->getRight() != nullptr) {
      Node = rotateLeft(Node);
    }
  }
}

// Name: addInBST()
// Desc: helper function to recurse and insert node in BST tree
// parameters: Robot pointer, and Node itself
// Preconditions: tree must exist
// Postconditions: Node is added to BST tree
void Swarm::addInBST(Robot *&pointer, const Robot &robot) {

  // first detirmine if tree is empty, thus we add in a new node at m_root
  if (pointer == nullptr) {
    // if we have reached the bottom of the tree allocate memory for new object
    pointer = new Robot(robot.getID(), robot.getType(), robot.getState());
    pointer->setRight(nullptr);
    pointer->setLeft(nullptr);
    return;

  } else if (robot.getID() < pointer->getID()) {

    // iterate the tree to find accurate positioning
    addInBST(pointer->m_left, robot);
  } else if (robot.getID() > pointer->getID()) {

    // iterate the tree to find accurate positioning
    addInBST(pointer->m_right, robot);
  }

  pointer->setHeight(getHighestH(pointer));
}

// Name: addInAVL()
// Desc: Helper function to recurse and insert node in AVL tree
// parameters: Robot pointer, and Node itself
// Preconditions: tree must exist
// Postconditions: Node is added to AVL tree
void Swarm::addInAVL(Robot *&pointer, const Robot &robot) {

  // first detirmine if tree is empty
  if (pointer == nullptr) {

    // if we have reached the bottom of the tree allocate memory for new object
    pointer = new Robot(robot.getID(), robot.getType(), robot.getState());
    pointer->setRight(nullptr);
    pointer->setLeft(nullptr);
    return;

  } else if (robot.getID() < pointer->getID()) {

    // iterate the tree to find accurate positioning
    addInAVL(pointer->m_left, robot);
  } else if (robot.getID() > pointer->getID()) {

    // iterate the tree to find accurate positioning
    addInAVL(pointer->m_right, robot);
  }

  // set the new height
  pointer->setHeight(getHighestH(pointer));

  CheckBalance(pointer);
}

// Name: getHighestH()
// Desc: Helper function to detirmine AVL balance
// parameters: Robot pointer to current node
// Preconditions: tree must exist
// Postconditions: Return the higher hieght of either child Nodes
int Swarm::getHighestH(Robot *pointer) {

  // If the node has no children, return 0 (indicating leaf node)
  if (pointer == nullptr) {
    return -1;
  } else if (pointer->getRight() == nullptr && pointer->getLeft() == nullptr) {
    return 0;
  }
  // If the node has no right child but has a left child, return the height of
  // the left child
  else if (pointer->getRight() == nullptr && pointer->getLeft() != nullptr) {
    return pointer->m_left->getHeight() + 1;
  }
  // If the node has no left child but has a right child, return the height of
  // the right child
  else if (pointer->getRight() != nullptr && pointer->getLeft() == nullptr) {
    return pointer->m_right->getHeight() + 1;
  }
  // If the node has both children, return the height of the higher child
  else if (pointer->m_left->getHeight() > pointer->m_right->getHeight()) {
    return pointer->m_left->getHeight() + 1;
  }
  // If the right child is taller than the left child, return the height of the
  // right child
  else if (pointer->m_right->getHeight() > pointer->m_left->getHeight()) {
    return pointer->m_right->getHeight() + 1;
  }

  return pointer->getHeight();
}

// Name: treeImbalance()
// Desc: Detirmines if the nodes are imbalanced
// parameters: Robot pointer to current node
// Preconditions: tree must exist
// Postconditions: Return the difference in height between a nodes child
int Swarm::treeImbalance(Robot *Node) {
  // Check if the node is nullptr (base case for an empty node)
  if (Node == nullptr) {
    return -1; // Return -1 to indicate there is no node (height of an empty
               // tree)
  } else {
    // Initialize heights of the right and left subtrees
    int rightH = -1; // Default height for right subtree
    int leftH = -1;  // Default height for left subtree

    // Check if the right child exists and retrieve its height
    if (Node->getRight() != nullptr) {
      rightH =
          Node->getRight()->getHeight(); // Get the height of the right child
    }

    // Check if the left child exists and retrieve its height
    if (Node->getLeft() != nullptr) {
      leftH = Node->getLeft()->getHeight(); // Get the height of the left child
    }

    // Calculate and return the balance factor of the node
    // This is the difference in height between the left and right subtrees
    return leftH - rightH; // Positive value indicates left-heavy, negative
                           // indicates right-heavy
  }
}

// Name: CheckBalance()
// Desc: Helper function to detirmine rotation if AVL tree is unbalanced
// parameters: Robot pointer
// Preconditions: tree must exist
// Postconditions: Call necesary rotation to balance AVL Tree
void Swarm::CheckBalance(Robot *&Node) {
  // If the node is nullptr, there's nothing to balance
  if (Node == nullptr) {
    return;
  }

  // Calculate the balance factor
  int balance = treeImbalance(Node);

  // If the node is left heavy
  if (balance > 1) {
    int childBalance = treeImbalance(Node->getLeft());

    // Left-Right Case: left child's right subtree is heavier
    if (childBalance < 0) {
      Node->setLeft(
          rotateLeft(Node->getLeft())); // First, rotate left on the left child
    }
    // Now perform right rotation on the node
    Node = rotateRight(Node);
  }
  // If the node is right heavy
  else if (balance < -1) {
    int childBalance = treeImbalance(Node->getRight());

    // Right-Left Case: If right child's left subtree is heavier
    if (childBalance > 0) {
      Node->setRight(rotateRight(
          Node->getRight())); // First, rotate right on the right child
    }
    // Now perform left rotation on the node
    Node = rotateLeft(Node);
  }
}

// Name: rotateLeft()
// Desc: Helper function to perform Left rotation on node for AVL tree
// parameters: Robot pointer
// Preconditions: tree must exist
// Postconditions: Perform neccesary rotation on Node
Robot *Swarm::rotateLeft(Robot *newRoot) {

  // Make sure node is not null
  if (newRoot != nullptr && newRoot->getRight() != nullptr) {

    // z is the node to be rotated
    Robot *z = newRoot;

    // y is the right child of z which will become the new root after rotation
    Robot *y = z->getRight();

    Robot *x = y->getLeft();

    // Move y's left subtree to be the right subtree of z
    z->setRight(y->getLeft());

    // Make z the left child of y
    y->setLeft(z);

    // Update the root to be y
    newRoot = y;

    // Update the heights of the affected nodes
    reAdjustHeight(x);
    reAdjustHeight(z);
    reAdjustHeight(y);

    return newRoot;
  }

  // if the root does equal nullptr (ie we're in a situation wherein theres
  // nothing to rotate) we just return nullptr
  return nullptr;
}

// Name: rotateRight()
// Desc: Helper function to perform right rotation on node for AVL tree
// parameters: Robot pointer
// Preconditions: tree must exist
// Postconditions: Perform neccesary rotation on Node
Robot *Swarm::rotateRight(Robot *newRoot) {

  // Make sure the node to be rotated is not null
  if (newRoot != nullptr && newRoot->getLeft() != nullptr) {

    // Let z be the node to be rotated
    Robot *z = newRoot;

    // y is the left child of z which will become the new root after rotation
    Robot *y = z->getLeft();

    Robot *x = y->getRight();

    z->setLeft(y->getRight());

    // Make z the right child of y
    y->setRight(z);

    // Update the root to be y
    newRoot = y;

    // Update the heights of the affected nodes
    reAdjustHeight(x);
    reAdjustHeight(z);
    reAdjustHeight(newRoot);

    return newRoot;
  }

  // if the root does equal nullptr (ie we're in a situation wherein theres
  // nothing to rotate) we just return nullptr
  return nullptr;
}

// Name: reAdjustHeight()
// Desc: Helper function to readjust node height after rotation on node for AVL
// tree parameters: Robot pointer Preconditions: tree must exist Postconditions:
// Heights for node is updated
void Swarm::reAdjustHeight(Robot *Node) {

  if (Node == nullptr) {
    return;
  }
  if (Node != nullptr) {

    if (Node->getRight() == nullptr && Node->getLeft() == nullptr) {

      // Node has no children, must be a leaf node
      Node->setHeight(0);

    } else if (Node->getLeft() == nullptr && Node->getRight() != nullptr) {

      // take the height of the nodes right child since its the only child to
      // exist
      Node->setHeight(1 + Node->getRight()->getHeight());

    } else if (Node->getLeft() != nullptr && Node->getRight() == nullptr) {

      // take the height of the nodes right child since its the only child to
      // exist
      Node->setHeight(1 + Node->getLeft()->getHeight());
    } else {

      if (Node->getLeft()->getHeight() > Node->getRight()->getHeight()) {

        // if height of the left is greater, set height of the left + 1 to
        // current Node
        Node->setHeight(1 + Node->getLeft()->getHeight());

      } else {

        // else the rights child's height is greater than or equal to the right
        // child's height
        Node->setHeight(1 + Node->getRight()->getHeight());
      }
    }
  }
}

// Name: overloaded assignment()
// Desc: Make a deep copy of an existing tree
// parameters: RHS
// Preconditions: tree must exist
// Postconditions: A deep copy of tree is made
const Swarm &Swarm::operator=(const Swarm &rhs) {

  // protect against self-assignment
  if (this == &rhs) {
    return *this;
  }

  // deallocate memory for copy robot BST
  clear();

  if (rhs.m_root != nullptr) {

    // assign old member variable data to new member variable data
    m_type = rhs.m_type;
    m_root = new Robot(rhs.m_root->getID(), rhs.m_root->getType(),
                       rhs.m_root->getState());

    // call a recursive copy function to copy rest of tree
    copyTree(m_root, rhs.m_root);
  }

  m_type = rhs.m_type;

  // return copied tree
  return *this;
}

// Name:copyTree()
// Desc: Recursive function to help with deep copy of a tree
// parameters: new m_root, and old m_root from rhs
// Preconditions: rhs tree must exist
// Postconditions: A deep copy of tree is made
void Swarm::copyTree(Robot *newNode, const Robot *oldNode) {

  // base case, there is no node so return call;
  if (oldNode == nullptr) {
    newNode = nullptr;
    return;
  }

  // copy left tree of old tree
  if (oldNode->getLeft() != nullptr) {
    newNode->setLeft(new Robot(oldNode->getLeft()->getID(),
                               oldNode->getLeft()->getType(),
                               oldNode->getLeft()->getState()));

    copyTree(newNode->getLeft(), oldNode->getLeft());
  }

  // copy right tree of old tree
  if (oldNode->getRight() != nullptr) {
    newNode->setRight(new Robot(oldNode->getRight()->getID(),
                                oldNode->getRight()->getType(),
                                oldNode->getRight()->getState()));

    copyTree(newNode->getRight(), oldNode->getRight());
  }

  // adjust to new node height of the copy tree
  reAdjustHeight(newNode);
}

// Name: getType()
// Desc: Returns the type of tree object being currently used
// parameters: NONE
// Preconditions: Robot BST must exist
// Postconditions: Returns TREETYPE type of private variable m_type
TREETYPE Swarm::getType() const { return m_type; }

// Name: setType()
// Desc: set m_type private variable to a TREETYPE type
// parameters: TREETYPE type
// Preconditions: Valid TREETYPE type must be valid
// Postconditions: sets TREETYPE type of private variable m_type
void Swarm::setType(TREETYPE type) {

  TREETYPE oldType = getType();

  // if the type is the same as m_type there is no reason to change m_type or
  // restructure
  if (oldType != type) {

    switch (oldType) {

    case BST:

      if (type == AVL) {
        m_type = type;
        restructure(m_root);
      } else if (type == SPLAY) {
        m_type = type;
      } else if (type == NONE) {
        m_type = type;
        clear();
      }
      break;
    case AVL:

      if (type == BST) {
        m_type = type;
      } else if (type == SPLAY) {
        m_type = type;
      } else if (type == NONE) {
        m_type = type;
        clear();
      }
      break;
    case SPLAY:

      if (type == AVL) {
        m_type = type;
        restructure(m_root);
      } else if (type == NONE) {
        m_type = type;
        clear();
      } else if (type == BST) {
        m_type = type;
      }
      break;
    default:
      // assume m_type is NONE

      if (type == AVL) {
        m_type = type;
      } else if (type == SPLAY) {
        m_type = type;
      } else if (type == NONE) {
        m_type = type;
      }
      break;
    }
  }
}

// Name: flattenHelper()
// Desc: Helper function to flatten the BST into
// a singly linked list using in-order traversal
// parameters: Robot* Node, Robot* &prev
// Preconditions: tree must exist
// Postconditions: a singly linked list (still a tree) is created
void Swarm::flattenHelper(Robot *Node, Robot *&prev) {

  // base case
  if (Node == nullptr) {
    return;
  }

  // recursively travel the left subtree
  flattenHelper(Node->getLeft(), prev);

  // if there's a previous node, link to current node
  if (prev != nullptr) {
    prev->setRight(Node);
  } else {
    m_root = Node;
  }
  // Nullify the left pointer to form LL
  Node->setLeft(nullptr);

  // update the previous node to current
  prev = Node;

  // recursively travel the right subtree
  flattenHelper(Node->getRight(), prev);
}

// Name: flattenBST()
// Desc: function to convert tree to a LL
// parameters: Robot* node
// Preconditions: tree must exist
// Postconditions: a singly linked list (still a tree) is created
void Swarm::flattenBST(Robot *Node) {

  Robot *previous = nullptr;     // initalize node as null
  flattenHelper(Node, previous); // Call helper
}

// Name: countNodes()
// Desc: function to detirmine nodes in a tree
// parameters: Robot* node
// Preconditions: tree must exist
// Postconditions: number of nodes in tree is returned
int Swarm::countNodesinTree(Robot *root) {
  if (root == nullptr) {
    return 0;
  }

  int leftCount = countNodesinTree(root->getLeft());
  int rightCount = countNodesinTree(root->getRight());

  int totalCount = 1 + leftCount + rightCount;

  return totalCount;
}

// Function: isAVL(int);
// Function will verify if tree follows AVL properties
// Expected result: Tree heights only differ in 1 unit of height
bool Swarm::isAvl(Robot *start) { return traverseAvlNodes(start); }
// helper function for testing to count number of nodes in tree
bool Swarm::traverseAvlNodes(Robot *start) {

  if (start == nullptr) {
    return true;
  }

  if (treeImbalance(start) > 1 || treeImbalance(start) < -1) {
    return false;
  }

  // recursively checks the entire tree
  return traverseAvlNodes(start->m_left) and traverseAvlNodes(start->m_right);
}

// Name: countNodes()
// Desc: return count of nodes in tree
// parameters: Robot* node
// Preconditions: tree must exist
// Postconditions: Num of elements in tree
int Swarm::countNodes(Robot *start) {
  int count = 0;
  Robot *current = start;
  while (current != nullptr) {
    count++;
    current = current->getRight();
  }

  return count;
}

// Name: convertToAVL()
// Desc: convert BST into an AVL tree
// parameters: Robot* node, and Num of Nodes
// Preconditions: tree must exist
// Postconditions: Reconstructed AVL tree, should already have AVL balance
// properties from implementation
Robot *Swarm::convertToAVL(Robot *&Node, int Num) {

  if (Num <= 0) {
    return nullptr;
  }

  int LeftTreeElem = Num / 2;

  Robot *leftTree = convertToAVL(Node, LeftTreeElem);

  // Make middle node the root
  Robot *root = Node;

  // assign the left tree to the left of root
  root->setLeft(leftTree);

  // move to the next node
  Node = Node->getRight();

  int rightTreeElem = Num - LeftTreeElem - 1;

  Robot *rightTree = convertToAVL(Node, rightTreeElem);
  root->setRight(rightTree);

  // Adjust height of the root node immediately after setting left and right
  // children
  reAdjustHeight(root);

  return root;
}

// Name: restructure()
// Desc: convert BST into an AVL tree
// parameters: Robot* node
// Preconditions: tree must exist
// Postconditions: Reconstructed AVL tree
void Swarm::restructure(Robot *Node) {

  // resturctuing to AVL tree
  flattenBST(Node);
  int numN = countNodes(m_root);

  m_root = convertToAVL(m_root, numN);
}

void Swarm::dumpTree() const { dump(m_root); }
void Swarm::dump(Robot *aBot) const {
  if (aBot != nullptr) {
    cout << "(";
    dump(aBot->m_left);                          // first visit the left child
    cout << aBot->m_id << ":" << aBot->m_height; // second visit the node itself
    dump(aBot->m_right);                         // third visit the right child
    cout << ")";
  }
}