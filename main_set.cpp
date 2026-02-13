// This file should implement the game using the std::set container class
// Do not include card_list.h in this file
#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include "card.h"
#include <algorithm>
#include <cctype>

using namespace std;

// forward declaration for the parse helper defined below
Card parseCard(const string &s);

// trim helpers
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

  // Read each file into sets
  set<Card> alice;
  set<Card> bob;

  while (getline(cardFile1, line)){
    if(line.empty()) continue;
    alice.insert(parseCard(line));
  }
  cardFile1.close();

  while (getline(cardFile2, line)){
    if(line.empty()) continue;
    bob.insert(parseCard(line));
  }
  cardFile2.close();
  
  // Game loop
  while(true){
    bool aliceFound = false;
    // Alice: iterate from smallest to largest
    for(auto it = alice.begin(); it != alice.end(); ++it){
      const Card c = *it;
      auto itb = bob.find(c);
      if(itb != bob.end()){
        cout << "Alice picked matching card " << c << endl;
        // remove from both sets
        bob.erase(itb);
        // erase current alice iterator safely
        alice.erase(it);
        aliceFound = true;
        break;
      }
    }
    if(!aliceFound) break;

    bool bobFound = false;
    // Bob: iterate from largest to smallest
    for(auto rit = bob.rbegin(); rit != bob.rend(); ++rit){
      const Card c = *rit;
      auto ita = alice.find(c);
      if(ita != alice.end()){
        cout << "Bob picked matching card " << c << endl;
        // remove from both sets: erase from alice and bob
        alice.erase(ita);
        // erase element pointed by reverse_iterator
        bob.erase(std::next(rit).base());
        bobFound = true;
        break;
      }
    }
    if(!bobFound) break;
  }

  // Print remaining cards in per-line format to match o_*.txt expectations
  cout << endl;
  cout << "Alice's cards:" << endl;
  for(const auto &c : alice) cout << c << endl;
  cout << endl;
  cout << "Bob's cards:" << endl;
  for(const auto &c : bob) cout << c << endl;

  return 0;
}

Card parseCard(const string &s) {
  string t = trim_copy(s);
  if(t.empty()) return Card(); // return default card if empty

  char suit = '\0';
  string ranktoken;

  if(t.size() >= 2 && isalpha(t[0])) {
    suit = tolower(t[0]);
    ranktoken = t.substr(1);
    auto pos = ranktoken.find_first_not_of(" \t");
    if(pos != string::npos) ranktoken = ranktoken.substr(pos); // trim leading whitespace
  }

  //normalize rank
  char rankc = ' ';
  if(ranktoken == "10") rankc = 't';
  else if (!ranktoken.empty()) rankc = tolower(ranktoken[0]);

  return Card(suit, rankc);
}
