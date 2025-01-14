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
    bool touchingWall;
    bool spacePressed;
    bool spaceJustPressed;
    bool canDash;
    bool dashing;
    float dashTime;

    // Konstanten
    static constexpr float GRAVITY = 0.5f;
    static constexpr float JUMP_FORCE = -10.0f;
    static constexpr float FALL_SPEED = 10.0f;
    static constexpr float MOVE_SPEED = 5.0f;
    static constexpr float DASH_SPEED = 10.0f;
    static constexpr float DASH_DURATION = 20;
    static constexpr float WALL_JUMP_FORCE_Y = -8.0f;
    static constexpr float WALL_JUMP_FORCE_X = 5.0f;

    // Hilfsfunktionen
    bool checkGroundCollision(int screenHeight);
    bool checkWallCollision();

public:
    // Konstruktor
    Player();

    // Methoden
    void handleInput();
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




/*class Players {
private:
    float x, y;        // Position
    float vx, vy;      // Geschwindigkeit
    float width, height;

    enum JumpState { ON_GROUND, JUMPING, DOUBLE_JUMPING, DASHING, WALL_JUMPING, FALLING
    } jumpState; // Sprung- und Dash-Zustände
    bool spacePressed; // Ob die Leertaste bereits gedrückt wurde
    bool canDash;      // Ob der Dash verfügbar ist
    bool dashing;      // Ob der Spieler gerade dasht
    float dashTime;    // Timer für die Dash-Dauer
    bool touchingWall; // Ob der Spieler eine Wand berührt

    static constexpr float GRAVITY = 0.5f;      // Schwerkraft
    static constexpr float JUMP_FORCE = -10.0f; // Sprungkraft
    static constexpr float FALL_SPEED = 10.0f;  // Maximale Fallgeschwindigkeit
    static constexpr float MOVE_SPEED = 5.0f;   // Bewegungsgeschwindigkeit
    static constexpr float DASH_SPEED = 10.0f;  // Geschwindigkeit während des Dashs
    static constexpr float DASH_DURATION = 20;  // Dash-Dauer 
    static constexpr float WALL_JUMP_FORCE = -8.0f; // Walljump Sprungkraft
    static constexpr float WALL_JUMP_SPEED = 5.0f;  // Walljump seitliche Geschwindigkeit


public:
    // Konstruktor
    Players(float startX, float startY, float width, float height);

    // Spieler-Eigenschaften
    float getX() const;
    float getY() const;
    float getWidth() const;
    float getHeight() const;

    // Steuerung
    void handleInput();
    void update();
    void render(SDL_Renderer* renderer) const;

    // Zustand ausgeben
    void printState() const;

    // Kollision mit dem Boden
    void checkGroundCollision(int screenHeight);
    void checkWallCollision();  // Kollision mit Wänden überprüfen
};
*/
#endif // Players_H