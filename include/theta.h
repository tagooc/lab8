#ifndef THETA_H
#define THETA_H

#include "graph.h"
#include "utils.h"
#include "config_loader.h"
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iostream>

// Структура для узла в алгоритме Theta*
struct ThetaNode {
    std::string vertex;
    std::string parent;
    double g; // стоимость пути от старта
    double h; // эвристическая оценка до цели
    double f; // f = g + h
    
    ThetaNode(const std::string& v, const std::string& p, double g_val, double h_val) 
        : vertex(v), parent(p), g(g_val), h(h_val), f(g_val + h_val) {}
    
    bool operator>(const ThetaNode& other) const {
        return f > other.f;
    }
};

// Линейная интерполяция для проверки прямой видимости
bool lineOfSight(const std::string& a, const std::string& b, Graph& graph) {
    auto [x1, y1] = parseCoordinates(a);
    auto [x2, y2] = parseCoordinates(b);
    
    int dx = std::abs(x2 - x1);
    int dy = std::abs(y2 - y1);
    int x = x1, y = y1;
    int n = 1 + dx + dy;
    int x_inc = (x2 > x1) ? 1 : -1;
    int y_inc = (y2 > y1) ? 1 : -1;
    int error = dx - dy;
    dx *= 2;
    dy *= 2;
    
    for (; n > 0; --n) {
        // Проверяем, является ли текущая клетка препятствием
        std::string current_vertex = "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
        
        // Если клетка не существует в графе (является препятствием)
        if (graph.getNeighbors(current_vertex).empty()) {
            return false;
        }
        
        if (error > 0) {
            x += x_inc;
            error -= dy;
        } else {
            y += y_inc;
            error += dx;
        }
    }
    
    return true;
}

// Эвристическая функция (Евклидово расстояние)
double euclideanDistance(const std::string& a, const std::string& b) {
    auto [x1, y1] = parseCoordinates(a);
    auto [x2, y2] = parseCoordinates(b);
    return std::sqrt(std::pow(x1 - x2, 2) + std::pow(y1 - y2, 2));
}

// Алгоритм Theta* поиска пути
std::vector<std::string> thetaStarSearch(Graph& graph, const std::string& start, const std::string& goal) {
    // Приоритетная очередь для открытого множества
    std::priority_queue<ThetaNode, std::vector<ThetaNode>, std::greater<ThetaNode>> openSet;
    
    // Для отслеживания стоимости пути
    std::unordered_map<std::string, double> gScore;
    std::unordered_map<std::string, std::string> parent;
    std::unordered_set<std::string> closedSet;
    
    // Инициализация
    gScore[start] = 0.0;
    parent[start] = start;
    openSet.push(ThetaNode(start, start, 0.0, euclideanDistance(start, goal)));
    
    while (!openSet.empty()) {
        // Извлекаем узел с наименьшим f
        ThetaNode current = openSet.top();
        openSet.pop();
        
        std::string current_vertex = current.vertex;
        
        // Если достигли цели, восстанавливаем путь
        if (current_vertex == goal) {
            std::vector<std::string> path;
            std::string node = goal;
            
            while (node != start) {
                path.push_back(node);
                node = parent[node];
            }
            path.push_back(start);
            std::reverse(path.begin(), path.end());
            return path;
        }
        
        // Добавляем в закрытое множество
        closedSet.insert(current_vertex);
        
        // Проверяем всех соседей
        auto neighbors = graph.getNeighbors(current_vertex);
        for (const auto& neighbor : neighbors) {
            // Если сосед уже в закрытом множестве, пропускаем
            if (closedSet.find(neighbor) != closedSet.end()) {
                continue;
            }
            
            double tentative_g = gScore[current_vertex] + euclideanDistance(current_vertex, neighbor);
            
            // Основная логика Theta* - проверка прямой видимости к родителю
            if (parent.find(current_vertex) != parent.end() && 
                lineOfSight(parent[current_vertex], neighbor, graph)) {
                
                // Путь через родителя текущей вершины
                double alternative_g = gScore[parent[current_vertex]] + 
                                      euclideanDistance(parent[current_vertex], neighbor);
                
                if (alternative_g < tentative_g) {
                    tentative_g = alternative_g;
                    parent[neighbor] = parent[current_vertex];
                } else {
                    parent[neighbor] = current_vertex;
                }
            } else {
                parent[neighbor] = current_vertex;
            }
            
            // Если нашли лучший путь к соседу
            if (gScore.find(neighbor) == gScore.end() || tentative_g < gScore[neighbor]) {
                gScore[neighbor] = tentative_g;
                double h = euclideanDistance(neighbor, goal);
                openSet.push(ThetaNode(neighbor, parent[neighbor], tentative_g, h));
            }
        }
    }
    
    // Путь не найден
    return {};
}

// Функция для вывода пути Theta*
void printThetaPath(const std::vector<std::string>& path) {
    if (!path.empty()) {
        std::cout << "✅ Theta* путь найден!" << std::endl;
        std::cout << "🛣️  Путь (" << path.size() << " вершин):" << std::endl;
        
        for (size_t i = 0; i < path.size(); ++i) {
            std::cout << path[i];
            if (i != path.size() - 1) {
                std::cout << " -> ";
            }
            if ((i + 1) % 5 == 0) std::cout << std::endl;
        }
        std::cout << std::endl;
        
        // Вычисляем длину пути
        double path_length = 0.0;
        for (size_t i = 1; i < path.size(); ++i) {
            path_length += euclideanDistance(path[i-1], path[i]);
        }
        
        std::cout << "📏 Длина пути: " << path_length << " единиц" << std::endl;
        std::cout << "🔢 Количество сегментов: " << path.size() - 1 << std::endl;
    } else {
        std::cout << "❌ Theta* путь не найден!" << std::endl;
    }
}

#endif // THETA_H