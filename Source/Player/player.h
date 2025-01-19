#ifndef Players_H
#define Players_H

#include <SDL.h>
#include <vector>
#include <tuple>
#include <iostream> // Für Konsolenausgabe
#include "../engine/Utility.h"
#include "../engine/GameObject.h" 

class Player : public GameObject {
private:
    // Position und Geschwindigkeit
    float x, y;        // **Position des Spielers**
    float vx, vy;      // Geschwindigkeit
    float width, height;

    // Spielerzustände
    enum class PlayerState { ON_GROUND, JUMPING, DOUBLE_JUMPING, DASHING, WALL_JUMPING, FALLING };
    PlayerState playerState;

    // Kollisionsflags
    bool touchingWall = false;       // Gibt an, ob der Spieler eine Wand berührt
    bool canWallJump = true;         // Gibt an, ob ein Walljump erlaubt ist
    float wallStickTime = 0.0f;      // Zeit, die der Spieler an der Wand bleibt
    const float MAX_WALL_STICK_TIME = 1000.0f;  // Maximalzeit in Millisekunden
    bool spacePressed;
    bool spaceJustPressed;
    bool canDash;
    bool dashing;
    float dashTime;
    bool canDoubleJump = true; // Erlaubt den Doppelsprung nur einmal pro Bodenberührung

    // Konstanten
    static constexpr float GRAVITY = 0.5f;
    static constexpr float JUMP_FORCE = -10.0f;
    static constexpr float FALL_SPEED = 10.0f;
    static constexpr float MOVE_SPEED = 5.0f;
    static constexpr float DASH_SPEED = 200.0f;
    static constexpr float DASH_DURATION = 0.5;
    static constexpr float WALL_JUMP_FORCE_Y = -8.0f;
    static constexpr float WALL_JUMP_FORCE_X = 5.0f;
    int dashDirection = 0;     // Speichert die Dash-Richtung (-1 = links, 1 = rechts)

    // Hilfsfunktionen
    bool checkGroundCollision(int screenHeight);
    bool checkWallCollision();

public:
    // Konstruktor
    Player();

    // Methoden
    
    void update(int deltaT);
    void moveAndCollide(Vector2 direction);
    void printState() const;

    // Zugriffsmethoden
    float getX() const { return x; }
    float getY() const { return y; }
    float getWidth() const { return width; }
    float getHeight() const { return height; }

    void setPosition(float newX, float newY) {
        x = newX;
        y = newY;
    }
};


#endif // Players_H