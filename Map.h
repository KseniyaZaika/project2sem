#pragma once
#include "Player.h"

#include <array>

#include "GlobalSizes.h"
#include <chrono>
#include <SFML/Graphics.hpp>

std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH> MapGeneration();//Player& i_player

bool wallCollision(float i_x, float y_x, const std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& i_map);