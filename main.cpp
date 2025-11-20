#include "include/graph.h"
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

int main() {
    // Создаем граф
    Graph g;

    try {
        // Загружаем JSON файл
        std::ifstream file("graph.json");
        if (!file.is_open()) {
            std::cerr << "Ошибка: Не удалось открыть graph.json" << std::endl;
            return 1;
        }

        // Парсим JSON
        json graph_data = json::parse(file);
        file.close();

        // Добавляем вершины и ребра из JSON
        for (auto& [vertex, neighbors] : graph_data.items()) {
            std::string vertex_str = vertex;
            
            // Добавляем все ребра для этой вершины
            for (auto& neighbor : neighbors) {
                std::string neighbor_str = neighbor;
                g.addEdge(vertex_str, neighbor_str);
            }
        }

        std::cout << "✅ Граф успешно загружен из graph.json" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "❌ Ошибка при загрузке графа: " << e.what() << std::endl;
        return 1;
    }

    // Выводим граф
    std::cout << "\n📊 Структура графа:" << std::endl;
    g.print();

    return 0;
}