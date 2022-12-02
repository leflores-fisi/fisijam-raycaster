#include "include/AudioManager.hpp"
#include "include/camera.hpp"
#include "include/utils.hpp"
#include "include/map.hpp"
#include "include/textures.hpp"
#include <cmath>
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

void jam::setColor(sf::Vertex& v, unsigned char value, unsigned char alpha) {
    v.color.r = v.color.g = v.color.b = value;
    if (alpha != -1) {
        v.color.a = alpha;
    }
}
void jam::setBrightness(sf::Vertex& v, const float distance, const float maxDist, float myBrightness) {
    const float dark = std::max(std::min(myBrightness * distance / maxDist, myBrightness), 0.0f);
    const float brightnessToSet = myBrightness - dark;
    setColor(v, brightnessToSet);
}

jam::Camera::Camera() { }

jam::Camera::Camera(sf::Vector2f position, sf::Vector2f direction) : 
    position(position), direction(direction), aerial(),
    ceiling(sf::Quads, 4), floor(sf::Quads, 4), wallStructures(sf::Lines, SCREEN_WIDTH * 2) {

    wallTexture.setSmooth(true);
    bool loaded = wallTexture.loadFromFile("assets/walls.png");
    if (!loaded) {
        throw std::runtime_error("Failed to load wall texture");
    }
}

void jam::Camera::moveCamera(const float mouseMove, const float constant) {
    const float rotSpeed = mouseMove * constant; // Constant value is in radians/second

    sf::Vector2f &dir = this->direction;
    // Both camera direction and camera plane must be rotated
    const float olddirectionX = dir.x;
    dir.x = dir.x * std::cos(-rotSpeed) - dir.y * std::sin(-rotSpeed);
    dir.y = olddirectionX * std::sin(-rotSpeed) + dir.y * std::cos(-rotSpeed);

    const float oldaerialX = this->aerial.x;
    this->aerial.x = this->aerial.x * std::cos(-rotSpeed) - this->aerial.y * std::sin(-rotSpeed);
    this->aerial.y = oldaerialX * std::sin(-rotSpeed) + this->aerial.y * std::cos(-rotSpeed);
}

void jam::Camera::displaceCamera(const sf::Vector2f& vec, char map[MAP_HEIGHT][MAP_WIDTH]) {
    
    static const float coveredDistance = 12.0f;
    sf::Vector2i nowPosition(std::floor(position.x), std::floor(position.y));
    sf::Vector2i nextPosition(
        this->position.x + coveredDistance * vec.x,
        this->position.y + coveredDistance * vec.y
    );
    if (is_empty_wall(map[nextPosition.x][nowPosition.y])) {
        this->position.x += vec.x;
    }
    if (is_empty_wall(map[nowPosition.x][nextPosition.y])) {
        this->position.y += vec.y;
    }
}

