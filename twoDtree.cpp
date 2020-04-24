#include "twoDtree.h"
#include <cmath>

// Node constructor
twoDtree::Node::Node(pair<int,int> ul, pair<int,int> lr, RGBAPixel a)
	:upLeft(ul),lowRight(lr),avg(a),left(NULL),right(NULL)
	{}

// twoDtree destructor
twoDtree::~twoDtree(){
	clear();
}

// twoDtree copy constructor
twoDtree::twoDtree(const twoDtree & other) {
	copy(other);
}


// twoDtree assignment operator
twoDtree & twoDtree::operator=(const twoDtree & rhs){
	if (this != &rhs) {
		clear();
		copy(rhs);
	}
	return *this;
}

// twoDtree constructor
twoDtree::twoDtree(PNG & imIn){
	height = imIn.height();
	width = imIn.width();

	// create instance of stats class
	stats s(imIn);

	// build defualt tree for image
	root = buildTree(s, pair<int, int> (0,0), pair<int, int> (width-1, height-1), true);
}

// builds tree by splitting image then recusevly splits the new made halves
twoDtree::Node * twoDtree::buildTree(stats & s, pair<int,int> ul, pair<int,int> lr, bool vert) {
	// create new node and get columns and rows in rectangle
	auto node = new twoDtree::Node(ul, lr, s.getAvg(ul, lr));
	int numCols = lr.first - ul.first + 1;
	int numRows = lr.second - ul.second + 1;

	// make the current nodes children based on the position of the ul and lr corner, and if splitting vert or horz
	if (vert == true) {
		if (ul.first != lr.first) {
			long maxScore = LONG_MAX;
			long maxX = ul.first;
			for (int x = ul.first; x < lr.first; x++) {
				long leftScore = s.getScore(ul, pair<int, int>(x, lr.second));
				long rightScore = s.getScore(pair<int, int>(x + 1, ul.second), lr);
				long thisScore = leftScore + rightScore;
				if (thisScore <= maxScore) {
					maxScore = thisScore;
					maxX = x;
				}
			}

			node->left = buildTree(s, ul, pair<int, int>(maxX, lr.second), false);
			node->right = buildTree(s, pair<int, int>(maxX + 1, ul.second), lr, false);
		} else if (ul.second != lr.second) {
				long maxScore = LONG_MAX;
				long maxY = ul.second;
				for (int y = ul.second; y < lr.second; y++) {
					long upperScore = s.getScore(ul, pair<int, int>(lr.first, y));
					long lowerScore = s.getScore(pair<int, int>(ul.first, y + 1), lr);
					long thisScore = upperScore + lowerScore;
					if (thisScore <= maxScore) {
						maxScore = thisScore;
						maxY = y;
					}
				}

				node->left = buildTree(s, ul, pair<int, int>(lr.first, maxY), true);
				node->right = buildTree(s, pair<int, int>(ul.first, maxY + 1), lr, true);
		}
	} else {
			if (ul.second != lr.second) {
				long maxScore = LONG_MAX;
				long maxY = ul.second;
				for (int y = ul.second; y < lr.second; y++) {
					long upperScore = s.getScore(ul, pair<int, int>(lr.first, y));
					long lowerScore = s.getScore(pair<int, int>(ul.first, y + 1), lr);
					long thisScore = upperScore + lowerScore;
					if (thisScore < maxScore) {
						maxScore = thisScore;
						maxY = y;
					}
				}

				node->left = buildTree(s, ul, pair<int, int>(lr.first, maxY), true);
				node->right = buildTree(s, pair<int, int>(ul.first, maxY + 1), lr, true);
			} else if (ul.first != lr.first) {
					long maxScore = LONG_MAX;
					long maxX = ul.first;
					for (int x = ul.first; x < lr.first; x++) {
						long leftScore = s.getScore(ul, pair<int, int>(x, lr.second));
						long rightScore = s.getScore(pair<int, int>(x + 1, ul.second), lr);
						long thisScore = leftScore + rightScore;
						if (thisScore < maxScore) {
							maxScore = thisScore;
							maxX = x;
						}
					}

					node->left = buildTree(s, ul, pair<int, int>(maxX, lr.second), false);
					node->right = buildTree(s, pair<int, int>(maxX + 1, ul.second), lr, false);
				}
		}

	return node;
}

// renders current twoDtree into a PNG image
PNG twoDtree::render(){
	// make new PNG and call helper function
	PNG * renderedImage = new PNG(this->width, this->height);
	renderHelper(*renderedImage, this->root);

	return *renderedImage;
}

