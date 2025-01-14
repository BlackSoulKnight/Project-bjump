#include "../SpriteController.h"
#include "../../World.h"

SpriteController::SpriteController() : _x(0), _y(0) {}

SpriteController::SpriteController(int logicPixelsX, int logicPixelsY, int spritesPerRow, int spritesPerColumn, SDL_Surface *spriteSurface,SDL_Texture *texture)
    : _logicPixelsX(logicPixelsX), _logicPixelsY(logicPixelsY), _x(0), _y(0), _spritesPerRow(spritesPerRow), _spritesPerColumn(spritesPerColumn), _spriteSurface(spriteSurface),_Texture(texture) {}

SpriteController::SpriteController(std::vector<SDL_Rect> sprite, int logicPixelsX, int logicPixelsY, int spritesPerRow, int spritesPerColumn)
    : _sprite(sprite), _logicPixelsX(logicPixelsX), _logicPixelsY(_logicPixelsY), _x(0), _y(0), _spritesPerRow(spritesPerRow), _spritesPerColumn(spritesPerColumn) {}

SpriteController::~SpriteController() {}

SpriteController &SpriteController::x(int x)
{
    _x = x;
    return *this;
}

SpriteController &SpriteController::y(int y)
{
    _y = y;
    return *this;
}

SpriteController &SpriteController::row(int row) { return this->y(row); }

SpriteController &SpriteController::col(int col) { return this->x(col); }

int SpriteController::getSpriteSize() { return _sprite.size(); }

SDL_Rect *SpriteController::getSprite()
{
    return &_sprite.at((_y * _spritesPerRow) + _x);
}

SDL_Rect *SpriteController::getSprite(int index)
{
    return &_sprite.at(index);
}

void SpriteController::setSprite(std::vector<SDL_Rect> sprite)
{
    _sprite = sprite;
}

SpriteController  &SpriteController::generateController(const char *fileName, int logicPixelHorizontal, int logicPixelVertical)
{
    SDL_Surface *spriteSheet = IMG_Load(fileName);
    SDL_Texture *texture = IMG_LoadTexture(World::Instance->getRenderer(), fileName);
    int imageWidth = spriteSheet->w;
    int imageHeight = spriteSheet->h;

    //Potentially free sheet in future ?
    /*
    SDL_FreeSurface(spriteSheet);
    delete (spriteSheet);
    */

    int spritesPerRow = 0;
    int spritesPerColumn = 0;

    std::vector<SDL_Rect> resultSprite;

    for (int y = 0; y < imageHeight; y += logicPixelVertical)
    {
        for (int x = 0; x < imageWidth; x += logicPixelHorizontal)
        {
            SDL_Rect temp = {x,
                             y,
                             logicPixelHorizontal,
                             logicPixelVertical};
            resultSprite.push_back(temp);
        }
    }

    spritesPerRow = imageWidth / logicPixelHorizontal;
    spritesPerColumn = imageHeight / logicPixelVertical;

    SpriteController* returnController = new SpriteController(
        logicPixelHorizontal,
        logicPixelVertical,
        spritesPerRow,
        spritesPerColumn,
        spriteSheet,
        texture
    );

    returnController->setSprite(resultSprite);

    return *returnController;
}

SpriteController &SpriteController::generateController(const char *fileName, int logicPixel)
{
    return SpriteController::generateController(fileName, logicPixel, logicPixel);
}

int SpriteController::getSpritesPerX() {
    return _spritesPerRow;
}
int SpriteController::getSpritesPerY() {
    return _spritesPerColumn;
}

int SpriteController::getSpriteSizeX() {
    return _logicPixelsX;
}
int SpriteController::getSpriteSizeY() {
    return _logicPixelsY;
}

SDL_Surface* SpriteController::getSpriteSurface() {
    return _spriteSurface;
}