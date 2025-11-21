#ifndef A_STAR_H
#define A_STAR_H

#include "graph.h"
#include "utils.h"
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <cmath>

// Структура для хранения информации о вершине в алгоритме A*
struct Node {
    std::string vertex;
    int f; // f = g + h
    int g; // стоимость пути от старта
    int h; // эвристическая оценка до цели
    
    Node(const std::string& v, int g_val, int h_val) 
        : vertex(v), g(g_val), h(h_val), f(g_val + h_val) {}
    
    // Для приоритетной очереди (меньший f имеет высший приоритет)
    bool operator>(const Node& other) const {
        return f > other.f;
    }
};

// Манхэттенское расстояние между двумя вершинами
int manhattanDistance(const std::string& a, const std::string& b) {
    auto [x1, y1] = parseCoordinates(a);
    auto [x2, y2] = parseCoordinates(b);
    return std::abs(x1 - x2) + std::abs(y1 - y2);
}

// Алгоритм A* поиска пути
std::vector<std::string> aStarSearch(Graph& graph, const std::string& start, const std::string& goal) {
    // Для работы нам нужен доступ к adjList, добавим метод в Graph:
    // В классе Graph добавить: 
    // const auto& getNeighbors(const std::string& vertex) const {
    //     static std::vector<std::string> empty;
    //     auto it = adjList.find(vertex);
    //     return it != adjList.end() ? it->second : empty;
    // }
    
    // Приоритетная очередь для открытого множества
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> openSet;
    
    // Для отслеживания стоимости пути
    std::unordered_map<std::string, int> gScore;
    
    // Для восстановления пути
    std::unordered_map<std::string, std::string> cameFrom;
    
    // Закрытое множество
    std::unordered_set<std::string> closedSet;
    
    // Инициализация
    gScore[start] = 0;
    openSet.push(Node(start, 0, manhattanDistance(start, goal)));
    
    while (!openSet.empty()) {
        // Извлекаем узел с наименьшим f
        Node current = openSet.top();
        openSet.pop();
        
        std::string currentVertex = current.vertex;
        
        // Если достигли цели, восстанавливаем путь
        if (currentVertex == goal) {
            std::vector<std::string> path;
            std::string node = goal;
            
            while (node != start) {
                path.push_back(node);
                node = cameFrom[node];
            }
            path.push_back(start);
            std::reverse(path.begin(), path.end());
            return path;
        }
        
        // Добавляем в закрытое множество
        closedSet.insert(currentVertex);
        
        // Получаем соседей текущей вершины
        // Для этого нужно добавить метод в Graph класс
        std::vector<std::string> neighbors;
        // Временная реализация - нужно добавить getNeighbors в Graph
        auto it = graph.getAdjList().find(currentVertex);
        if (it != graph.getAdjList().end()) {
            neighbors = it->second;
        }
        
        // Проверяем всех соседей
        for (const auto& neighbor : neighbors) {
            // Если сосед уже в закрытом множестве, пропускаем
            if (closedSet.find(neighbor) != closedSet.end()) {
                continue;
            }
            
            // Вычисляем стоимость пути до соседа
            int tentativeG = gScore[currentVertex] + 1; // все ребра имеют вес 1
            
            // Если это новый путь или лучший, чем предыдущий
            if (gScore.find(neighbor) == gScore.end() || tentativeG < gScore[neighbor]) {
                cameFrom[neighbor] = currentVertex;
                gScore[neighbor] = tentativeG;
                int h = manhattanDistance(neighbor, goal);
                openSet.push(Node(neighbor, tentativeG, h));
            }
        }
    }
    
    // Путь не найден
    return {};
}

// Функция для вывода пути
void printPath(const std::vector<std::string>& path) {
    if (!path.empty()) {
        std::cout << "✅ Путь найден!" << std::endl;
        std::cout << "🛣️  Путь (" << path.size() << " шагов):" << std::endl;
        
        for (size_t i = 0; i < path.size(); ++i) {
            std::cout << path[i];
            if (i != path.size() - 1) {
                std::cout << " -> ";
            }
            if ((i + 1) % 5 == 0) std::cout << std::endl;
        }
        std::cout << std::endl;
        
        std::cout << "📏 Длина пути: " << path.size() - 1 << " шагов" << std::endl;
    } else {
        std::cout << "❌ Путь не найден!" << std::endl;
    }
}

#endif // A_STAR_H