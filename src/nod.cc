#include <cassert>
#include <iostream>
#include <map>
#include <unordered_map>
#include <regex>

//#define NDEBUG

// ROAD_MAP DATA TYPE
enum road_type {
    A, S
};

using road = std::pair<road_type, int>;

struct road_comparator {
    bool operator()(const road& a, const road& b) const {
        if (a.second == b.second)
            return a.first < b.first;
        return a.second < b.second;
    }
};

using road_map = std::map<road, unsigned long, road_comparator>;
static road_map road_km;

// VEHICLE MAP
static std::map<std::string, std::pair<unsigned long, unsigned long>> vehicle_km;

using line = std::pair<int, std::string>;
static std::unordered_map<std::string, line> driving_vehicles;

// Important regexes.
static std::string white_space = R"([ \t\r\v\f])";
static std::string road_name = "[AS][1-9]\\d{0,2}";
static std::string vehicle_name = "[[:alnum:]]{3,11}";
static std::string kilometer = "([1-9]\\d*|0),\\d";
static std::string vehicle_line = white_space + "*" + vehicle_name + white_space
                                  + "+" + road_name + white_space + "+" +
                                  kilometer + "\\s*";
static std::string road_or_vehicle = "(" + vehicle_name + "|" + road_name + ")";
static std::string request_line = white_space + "*\\?" + white_space + "*" +
                                  road_or_vehicle + "?\\s*";
static std::string correct_line = "(" + vehicle_line + "|" + request_line
                                  + ")|\\n";

// Checks if an inputted line is a request line.
bool is_request(const std::string& s) {
    return std::regex_match(s, std::regex(request_line));
}

// Checks if an inputted line is correct.
bool is_correct_line(const std::string& s) {
    return std::regex_match(s, std::regex(correct_line));
}

// Extracts a road string or a vehicle string from an inputted line.
// if the request is "?" then returned string is empty.
std::string extract_request_data(const std::string& s) {
    std::smatch m;
    std::regex_search(s, m, std::regex(road_or_vehicle));
    if (!m.empty())
        return m[0];
    return "";
}

// Removes the first comma from a string.
std::string remove_comma(const std::string& s) {
    std::string ans = std::string(s);
    return ans.erase(ans.find(','), 1);
}

// Converts a string to the road type.
road str_to_road(const std::string& s) {
    road_type t;
    int number = std::stoi(s.substr(1));
    if (s[0] == 'A')
        t = A;
    else
        t = S;
    return std::make_pair(t, number);
}

// Extracts vehicle name, road name (as needed for a key in a road_map)
// and kilometers.
std::tuple<std::string, road, unsigned long> extract_vehicle_data(const std::string& s) {
    std::smatch vehicle_match;
    std::smatch road_match;
    std::smatch km_match;

    std::regex_search(s, km_match, std::regex(kilometer));
    std::regex_search(s, vehicle_match, std::regex(vehicle_name));
    std::string tmp = vehicle_match.suffix().str();
    std::regex_search(tmp, road_match, std::regex(road_name));

    assert(!vehicle_match.empty());
    assert(!road_match.empty());
    assert(!km_match.empty());

    return {vehicle_match[0], str_to_road(road_match[0]),
            std::stoul(remove_comma(km_match[0]))};
}

// Handles "?" request.
void general_request() {
    // TODO
}

// Handles "?VEHICLE" request.
void vehicle_request(const std::string& v) {
    // TODO
}

// Handles "?ROAD" request.
void road_request(road r) {
    // TODO
}

// Handles requests.
void request(const std::string& s) {
    std::string vehicle_or_road = extract_request_data(s);
    if (vehicle_or_road.empty()) {
        general_request();
    } else {
        if (std::regex_match(s, std::regex(vehicle_name)))
            vehicle_request(vehicle_or_road);
        if (std::regex_match(s, std::regex(road_name)))
            road_request(str_to_road(vehicle_or_road));
    }
}

// Inserts vehicle info into vehicle maps, prints error if needed.
// Updates road_km.
// line l has correct syntax.
void insert(const line& l) {
    auto [v, r, km] = extract_vehicle_data(l.second);
    // TODO
}

// Handles a line.
void handle(const line& l) {
    if (l.second.compare("\n") == 0)
        return;
    if (is_request(l.second))
        request(l.second);
    else
        insert(l);
}

// Prints errors of vehicles still being on roads.
void print_errors() {
    // TODO
}

// Gets line and tries to extract 1) request or 2) vehicle information.
// Calls error handler if fails.
// If succeeds, passes extracted information to the engine.
void road_tolls() {
    int line_count = 1;
    std::string str;
    std::getline(std::cin, str);

    while (std::cin.good()) {
        // \n is not stored! So need to append it to get a raw input string.
        str += "\n";
        if (is_correct_line(str))
            // will the str stay?
            handle(std::make_pair(line_count, str));
        else
            std::cerr << "Error in line " << line_count << ": " << str;
        std::getline(std::cin, str);
        ++line_count;
    }

    if (std::cin.bad())
        std::cerr << "Error in the input\n";
    else if (std::cin.eof()) {
        if (is_correct_line(str))
            handle(std::make_pair(line_count, str));
        else
            std::cerr << "Error in line " << line_count << ": " << str
                      << std::endl;
    }

    print_errors();
}

int main() {
    road_tolls();
    return 0;
}
