#include "player.h"

Player::Player(float startX, float startY, float w, float h)
    : x(startX), y(startY), vx(0), vy(0), width(w), height(h),
    jumpState(ON_GROUND), spacePressed(false), canDash(true), dashing(false), dashTime(0) {}

float Player::getX() const { return x; }
float Player::getY() const { return y; }
float Player::getWidth() const { return width; }
float Player::getHeight() const { return height; }

void Player::handleInput() {
    const Uint8* state = SDL_GetKeyboardState(NULL);

    // Bewegungslogik (links und rechts)
    if (state[SDL_SCANCODE_A]) {
        vx = dashing ? -DASH_SPEED : -MOVE_SPEED; // Doppelte Geschwindigkeit beim Dash
    }
    else if (state[SDL_SCANCODE_D]) {
        vx = dashing ? DASH_SPEED : MOVE_SPEED;  // Doppelte Geschwindigkeit beim Dash
    }
    else {
        vx = 0;
    }

    // Leertaste gedrückt für Sprünge
    if (state[SDL_SCANCODE_SPACE]) {
        if (!spacePressed) { // Nur reagieren, wenn die Taste neu gedrückt wurde
            spacePressed = true;
            if (jumpState == ON_GROUND) {
                vy = JUMP_FORCE;       // Erster Sprung
                jumpState = JUMPING;  // In den "JUMPING"-Zustand wechseln
            }
            else if (jumpState == JUMPING) {
                vy = JUMP_FORCE;            // Doppelsprung
                jumpState = DOUBLE_JUMPING; // In den "DOUBLE_JUMPING"-Zustand wechseln
            }
        }
    }
    else {
        spacePressed = false; // Zurücksetzen, wenn Leertaste losgelassen wurde
    }

    // Dash-Logik
    if (state[SDL_SCANCODE_LSHIFT] && canDash && !dashing) {
        if (jumpState == JUMPING || jumpState == DOUBLE_JUMPING) { // Nur in der Luft dashen
            dashing = true;
            canDash = false;   // Dash kann nur einmal pro Luftphase verwendet werden
            vy = 0;            // Fallgeschwindigkeit stoppen
            dashTime = DASH_DURATION; // Dash-Timer setzen
            jumpState = DASHING;      // Zustand auf "DASHING" setzen
        }
    }
}

void Player::update() {
    if (dashing) {
        dashTime--; // Dash-Timer herunterzählen
        if (dashTime <= 0) {
            dashing = false; // Dash beenden
            // Zurück in den vorherigen Zustand wechseln
            jumpState = (vy < 0) ? JUMPING : DOUBLE_JUMPING;
        }
    }
    else {
        // Schwerkraft anwenden, wenn nicht im Dash
        vy += GRAVITY;
    }

    // Maximale Fallgeschwindigkeit begrenzen
    if (vy > FALL_SPEED) {
        vy = FALL_SPEED;
    }

    // Position aktualisieren
    x += vx;
    y += vy;

    // Kollision mit dem Boden überprüfen
    checkGroundCollision(320);

    // Begrenzung der x-Position (Verhindern, dass der Spieler aus dem Fenster läuft)
    if (x < 0) {
        x = 0;
    }
    if (x + width > 640) { // Annahme: Fensterbreite 640
        x = 640 - width;
    }
}

void Player::checkGroundCollision(int screenHeight) {
    // Kollision mit dem Boden
    if (y + height >= screenHeight) {
        y = screenHeight - height; // Spieler auf den Boden setzen
        vy = 0;                    // Geschwindigkeit in Y-Richtung zurücksetzen
        jumpState = ON_GROUND;     // Zustand auf "ON_GROUND" setzen
        canDash = true;            // Dash zurücksetzen
    }
}

void Player::render(SDL_Renderer* renderer) const {
    SDL_Rect rect = { (int)x, (int)y, (int)width, (int)height };
    SDL_SetRenderDrawColor(renderer, dashing ? 0 : 255, dashing ? 255 : 0, 0, 255); // Grün bei Dash, Rot sonst
    SDL_RenderFillRect(renderer, &rect);
}

void Player::printState() const {
    switch (jumpState) {
    case ON_GROUND:
        std::cout << "State: ON_GROUND" << std::endl;
        break;
    case JUMPING:
        std::cout << "State: JUMPING" << std::endl;
        break;
    case DOUBLE_JUMPING:
        std::cout << "State: DOUBLE_JUMPING" << std::endl;
        break;
    case DASHING:
        std::cout << "State: DASHIN" << std::endl;
        break;
    }
}
