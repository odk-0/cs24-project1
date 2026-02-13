// card.h
// Author: Owen Kirchner
// All class declarations related to defining a single card go here

#ifndef CARD_H
#define CARD_H

#include <iosfwd>

class Card {
private:
    char suit;
    char rank;

public:
    // Constructors
    Card();
    Card(char suit, char rank);

    // Accessors
    char getSuit() const;
    char getRank() const;

    // Print to a stream
    void print(std::ostream &os) const;


};
//operators
bool operator==(const Card &a, const Card &b);
bool operator<(const Card &a, const Card &b);
bool operator>(const Card &a, const Card &b);
std::ostream &operator<<(std::ostream &os, const Card &c);

#endif
