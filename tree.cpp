#include <iostream>
#include <cstring>
#include "tree.h"

using namespace std; 

tree::tree(){

}
tree::~tree(){

}
void tree::insert(int num){
  //insert like normal binary tree
  node* n = new node();
  n->data = num;
  if(root == NULL){
    //covers case 1, root is null
    n->isred = false;
    root = n;
    return;
  }
  node* current = root;
  node* p = current; 
  while(current != NULL){
    //loop until the place to insert is found
    p = current; //p is the parent of the null node
    if(n->data < current->data){
      current = current->left;
    }
    else{
      current = current->right; 
    }
  }
  n->parent = p;
  //insert, updating parent and children accordingly
  if(n->data < n->parent->data){
    n->parent->left = n; 
  }
  else{
    n->parent->right = n; 
  }
  //fix to maintain red black properties
  this->build(n); 
}
void tree::display(node* current){
  if(current != NULL){
    //dispaly with left child on the left and right child on the right
    display(current->left);
    if(current==root){
      //no parent is we are at the root
      cout << current->data << "(B) "; 
    }
    else{
      //print with indication of red/black and parent
      if(current->isred){
	cout << current->data << "(R, " << current->parent->data << ") "; 
      }
      else{
	cout << current->data << "(B, " << current->parent->data << ") "; 
      }
    }
    display(current->right);
  }
}
void tree::leftrotate(node* n){
  //rotate left with node
  node* nn = n->right; //new n
  node* p = n->parent; 
  if(nn == NULL){
    //if nn is null, finish
    return;
  }
  n->right = nn->left;
  nn->left = n;
  n->parent = nn;
  if(n->right != NULL){
    n->right->parent = n; 
  }
  if(p != NULL){
    if(n == p->left){
      p->left = nn; 
    }
    else if(n == p->right){
      p->right = nn; 
    }
  }
  nn->parent = p;
  if(nn->parent == NULL){
    root = nn; 
  }
}
void tree::rightrotate(node* n){
  //rotate right with node
  node* nn = n->left;
  node* p = n->parent;
  if(nn == NULL){
    return;
  }
  n->left = nn->right;
  nn->right = n;
  n->parent = nn;
  if(n->left != NULL){
    n->left->parent = n;
  }
  if(p != NULL){
    if(n == p->left){
      p->left = nn; 
    }
    else if(n == p->right){
      p->right = nn; 
    }
  }
  nn->parent = p;
  if(nn->parent==NULL){
    root = nn;
  }
}
void tree::build(node* n){
  //case 1 taken care of in insert
  //case 2
  if(!n->parent->isred){
    return;
  }
  node* uncle = new node();
  if(n->parent->parent->left == n->parent){
    uncle = n->parent->parent->right;
  }
  else if(n->parent->parent->right == n->parent){
    uncle = n->parent->parent->left;
  }
  if(uncle != NULL && n->parent != NULL ){
    //case 3
    if(uncle->isred){
      uncle->isred = false; 
      n->parent->isred = false;
      n->parent->parent->isred = true;
      if(n->parent->parent != root){
	if(n->parent->parent->parent->parent != NULL){
	  this->build(n->parent->parent);
	}
      }
      else{
	return;
      }
    }
    else{
      node* p = n->parent;
      node* g = n->parent->parent;
      //case 4
      if(n == p->right && p == g->left){
	leftrotate(p);
	n = n->left;
      }
      else if(n == p->left && p == g->right){
	rightrotate(p);
	n = n->right;
      }
      p = n->parent;
      g = p->parent;
      if(n == p->left){
	rightrotate(g);
      }
      else{
	leftrotate(g);
      }
      p->isred = false;
      g->isred = true;
    }
  }
  else{
    node* p = n->parent;
    node* g = n->parent->parent;
    //case 4
    if(n == p->right && p == g->left){
      leftrotate(p);
      n = n->left;
    }
    else if(n == p->left && p == g->right){
      rightrotate(p);
      n = n->right;
    }
    p = n->parent;
    g = p->parent;
    if(n == p->left){
      rightrotate(g);
    }
    else{
      leftrotate(g);
    }
    p->isred = false;
    g->isred = true;
  }
}
node* tree::remove(node* &current, int n){
  if(root==NULL){ //nothing in tree
    cout << "no data in tree yet" << endl;
    return NULL;
  }
  //traverse through tree until the correct node is found
  if(n < current->data){
    current->left = remove(current->left, n);
  }
  else if(n > current->data){
    root->right = remove(current->right, n);
  }
  else{
    //node has no children
    if(current->right == NULL && current->left == NULL){
      delete current; //just delete the node
      current = NULL;
    }
    else if(current->left == NULL){
      //node has a right child
      node* no = current;
      current = current->right;
      delete no;
    }
    else if(current->right == NULL){
      //node has a left child
      node* no = current;
      current = current->left;
      delete no;
    }
    else{
      //hibbard's algorithm to delete a node with left and right child
      //node has two children//node has a right child
      node* no = current; //node to delete
      node* min = current->right; //leftmost element in right subtree
      //preparing to replace node to delete with min
      node* parent = current;  //parent of min
      if(min->left == NULL){
	//if the node is already at the bottom
	//replace parent (unchanged so node to delete) with min
	parent->data = min->data;
	parent->right = min->right;
      }
      else if(parent->right->left == NULL){
	//if the right node of the parent is the bottom node
	parent->data = parent->right->data;
	parent->right = parent->right->right;
	//parent is now parent's right now
      }
      else{
	while(min->left != NULL){
	  //loop until we get to the bottom
	  parent = min;
	  min = min->left;
	}
	//replace the node to delete
	no->data = min->data;
	//right subtree link to parent's left
	parent->left = min->right;
      }
    }
  }
  return current; //for recursion
}
void tree::fixRemove(node* n){

}
node*& tree::getRoot(){
  return root; 
}
