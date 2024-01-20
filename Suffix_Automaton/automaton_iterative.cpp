#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stack>
#include <map>
#include <string>
#include <vector>

using namespace std;

#define ALPHABETSIZE 26

inline void countSquare(uint32_t node);

string input;

struct State {
    uint32_t len, link;
    vector<uint32_t> suffixChildren;
    uint32_t next[ALPHABETSIZE];
    uint32_t firstpos;
    uint32_t candidateFor = -1;

    State() {
        for (int i = 0; i < ALPHABETSIZE; i++) {
            next[i] = -1;
        }
    }
};

vector<State> st;
uint32_t sz, last, transitions, finals, doublesCount;

void sa_init(uint32_t n) {
    st.resize(2 * n);
    st[0].len = 0;
    st[0].link = -1;
    st[0].firstpos = 0;
    sz++;
    last = 0;
}

inline void sa_extend(char c) {

    // Ugly hack, don't do this
    c = c - 97;

    uint32_t cur = sz++;
    st[cur].len = st[last].len + 1;
    st[cur].firstpos = 0;
    uint32_t p = last;
    while (p != -1 && st[p].next[c] == -1) {
        st[p].next[c] = cur;

        // We're sure there's no transition for the character c
        transitions++;
        p = st[p].link;
    }
    if (p == -1) {
        st[cur].link = 0;

    } else {
        uint32_t q = st[p].next[c];
        if (st[p].len + 1 == st[q].len) {
            st[cur].link = q;

        } else {
            uint32_t clone = sz++;
            st[clone].len = st[p].len + 1;

            for (int i = 0; i < ALPHABETSIZE; i++) {
                st[clone].next[i] = st[q].next[i];
                if (st[clone].next[i] != -1)
                    transitions++;
            }
            st[clone].link = st[q].link;

            st[clone].firstpos = st[cur].len - st[clone].len;

            while (p != -1 && st[p].next[c] == q) {
                st[p].next[c] = clone;
                p = st[p].link;
            }
            st[q].link = st[cur].link = clone;
        }
    }
    last = cur;
}

vector<uint32_t> vecStack;

void constructSuffixChildren() {
    
    for (int i = 0; i < sz; i++) {
        if (st[i].link != -1) st[st[i].link].suffixChildren.push_back(i);
    }
}

void prefix_dfs() {

    std::stack<pair<uint32_t, uint32_t>> recursion;

    recursion.push({0, 0});

    uint32_t depth = 0;
    while (!recursion.empty()) {
        pair<uint32_t, uint32_t> curr = recursion.top();

        if (depth > curr.second) {
            int diff = depth - curr.second;
            while (diff--) vecStack.pop_back();
            depth = curr.second;
        }
        
        vecStack.push_back(curr.first);
        recursion.pop();

        uint32_t check = vecStack.size() / 2;

        if (st[vecStack[check]].len * 2 == st[curr.first].len) {
            st[curr.first].candidateFor = vecStack[check];
        }

        for (uint32_t neighbour : st[curr.first].next) {
            if (neighbour != -1 && st[neighbour].len == st[curr.first].len + 1) {

                recursion.push({ neighbour, curr.second + 1 });

            }
        }
        depth++;


    }
}

void debugPrintSuffixChildren() {


    for (int i = 0; i < sz; i++) {

        cout << "CURR: " << i << '\n';
        cout << "CHILDREN\n";
        for (auto child : st[i].suffixChildren) {
            cout << child << '\n';
        }
    }

}

void suffix_dfs() {
    

    std::stack<pair<uint32_t, uint32_t>> recursion;

    recursion.push({0, 0});

    uint32_t depth = 0;
    while (!recursion.empty()) {
        pair<uint32_t, uint32_t> curr = recursion.top();

        if (depth > curr.second) {
            int diff = depth - curr.second;
            while (diff--) vecStack.pop_back();
            depth = curr.second;
        }
        

        vecStack.push_back(curr.first);
        recursion.pop();

        uint32_t check = vecStack.size() / 2;

        if (st[vecStack[check]].len * 2 == st[curr.first].len && st[curr.first].candidateFor == vecStack[check]) {
            doublesCount++;
        }

        for (uint32_t neighbour : st[curr.first].suffixChildren) {
            if (neighbour != -1 && st[neighbour].len == st[curr.first].len + 1) {

                recursion.push({ neighbour, curr.second + 1 });

                //prefix_dfs(neighbour);
            }
        }
        depth++;


    }
}

inline void find_finals() {
    uint32_t p = last;
    while (p != 0) {
        finals++;
        p = st[p].link;
    }
    finals++;
}

int main(int argc, char** argv) {

    if (argc != 2) {
        printf("Wrong number of input\n");
    }

    ifstream myfile(argv[1]);

    myfile >> input;
    sa_init(input.size());

    for (uint32_t i = 0; i < input.size(); i++) {
        sa_extend(input[i]);
    }

    constructSuffixChildren();

    find_finals();

    prefix_dfs();

    vecStack.clear();

    suffix_dfs();

    printf("%d\n%d\n%d\n%d\n", sz,
            transitions, finals, doublesCount);

    myfile.close();
}
