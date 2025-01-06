#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>

class Player {
private:
    float x, y;        // Position
    float vx, vy;      // Geschwindigkeit
    float width, height;
    bool onGround;     // Ob der Spieler auf dem Boden ist
    bool canDoubleJump; // Ob der Doppelsprung verfügbar ist

    static constexpr float GRAVITY = 0.5f;      // Schwerkraft
    static constexpr float JUMP_FORCE = -10.0f; // Sprungkraft
    static constexpr float FALL_SPEED = 10.0f;  // Maximale Fallgeschwindigkeit
    static constexpr float MOVE_SPEED = 5.0f;   // Bewegungsgeschwindigkeit

public:
    // Konstruktor
    Player(float startX, float startY, float width, float height);

    // Spieler-Eigenschaften
    float getX() const;
    float getY() const;
    float getWidth() const;
    float getHeight() const;

    // Steuerung
    void handleInput();
    void update();
    void render(SDL_Renderer* renderer) const;

    // Kollision mit dem Boden
    void checkGroundCollision(int screenHeight);
};

#endif // PLAYER_H
