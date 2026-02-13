// card.cpp
// Author: Owen Kirchner
// Implementation of the classes defined in card.h

#include "card.h"
#include <ostream>
#include <array>
#include <algorithm>

// Constructors
Card::Card() : suit(' '), rank(' ') {}
Card::Card(char s, char r) : suit(s), rank(r) {}

// Accessors
char Card::getSuit() const { return suit; }
char Card::getRank() const { return rank; }

// Print to a stream (simple default format)
void Card::print(std::ostream &os) const {
	// print rank 't' as "10" for human-readable output
	os << suit << " ";
	if (rank == 't') os << "10";
	else os << rank;
}

// Operators
bool operator==(const Card &a, const Card &b) {
	return (a.getRank() == b.getRank() && a.getSuit() == b.getSuit());
}

bool operator<(const Card &a, const Card &b) {
    constexpr std::array<char,4> suit_order = {'c', 'd', 's', 'h'}; // ascending order
    auto suit_a = std::find(suit_order.begin(), suit_order.end(), a.getSuit()) - suit_order.begin();
    auto suit_b = std::find(suit_order.begin(), suit_order.end(), b.getSuit()) - suit_order.begin();
    if (suit_a != suit_b) return suit_a < suit_b; // compare suits first

    constexpr std::array<char,13> rank_order = {'a', '2', '3', '4', '5', '6', '7', '8', '9', 't', 'j', 'q', 'k'}; // ascending order
    auto rank_a = std::find(rank_order.begin(), rank_order.end(), a.getRank()) - rank_order.begin();
    auto rank_b = std::find(rank_order.begin(), rank_order.end(), b.getRank()) - rank_order.begin();
    return rank_a < rank_b; // compare ranks if suits are the same
    
}

bool operator>(const Card &a, const Card &b) { return b < a; }

std::ostream &operator<<(std::ostream &os, const Card &c) {
	c.print(os);
	return os;
}