// recersive helper function for render
void twoDtree::renderHelper(PNG & im, Node* curr) {
	if ((curr->left == NULL) && (curr->right == NULL)) {

		// add pixels from current node onto the PNG image
		for (int x = curr->upLeft.first; x <= curr->lowRight.first; x++) {
			for (int y = curr->upLeft.second; y <= curr->lowRight.second; y++) {
					RGBAPixel* currPixel = im.getPixel(x, y);
					currPixel->r = curr->avg.r;
					currPixel->b = curr->avg.b;
					currPixel->g = curr->avg.g;
			}
		}

	} else {
		if (curr->left != NULL) {
			renderHelper(im, curr->left);
		}
		if (curr->right != NULL) {
			renderHelper(im, curr->right);
		}
	}
}

// returns minimum tolerance amount to have the given amount of leaves
int twoDtree::idealPrune(int leaves){
	int low = 0;
	int high = 1;
	int result = -1;

	//increases end by a factor of 2 until tolerance = end produces less than the required leaves
	// essentially finding an upper bound
	while(pruneSize(high) > leaves) {
		high = high * 2;
	}

	while (low <= high) {
		int mid = (low + high) / 2;
		int pruneMid = pruneSize(mid);
		if (leaves == pruneMid) {
			result = mid;
			high = mid - 1;
		} else if (pruneMid < leaves) {
			high = mid - 1;
		} else {
			low = mid + 1;
		}
	}

	if (result == -1) {
		return high;
	} else {
		return result;
	}
}

// recersive helper function for pruneSize
int twoDtree::findHighestPruneSize(int tol, Node* currNode) {
	int count = 0;
	int countL = 0;
	int countR = 0;

	if (pruneCheckLeaves(tol, currNode->avg, currNode) == true) {
		// this is the highest height so will become a leaf
		count++;
	}
	else {
		// recursively call on the left if there is a left, and then right if there is a right
		if (currNode->left != NULL) {
			countL = findHighestPruneSize(tol, currNode->left);
		}

		if (currNode->right != NULL) {
			countR = findHighestPruneSize(tol, currNode->right);
		}
	}

	if ((currNode->left == NULL) && (currNode->right == NULL)) {
		return 1;
	} else {
			return count + countL + countR;
	}
}

// returns number of leaves after a prune
int twoDtree::pruneSize(int tol){
	return findHighestPruneSize(tol, root);
}

// prunes current twoDtree
void twoDtree::prune(int tol){
	findHighestPrune(tol, root);
}

// prunes to the closest level to the root possible
void twoDtree::findHighestPrune(int tol, Node* currNode) {
	if (pruneCheckLeaves(tol, currNode->avg, currNode) == true) {
		// this is the highest height. Need to now actually do the prune
		delete currNode->left;
		currNode->left = NULL;
		delete currNode->right;
		currNode->right = NULL;

	} else {
		// recursively call on the left if there is a left, and then right if there is a right
		if (currNode->left != NULL) {
			findHighestPrune(tol, currNode->left);
		}

		if (currNode->right != NULL) {
			findHighestPrune(tol, currNode->right);
		}
	}

}

// returns true if node can be pruned
bool twoDtree::pruneCheckLeaves(int tol, RGBAPixel avg, Node* currNode) {

	if ((currNode->left == NULL) && (currNode->right == NULL)) {
		// want to return true if within tolerance, false otherwise
		return (pow((avg.r - currNode->avg.r), 2) + pow((avg.b - currNode->avg.b), 2) + pow((avg.g - currNode->avg.g), 2)) <= tol;

	} else {
		bool leftState;
		bool rightState;

		//if there is a left, recursively call on left. If there isnt a left, assume true
		if (currNode->left != NULL) {
			leftState = pruneCheckLeaves(tol, avg, currNode->left);
		} else {
			leftState = true;
		}

		//if there is a right, recursively call on right. If there isnt a right, assume structure
		if (currNode->right != NULL) {
			rightState = pruneCheckLeaves(tol, avg, currNode->right);
		} else {
			rightState = true;
		}

		//return true if both left and right are true, false otherwise
		return ((rightState == true) && (leftState == true));
	}


}

// clears current twoDtree
void twoDtree::clear() {
	clearCurr(this->root);
}

// recersive helper function for clear
void twoDtree::clearCurr(Node *root) {
	if (root->left != NULL) {
		clearCurr(root->left);
	}
	if (root->right != NULL) {
		clearCurr(root->right);
	}

	delete root;
	root = NULL;
}

// makes a copy of the current twoDtree
void twoDtree::copy(const twoDtree & orig){
	this->root = copyCurr(orig.root);
	this->height = orig.height;
	this->width = orig.width;

}

// recersive helper function for copy
twoDtree::Node * twoDtree::copyCurr(Node *head) {

	auto node = new twoDtree::Node(head->upLeft, head->lowRight, head->avg);
	if (head->left != NULL) {
		node->left = copyCurr(head->left);
	}
	if (head->right != NULL) {
		node->right = copyCurr(head->right);
	}
	return node;
}
