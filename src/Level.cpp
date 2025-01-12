//
// Created by Jamie Huta on 12.01.25.
//

#include "Level.h"

#include <iostream>

Level::Level() {
    m_tilemap = loadTilemap("../res/TestMap.csv");
}

Level::~Level() {

}

std::vector<std::vector<int>> Level::loadTilemap(const std::string &filename) {
    std::vector<std::vector<int>> tilemap;
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Could not open file" + filename);
    }

    std::string line;
    int lineNum = 0;
    int rowNum = 0;
    while (std::getline(file, line)) {
        lineNum++;
        std::vector<int> row;
        std::stringstream ss(line);
        std::string value;
        while (std::getline(ss, value, ',')) {
            try {
                row.push_back(std::stoi(value));
            } catch (std::invalid_argument &e) {
                throw std::runtime_error("Error: Invalid value at line " + std::to_string(lineNum) + ": " + value);
            } catch (std::out_of_range &e) {
                throw std::runtime_error("Error: Value out of range at line: " + std::to_string(lineNum) + ": " + value);
            }

        }
        tilemap.push_back(row);
        rowNum++;
    }
    std::cout << "rows: "<< rowNum << " lines: " << lineNum << std::endl;
    std::cout << "tilemap loaded successfully" << std::endl;
    return tilemap;
}


