#pragma once
#include <string>

#define MAP_WIDTH 20  // also width of the txt file
#define MAP_HEIGHT 29 // also height of the txt file

// FLOORS
#define FLOOR_NORMAL '0'
#define FLOOR_BACKROOMS '1'
#define FLOOR_FINAL_DOOR '2'
#define FLOOR_SCREAMER_MARIAELENA '5'
#define FLOOR_SCREAMER_HUAYNA '6'

bool is_empty_wall(char c);
bool load_map(char map[MAP_HEIGHT][MAP_WIDTH], std::string filename);
