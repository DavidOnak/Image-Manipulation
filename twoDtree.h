#ifndef _TWODTREE_H_
#define _TWODTREE_H_

#include <utility>
#include "cs221util/PNG.h"
#include "cs221util/RGBAPixel.h"
#include "stats.h"
using namespace std;
using namespace cs221util;

/**
 * twoDtree: This is a structure used in decomposing an image
 * into rectangles of similarly colored pixels.
 */

class twoDtree {
private:

   /**
    * The Node class is private to the tree class via the principle of
    * encapsulation---the end user does not need to know our node-based
    * implementation details.
    */
   class Node {
   public:
      Node(pair<int,int> ul, pair<int,int> lr, RGBAPixel a); // Node constructor

      pair<int,int> upLeft;
      pair<int,int> lowRight;
      RGBAPixel avg;
      Node * left; // ptr to left subtree
      Node * right; // ptr to right subtree

   };

public:
   /**
    * twoDtree destructor.
    * Destroys all of the memory associated with the
    * current twoDtree. This function should ensure that
    * memory does not leak on destruction of a twoDtree.
    *
    * @see twoDtree.cpp
    */
   ~twoDtree();

   /**
    * Copy constructor for a twoDtree.
    * Since twoDtrees allocate dynamic memory (i.e., they use "new", we
    * must define the Big Three). 
    * @see twoDtree.cpp
    *
    * @param other The twoDtree  we are copying.
    */
   twoDtree(const twoDtree & other);

   /**
    * Overloaded assignment operator for twoDtrees.
    * Part of the Big Three that we must define because the class
    * allocates dynamic memory. 
    * @see twoDtree.cpp
    *
    * @param rhs The right hand side of the assignment statement.
    */
   twoDtree & operator=(const twoDtree & rhs);

   /**
    * Constructor that builds a twoDtree out of the given PNG.
    * Every leaf in the tree corresponds to a pixel in the PNG.
    * Every non-leaf node corresponds to a rectangle of pixels
    * in the original PNG, represented by an (x,y) pair for the
    * upper left corner of the rectangle and an (x,y) pair for
    * lower right corner of the rectangle. In addition, the Node
    * stores a pixel representing the average color over the
    * rectangle.
    *
    * Every node's left and right children correspond to a partition
    * of the node's rectangle into two smaller rectangles. The
    * current node's rectangle is split by a horizontal or vertical
    * line, depending on which direction corresponds to the node's
    * level in the tree. Even numbered levels will split vertically
    * and odd numbers will split horizontally.
    *
    * The best splitting line in either dimension, will be the
    * one that minimizes the sum of the variances of the two
    * resulting rectangles.
    *
    * The left child of the node will contain the upper left corner
    * of the node's rectangle, and the right child will contain the
    * lower right corner.
    *
   * This function will build the stats object used to score the
   * splitting lines. It will also call helper function buildTree.
    */
   twoDtree(PNG & imIn);

   /**
    * Render returns a PNG image consisting of the pixels
    * stored in the tree. It may be used on pruned trees. Draws
    * every leaf node's rectangle onto a PNG canvas using the
    * average color stored in the node.
    */
   PNG render();

   /*
    *  Prune function trims subtrees as high as possible in the tree.
    *  A subtree is pruned (cleared) if all of its leaves are within
    *  tol of the average color stored in the root of the subtree.
    *  Pruning criteria should be evaluated on the original tree, not
    *  on a pruned subtree.
    */
   void prune(int tol);

   /*
    * The pruneSize function takes a tolerance as input, and returns
    * the number of leaves that would result if the tree were to
    * be pruned with that tolerance. Consistent with the definition
    * of prune, a node is counted if all of the leaves in its subtree
    * are within tol of the node's color.
    */
   int pruneSize(int tol);

   /*
    * The idealPrune function can be considered to be the inverse
    * of the pruneSize function. It takes as input a number of leaves
    * and returns the minimum tolerance that would produce that resolution
    * upon a prune. It does not change the structure of the tree.
    */
   int idealPrune (int leaves);

private:
   // Private member variables.

   Node* root; // ptr to the root of the twoDtree

   int height; // height of PNG represented by the tree
   int width; // width of PNG represented by the tree

   /**
    * Destroys all dynamically allocated memory associated with the
    * current twoDtree class. Complete for PA3.
    */
   void clear();

   /**
   * Copies the parameter other twoDtree into the current twoDtree.
   * Does not free any memory. Called by copy constructor and op=.
   * @param other The twoDtree to be copied.
   */
   void copy(const twoDtree & other);

   /**
   * Private helper function for the constructor. Recursively builds
   * the tree according to the specification of the constructor.
   * @param s Contains the data used to split the rectangles
   * @param ul upper left point of current node's rectangle.
   * @param lr lower right point of current node's rectangle.
   */
   Node * buildTree(stats & s,pair<int,int> ul, pair<int,int> lr, bool vert);

   /**
    * Helper function for recursively finding leaves, and adding them to image
    * Check if a node has a child, if so, call recursively. If not, use information of this
    * leaf to add to the PNG
    *
    * @param im	The image that we are modifying and colouring
    * @param curr	The current node that we are looking at
    */
   void renderHelper(PNG & im, Node* curr);

   /**
    * Helper method for recursively clearing the tree.
    * Checks children, recursively calls, and then clears itself
    *
    * @param root the twoDtree that we are currently clearing
    */
   void clearCurr(Node *root);

   /**
    * Helper method for recursively copying a tree from other to this.
    * Copies head, and then calls recursively on children.
    * Returns a pointer to the node that has just been copied
    *
    * @param head	the node that we are currently copying
    */
   Node* copyCurr(Node *head);

   /**
    * Helper method used in prune, checks whether a prune can occur at currNode
    * If this is a leaf, return true if within tolerance and false otherwise.
    * If not a leaf, recursively calls on left and right, and return true if both
    * Calls to the left and right are true.
    */
   bool pruneCheckLeaves(int tol, RGBAPixel avg, Node* currNode);

   /**
    * Helper method used in prune. Calls pruneCheckLeaves to see if a prune can occur
    * at currNode. If not, recursively calls itself on left and right children.
    * Navigates tree while calling pruneCheckLeaves, to find the highest prune.
    */
   void findHighestPrune(int tol, Node* currNode);

   /**
    * Helper function used in pruneSize. Checks whether a prune can occur, and returns true if it can.
    * If true, modifies count variable to count for the leaves that would be pruned.
    */
   bool countPruneSize(int tol, RGBAPixel avg, Node* currNode, int & count);

   /**
    * Finds the highest subtree that can be pruned starting currNode, and returns the number of
    * leaves that would be prune as a result of countPruneSizes.
    */
   int findHighestPruneSize (int tol, Node* currNode);
};

#endif
