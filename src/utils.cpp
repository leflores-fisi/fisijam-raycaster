#include <SFML/Graphics.hpp>
#include <cmath>
#include <random>
#include <iostream>
#include "include/utils.hpp"

//Needed for setting up Field of View
sf::Vector2f angleToVector(const float& degree) {
    if (degree == 0 || degree == 360) return TOP;
    if (degree == 180) return BOTTOM;
    if (degree == 90 ) return RIGHT;
    if (degree == 270) return LEFT;
    if (degree == 45 ) return TOP_RIGHT;
    if (degree == 135) return BOTTOM_RIGHT;
    if (degree == 225) return BOTTOM_LEFT;
    if (degree == 315) return TOP_LEFT;

    const float rad = (-degree + 90.0f) / RADIANS_TO_DEGREES;
    return sf::Vector2f(cosf(rad), -sinf(rad));
}
// Easier math for setting up teleport and making hitbox for it
float magnitude(const sf::Vector2f& a) {
    return sqrt(a.x * a.x + a.y * a.y);
}
// More vector math, constants from trygonometry atan2
float vectorToAngle(const sf::Vector2f& vec) {
    if (vec.x == 0.0f && vec.y == 0.0f)
        return 0.0f;
    /** http://math.stackexchange.com/questions/1098487/atan2-faster-approximation
    * atan2(y,x)
    * a := min (|x|, |y|) / max (|x|, |y|)
    * s := a * a
    * r := ((-0.0464964749 * s + 0.15931422) * s - 0.327622764) * s * a + a
    * if |y| > |x| then r := 1.57079637 - r
    * if x < 0 then r := 3.14159274 - r
    * if y < 0 then r := -r
    **/
    const float absx = std::abs(vec.x), absy = std::abs(vec.y);
    const float a = absx > absy ? absy / absx : absx / absy;
    const float s = a * a;
    float r = ((-0.0464964749 * s + 0.15931422) * s - 0.327622764) * s * a + a;

    if (absy > absx)
        r = 1.57079637 - r;
    if (vec.x < 0)
        r = 3.14159274 - r;
    if (vec.y < 0)
        r = -r;

    float ang = r * RADIANS_TO_DEGREES + 90.0f;
    if      (ang < 0.0f  ) ang += 360.0f;
    else if (ang > 360.0f) ang -= 360.0f;
    return ang + 90;
}

std::random_device randomSeed;	// Getting seed from random number engine
std::mt19937 gen(randomSeed()); // Mersenne Twister pseudo-random generator of 32-bit numbers seeded with randomSeed()
float random_float(float lower, float higher) {
    std::uniform_real_distribution<float> dist(lower, higher);
    float random = dist(gen);
    std::cout << "random: " << random << std::endl;
    return random;
}
