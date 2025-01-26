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
    float wallJumpTimerMax = 0.2;
    float KNOCKBACK_FORCE = 700;
    // Konstanten
    float GRAVITY = 167.091;
    float JUMP_FORCE = -2770.628;
    float FALL_SPEED = 600;
    float MOVE_SPEED = 700;
    float MOVE_SLOWER = MOVE_SPEED / 2; 
    float DASH_SPEED = 3498.901;
    float DASH_DURATION = 0.25;
    float WALL_JUMP_FORCE_Y = -769.138*2;
    float WALL_JUMP_FORCE_X = 1366.19;
    float BOUNCE_BREAK_POINT = 575;

    float BOUNCE_FACTOR = 0.3; // Always Below 1
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
    bool checkCeilCollision();
    void setPosition(float newX, float newY) {
        x = newX;
        y = newY;
    }

    void LoadStats1();
};


#endif // Players_H