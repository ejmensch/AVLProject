/*************************************************************/
/*Part 3                                                     */
/* Write the methods (in red, below) that are part of the    */
/* AVL Tree class                                            */
/*************************************************************/

#include <iostream>
#include <stdlib.h>
#include "AVL.hpp"
using namespace std;

/*************************************************************/
/* Constructors
   debug is a boolean flag that, if set, will result in the
   printing of a number of debug statements.  If it isn't
   set, the code will print without the debug statements.
*/
AVL::AVL() {
	root = NULL;
	debug = false;
}

AVL::AVL(bool debflag) {
	root = NULL;
	debug = debflag;
}

AVL::AVL(string ab, string d) {
	root = new TNode(ab, d, false);
	debug = false;

}

AVL::AVL(string ab, string d, bool debflag) {
	root = new TNode(ab, d, debflag);
	debug = debflag;

}

void AVL::printTreePre(TNode *tmp) {
	if (tmp == NULL) {
		return;
	}
	else {
		tmp->printNode(debug);
		printTreePre(tmp->left);
		printTreePre(tmp->right);
	}
}

void AVL::printTreePost(TNode *tmp) {
	if (tmp == NULL) {
		return;
	}
	else {
		printTreePost(tmp->left);
		printTreePost(tmp->right);
		tmp->printNode(debug);
	}
}

TNode *AVL::find(string a) {
	int ct = 0;
	if (root == NULL) {
		if (debug) {
		   cout << "root is null " << a << endl;
		}
		return NULL;
	}
	else {
		TNode *tmp = root;
		while (tmp != NULL) {
			ct+=1;
			if (debug) {
				cout <<tmp->abbr << " and "<<a << endl;
			}
			if (a < tmp->abbr) {
				if (debug) {
					cout << "going left " << endl;
				}
				if (tmp->left == NULL) {
					return NULL;
				}
				else {
					tmp=tmp->left;
				}
			}
			else if (a > tmp->abbr) {
				if (debug) {
					cout << "going right " << endl;
				}
				if (tmp->right == NULL) {
					return NULL;
				}
				else {
					tmp=tmp->right;
				}
			}
			else {
				if (debug) {
					cout << " in " << ct << endl;
				}
				return tmp;
			}
		}
	}
	return NULL;
}

void AVL::printTreeIO(TNode *tmp) {
	if (tmp == NULL) {
		return;
	}
	else {
		printTreeIO(tmp->left);
		tmp->printNode(debug);
		printTreeIO(tmp->right);
	}
}

bool AVL::insert(string ab, string d) {
	TNode *newnode = new TNode(ab, d, debug);

	if (root == NULL) {
		if (debug ) {
		    cout << "root NULL" << endl;
		}
		root = newnode;
		newnode->height=1;
		return true;
	}
	else {
		TNode *tmp = root;
		while (tmp != NULL){
			if (newnode->abbr < tmp->abbr) {
				if (tmp->left == NULL) {
					tmp->left = newnode;
					tmp->left->parent = tmp;
					newnode->height=1;
					setHeight(newnode);
					return true;
				}
				else {
					tmp=tmp->left;
				}
			}
			else if (newnode->abbr > tmp->abbr) {
				if (tmp->right == NULL) {
					tmp->right = newnode;
					tmp->right->parent = tmp;
					newnode->height=1;
					setHeight(newnode);
					return true;
				}
				else {
					tmp=tmp->right;
				}
			}
			else {
				return false;
			}
		}
	}
	return false;
}
/*************************************************************/
/*Methods you have to write                                  */
/*These methods are specifically for an AVL tree, which means*/
/*they involve keeping track of a node's height and balance, */
/*and rotating when a node becomes unbalanced                */
/*WATCH OUT FOR:                                             */
/*   NULL nodes (they don't have fields                      */
/*   Attaching parents properly, especially during rotations */
/*************************************************************/
int AVL::getBalance(TNode *tmp) {
/* method that finds the balance of a node tmp and returns that balance as an int
*/
//left kid - right kid == balance
	int leftkid=0;
	int rightkid=0; //initializes ends (leaves?) to 0
	if(tmp->left != NULL){
		leftkid = tmp->left->height;
	}
	if(tmp->right != NULL){
		rightkid = tmp->right->height;
	}
	return (leftkid-rightkid);   //should work hopefully, we'll find out when we test
}

