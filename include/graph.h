#ifndef GRAPH_H
#define GRAPH_H

#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

class Graph {
private:
    std::unordered_map<std::string, std::vector<std::string>> adjList;

public:
    // Добавить вершину
    void addVertex(const std::string& vertex) {
        if (adjList.find(vertex) == adjList.end()) {
            adjList[vertex] = std::vector<std::string>();
        }
    }

    // Добавить ребро между двумя вершинами
    void addEdge(const std::string& v1, const std::string& v2) {
        addVertex(v1);
        addVertex(v2);
        adjList[v1].push_back(v2);
        adjList[v2].push_back(v1);
    }

    // Вывести граф
    void print() {
        std::cout << "Граф (список смежности):" << std::endl;
        for (const auto& pair : adjList) {
            std::cout << pair.first << " -> ";
            for (const auto& neighbor : pair.second) {
                std::cout << neighbor << " ";
            }
            std::cout << std::endl;
        }
    }

};
#endif