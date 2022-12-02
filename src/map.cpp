#include "include/map.hpp"
#include <string>
#include <fstream>

bool is_empty_wall(char w) {
    return w >= '0' && w <= '9';
}

bool load_map(char map[MAP_HEIGHT][MAP_WIDTH], std::string filename) {

    // Open map file (currently didn't valid the file format)
    std::ifstream map_file(filename);
    if (!map_file.is_open()) {
        throw std::runtime_error("No se pudo abrir el archivo de mapa " + filename);
    }

    // Read an save the pattern of the map (line by line)
    int lineNumber = 0;
    std::string line;

    while (std::getline(map_file, line)) {
        if (line.empty()) {
            continue;
        }
        for (size_t i = 0; i < line.size(); i++) {
            map[lineNumber][i] = line[i];
        }
        lineNumber++;
    }
    map_file.close();
    return true;
}
