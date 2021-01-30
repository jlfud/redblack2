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
void tree::remove(node* current, int num){
  //mostly standard bst deletion
  if(current == NULL){
    return;
  }
  else if(num < current->data){
    remove(current->left, num); 
  }
  else if(num > current->data){
    remove(current->right, num);
  }
  else if(current->left != NULL && current->right != NULL){
    node* temp = current->right;
    while(temp->left != NULL){
      temp = temp->left; 
    }
    current->data = temp->data;
    this->remove(current->right, temp->data);
  }
  else if(current->right == NULL && current->right == NULL){
    if(current == root){
      root = NULL;
      return;
    }
    else if(current->parent->right == current){
      node* parent = current->parent;
      this->fixremove(current);
      parent->right = NULL; 
    }
    else{
      node* parent = current->parent;
      this->fixremove(current);
      parent->left = NULL;
    }
  }
  else{
    node* temp = (current->right) ? current->left : current->right;
    if(temp == NULL){
      if(!current->isred){
	this->fixremove(current);
	delete current;
        return;
      }
    }
    temp->parent = current->parent; 
    if(current == current->parent->left){
      current->parent->left = temp; 
    }
    else{
      current->parent->right = temp; 
    }
    if(!current->isred){
      if(temp->isred){
        temp->isred = false;
      }
      else{
	this->fixremove(temp);
      }
    }
    delete current;
  }
}
void tree::fixremove(node* current){
  //case 1
  if(current == root){
    return;
  }
  node* sibling = new node();
  if(current->parent->right == current){
    sibling = current->parent->left;
  }
  else{
    sibling = current->parent->right; 
  }
  if(sibling == NULL){
    return;
  }
  //case 2
  if(sibling->isred){
    current->parent->isred = true; 
    sibling->isred = false;
    if(current == current->parent->left){
      leftrotate(current->parent);  
    }
    else{
      rightrotate(current->parent);
    }
  }
  if((!current->parent->isred) && (!sibling->isred) && (!sibling->left->isred) && (!sibling->right->isred)){
     //case 3
    sibling->isred = true;
    fixremove(current->parent);
    return;
  }
  //case 4
  if(sibling->left == NULL || sibling->right == NULL){
    return;
  }
  if(current->parent->isred && !sibling->isred && !sibling->left->isred && !sibling->right->isred){
    current->parent->isred = false;
    sibling->isred = true;
    return;
  }
  //case 5
  if(!sibling->isred){
    if((current == current->parent->left)&&(sibling->right->isred) && (sibling->left->isred)){
      sibling->isred = true;
      sibling->left->isred = false;
      this->rightrotate(sibling); 
    }
    else if((current == current->parent->right) && (!sibling->left->isred) && (sibling->right->isred)){
      sibling->isred = true;
      sibling->right->isred = false;
      this->leftrotate(sibling);
    }
  }
    //case 6
  sibling->isred = current->parent->isred;
  current->parent->isred = false;
  if(current == current->parent->left){
    current->right->isred = false;
    this->leftrotate(current->parent);
  }
  else{
    current->left->isred = false;
    this->rightrotate(current->parent);
  }
}
bool tree::search(node* current, int n){
  if(current == NULL){
    return false; 
  }
  else if(current->data > n){
    return search(current->left, n); 
  }
  else if(current->data < n){
    return search(current->right, n);
  }
  else{
    return true; 
  }
}
node*& tree::getRoot(){
  return root; 
}
