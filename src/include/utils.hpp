#pragma once

// Mathhhhh translating angles to vectors stackoverflow help
#define TOP sf::Vector2f(0, -1);
#define TOP_RIGHT sf::Vector2f(1, -1);
#define TOP_LEFT sf::Vector2f(-1, -1);
#define BOTTOM sf::Vector2f(0, 1);
#define BOTTOM_RIGHT sf::Vector2f(1, 1);
#define BOTTOM_LEFT sf::Vector2f(-1, 1);
#define RIGHT sf::Vector2f(1, 0);
#define LEFT sf::Vector2f(-1, 0);

// For converting radians to degrees
#define RADIANS_TO_DEGREES 57.295779513082320876798154814105f;

sf::Vector2f angleToVector(const float& degree);

float magnitude(const sf::Vector2f& a);

float vectorToAngle(const sf::Vector2f& vec);

float random_float(float lower, float higher);