TNode *AVL::rotateRight(TNode *tmp) {
	/* rotates right around node tmp and returns the node rotated up.  Note: this method must reset the heights of the node rotated down and the nodes rotated up, and you must reset the heights of all nodes that are ancestors of the node rotated down.  You will also need to reattach the newly rotated up node to the rest of the tree either in this method or in setheights.
	 */
	TNode *first = tmp->left;
	TNode *second = first->right;

	first->right = tmp;
	tmp->left = second;
	if (tmp==root) {
		first->parent=NULL;
		root=first;
	}
	else {
		first->parent=tmp->parent;
		if (tmp->parent->right != NULL) {
			if (tmp->parent->right == tmp) {
				tmp->parent->right = first;
			}
		}
		if (tmp->parent->left!=NULL) {
			if (tmp->parent->left == tmp) {
				tmp->parent->left = first;
			}
		}
	}

	tmp->parent = first;
	//second->parent = tmp; //possibly not needed,idk might not even be correct anyway

	//have to update heights now, prolly some edge cases too.
	//order of height checking should be second,tmp,first i think (if im doing right rotation correctly)
	//second
	if(second->left == NULL && second->right == NULL){  //if no children, set heights to 1
		second->height=1;
	}
	if(second->right == NULL){ //if it only has a left child, base height off of left child
		second->height= second->left->height;
	}
	if(second->left == NULL){  //if it only has a right child, base height off of right child
		second->height= second->right->height + 1;
	}

	//tmp ... could be unnecessary, idk
	if (tmp->left == NULL && tmp->right == NULL) { //if no children, set heights to 1
		tmp->height = 1;
	}
	if (tmp->right == NULL) { //if it only has a left child, base height off of left child
		tmp->height = tmp->left->height;
	}
	if (tmp->left == NULL) { //if it only has a right child, base height off of right child
		tmp->height = tmp->right->height + 1;
	}

	//first... could also be unnecessary
	if (first->left == NULL && first->right == NULL) { //if no children, set heights to 1
		first->height = 1;
	}
	if (first->right == NULL) { //if it only has a left child, base height off of left child
		first->height = first->left->height;
	}
	if (first->left == NULL) { //if it only has a right child, base height off of right child
		first->height = first->right->height + 1;
	}

	if(second->left->height > second->right->height){
		second->height = second->left->height + 1;
	}
	else{
		second->height = second->right->height + 1;
	}
	//do we need to put tmp in the middle here?
	if(first->left->height > first->right->height){
		first->height = first->left->height +1;
	}
	else{
		first->height = first->right->height +1;
	}
	return first; //this is not  done
}

TNode *AVL::rotateLeft(TNode *tmp) {
	TNode *first= tmp->right;
	TNode *second = first->left;

	first->left = tmp;
	tmp->right = second;
	if (tmp==root){
		first->parent = NULL;
		root = first;
	}
	else{
		first->parent = tmp->parent;
		if (tmp->parent->right != NULL){
			if (tmp->parent->right == tmp) {
				tmp->parent->right=first;
			}
		}
		if (tmp->parent->left != NULL){
			if(tmp->parent->left == tmp){
				tmp->parent->left = first;
			}
		}
	}
	tmp->parent=first;
	//second->parent=tmp;
	//copied over from rotate right, just commented out for now
//	//second
//	if (second->left == NULL && second->right == NULL) { //if no children, set heights to 1
//		second->height = 1;
//	}
//	if (second->right == NULL) { //if it only has a left child, base height off of left child
//		second->height = second->left->height;
//	}
//	if (second->left == NULL) { //if it only has a right child, base height off of right child
//		second->height = second->right->height + 1;
//	}
//
//	//tmp ... could be unnecessary, idk
//	if (tmp->left == NULL && tmp->right == NULL) { //if no children, set heights to 1
//		tmp->height = 1;
//	}
//	if (tmp->right == NULL) { //if it only has a left child, base height off of left child
//		tmp->height = tmp->left->height;
//	}
//	if (tmp->left == NULL) { //if it only has a right child, base height off of right child
//		tmp->height = tmp->right->height + 1;
//	}
//
//	//first... could also be unnecessary
//	if (first->left == NULL && first->right == NULL) { //if no children, set heights to 1
//		first->height = 1;
//	}
//	if (first->right == NULL) { //if it only has a left child, base height off of left child
//		first->height = first->left->height;
//	}
//	if (first->left == NULL) { //if it only has a right child, base height off of right child
//		first->height = first->right->height + 1;
//	}

//plz let me push






	if(second->right->height > second->left->height) {
		second->height = second->right->height + 1;
	}
	else {
		second->height = second->left->height + 1;
	}
	if(first->right->height > first->left->height) {
		first->height = first->right->height + 1;
	}
	else {
		first->height = first->left->height + 1;
	}
	return first;
/* rotates down around node tmp and returns the node rotated up.  Note: this method must reset the heights of the node rotated down and the nodes rotated up, and you must reset the heights of all nodes that are ancestors of the node rotated down. You will also need to reattach the newly rotated up node to the rest of the tree either in this method or in setheights.
*/
}

