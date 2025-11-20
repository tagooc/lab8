#ifndef UTILS_H
#define UTILS_H

#include <unordered_set>

std::unordered_set<int, int> parseCoordinates(const std::string& coord) {
    std::stringstream ss(coord);
    char discard;
    int x, y;
    ss >> discard >> x >> discard >> y >> discard;
    return {x, y};
}

#endif