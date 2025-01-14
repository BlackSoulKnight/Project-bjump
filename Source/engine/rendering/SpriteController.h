#pragma once
#include <SDL_rect.h>
#include <SDL_image.h>
#include <vector>
#include <string>

/**
 *
 * CPU based SpriteController
 *
 */
class SpriteController
{
private:
    std::vector<SDL_Rect> _sprite;

    int _logicPixelsX;
    int _logicPixelsY;
    int _x;
    int _y;

    int _spritesPerRow;
    int _spritesPerColumn;

    SDL_Surface* _spriteSurface;
    SDL_Texture* _Texture;

    void setSprite(std::vector<SDL_Rect> sprite);

public:
    
    /**
     * generateController - Generates a SpriteController with initialized values to use for a SpriteController initialization.
     *
     * Example:
     *      SpriteController Controller = SpriteController::generateController("asset/image", logicalPixelX, logicalPixelY);
     *
     * This is the preffered way to create a SpriteController.
     */
    static SpriteController &generateController(const char *fileName, int logicPixelHorizontal, int logicPixelVertical, SDL_Renderer* renderer);

    /**
     * Constructor
     * SpriteController - Initializes the SpriteController for usage.
     *                    NOTE: It is recommended to use generateController()
     *                          when using this Library as it is the intended
     *                          way to initialize the Class properly.
     *                          See generateController for usage.
     *                    CAUTION: You are on your own on using those.
     *
     */
    SpriteController();
    SpriteController(int logicPixelsX, int logicPixelsY, int spritesPerRow, int spritesPerColumn, SDL_Surface *spriteSurface,SDL_Texture *texture);
    SpriteController(std::vector<SDL_Rect> sprite, int logicPixelsX, int logicPixelsY, int spritesPerRow, int spritesPerColumn);

    ~SpriteController();
    /**
     * x -  Sets the X coordinate of the SpriteController to the same Value as the Parameter.
     *      It then returns itself to implement Method Chaining.
     *
     * Examples:
     *      yourSpriteController.x(1);          // Sets the x coordinate to 0.
     *
     * Examples with Method Chaining:
     *      yourSpriteController.x(0).y(1);     // Sets the x coordinate to 0 and the y coordinate to 1.
     */
    SpriteController &x(int x);

    /**
     * y -  Same as X but for the Y coordinate. See x() for more Information.
     *      For Examples of Mehtod Chaining: See x().
     * Examples:
     *      yourSpriteController.y(1);
     */
    SpriteController &y(int y);

    /**
     * row - No difference between y() or row(), row() calls y() internally.
     *       Was implemented for preference's sake. For Examples of
     *       Mehtod Chaining: See x().
     *
     * Examples:
     *      yourSpriteController.row(1);
     */
    SpriteController &row(int row);

    /**
     * col - See row() for implementation.
     *       For Examples of Mehtod Chaining: See x().
     *
     * Examples:
     *      yourSpriteController.col(1);        // Sets x coordinate to 1.
     */
    SpriteController &col(int col);

    /**
     * getSpriteSize - Returns the total ammount of sprites. When you have
     *                 a SpriteSheet with 5 Columns and 3 Rows you will
     *                 recieve the multiplication of it.
     *
     * Examples:
     *      size_t size = yourSpriteController.getSpriteSize();
     */
    int getSpriteSize();

    /**
     * getSprite() - Returns the sprite based on the X and Y coordinate OR
     *               based on the Index provided by the parameter.
     *               NOTE: When using the index the x and y coordinates will
     *                     be ignored.
     *
     * Examples:
     *      SDL_Rect& sprite1 = yourSpriteController.getSprite();
     *      SDL_Rect& sprite2 = yourSpriteController.getSprite(8);
     */
    SDL_Rect *getSprite();
    SDL_Rect *getSprite(int index);

    /**
     * getSpritesPerX/Y() - Returns the value of how many sprites per row/column exists.
     *
     * Example:
     *          int spritesPerRow       = yourSpriteController.getSpritesPerX();
     *          int spritesPerColumn    = yourSpriteController.getSpritesPerY();
     *
     */
    int getSpritesPerX();
    int getSpritesPerY();

    /**
     * getSpriteSizeX/Y() - Returns the value of the size of one sprite in direction X/Y.
     *
     * Example:
     *          int horizontalSpriteSize    = yourSpriteController.getSpriteSizeX();
     *          int verticalSpriteSize      = yourSpriteController.getSpriteSizeY();
     *
     */
    int getSpriteSizeX();
    int getSpriteSizeY();

    /**
     * getSpriteSurface - Returns the spriteSurface.
     *
     * Example:
     *          SpriteSurface* yourSpriteSurface    = yourSpriteController.getSpriteSurface();
     *
     */
    SDL_Surface* getSpriteSurface();
    SDL_Texture* getTexture() {return _Texture;};
};
