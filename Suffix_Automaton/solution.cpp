#include <cstdint>
#include <map>
#include <vector>

#pragma GCC optimize ("O3")

const int LEN = 2 * 1e8;

using namespace std;

struct State {
    int len, link;

    map<char, int> next;
};

vector<uint32_t> states;

void init(uint32_t n) {
    states.resize(2 * n);
}

int main() {

}
