#include <iostream>
#include <map>
#include <unordered_map>
#include <string>

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

// VEHICLE MAP
// same problem with km's
std::map<std::string, unsigned long> vehicle_km;

using line = std::pair<int, std::string>;
std::unordered_map<std::string, line> driving_vehicles;

// INPUT
// Gets line and tries to extract 1) request or 2) vehicle information.
// Calls error handler if fails. If succeeds, passes extracted information to the engine.
void road_tolls() {
    std::string str;
    std::getline(std::cin, str);

    while (std::cin.good()) {
        std::cout << str << std::endl;
        std::getline(std::cin, str);
    }

    if (std::cin.fail()) {
        // error dont parse the rest
    }

    if (std::cin.bad()) {
        // error dont parse the rest
    }

    if (std::cin.eof()) {
        // parse
    }
}

int main() {
    /* Test tego jak działa mapa.
    road_map x;
    x[std::make_pair(S, 1)] = 4;
    x[std::make_pair(S, 0)] = 3;
    x[std::make_pair(A, 0)] = 1;
    x[std::make_pair(A, 1)] = 2;
    for (auto const& [key, val] : x)
        std::cout << val << std::endl;

    vehicle_km["Car1"] = 100;

    driving_vehicles["Car1"] = std::make_pair(1, "Car1 S1 10,0");
    */

    road_tolls();
    return 0;
}
