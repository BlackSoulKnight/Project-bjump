#ifndef Players_H
#define Players_H

#include <SDL.h>
#include <vector>
#include <tuple>
#include <iostream> // Für Konsolenausgabe
#include "../engine/Utility.h"
#include "../engine/GameObject.h" 

class Player : public GameObject {
public:
    // Position und Geschwindigkeit
    float x, y;        // **Position des Spielers**
    float vx, vy;      // Geschwindigkeit
    float width, height;

    // Spielerzustände
    enum class PlayerState { ON_GROUND, JUMPING, DOUBLE_JUMPING, DASHING, WALL_SLIDING,WALL_JUMPING, FALLING,KNOCKBACK };
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
    int dashDirection;
    bool touchingLeftWall = false;
    bool touchingRightWall = false;
    bool lastTouchedWallWasRight = false;
    bool gotHit = false;
    float fallDirection = 0;
    float wallJumpTimer = 0.20; // Timer für die halbe Sekunde
    float wallJumpTimerMax = 0.1;
    float KNOCKBACK_FORCE = 940;
    // Konstanten
    float GRAVITY = 221;
    float JUMP_FORCE = -3395;
    float FALL_SPEED = 600;
    float MOVE_SPEED = 900;
    float MOVE_SLOWER = 250; 
    float DASH_SPEED = 3200;
    float DASH_DURATION = 0.333;
    float WALL_JUMP_FORCE_Y = -3000;
    float WALL_JUMP_FORCE_X = 16.0 ;
    float BOUNCE_BREAK_POINT = 600;

    float BOUNCE_FACTOR = 0.6; // Always Below 1
    // Hilfsfunktionen
    bool checkGroundCollision(int screenHeight = 0);
    bool checkWallCollision();

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