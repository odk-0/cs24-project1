// This file should implement the game using a custom implementation of a BST (based on your earlier BST implementation)
#include <iostream>
#include <fstream>
#include <string>
#include "card.h"
#include "card_list.h"
#include <algorithm>
#include <cctype>
//Do not include set in this file

using namespace std;

// trim helpers (copied from main_set.cpp)
static inline std::string ltrim_copy(std::string s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch){ return !std::isspace(ch); }));
  return s;
}

static inline std::string rtrim_copy(std::string s) {
  s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch){ return !std::isspace(ch); }).base(), s.end());
  return s;
}

static inline std::string trim_copy(std::string s) {
  return ltrim_copy(rtrim_copy(std::move(s)));
}

// parse helper (same normalization as main_set.cpp)
Card parseCard(const std::string &s) {
  std::string t = trim_copy(s);
  if(t.empty()) return Card();

  char suit = '\0';
  std::string ranktoken;

  if(t.size() >= 2 && isalpha(t[0])) {
    suit = tolower(t[0]);
    ranktoken = t.substr(1);
    auto pos = ranktoken.find_first_not_of(" \t");
    if(pos != std::string::npos) ranktoken = ranktoken.substr(pos);
  }

  char rankc = ' ';
  if(ranktoken == "10") rankc = 't';
  else if(!ranktoken.empty()) rankc = tolower(ranktoken[0]);

  return Card(suit, rankc);
}

int main(int argv, char** argc){
  if(argv < 3){
    cout << "Please provide 2 file names" << endl;
    return 1;
  }
  
  ifstream cardFile1 (argc[1]);
  ifstream cardFile2 (argc[2]);
  string line;

  if (cardFile1.fail() || cardFile2.fail() ){
    cout << "Could not open file " << argc[2];
    return 1;
  }

  // Read each file into CardList
  CardList alice;
  CardList bob;

  while (getline(cardFile1, line)){
    if(line.empty()) continue;
    // parse and insert
    auto c = parseCard(line);
    alice.insert(c);
  }
  cardFile1.close();

  while (getline(cardFile2, line)){
    if(line.empty()) continue;
    auto c = parseCard(line);
    bob.insert(c);
  }
  cardFile2.close();

  // play the game using CardList implementation
  playGame(alice, bob);

  // Print final hands (single-line format expected by autograder)
  std::cout << "Alice's final hand:";
  alice.print(std::cout);
  std::cout << std::endl;

  std::cout << "Bob's final hand:";
  bob.print(std::cout);
  std::cout << std::endl;

  return 0;
}
