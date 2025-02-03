#include "../SpriteController.h"

SpriteController::SpriteController() : _x(0), _y(0) {}

SpriteController::SpriteController(int logicPixelsX, int logicPixelsY, int spritesPerRow, int spritesPerColumn, SDL_Surface *spriteSurface,SDL_Texture *texture, SDL_Texture* bgTexture, SDL_Texture* bullet)
    : _logicPixelsX(logicPixelsX), _logicPixelsY(logicPixelsY), _x(0), _y(0), _spritesPerRow(spritesPerRow), _spritesPerColumn(spritesPerColumn), _spriteSurface(spriteSurface),_Texture(texture), _background(bgTexture), _bulletTexture(bullet) {}

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

SpriteController  &SpriteController::generateController(const char *fileName, int logicPixelHorizontal, int logicPixelVertical, SDL_Renderer* renderer)
{
    SDL_Surface *spriteSheet = IMG_Load(fileName);
    SDL_Texture *texture = IMG_LoadTexture(renderer, fileName);
    SDL_Texture* bgTexture = IMG_LoadTexture(renderer, fileName);
    SDL_Texture* bulletTexture = IMG_LoadTexture(renderer, fileName);
    SDL_SetTextureColorMod(bgTexture, 51, 51, 51);
    SDL_SetTextureColorMod(bulletTexture, 255, 127, 127);
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
        texture,
        bgTexture,
        bulletTexture
    );

    returnController->setSprite(resultSprite);

    return *returnController;
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