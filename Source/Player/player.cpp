#include "player.h"

Player::Player(float startX, float startY, float w, float h)
    : x(startX), y(startY), vx(0), vy(0), width(w), height(h), onGround(false), canDoubleJump(true) {}  // Doppelsprung direkt aktiv beim Start

float Player::getX() const { return x; }
float Player::getY() const { return y; }
float Player::getWidth() const { return width; }
float Player::getHeight() const { return height; }

void Player::handleInput() {
    const Uint8* state = SDL_GetKeyboardState(NULL);

    // Bewegungslogik (links und rechts)
    if (state[SDL_SCANCODE_A]) {
        vx = -MOVE_SPEED;
    }
    else if (state[SDL_SCANCODE_D]) {
        vx = MOVE_SPEED;
    }
    else {
        vx = 0;
    }

    // Fallgeschwindigkeit (beschleunigtes Fallen)
    if (state[SDL_SCANCODE_S]) {
        vy += GRAVITY * 2;  // Schneller fallen
    }

    // Sprunglogik mit Leertaste
    if (state[SDL_SCANCODE_SPACE]) {  // Wenn Leertaste gedrückt wird
        if (onGround) {  // Wenn der Spieler auf dem Boden ist
            vy = JUMP_FORCE;  // Erster Sprung
            onGround = false;  // Der Spieler ist jetzt in der Luft
            canDoubleJump = true;  // Doppelsprung aktivieren
        }
        else if (canDoubleJump) {  // Wenn der Spieler in der Luft ist und der Doppelsprung aktiviert ist
            vy = JUMP_FORCE;  // Doppelsprung
            canDoubleJump = false;  // Doppelsprung deaktivieren, damit er nur einmal ausgeführt wird
        }
    }
}

void Player::update() {
    // Schwerkraft anwenden, nur wenn der Spieler nicht auf dem Boden ist
    if (!onGround) {
        vy += GRAVITY;  // Schwerkraft nur anwenden, wenn der Spieler in der Luft ist
    }

    // Maximale Fallgeschwindigkeit begrenzen
    if (vy > FALL_SPEED) {
        vy = FALL_SPEED;
    }

    // Position aktualisieren
    x += vx;
    y += vy;

    // Kollision mit dem Boden überprüfen
    checkGroundCollision(320);  // 320 ist die Höhe des Fensters

    // Kollision mit dem Boden überprüfen und verhindern, dass der Spieler durch den Boden fällt
    if (y + height >= 320) {
        y = 320 - height;  // Spieler auf den Boden setzen
        vy = 0;  // Geschwindigkeit in Y-Richtung zurücksetzen
        onGround = true;  // Spieler ist auf dem Boden
        canDoubleJump = true;  // Doppelsprung zurücksetzen (wieder aktivieren)
    }
}

void Player::checkGroundCollision(int screenHeight) {
    // Kollision mit dem Boden
    if (y + height >= screenHeight) {
        y = screenHeight - height;  // Spieler auf den Boden setzen
        vy = 0;  // Geschwindigkeit in Y-Richtung zurücksetzen
        onGround = true;  // Spieler ist auf dem Boden
        canDoubleJump = true;  // Doppelsprung zurücksetzen
    }
    else {
        onGround = false;  // Spieler ist nicht auf dem Boden
    }

    // Begrenzung der x-Position (Verhindern, dass der Spieler aus dem Fenster läuft)
    if (x < 0) {
        x = 0;  // Spieler darf nicht über den linken Bildschirmrand hinaus
    }
    if (x + width > 640) {  // Annahme: Fensterbreite 640
        x = 640 - width;  // Spieler darf nicht über den rechten Bildschirmrand hinaus
    }
}

void Player::render(SDL_Renderer* renderer) const {
    SDL_Rect rect = { (int)x, (int)y, (int)width, (int)height };
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Rot
    SDL_RenderFillRect(renderer, &rect);
}