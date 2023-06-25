#pragma once

constexpr float PI = 3.141592653589793116f;

constexpr unsigned short SCREEN_HEIGHT = 768;
constexpr unsigned short SCREEN_WIDTH = 1280;

constexpr unsigned char MAP_HEIGHT = 24; //клетки поля
constexpr unsigned char MAP_WIDTH = 40;

constexpr unsigned char CELL_SIZE = 32;

constexpr unsigned short ROTATION_SPEED = 2;
constexpr unsigned short MOVEMENT_SPEED = 2;

constexpr unsigned short RADIUS = 3;

constexpr unsigned short RENDER_DISTANCE = 20 * CELL_SIZE;

constexpr unsigned short FOV = 90; // поле зрения
constexpr unsigned short RAYS_AMOUNT = 180;

enum Cell
{
	Empty,
	Wall
};
