#include <iostream>
#include <utility>

#include <utility>
#include <map>
#include <string>
#include <set>
#include <unordered_set>
#include <vector>

template <typename T>
void PrintContainer(T &target) {
    for (const auto &i : target)
        std::cout << i << " ";
    std::cout << std::endl;
}

void test() {
    std::unordered_set<int> my_set = {1, 3, 45, 34, 2, 14, 14, 1, 4};

    std::cout << my_set.bucket(3);
}

int main() {
    test();
}