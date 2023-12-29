#include <cstdint>
#include <map>
#include <vector>

#pragma GCC optimize ("O3")

const uint32_t LEN = 2 * 1e8;

using namespace std;

struct State {
    uint32_t len, link;

    map<char, uint32_t> next;
};

vector<State> state;
uint32_t sz, last;

void init(uint32_t n) {
    state.resize(2 * n);
    state[0].len = 0;
    state[0].link = -1;
    sz++;
    last = 0;
}

void build(char c) {
    uint32_t curr = sz++;
    state[curr].len = state[last].len + 1;
    int p = last;
    while(p != -1 && state[p].next.count(c) == 0) {
        state[p].next[c] = curr;
        p = state[p].link;
    }

    if (p == -1) {
        state[curr].link = 0;
    } else {
        uint32_t q = state[p].next[c];
        if (state[p].len + 1 == state[q].len) {
            state[curr].link = q;
        } else {
            uint32_t clone = sz++;
            state[clone].len = state[p].len + 1;
            state[clone].next = state[q].next;
            state[clone].link = state[q].link;
            while (p != -1 && state[p].next[c] == q) {
                state[p].next[c] = clone;
                p = state[p].link;
            }
            state[q].link = state[curr].link = clone;
        }
    }
    last = curr;
}

int main() {



}
