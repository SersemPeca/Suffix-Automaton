#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

#define ALPHABETSIZE 26

inline void countSquare(uint32_t node);

struct State {
    uint32_t len, link;
    uint32_t next[ALPHABETSIZE];
    bool isPrefixState = true;

    State() {
        for (int i = 0; i < ALPHABETSIZE; i++) {
            next[i] = -1;
        }
    }
};

vector<State> st;
uint32_t sz, last, transitions, finals, prefixDoubles;

void sa_init(uint32_t n) {
    st.resize(2 * n);
    st[0].len = 0;
    st[0].link = -1;
    sz++;
    last = 0;
}

inline void sa_extend(char c) {

    // Ugly hack, don't do this
    c = c - 97;

    uint32_t cur = sz++;
    st[cur].len = st[last].len + 1;
    uint32_t p = last;
    while (p != -1 && st[p].next[c] == -1) {
        st[p].next[c] = cur;

        // We're sure there's no transition for the character c
        transitions++;
        p = st[p].link;
    }
    if (p == -1) {
        st[cur].link = 0;
        countSquare(cur);

    } else {
        uint32_t q = st[p].next[c];
        if (st[p].len + 1 == st[q].len) {
            st[cur].link = q;
            countSquare(cur);
        } else {
            uint32_t clone = sz++;
            st[clone].len = st[p].len + 1;
            // st[clone].next = st[q].next;

            for (int i = 0; i < ALPHABETSIZE; i++) {
                st[clone].next[i] = st[q].next[i];
                if (st[clone].next[i] != -1)
                    transitions++;
            }
            // transitions += st[clone].next.size();
            st[clone].link = st[q].link;

            // st[clone].isPrefixState = false;

            while (p != -1 && st[p].next[c] == q) {
                st[p].next[c] = clone;
                p = st[p].link;
            }
            st[q].link = st[cur].link = clone;
            countSquare(clone);
            //  transitions++;
        }
    }
    last = cur;
}

inline void find_finals() {
    uint32_t p = last;
    while (p != 0) {
        finals++;
        p = st[p].link;
    }
    finals++;
}

inline void countSquare(uint32_t node) {
    uint32_t len = st[node].len;
    uint32_t p = node;

    while (p != 0) {
        if ( (st[p].len << 1) == st[node].len) {
            prefixDoubles++;
            return;
        }
        p = st[p].link;
    }
}

uint32_t countPrefixSquares() {
    uint32_t count = 0;
    for (size_t i = 2; i < sz; i += 2) {
        uint32_t iter = st[i].link;
        while (st[iter].len > (i >> 1)) {
            iter = st[iter].link;
        }
        if (st[iter].len == (i >> 1) && iter < sz && iter != 0 &&
                st[iter].isPrefixState)
            count++;
    }
    return count;
}

int main() {

    ifstream myfile;
    myfile.open("./word.txt");

    string input;
    myfile >> input;
    sa_init(input.size());
    // cout << "DONE\n";

    for (uint32_t i = 0; i < input.size(); i++) {
        sa_extend(input[i]);
    }

    find_finals();

    printf("STATES: %d, TRANSITIONS: %d, FINALS: %d, (PREF) STATES: %d\n", sz,
            transitions, finals, prefixDoubles);
}
