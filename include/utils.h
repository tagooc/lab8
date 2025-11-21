#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <utility>

// Функция для парсинга координат из строки "(x, y)"
std::pair<int, int> parseCoordinates(const std::string& coord) {
    // Убираем скобки и разделяем по запятой
    std::string clean = coord.substr(1, coord.length() - 2);
    size_t comma_pos = clean.find(',');
    int x = std::stoi(clean.substr(0, comma_pos));
    int y = std::stoi(clean.substr(comma_pos + 1));
    return {x, y};
}

#endif // UTILS_H