void AVL::setHeight(TNode *tmp) {
/*
This method sets the height of tmp and then of all the ancestors of tmp.  It stops when the height of a node does not change. Note that this method most likely calls getBalance and possibly the rotate methods, and may even set the newly rotated up node's parent attachement, although you could do that in the rotate method.
*/
	if (tmp==root){
		// this catches "if (tmp->parent == NULL)"
		return;
	}
	int bal = getBalance(tmp);
	int rightBal = getBalance(tmp->right);
	int leftBal = getBalance(tmp->left);
	if (tmp->parent->left != NULL && tmp->parent->right != NULL) {
		if (tmp->parent->right == tmp ) {
			if (tmp->height >= tmp->parent->left->height) {
				tmp->parent->height = tmp->height + 1;
				//rotateRight(tmp);
			}
		}
		else{ // tmp is left child
			if (tmp->height >= tmp->parent->right->height) {
				tmp->parent->height = tmp->height + 1;
			}
		}
	}
	else {
		if(tmp->parent->height != tmp->height + 1) {
			tmp->parent->height = tmp->height + 1;
		}
	}
	if (bal == 2) { // RR rot or LR rot
		rotateRight(tmp->left);
	} else if (bal==-2) { // LL rot or RL rot

	} // source :     btechsmartclass.com/data_structures/avl-trees.html
	setHeight(tmp->parent);



//	if(getBalance(tmp)==2){  //balance is left kid - right kid
//		if(getBalance(tmp->left) == 1){
//			rotateRight(tmp); //do we set something to this since it returns a node?
//		}
//		else{
//			rotateLeft(tmp);
//		}
//	}
//	else if(getBalance(tmp)==-2){
//		if (getBalance(tmp->right) == 1) {
//			rotateRight(tmp); //do we set something to this since it returns a node?
//		} else {
//			rotateLeft(tmp);
//		}
//	}
	return;

}
void AVL::printTreeIO() {
	if (root == NULL ) {
		cout << "Empty Tree" << endl;
	}
	else {
		cout << endl<<"Printing In Order:" <<endl;
		printTreeIO(root);
	}
}

void AVL::printTreePre() {
	if (root == NULL ) {
		cout << "Empty Tree" << endl;
	}
	else {
		cout << endl<<"Printing PreOrder:" <<endl;
		printTreePre(root);
	}
}

void AVL::printTreePost() {
	if (root == NULL ) {
		cout << "Empty Tree" << endl;
	}
	else {
		cout << endl<<"Printing PostOrder:" <<endl;
		printTreePost(root);
	}
}

void AVL::clearTree() {
	if (root == NULL) {
		cout << "Tree already empty" << endl;
	}
	else {
		cout << endl << "Clearing Tree:" << endl;
		clearTree(root);
		root = NULL;
	}
}
void AVL::clearTree(TNode *tmp) {
	if (tmp == NULL) {
		return;
	}
	else {
		clearTree(tmp->left);
		clearTree(tmp->right);
		tmp->printNode();
		delete(tmp);
	}
}
