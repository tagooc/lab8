#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <utility>
#include "config_loader.h"


// Функция для парсинга координат из строки "(x, y)"
std::pair<int, int> parseCoordinates(const std::string& coord) {
    // Убираем скобки и разделяем по запятой
    std::string clean = coord.substr(1, coord.length() - 2);
    size_t comma_pos = clean.find(',');
    int x = std::stoi(clean.substr(0, comma_pos));
    int y = std::stoi(clean.substr(comma_pos + 1));
    return {x, y};
}
void save_path(const std::vector<std::string>& path, const std::string& filename) {
    auto& cfg = Config::get();
    std::ofstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "❌ Ошибка: Не удалось открыть файл " << filename << " для записи" << std::endl;
        return;
    }
    
    // Записываем только вершины через пробел
    for (const auto& vertex : path) {
        file << vertex << " ";
    }
    
    file.close();
    std::cout << "✅ Путь сохранен в файл: " << filename << std::endl;
}

#endif // UTILS_H