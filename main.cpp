#include "include/graph.h"
#include "include/a_star.h"
#include "include/utils.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <sstream>



#include <vector>


using json = nlohmann::json;



int main() {
    // Создаем граф
    Graph g;

    try {
        // Загружаем JSON файл с графом
        std::ifstream graph_file("graph.json");
        if (!graph_file.is_open()) {
            std::cerr << "Ошибка: Не удалось открыть graph.json" << std::endl;
            return 1;
        }
        json graph_data = json::parse(graph_file);
        graph_file.close();

        // Добавляем вершины и ребра из JSON
        for (auto& [vertex, neighbors] : graph_data.items()) {
            for (auto& neighbor : neighbors) {
                g.addEdge(vertex, neighbor);
            }
        }

        std::cout << "✅ Граф успешно загружен из graph.json" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "❌ Ошибка при загрузке данных: " << e.what() << std::endl;
        return 1;
    }

//---------------------
    std::string startVertex = "(1, 1)";
    std::string goalVertex = "(6, 4)";

    std::cout << "🎯 Начальная вершина: " << startVertex << std::endl;
    std::cout << "🏁 Конечная вершина: " << goalVertex << std::endl;
    std::cout << "🔍 Запуск алгоритма A*..." << std::endl;

    // Выполняем поиск A*
    std::vector<std::string> path = aStarSearch(g, startVertex, goalVertex);

    // Выводим результат
    std::cout << "\n📊 Результат поиска A*:" << std::endl;
    printPath(path);
    save_path(path);



    return 0;
}
