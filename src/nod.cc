#include <iostream>
#include <map>
#include <unordered_map>
//#include <tuple>
#include <string>
#include <regex>

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
road_map road_km;

// VEHICLE MAP
// same problem with km's
std::map<std::string, std::pair<unsigned long, unsigned long>> vehicle_km;

using line = std::pair<int, std::string>;
std::unordered_map<std::string, line> driving_vehicles;

// INPUT
// Important regexes.
std::string white_space = R"([ \t\r\v\f])";
std::string road_name = "[AS][1-9]\\d{0,2}";
std::string car_name = "[[:alnum:]]{3,11}";
std::string kilometer = "([1-9]\\d*|0),\\d";
std::string vehicle_line =
    "(" + white_space + "*" + car_name + white_space + "+" + road_name
    + white_space + "+" + kilometer + white_space + "*)?\\n?";
std::string request_line =
    "(" + white_space + "*\\?" + white_space + "*(" + car_name + "|" + road_name
    + ")?" + white_space + "*)?\\n?";
std::string correct_line = vehicle_line + "|" + request_line;

bool is_correct_line(const std::string& s) {
    return std::regex_match(s, std::regex(correct_line));
}

// Gets line and tries to extract 1) request or 2) vehicle information.
// Calls error handler if fails.
// If succeeds, passes extracted information to the engine.
void road_tolls() {
    std::string str;
    std::getline(std::cin, str);

    while (std::cin.good()) {
        // \n is not stored! So need to append it to get a raw input string.
        str += "\n";
        // check if line has correct syntax
        std::cout << is_correct_line(str) << std::endl;
        std::getline(std::cin, str);
    }

    if (std::cin.fail() || std::cin.bad())
        std::cerr << "Error in the input\n";
    else if (std::cin.eof()) {
        std::cout << str << std::endl;
        // check if line has correct syntax
    }
}
/*
std::tuple<std::string, std::string, unsigned long> extract(string s) {
    // do some work
    return {vehicle_name, road_name, km};
}

// Inserts vehicle info into vehicle maps, prints error if neede.
// Updates road_km.
// line l has correct syntax.
void insert(line l) {
    auto [vehicle_name, road_name, km] = extract(l.second);
    // do some work
}
*/

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
