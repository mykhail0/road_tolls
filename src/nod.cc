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

//
void print_vehicle(const std::string& v) {
	std::pair<unsigned long, unsigned long> km = vehicle_km[v];
    std::cout << v;
    if (km.first != 0) {
        std::cout << " A " << (km.first - 1) / 10 << "," << (km.first - 1) % 10;
    }
    if (km.second != 0) {
        std::cout << " S " << (km.second - 1) / 10 << "," << (km.second - 1) % 10;
    }
    std::cout << std::endl;
}

void print_road(road r) {
	if (r.first == A) {
		std::cout << "A";
	} else {
		std::cout << "S";
	}
	std::cout << r.second << " " << road_km[r] / 10 << "," << road_km[r] % 10 << std::endl;
}

// Handles "?" request.
void general_request() {
    for (auto e : vehicle_km)
        print_vehicle(e.first);
        
    for (auto e : road_km)
        print_road(e.first);

}

// Handles "?VEHICLE" request.
void vehicle_request(const std::string& v) {
    if(vehicle_km.find(v) != vehicle_km.end()) {
        print_vehicle(v);
    }
}

// Handles "?ROAD" request.
void road_request(road r) {
    if(road_km.find(r) != road_km.end()) {
    	print_road(r);
    }
}

// Handles requests.
void request(const std::string& s) {
    std::string vehicle_or_road = extract_request_data(s);
    if (vehicle_or_road.empty()) {
        general_request();
    } else {
        if (std::regex_match(vehicle_or_road, std::regex(vehicle_name)))
            vehicle_request(vehicle_or_road);
        if (std::regex_match(vehicle_or_road, std::regex(road_name)))
            road_request(str_to_road(vehicle_or_road));
    }
}

void print_error(const line& l) {
    std::cerr << "Error in line " << l.first << ": " << l.second;
}

void increase_road_km(const road r, const unsigned long km) {
    if (road_km.find(r) != road_km.end()) {
        auto km_to_add = road_km[r] + km;
        road_km.at(r) = km_to_add;
    } else {
        road_km[r] = km;
    }
}

void increase_vehicle_km(const std::string vehicle, const unsigned long km, const road_type type) {
    if (vehicle_km.find(vehicle) == vehicle_km.end()) {
        vehicle_km[vehicle] = std::make_pair(0,0);
    }
    auto km_to_add = vehicle_km[vehicle];
    if (type == A) {
        if (km_to_add.first == 0) {
            km_to_add.first++;
        }
        km_to_add.first += km;
    } else {
        if (km_to_add.second == 0) {
            km_to_add.second++;
        }
        km_to_add.second += km;
    }
    vehicle_km.at(vehicle) = km_to_add;
    
}

unsigned long distance(unsigned long km, unsigned long km2) {
	if (km > km2) {
		return km - km2;
	} else {
		return km2 - km;
	}
}

// Inserts vehicle info into vehicle maps, prints error if neede.
// Updates road_km.
// line l has correct syntax.
void insert(const line& l) {
    auto [v, r, km] = extract_vehicle_data(l.second);
    if (driving_vehicles.find(v) != driving_vehicles.end()) {
        auto l2 = driving_vehicles[v];
        auto [v2, r2, km2] = extract_vehicle_data(l2.second);
        assert(v == v2);
        if (r == r2) {
            increase_road_km(r, distance(km, km2));
            increase_vehicle_km(v, distance(km, km2), r.first);
            driving_vehicles.erase(v);

        } else {
            print_error(driving_vehicles[v]);
            driving_vehicles.at(v) = l;
        }

    } else {
        driving_vehicles[v] = l;
    }
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

// temporary testing function
void print_maps() {
    std::cout << "Road kilometers\n";
    for (auto e : road_km)
        std::cout << e.first.first << " " << e.first.second << " " << e.second << std::endl;

    std::cout << "vehicle kms\n";
    for (auto e : vehicle_km)
        std::cout << e.first << " " << e.second.first << " " << e.second.second << std::endl;

    std::cout << "driving vehicles\n";
    for (auto e : driving_vehicles)
        std::cout << e.first << " " << e.second.first << " " << e.second.second;
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
        else if (!std::cin.fail())
            std::cerr << "Error in line " << line_count << ": " << str
                      << std::endl;
    }
}

int main() {
    road_tolls();
    return 0;
}
