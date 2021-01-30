#include <iostream>
#include <cstring>
#include <fstream>
#include "tree.h"

using namespace std;

void parse(tree* &t, char in[80]); //for parsing file input

int main(){
  char in[80]; //user input
  int intin; //integer input
  tree* t = new tree(); //red black tree
  cout << "red black tree part 1: add, read, print, search, delete, quit" << endl;
  //program loop
  while(true){
    cout << "command: " << endl;
    cin >> in;
    if(strcmp(in, "add")==0){
      //if add, get a value to insert and insert it
      cout << "value to input: " << endl;
      cin >> intin; 
      t->insert(intin); 
    }
    else if(strcmp(in, "read")==0){
      //get the file abd parse the content of the file
      cout << "file address: " << endl;
      cin >> in;
      ifstream text;
      text.open(in);
      text.getline(in, 80);
      parse(t, in);
    }
    else if(strcmp(in, "print")==0){
      //display the tree, starting with the root
      t->display(t->getRoot());
      cout << endl;
    }
    else if(strcmp(in, "delete")==0){
      cout << "value to delete: " << endl;
      cin >> intin;
      t->remove(t->getRoot(), intin);
    }
    else if(strcmp(in, "search")==0){
      cout << "number to search: " << endl;
      cin >> intin;
      if(t->search(t->getRoot(), intin)){
	cout << intin << " is in the tree" << endl; 
      }
      else{
	cout << intin << " is not in the tree" << endl; 
      }
    }
    else if(strcmp(in, "quit")==0){
      return 0;
    }
    else{
      cout << "invalid command. " << endl;
    }
  }
}
void parse(tree* &t, char in[80]){
  char token[80]; //for each number
  int a = 0; //what index on token we are on
  for(int i = 0; i < strlen(in); i++){
    //loop through string
    if(in[i] == ' '){
      //if we are at a space insert int of token and reset
      t->insert(atoi(token));
      strcpy(token, "   ");
      a = 0;
    }
    else{
      //add characters to token
      token[a] = in[i];
      a++;
    }
  }
  //insert the last number
  t->insert(atoi(token));
}
