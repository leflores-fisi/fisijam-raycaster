#include "include/item.hpp"
#include "include/utils.hpp"
#include "include/camera.hpp"
#include "include/textures.hpp"

jam::Item::Item(std::string filename, sf::Vector2f pos) :
    position(pos),
    itemTexture(sf::Texture()),
    itemRenderTexture(),
    itemSprite(sf::Sprite()),
    itemLines(sf::VertexArray(sf::Lines)
) {
    // setup item texture
	this->itemTexture.loadFromFile(filename);
	this->itemTexture.setSmooth(true);
    // set texture to sprite
	this->itemSprite.setOrigin(
        this->itemTexture.getSize().x / 2.f,
        this->itemTexture.getSize().y / 2.f
    );
	this->itemSprite.setPosition(itemSprite.getOrigin());
    this->itemSprite.setTexture(this->itemTexture);

    // setup item render texture
	this->itemRenderTexture.create(
        this->itemTexture.getSize().x,
        this->itemTexture.getSize().y
    );
}

jam::Item::~Item() { }

void jam::Item::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(this->itemLines, &this->itemRenderTexture.getTexture());
}

void jam::Item::render() {
    // Doesn't needed yet
}

void jam::Item::update(
    const jam::Camera &camera
) {
    this->itemRenderTexture.clear(sf::Color::Transparent);
    this->itemLines.clear();

    sf::Vector2f spritePos = this->position - camera.position;

    // constant for correct matrix calculations
    float fixingVal = 1.0f / (camera.aerial.x * camera.direction.y - camera.direction.x * camera.aerial.y);

    float transformX = fixingVal * (camera.direction.y * spritePos.x - camera.direction.x * spritePos.y);
    float transformY = fixingVal * (-camera.aerial.y * spritePos.x + camera.aerial.x * spritePos.y);

    int spriteScreenX = int((SCREEN_WIDTH / 2) * (1 + transformX / transformY));

    // Get height of the teleport sprite on the screen
    int spriteHeight = std::abs(int(SCREEN_HEIGHT / (transformY))); // Using "transformY" instead of the real distance prevents fish-eye
    // Calculate lowest and highest pixel to fill in current stripe
    int drawStartY = -spriteHeight / 2 + SCREEN_HEIGHT / 2;
    if (drawStartY < 0)
        drawStartY = 0;
    int drawEndY = spriteHeight / 2 + SCREEN_HEIGHT / 2;
    if (drawEndY >= SCREEN_HEIGHT)
        drawEndY = SCREEN_HEIGHT - 1;

    // Get width of the teleport sprite
    int spriteWidth = std::abs(int(SCREEN_HEIGHT / (transformY)));
    int drawStartX = -spriteWidth / 2 + spriteScreenX;
    if (drawStartX < 0)
        drawStartX = 0;
    int drawEndX = spriteWidth / 2 + spriteScreenX;
    if (drawEndX >= SCREEN_WIDTH)
        drawEndX = SCREEN_WIDTH - 1;

    // Loop for scaling our sprite, so it changes when we move
    // https://lodev.org/cgtutor/raycasting3.html#Scale
    for (int stripe = drawStartX; stripe < drawEndX; stripe++) {
        sf::Vertex itemVertex;
        int textureLocationX = int(TEXTURES_WIDTH * (stripe - (-spriteWidth / 2 + spriteScreenX)) * this->itemTexture.getSize().x / spriteWidth) / TEXTURES_WIDTH;
        // The conditions in the if are:
        // 1) It's in front of camera plane so you don't see things behind you
        // 2) It's on the screen (left)
        // 3) It's on the screen (right)
        // 4) ZDistance, with perpendicular distance
        if (transformY > 0 && stripe > 0 && stripe < SCREEN_WIDTH && transformY < camera.ZDistance[stripe]) {
            int topD = drawStartY * TEXTURES_WIDTH - SCREEN_HEIGHT * TEXTURES_WIDTH/2 + spriteHeight * TEXTURES_WIDTH/2;
            int topTextureY = ((topD * this->itemTexture.getSize().y) / spriteHeight) / TEXTURES_WIDTH;

            int bottomD = drawEndY * TEXTURES_WIDTH - SCREEN_HEIGHT * TEXTURES_WIDTH/2 + spriteHeight * TEXTURES_WIDTH/2;
            int bottomTextureY = ((bottomD * this->itemTexture.getSize().y) / spriteHeight) / TEXTURES_WIDTH;

            // Top
            itemVertex.texCoords = sf::Vector2f(textureLocationX, topTextureY);
            itemVertex.position = sf::Vector2f(stripe, camera.walkSwing + drawStartY);
            this->itemLines.append(itemVertex);

            // Bottom
            itemVertex.texCoords = sf::Vector2f(textureLocationX, bottomTextureY);
            itemVertex.position = sf::Vector2f(stripe, camera.walkSwing + drawEndY);
            this->itemLines.append(itemVertex);
            setBrightness(this->itemLines[this->itemLines.getVertexCount() - 1], transformY, CAMERA_DEFAULT_VISIBILITY);
        }
    }
    this->itemRenderTexture.draw(this->itemSprite);
    this->itemRenderTexture.display();
}

bool jam::Item::isColliding(sf::Vector2f playerPos) {
    sf::Vector2f distance = this->position - playerPos;
    return magnitude(distance) < 0.5f;
}