void jam::Camera::update(const float deltaTime, bool isMoving, char map[MAP_HEIGHT][MAP_WIDTH]) {
    if (isMoving) {
        this->walkingTimer += deltaTime;
    }
    this->fullTimer += deltaTime;

    float calculateSin = std::sin(15.0f * walkingTimer + 2.0f * this->fullTimer);
    walkSwing = SCREEN_HEIGHT * (isMoving ? 0.016f : 0.008f) * calculateSin;

    // Walking sound effect
    // When bobbing reaches to bottom, play walk sound once
    static bool walkSoundPlayed = false;
    if (calculateSin < -0.98f) {
        if (!walkSoundPlayed) {
            // get random step sound from loaded files
            walkSoundPlayed = true;
            if (isMoving) {
                jam::AudioManager::playSound(
                    "audio/step_" + std::to_string(static_cast<int>(std::floor(random_float(1, 3 + 1)))),
                    random_float(0.75f, 1.25f)
                );
            }
        }
    }
    else {
        walkSoundPlayed = false;
    }

    // Breathing for standing still and movement
    const float nextBreathe = isMoving ? 2.f : 1.f;
    float currentBreathe = jam::AudioManager::getMusic("breathing_music")->getPitch();
    if (nextBreathe != currentBreathe) {
        float newBreathe = currentBreathe + (nextBreathe - currentBreathe) * deltaTime;
        if (newBreathe != currentBreathe) {
            jam::AudioManager::getMusic("breathing_music")->setPitch(newBreathe);
        }
    }

    // Raycasting algorithm, Loop every vertical line of the screen
    for (int x = 0, indexWallVectorX = 0; x < SCREEN_WIDTH; ++x, indexWallVectorX += 2) {

        // Calculate ray position and direction, translated from:
        // https://github.com/williamblair/williamblair.github.io/blob/376117c88ee7774b229ec33533f9510ee9878e31/templates/raycasting.html
        const float cameraCoorX = 2.0f * x / SCREEN_WIDTH - 1; // x coordinate in camera space

        // Using camera, direction vector and cmaera "plane" we get ray direction coords
        const sf::Vector2f rayDirection = this->direction + this->aerial * cameraCoorX;

        // caltulating distance Length of ray from one x or y-side to next x or y-side
        const float deltaDistanceX = std::sqrt(1 + (rayDirection.y * rayDirection.y) / (rayDirection.x * rayDirection.x));
        const float deltaDistanceY = std::sqrt(1 + (rayDirection.x * rayDirection.x) / (rayDirection.y * rayDirection.y));

        // length of ray from current position to next x or y side
        float sideDistanceX;
        float sideDistanceY;

        // left or right for x and y side
        int stepX;
        int stepY;

        // get our current position box
        sf::Vector2i mapPos = sf::Vector2i(std::floor(position.x), std::floor(position.y));

        // height is the original wall height divided by the distance to the wall (or length of the ray cast for each column).
        // calculate step and initial side dist
        // Rendering in horizontal direction
        if (rayDirection.x < 0) {
            stepX = -1;
            sideDistanceX = (this->position.x - mapPos.x) * deltaDistanceX;
        }
        else {
            stepX = 1;
            sideDistanceX = (mapPos.x + 1.0f - this->position.x) * deltaDistanceX;
        }

        // Rendering in vertical direction
        if (rayDirection.y < 0) {
            stepY = -1;
            sideDistanceY = (this->position.y - mapPos.y) * deltaDistanceY;
        }
        else {
            stepY = 1;
            sideDistanceY = (mapPos.y + 1.0f - this->position.y) * deltaDistanceY;
        }

        // perform DDA (digital differential analysis)
        int side;	 // North/South or East/West wall hit?
        bool hit = false; // true once we've hit a wall
        float perpWallDist;

        while (!hit) {
            // Jump to the next square
            if (sideDistanceX < sideDistanceY) {
                sideDistanceX += deltaDistanceX;
                mapPos.x += stepX;
                side = 0;
            }
            else {
                sideDistanceY += deltaDistanceY;
                mapPos.y += stepY;
                side = 1;
            }

            // check if we hit a wall
            if (!is_empty_wall(map[mapPos.x][mapPos.y])) {
                hit = true;
            }
        }

        // calculate distance projected on camera direction
        if (side == 0) {
            perpWallDist = std::fabs((mapPos.x - this->position.x + (1 - stepX) / 2) / rayDirection.x);
        }
        else {
            perpWallDist = std::fabs((mapPos.y - this->position.y + (1 - stepY) / 2) / rayDirection.y);
        }

        // Calculate height of line to draw on screen
        const float lineHeight = std::floor(std::abs(SCREEN_HEIGHT / perpWallDist));

        // Calculate lowest and highest pixel to fill in current stripe added mine walkSwing (changes height)
        const float drawStart = walkSwing - lineHeight/2 + SCREEN_HEIGHT/2;
        const float drawEnd   = walkSwing + lineHeight/2 + SCREEN_HEIGHT/2;

        // Calculate value of wallLocationX, we want to know from our position where did we hit the wall
        float wallLocationX;
        if (side == 0)
            wallLocationX = this->position.y + perpWallDist * rayDirection.y;
        else
            wallLocationX = this->position.x + perpWallDist * rayDirection.x;
        wallLocationX -= std::floor((wallLocationX));

        // X coordinate on the texture
        int textureLocationX = wallLocationX * TEXTURES_WIDTH;
        if (side == 0 && rayDirection.x > 0)
            textureLocationX = TEXTURES_WIDTH - textureLocationX - 1;
        if (side == 1 && rayDirection.y < 0)
            textureLocationX = TEXTURES_HEIGHT - textureLocationX - 1;

        

        // Save distance to ZDistance
        ZDistance[x] = perpWallDist;

        // Prepare wall structures, bottom left and top right corners
        sf::Vector2f base = getTexturePlacement(map[mapPos.x][mapPos.y]);
        wallStructures[indexWallVectorX].texCoords = base + sf::Vector2f(textureLocationX, 0);
        wallStructures[indexWallVectorX].position  = sf::Vector2f(x, drawStart);
        wallStructures[indexWallVectorX + 1].texCoords = base + sf::Vector2f(textureLocationX, TEXTURES_HEIGHT);
        wallStructures[indexWallVectorX + 1].position  = sf::Vector2f(x, drawEnd);

        // Brightness depending how close/far are we from the wall
        const float distance = magnitude(
            {
                mapPos.x - this->position.x + (side == 1 ? wallLocationX : 0),
                mapPos.y - this->position.y + (side == 0 ? wallLocationX : 0)
            }
        );
        setBrightness(wallStructures[indexWallVectorX + 0], distance, visibility);
        setBrightness(wallStructures[indexWallVectorX + 1], distance, visibility);
    }

    // Ceiling
    ceiling[0].position = { 0.0f, walkSwing };
    ceiling[1].position = { SCREEN_WIDTH, walkSwing };
    ceiling[2].position = { SCREEN_WIDTH, walkSwing + SCREEN_HEIGHT * 0.51f };
    ceiling[3].position = { 0.0f, walkSwing + SCREEN_HEIGHT * 0.51f };
    ceiling[2].color = ceiling[3].color = sf::Color { 20, 10, 7 };
    ceiling[0].color = ceiling[1].color = sf::Color { 3, 6, 15 };

    // Floor
    floor[0].position = { 0.0f, walkSwing + SCREEN_HEIGHT * 0.5f };
    floor[1].position = { SCREEN_WIDTH, walkSwing + SCREEN_HEIGHT * 0.5f };
    floor[2].position = { SCREEN_WIDTH, walkSwing + SCREEN_HEIGHT * 1.1f };
    floor[3].position = { 0.0f, walkSwing + SCREEN_HEIGHT * 1.1f };
    sf::Color c { 25, 24, 23 };
    floor[2].color = floor[3].color = c;
    floor[0].color = floor[1].color = c;
}

void jam::Camera::renderIntoTexture(sf::RenderTexture& renderTexture) {
    // Cleanup
    // window.clear(sf::Color::Black);
    renderTexture.clear(sf::Color::Black);

    // Draw everything ENCAPSULATE THIS
    // Draw ceiling
    renderTexture.draw(ceiling);

    // Draw floor
    renderTexture.draw(floor);
    
    // Draw walls
    renderTexture.draw(wallStructures, &wallTexture);
    
    // if (!hasTheKey) {
        // item_keyToFind.update();
        // renderTexture.draw(item_keyToFind);
    // }

    // if (opacityStartedDecreasing) {
        // Last render and window render
        renderTexture.display(); // if endedScene is true, starting drawing this
        // window.draw(renderTextureSprite);
    // }
}
