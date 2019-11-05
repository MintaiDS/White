#include <iostream>

#include "../Math/Vector.h"

int main() {
    int n;
    std::vector<int> a;
    std::cin >> n;
    for (int i = 0; i < n; ++i) {
        int x;
        std::cin >> x;
        a.push_back(x);
    }
}
