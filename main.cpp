#include "include/graph.h"

int main() {
    // Создаем граф
    Graph g;

    // Добавляем вершины и ребра
    g.addEdge("(0,0)", "(0,1)");
    g.addEdge("(0,0)", "(1,0)");
    g.addEdge("(1,0)", "(2,0)");
    g.addEdge("(2,0)", "(3,0)");
    g.addEdge("(0,1)", "(1,1)");

    // Выводим граф
    g.print();

 

    return 0;
}