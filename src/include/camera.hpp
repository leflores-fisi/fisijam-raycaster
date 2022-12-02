#pragma once
#include <SFML/Graphics.hpp>
#include "map.hpp"

// Setting locals
#define SCREEN_WIDTH 1280 // FIX: defined in other files
#define SCREEN_HEIGHT 720

#define CAMERA_DEFAULT_VISIBILITY 10.0f
#define CAMERA_DEFAULT_FOV_ANGLE 103
#define CAMERA_DEFAULT_FOV (fovAngle / 100.0f)
#define CAMERA_DEFAULT_SENSITIVITY 0.00125
#define CAMERA_DEFAULT_MOVEMENT_SPEED 1.5f
#define CAMERA_DEFAULT_BRIGHTNESS 120.0f //TO DO changing BRIGHTNESS

namespace jam {

class Camera {
  public:
    float visibility = CAMERA_DEFAULT_VISIBILITY;
    float fovAngle = CAMERA_DEFAULT_FOV_ANGLE;
    float fov = (CAMERA_DEFAULT_FOV_ANGLE / 100.0f);
    float sensitivity = CAMERA_DEFAULT_SENSITIVITY;
    float movementSpeed = CAMERA_DEFAULT_MOVEMENT_SPEED;
    float BRIGHTNESS = CAMERA_DEFAULT_BRIGHTNESS; //TO DO changing BRIGHTNESS
    sf::Vector2f position; // X and Y start position
    sf::Vector2f direction;// Initial direction vector
    sf::Vector2f aerial;   // The 2d ray caster version of camera plane


    // Walk variables
    float walkSwing;
	float walkingTimer = 0.0f;
	sf::Vector2i previousMousePosition;
    float fullTimer = 0.0f;

    // Arrays for the ray casting
    sf::VertexArray ceiling;
	sf::VertexArray floor;
	sf::VertexArray wallStructures;
    sf::Texture wallTexture;

    float ZDistance[SCREEN_WIDTH];

    // sf::RenderTexture renderTexture;

    Camera();
    Camera(sf::Vector2f position, sf::Vector2f direction);
    
    void moveCamera(const float mouseMove, const float constant = 1.f);
    void displaceCamera(const sf::Vector2f& vec, char map[MAP_HEIGHT][MAP_WIDTH]);
    void update(const float deltaTime, bool isMoving, char map[MAP_HEIGHT][MAP_WIDTH]);
    void renderIntoTexture(sf::RenderTexture &renderTexture);

};
    // Make the game darker
    void setColor(sf::Vertex& v, unsigned char value, unsigned char alpha = -1);
    void setBrightness(sf::Vertex& v, const float distance, const float maxDist, float myBrightness = CAMERA_DEFAULT_BRIGHTNESS);
}
