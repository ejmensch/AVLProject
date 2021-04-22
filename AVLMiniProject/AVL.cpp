/*************************************************************/
/*Part 3                                                     */
/* Write the methods (in red, below) that are part of the    */
/* AVL Tree class                                            */
/*************************************************************/

#include <iostream>
#include <stdio.h>
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


	cout << " yadda yadda " + ab << endl;
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
	if (tmp == NULL) {
		return 0;
	}

	int leftkid=0;
	int rightkid=0;
	if(tmp->left != NULL){
		leftkid = tmp->left->height;
	}
	if(tmp->right != NULL){
		rightkid = tmp->right->height;
	}
	return (leftkid-rightkid);
}

TNode *AVL::rotateRight(TNode *tmp) {
	cout << "rotateRight\n";
	cout << tmp->abbr << endl;
	TNode *first = tmp->left;
	TNode *second = first->right;

	if (tmp==root) {
		first->parent=NULL;
		root=first;
	}
	first->right = tmp;
	tmp->left = second;
	first->parent = tmp->parent;
	if (tmp->parent != NULL) {
		if (tmp->parent->left == tmp) {
			tmp->parent->left = first;
		}
		else if (tmp->parent->right == tmp) {
			tmp->parent->right = first;
		}
	}
	if (second != NULL) {
		second->parent = tmp;
	}
	tmp->parent = first;
	if (tmp->left == NULL && tmp->right == NULL) {
		tmp->height = 1;
	}
	else if (tmp->right == NULL) {
		tmp->height = tmp->left->height + 1;
	}
	else if (tmp->left == NULL) {
		tmp->height = tmp->right->height + 1;
	}
	else {
		tmp->height = max(tmp->right->height, tmp->left->height) + 1;
	}

	if (first->left == NULL && first->right == NULL) {
		first->height = 1;
	}
	else if (first->right == NULL) {
		first->height = first->left->height + 1;
	}
	else if (first->left == NULL) {
		first->height = first->right->height + 1;
	}
	else {
		first->height = max(first->right->height, first->left->height) + 1;
	}


	cout << tmp->abbr << endl;
	cout << "first->abbr : " << first->abbr << endl;
	if (tmp->left != NULL) {
		cout << "tmp->left->abbr : " << tmp->left->abbr << endl;
	}
	if (first->left != NULL) {
		cout << "first->left->abbr : "<< first->left->abbr << endl;
	}
	if (first->right != NULL) {
		cout << "first->right->abbr : "<< first->right->abbr << endl;
	}


	return first;
}

TNode *AVL::rotateLeft(TNode *tmp) {
	cout << "rotateLeft\n";

	TNode *first = tmp->right;
	TNode *second = first->left;

	if (tmp==root) {
		first->parent=NULL;
		root=first;
	}
	first->left = tmp;
	tmp->right = second;
	first->parent = tmp->parent;
	if (tmp->parent != NULL) {
		if (tmp->parent->left == tmp) {
			tmp->parent->left = first;
		}
		else if (tmp->parent->right == tmp) {
			tmp->parent->right = first;
		}
	}
	if (second != NULL) {
		second->parent = tmp;
	}
	tmp->parent = first;

	if (tmp->left == NULL && tmp->right == NULL) {
		tmp->height = 1;
	}
	else if (tmp->right == NULL) {
		tmp->height = tmp->left->height + 1;
	}
	else if (tmp->left == NULL) {
		tmp->height = tmp->right->height + 1;
	}
	else {
		tmp->height = max(tmp->right->height, tmp->left->height) + 1;
	}
	if (first->left == NULL && first->right == NULL) {
		first->height = 1;
	}
	else if (first->right == NULL) {
		first->height = first->left->height + 1;
	}
	else if (first->left == NULL) {
		first->height = first->right->height + 1;
	}
	else {
		first->height = max(first->right->height, first->left->height) + 1;
	}
	cout << tmp->abbr << endl;
	cout << "first->abbr : " << first->abbr << endl;
	if (tmp->left != NULL) {
		cout << "tmp->left->abbr : " << tmp->left->abbr << endl;
	}
	if (first->left != NULL) {
		cout << "first->left->abbr : "<< first->left->abbr << endl;
	}
	if (first->right != NULL) {
		cout << "first->right->abbr : "<< first->right->abbr << endl;
	}
	return first;
}

void AVL::setHeight(TNode *tmp) {
	cout << "setHeight on " << tmp->abbr << endl;
	if (tmp==root){
		cout << "tmp==root \n";
	}
	else {
		if (tmp->parent->left != NULL && tmp->parent->right != NULL) {
			if (tmp->parent->right == tmp ) {
				if (tmp->height >= tmp->parent->left->height) {
					tmp->parent->height = tmp->height + 1;
				}
			}
			else {
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
	}
	int bal = getBalance(tmp);
	int rightBal = getBalance(tmp->right);
	int leftBal = getBalance(tmp->left);
	if (bal == 2) {
		cout << "bal == 2\n";
		if(leftBal==1){
			rotateRight(tmp);
		}
		else{ // LR
			rotateLeft(tmp->left);
			rotateRight(tmp);
		}
	}
	else if (bal==-2) {
		if(rightBal==-1){
			rotateLeft(tmp);
		}
		else{
			rotateRight(tmp->right);
			rotateLeft(tmp);
		}
	}
	if (tmp->parent != NULL) {
		cout << "tmp->parent->abbr = " + tmp->parent->abbr << endl;
		setHeight(tmp->parent);
	}
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
