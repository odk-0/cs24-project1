#include "card_list.h"
#include "card.h"

#include <iostream>
#include <sstream>
#include <vector>
#include <cassert>
#include <algorithm>

using namespace std;

// helpers for tests
static vector<Card> seq_inorder(const CardList &t) {
    vector<Card> v;
    for (CardList::iterator it = t.begin(); it != t.end(); ++it) v.push_back(*it);
    return v;
}
static vector<Card> seq_reverse(const CardList &t) {
    vector<Card> v;
    for (CardList::iterator it = t.rbegin(); it != t.rend(); --it) v.push_back(*it);
    return v;
}
static int count_occurrences(const CardList &t, const Card &c) {
    int cnt = 0;
    for (CardList::iterator it = t.begin(); it != t.end(); ++it) if (*it == c) ++cnt;
    return cnt;
}

static string print_to_string(const CardList &t) {
    std::ostringstream ss;
    t.print(ss);
    return ss.str();
}

int main() {
    cout << "===== CardList/iterator/playGame tests =====" << endl;

    // ===== 1) Empty-tree behavior (search/print/iterators) =====
    {
        CardList t;
        assert(t.contains(Card('c','a')) == false); // empty contains check
        assert(t.begin() == t.end());
        assert(t.rbegin() == t.rend());
        assert(print_to_string(t).empty());
    }
    cout << "Empty-tree tests passed." << endl;

    // ===== 2) Single-node: insert, search, iterator increment/decrement, print =====
    {
        CardList t;
        Card a('h','k');
        t.insert(a);
        assert(t.contains(a) == true);

        // iterator points to the single node
        auto it = t.begin();
        assert(it != t.end());
        assert(*it == a);

        // advancing reaches end()
        ++it;
        assert(it == t.end());

        // decrementing end() returns last element
        --it;
        assert(it != t.end());
        assert(*it == a);

        // reverse iterators behave the same for single node
        auto rit = t.rbegin();
        assert(*rit == a);
        --rit; // -- on rbegin() should move toward rend() (becomes rend())
        assert(rit == t.rend());

        // print contains exactly one card (leading space + "suit rank")
        string s = print_to_string(t);
        assert(s == string(" ") + "h" + " " + "k");
    }
    cout << "Single-node tests passed." << endl;

    // ===== 3) Multiple nodes: ordering, remove (leaf/one-child/two-children), duplicates =====
    {
        CardList t;
        // insert in unsorted order
        Card c1('d','5');
        Card c2('c','a');
        Card c3('h','k');
        Card c4('s','2');
        Card c5('d','9');

        t.insert(c1);
        t.insert(c2);
        t.insert(c3);
        t.insert(c4);

        // inorder should be c2 (c a), c1 (d 5), c4 (s 2), c3 (h k)
        vector<Card> expected = { c2, c1, c4, c3 };
        assert(seq_inorder(t) == expected);

        // remove a leaf (s 2)
        t.remove(c4);
        expected = { c2, c1, c3 };
        assert(seq_inorder(t) == expected);

        // remove non-existent (no-op)
        t.remove(c5);
        assert(seq_inorder(t) == expected);

        // insert duplicate (should not create duplicate nodes)
        t.insert(c1);
        assert(count_occurrences(t, c1) == 1);

        // remove node with two children: make a situation with two children
        // insert s 2 back and another value so d5 has two children
        t.insert(c4);
        t.insert(Card('d','2')); // will be left child of d5
        // now remove d5 (has two children)
        t.remove(c1);
        // ensure d5 gone and tree still correct (no crash)
        for (auto &x : seq_inorder(t)) assert(!(x == c1));
    }
    cout << "Multiple-node/remove/duplicates tests passed." << endl;

    // ===== 4) Iterator traversal and comparisons (forward + reverse) =====
    {
        CardList t;
        vector<Card> input = { Card('h','q'), Card('c','2'), Card('d','3'), Card('s','t'), Card('c','k') };
        for (auto &c : input) t.insert(c);

        // forward traversal = sorted order per Card::operator< (suit then rank)
        vector<Card> forward = seq_inorder(t);
        // prepare expected by sorting a copy using operator<
        vector<Card> expected = input;
        sort(expected.begin(), expected.end());
        assert(forward == expected);

        // reverse traversal
        vector<Card> rev = seq_reverse(t);
        reverse(expected.begin(), expected.end());
        assert(rev == expected);

        // iterator equality/inequality tests
        auto it1 = t.begin();
        auto it2 = t.begin();
        assert(it1 == it2);
        ++it2;
        assert(it1 != it2);

        // safe behavior: ++end() stays end(); --rend() yields last element
        auto itend = t.end();
        ++itend;
        assert(itend == t.end());
        --itend; // now should be last element
        assert(*itend == rev.front());

        auto rit = t.rend();
        --rit;
        assert(*rit == rev.front());
        ++rit; // back to rend()
        assert(rit == t.rend());
    }
    cout << "Iterator traversal and comparison tests passed." << endl;

    // ===== 5) Copy constructor & assignment (deep copy / memory correctness) =====
    {
        CardList a;
        a.insert(Card('c','a'));
        a.insert(Card('d','2'));

        CardList b = a; // copy ctor
        assert(seq_inorder(a) == seq_inorder(b));

        // modify original, copy remains unchanged
        a.remove(Card('c','a'));
        assert(b.contains(Card('c','a')) == true);
        assert(a.contains(Card('c','a')) == false);

        // assignment
        CardList c;
        c = b;
        assert(seq_inorder(c) == seq_inorder(b));
        c.remove(Card('d','2'));
        assert(b.contains(Card('d','2')) == true);
    }
    cout << "Copy/assignment tests passed." << endl;

    // ===== 6) playGame tests =====
    {
        // A) both players with common cards
        CardList alice;
        CardList bob;
        alice.insert(Card('c','a'));
        alice.insert(Card('d','2'));
        bob.insert(Card('c','a'));
        bob.insert(Card('s','3'));

        playGame(alice, bob);
        // common card c a should be removed from both
        assert(!alice.contains(Card('c','a')));
        assert(!bob.contains(Card('c','a')));

        // B) one empty hand (early exit)
        CardList empty;
        CardList nonempty;
        nonempty.insert(Card('h','k'));
        playGame(empty, nonempty);
        assert(!empty.contains(Card('h','k')));
        assert(nonempty.contains(Card('h','k')) == true); // unchanged

        // C) no common cards -> no removals
        CardList A;
        CardList B;
        A.insert(Card('c','2'));
        A.insert(Card('d','3'));
        B.insert(Card('s','4'));
        B.insert(Card('h','5'));
        playGame(A, B);
        // both unchanged
        assert(A.contains(Card('c','2')) && A.contains(Card('d','3')));
        assert(B.contains(Card('s','4')) && B.contains(Card('h','5')));
    }
    cout << "playGame tests passed." << endl;

    cout << "\nALL tests passed successfully." << endl;
    return 0;
}
