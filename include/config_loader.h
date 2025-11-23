#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Config {
public:
    static Config& get() {
        static Config instance;
        return instance;
    }
    

    std::string file_breaks() { return breaks_path; }
    std::string file_graph() { return  graph_path; }
    std::string file_path() { return path_file; }
    std::string sheet_name() { return sheet; }
    std::string file_path_theta() { return path_file_theta; }
    int sleep_time() { return sleep; }
    
private:
    std::string path_file_theta;
    std::string breaks_path;
    std::string graph_path; 
    std::string path_file;
    float sleep;
    std::string sheet;
    
    Config() {
        std::ifstream file("config.json");
        if (!file.is_open()) {
            std::cerr << "❌ Cannot open config.json" << std::endl;
            return;
        }
        
        try {
            json data = json::parse(file);
            breaks_path = data.value("file_breaks", "breaks.json");
            graph_path = data.value("file_graph", "graph.json");
            path_file = data.value("file_path", "path.txt");
            sleep = data.value("sleep_time", 1);
            path_file_theta = data.value("file_path_theta", "path2.txt");
            sheet = data.value("sheet_name", "maze");
            
        } catch (const json::exception& e) {
            std::cerr << "❌ JSON error: " << e.what() << std::endl;
        }
    }
};

#endif