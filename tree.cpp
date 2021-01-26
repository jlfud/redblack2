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
void tree::stdremove(node* &current, int n){
  node* temp = current;
  while(true){
    if(temp == NULL){
      cout << "no node found" << endl;
      return; 
    }
    else if(n > temp->data){
      temp = temp->right; 
    }
    else if(n < temp->data){
      temp = temp->left; 
    }
    else{
      break; 
    }
  }
  node* x;
  node* y = temp;
  bool original_isred = y->isred; 
  if(temp->left == NULL && temp->right != NULL){ 
    x = temp->right; 
    this->swap(temp, temp->right);
  }
  else if(temp->right == NULL && temp->left != NULL){
    x = temp->left;
    this->swap(temp, temp->left); 
  }
  else{
    cout << temp->data << endl; 
    node* placeholder = temp->right;
    while(placeholder->left != NULL){
      placeholder = placeholder->left; 
    }
    cout << "1" << endl; 
    y = placeholder; 
    original_isred = y->isred;
    x = y->right;
    cout << "@" << endl;
    if(y->parent == temp){
      x->parent = y;
    }
    else{ 
      this->swap(y, y->right);
      y->right = temp->right;
      y->right->parent = y; 
    } 
    this->swap(temp, y);
    y->left = temp->left;
    y->left->parent = y;
    y->isred = temp->isred; 
  }
  delete temp;
  if(!original_isred){
    remove(x, n);
  }
}
void tree::swap(node* x, node* y){
  if(x->parent == NULL){
    root = y;
  }
  else if(x == x->parent->left){
    x->parent->left = y; 
  }
  else{
    x->parent->right = y;
  }
  y->parent = x->parent;
}
void tree::remove(node* &current, int num){
  node* sibling = new node();
  while(current != root && !current->isred){
    if(current == current->parent->left){
      sibling = current->parent->right;
      if(sibling->isred){
	sibling->isred = false;
	current->parent->isred = true;
	leftrotate(current->parent);
	sibling = current->parent->right;
      }
      if(!sibling->left->isred && !sibling->right->isred){
	sibling->isred = true;
	current = current->parent; 
      }
      else{
	if(!sibling->right->isred){
	  sibling->left->isred = false;
	  sibling->isred = true;
	  rightrotate(sibling);
	  sibling = current->parent->right;
	}
	sibling->isred = current->parent->isred;
	current->parent->isred = false;
	sibling->right->isred = false;
	leftrotate(current->parent);
	current = root;
      }
    }
    else{
      sibling = current->parent->left;
      if(sibling->isred){
	sibling->isred = false;
	current->parent->isred = true;
	rightrotate(current->parent); //could change to right rotate, rotation kept same
	sibling = current->parent->left;
      }
      if(!sibling->right->isred && !sibling->left->isred){ //might be without left check?
	sibling->isred = true;
	current = current->parent;
      }
      else{
	if(!sibling->left->isred){
	  sibling->right->isred = false;
	  sibling->isred = true;
	  leftrotate(sibling);
	  sibling = current->parent->left;
	}
	sibling->isred = current->parent->isred;
	current->parent->isred = false;
	sibling->left->isred = false;
	rightrotate(current->parent);
	current = root;
      }
    }
  }
  current->isred = false;
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
