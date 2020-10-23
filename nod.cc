#include <iostream>
#include <map>

// ROAD_MAP DATA TYPE
enum road_type {
    A, S
};

using road = std::pair<road_type, short>;

struct road_comparator {
    bool operator()(const road& a, const road& b) const {
        if (a.first == b.first)
            return a < b;
        return a.first < b.first;
    }
};

// Don't know if unsigned long is enough. Maybe long long?
// Or unsigned long long?
// No big_int since custom classes aren't allowed.
using road_map = std::map<road, unsigned long, road_comparator>;

int main()
{
    // Test tego jak działa mapa.
    road_map x;
    x[std::make_pair(S, 1)] = 4;
    x[std::make_pair(S, 0)] = 3;
    x[std::make_pair(A, 0)] = 1;
    x[std::make_pair(A, 1)] = 2;
    for (auto const& [key, val] : x)
        std::cout << val << std::endl;

    return 0;
}
