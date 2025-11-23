#include "include/graph.h"
#include "include/a_star.h"
#include "include/theta.h"
#include "include/utils.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <sstream>

#include "include/config_loader.h"



#include <vector>


using json = nlohmann::json;



int main() {
    auto& cfg = Config::get();

    Graph g;

    try {
        // Загружаем JSON файл с графом
        std::ifstream graph_file(cfg.file_graph());
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
        std::cerr << "❌ Ошибка при загрузке данных графа: " << e.what() << std::endl;
        return 1;
    }
    std::string startVertex, goalVertex;

//---------------------
    try {
        std::ifstream breaks_file(cfg.file_breaks());
        if (!breaks_file.is_open()) {
            std::cerr << "Ошибка: Не удалось открыть breaks.json" << std::endl;
            return 1;
        }
        json start_end = json::parse(breaks_file);
        breaks_file.close();
        startVertex = start_end["start"];
        goalVertex = start_end["end"];
        

        std::cout << "✅ Старт и конец успешно загружены из breaks.json" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "❌ Ошибка при загрузке данных: " << e.what() << std::endl;
        return 1;
    }
//-----------------------


    std::cout << "🎯 Начальная вершина: " << startVertex << std::endl;
    std::cout << "🏁 Конечная вершина: " << goalVertex << std::endl;
    std::cout << "🔍 Запуск алгоритма A*..." << std::endl;

    // Выполняем поиск A*
    std::vector<std::string> path = aStarSearch(g, startVertex, goalVertex);

    // Выводим результат
    std::cout << "\n📊 Результат поиска A*:" << std::endl;
    printPath(path);
    save_path(path, cfg.file_path());
//----------------------


    std::cout << "🎯 Запуск алгоритма Theta*..." << std::endl;
    std::vector<std::string> theta_path = thetaStarSearch(g, startVertex, goalVertex);

    std::cout << "\n📊 Результат поиска Theta*:" << std::endl;
    printThetaPath(theta_path);

    // Сохранение пути
    if (!theta_path.empty()) {
        auto& cfg = Config::get();
        save_path(theta_path, cfg.file_path_theta());
    }

    return 0;
}
