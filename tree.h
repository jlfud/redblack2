#ifndef TREE_H
#define TREE_H

#include <iostream>
#include <cstring>

using namespace std;

struct node{
  bool isred = true;
  int data;
  node* left = NULL;
  node* right = NULL;
  node* parent = NULL;
};
class tree{
 public:
  tree();
  ~tree();
  void insert(int);
  void display(node*); 
  void leftrotate(node*);
  void rightrotate(node*);
  void build(node*); //fix insert
  node* stdremove(node* &, int);
  void remove(node* &, int);
  bool search(node*, int);
  node*& getRoot();
private:
  node* root = NULL;
};

#endif
