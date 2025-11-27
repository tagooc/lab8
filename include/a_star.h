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
#include <functional>
#include <limits>

struct AStarNode {
    std::string vertex;
    double f;
    
    AStarNode(const std::string& v, double f_val) : vertex(v), f(f_val) {}
    
    bool operator>(const AStarNode& other) const {
        return f > other.f;
    }
};

double a_star_heuristic(const std::string& a, const std::string& b) {
    auto [x1, y1] = parseCoordinates(a);
    auto [x2, y2] = parseCoordinates(b);
    return std::abs(x1 - x2) + std::abs(y1 - y2);
}

std::vector<std::string> a_star_reconstruct_path(std::unordered_map<std::string, std::string>& parent, const std::string& goal) {
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

std::vector<std::string> a_star(Graph& graph, const std::string& start, const std::string& goal) {
    std::priority_queue<AStarNode, std::vector<AStarNode>, std::greater<AStarNode>> open;
    std::unordered_set<std::string> closed;
    std::unordered_map<std::string, double> g;
    std::unordered_map<std::string, std::string> parent;
    
    const auto& adjList = graph.getAdjList();
    for (const auto& [vertex, neighbors] : adjList) {
        g[vertex] = std::numeric_limits<double>::infinity();
        parent[vertex] = "";
    }
    
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
    open.push(AStarNode(start, g[start] + a_star_heuristic(start, goal)));
    
    while (!open.empty()) {
        AStarNode current = open.top();
        open.pop();
        std::string s = current.vertex;
        
        if (s == goal) {
            return a_star_reconstruct_path(parent, goal);
        }
        
        closed.insert(s);
        
        std::vector<std::string> neighbors;
        auto it = adjList.find(s);
        if (it != adjList.end()) {
            neighbors = it->second;
        }
        
        for (const auto& n : neighbors) {
            if (closed.find(n) != closed.end()) {
                continue;
            }
            
            double tentative_g = g[s] + 1;
            
            if (g.find(n) == g.end()) {
                g[n] = std::numeric_limits<double>::infinity();
                parent[n] = "";
            }
            
            if (tentative_g < g[n]) {
                g[n] = tentative_g;
                parent[n] = s;
                double h = a_star_heuristic(n, goal);
                open.push(AStarNode(n, g[n] + h));
            }
        }
    }
    
    return {};
}

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