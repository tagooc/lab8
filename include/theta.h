#ifndef THETA_STAR_H
#define THETA_STAR_H

#include "graph.h"
#include "utils.h"
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <cmath>
#include <functional>
#include <limits>

struct ThetaNode {
    std::string vertex;
    double f;
    
    ThetaNode(const std::string& v, double f_val) : vertex(v), f(f_val) {}
    
    bool operator>(const ThetaNode& other) const {
        return f > other.f;
    }
};

bool line_of_sight(const std::string& a, const std::string& b, Graph& graph) {
    // Упрощенная проверка прямой видимости
    // В реальной реализации нужно проверять все промежуточные вершины
    auto [x1, y1] = parseCoordinates(a);
    auto [x2, y2] = parseCoordinates(b);
    
    int dx = std::abs(x2 - x1);
    int dy = std::abs(y2 - y1);
    
    // Простая проверка: считаем что прямая видимость есть для смежных или диагональных вершин
    // В реальном проекте нужно реализовать алгоритм Брезенхема или подобный
    return (dx <= 1 && dy <= 1);
}

double cost(const std::string& a, const std::string& b) {
    auto [x1, y1] = parseCoordinates(a);
    auto [x2, y2] = parseCoordinates(b);
    return std::sqrt(std::pow(x1 - x2, 2) + std::pow(y1 - y2, 2));
}

double heuristic(const std::string& a, const std::string& b) {
    return cost(a, b);
}

void update_vertex(const std::string& s, const std::string& n, const std::string& goal,
                  std::priority_queue<ThetaNode, std::vector<ThetaNode>, std::greater<ThetaNode>>& open,
                  std::unordered_map<std::string, double>& g,
                  std::unordered_map<std::string, std::string>& parent,
                  Graph& graph) {
    
    if (line_of_sight(parent[s], n, graph)) {
        double tentative_g = g[parent[s]] + cost(parent[s], n);
        if (tentative_g < g[n]) {
            g[n] = tentative_g;
            parent[n] = parent[s];
            
            // Remove and reinsert to update priority
            std::vector<ThetaNode> temp;
            while (!open.empty()) {
                ThetaNode node = open.top();
                open.pop();
                if (node.vertex != n) {
                    temp.push_back(node);
                }
            }
            for (const auto& node : temp) {
                open.push(node);
            }
            open.push(ThetaNode(n, g[n] + heuristic(n, goal)));
        }
    } else {
        double tentative_g = g[s] + cost(s, n);
        if (tentative_g < g[n]) {
            g[n] = tentative_g;
            parent[n] = s;
            
            // Remove and reinsert to update priority
            std::vector<ThetaNode> temp;
            while (!open.empty()) {
                ThetaNode node = open.top();
                open.pop();
                if (node.vertex != n) {
                    temp.push_back(node);
                }
            }
            for (const auto& node : temp) {
                open.push(node);
            }
            open.push(ThetaNode(n, g[n] + heuristic(n, goal)));
        }
    }
}

std::vector<std::string> reconstruct_path(std::unordered_map<std::string, std::string>& parent, const std::string& goal) {
    std::vector<std::string> path;
    std::string s = goal;
    while (true) {
        path.insert(path.begin(), s);
        if (parent[s] == s) {
            break;
        }
        s = parent[s];
    }
    return path;
}

std::vector<std::string> theta_star(Graph& graph, const std::string& start, const std::string& goal) {
    std::priority_queue<ThetaNode, std::vector<ThetaNode>, std::greater<ThetaNode>> open;
    std::unordered_set<std::string> closed;
    std::unordered_map<std::string, double> g;
    std::unordered_map<std::string, std::string> parent;
    
    // Initialize all nodes from adjacency list
    const auto& adjList = graph.getAdjList();
    for (const auto& [vertex, neighbors] : adjList) {
        g[vertex] = std::numeric_limits<double>::infinity();
        parent[vertex] = "";
    }
    
    // Also add start and goal if they're not in adjList
    if (g.find(start) == g.end()) {
        g[start] = std::numeric_limits<double>::infinity();
        parent[start] = "";
    }
    if (g.find(goal) == g.end()) {
        g[goal] = std::numeric_limits<double>::infinity();
        parent[goal] = "";
    }
    
    g[start] = 0;
    parent[start] = start;
    open.push(ThetaNode(start, g[start] + heuristic(start, goal)));
    
    while (!open.empty()) {
        ThetaNode current = open.top();
        open.pop();
        std::string s = current.vertex;
        
        if (s == goal) {
            return reconstruct_path(parent, goal);
        }
        
        closed.insert(s);
        
        // Get neighbors from adjacency list
        std::vector<std::string> neighbors;
        auto it = adjList.find(s);
        if (it != adjList.end()) {
            neighbors = it->second;
        }
        
        for (const auto& n : neighbors) {
            if (closed.find(n) != closed.end()) {
                continue;
            }
            
            if (g.find(n) == g.end()) {
                g[n] = std::numeric_limits<double>::infinity();
                parent[n] = "";
            }
            
            update_vertex(s, n, goal, open, g, parent, graph);
        }
    }
    
    return {};
}

#endif // THETA_STAR_H