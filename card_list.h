// card_list.h
// Author: Owen Kirchner
// All class declarations related to defining a BST that represents a player's hand

#ifndef CARD_LIST_H
#define CARD_LIST_H

#include "card.h"
#include <iosfwd>
#include <iterator>

class CardList {
private:
    struct Node {
        Card card;
        Node* left;
        Node* right;
        
        Node(const Card& c);
    };
    
    Node* root;
    
    // Helper functions for recursive operations
    Node* insert_helper(Node* node, const Card& card);
    Node* remove_helper(Node* node, const Card& card);
    bool search_helper(Node* node, const Card& card) const;
    void print_helper(Node* node, std::ostream& os) const;
    void delete_helper(Node* node);
    Node* copy_helper(Node* node) const;

    // iterator helpers (minimum/maximum used by iterator implementations)
    static Node* minimumNode(Node* n);
    static Node* maximumNode(Node* n);
    
public:
    // Constructor, Destructor, and copy
    CardList();
    CardList(const CardList& other);
    CardList& operator=(const CardList& other);
    ~CardList();
    
    // Insertion and removal
    void insert(const Card& card);
    void remove(const Card& card);
    
    // Search (public API required by assignment)
    bool contains(const Card& card) const;

    // backward-compatible alias
    bool search(const Card& card) const;
    
    // Print
    void print(std::ostream& os) const;

    // Bidirectional iterator 
    class iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = Card;
        using reference = const Card&;
        using pointer = const Card*;

        iterator();
        reference operator*() const;
        pointer operator->() const;

        iterator& operator++();    // successor (forward)
        iterator operator++(int);
        iterator& operator--();    // predecessor (reverse)
        iterator operator--(int);

        bool operator==(const iterator& other) const;
        bool operator!=(const iterator& other) const;

    private:
        friend class CardList;
        iterator(Node* n, const CardList* tree);
        Node* node;
        const CardList* tree; // needed for successor/predecessor searches when no parent pointer

        Node* successor(Node* n) const;
        Node* predecessor(Node* n) const;
    };

    // iterator entry points
    iterator begin() const;
    iterator end() const;
    iterator rbegin() const; // returns iterator to largest
    iterator rend() const;   // past-the-begin (nullptr)
};

// Game logic function 
void playGame(CardList &alice, CardList &bob);

#endif
