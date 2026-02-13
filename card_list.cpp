// card_list.cpp
// Author: Owen Kirchner
// Implementation of the classes defined in card_list.h

#include "card_list.h"
#include <iostream>

// Node Constructor
CardList::Node::Node(const Card& c) : card(c), left(nullptr), right(nullptr) {
}

// CardList Constructor
CardList::CardList() : root(nullptr) {
}

// CardList Copy constructor
CardList::CardList(const CardList& other) : root(copy_helper(other.root)) {
}

// CardList Copy-assignment
CardList& CardList::operator=(const CardList& other) {
    if (this != &other) {
        delete_helper(root);
        root = copy_helper(other.root);
    }
    return *this;
}

// CardList Destructor
CardList::~CardList() {
    delete_helper(root);
}

// Helper function to delete all nodes
void CardList::delete_helper(Node* node) {
    if (node == nullptr) return;
    // post-order deletion
    delete_helper(node->left);
    delete_helper(node->right);
    delete node;
}

// Helper to deep-copy a subtree
CardList::Node* CardList::copy_helper(Node* node) const {
    if (node == nullptr) return nullptr;
    Node* n = new Node(node->card);
    n->left = copy_helper(node->left);
    n->right = copy_helper(node->right);
    return n;
}

// Insert a card into the BST
void CardList::insert(const Card& card) {
    root = insert_helper(root, card);
}

// Helper function for insert
CardList::Node* CardList::insert_helper(Node* node, const Card& card) {
    if (node == nullptr) {
        return new Node(card);
    }

    if (card < node->card) {
        node->left = insert_helper(node->left, card);
    } else if (card > node->card) {
        node->right = insert_helper(node->right, card);
    } // if equal, do nothing (no duplicates)

    return node;
}

// Remove a card from the BST
void CardList::remove(const Card& card) {
    root = remove_helper(root, card);
}

// Helper function for remove
CardList::Node* CardList::remove_helper(Node* node, const Card& card) {
    if (node == nullptr) return nullptr;

    if (card < node->card) {
        node->left = remove_helper(node->left, card);
        return node;
    } else if (card > node->card) {
        node->right = remove_helper(node->right, card);
        return node;
    }

    // node->card == card: remove this node
    if (node->left == nullptr) {
        Node* rightChild = node->right;
        delete node;
        return rightChild;
    } else if (node->right == nullptr) {
        Node* leftChild = node->left;
        delete node;
        return leftChild;
    } else {
        // two children: replace with inorder successor (smallest in right subtree)
        Node* succ = node->right;
        while (succ->left != nullptr) succ = succ->left;
        node->card = succ->card; // copy value
        node->right = remove_helper(node->right, succ->card); // remove successor
        return node;
    }
}

// Search for a card in the BST (internal)
bool CardList::search(const Card& card) const {
    return search_helper(root, card);
}

// Public API expected by assignment
bool CardList::contains(const Card& card) const {
    return search(card);
}

// Helper function for search
bool CardList::search_helper(Node* node, const Card& card) const {
    if (node == nullptr) return false;
    if (card == node->card) return true;
    if (card < node->card) return search_helper(node->left, card);
    return search_helper(node->right, card);
}

// Print all cards in the BST (in-order traversal)
void CardList::print(std::ostream& os) const {
    print_helper(root, os);
}

// Helper function for print
void CardList::print_helper(Node* node, std::ostream& os) const {
    if (node == nullptr) return;
    print_helper(node->left, os);
    os << ' ' << node->card;
    print_helper(node->right, os);
}

// iterator implementation

// iterator helpers
CardList::iterator::iterator() : node(nullptr), tree(nullptr) {}
CardList::iterator::iterator(Node* n, const CardList* t) : node(n), tree(t) {}

// dereference
CardList::iterator::reference CardList::iterator::operator*() const { return node->card; }
CardList::iterator::pointer CardList::iterator::operator->() const { return &node->card; }

// find minimum from a node
CardList::Node* CardList::minimumNode(Node* n) {
    if (!n) return nullptr;
    while (n->left) n = n->left;
    return n;
}
// find maximum from a node
CardList::Node* CardList::maximumNode(Node* n) {
    if (!n) return nullptr;
    while (n->right) n = n->right;
    return n;
}

// successor: next larger node in the tree (or nullptr if none)
CardList::Node* CardList::iterator::successor(Node* n) const {
    if (n == nullptr) return nullptr;
    if (n->right) return minimumNode(n->right);

    // walk from root to find lowest ancestor greater than n
    Node* succ = nullptr;
    Node* cur = tree->root;
    while (cur) {
        if (n->card < cur->card) {
            succ = cur;
            cur = cur->left;
        } else if (n->card > cur->card) {
            cur = cur->right;
        } else break;
    }
    return succ;
}

// predecessor: next smaller node in the tree (or nullptr if none)
CardList::Node* CardList::iterator::predecessor(Node* n) const {
    if (n == nullptr) return nullptr;
    if (n->left) return maximumNode(n->left);

    Node* pred = nullptr;
    Node* cur = tree->root;
    while (cur) {
        if (n->card > cur->card) {
            pred = cur;
            cur = cur->right;
        } else if (n->card < cur->card) {
            cur = cur->left;
        } else break;
    }
    return pred;
}

// pre-increment: move to successor
CardList::iterator& CardList::iterator::operator++() {
    node = successor(node);
    return *this;
}
CardList::iterator CardList::iterator::operator++(int) {
    iterator tmp = *this;
    ++(*this);
    return tmp;
}

// pre-decrement: move to predecessor; if node==nullptr (end()) move to maximum
CardList::iterator& CardList::iterator::operator--() {
    if (node == nullptr) {
        node = maximumNode(tree->root);
    } else {
        node = predecessor(node);
    }
    return *this;
}
CardList::iterator CardList::iterator::operator--(int) {
    iterator tmp = *this;
    --(*this);
    return tmp;
}

bool CardList::iterator::operator==(const iterator& other) const {
    return node == other.node;
}
bool CardList::iterator::operator!=(const iterator& other) const {
    return !(*this == other);
}

// CardList iterator entry points
CardList::iterator CardList::begin() const { return iterator(minimumNode(root), this); }
CardList::iterator CardList::end() const { return iterator(nullptr, this); }
CardList::iterator CardList::rbegin() const { return iterator(maximumNode(root), this); }
CardList::iterator CardList::rend() const { return iterator(nullptr, this); }

// playGame: manage game logic using only public CardList methods + iterators
void playGame(CardList &alice, CardList &bob) {
    while (true) {
        bool aliceFound = false;
        // Alice: smallest -> largest
        for (CardList::iterator it = alice.begin(); it != alice.end(); ++it) {
            Card c = *it;
            if (bob.contains(c)) {
                std::cout << "Alice picked matching card " << c << std::endl;
                bob.remove(c);
                alice.remove(c);
                aliceFound = true;
                break;
            }
        }
        if (!aliceFound) break;

        bool bobFound = false;
        // Bob: largest -> smallest (use rbegin()/rend() and operator--)
        for (CardList::iterator rit = bob.rbegin(); rit != bob.rend(); --rit) {
            Card c = *rit;
            if (alice.contains(c)) {
                std::cout << "Bob picked matching card " << c << std::endl;
                alice.remove(c);
                bob.remove(c);
                bobFound = true;
                break;
            }
        }
        if (!bobFound) break;
    }
}

