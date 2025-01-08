#include "Player.h"

Players::Players(float startX, float startY, float w, float h)
    : x(startX), y(startY), vx(0), vy(0), width(w), height(h),
    jumpState(ON_GROUND), spacePressed(false), canDash(true), dashing(false), dashTime(0) {}

float Players::getX() const { return x; }
float Players::getY() const { return y; }
float Players::getWidth() const { return width; }
float Players::getHeight() const { return height; }

void Players::handleInput() {
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
            else if (touchingWall && jumpState != WALL_JUMPING) { // Walljump aktivieren
                vy = WALL_JUMP_FORCE;  // Walljump Sprungkraft
                vx = (x < 320) ? WALL_JUMP_SPEED : -WALL_JUMP_SPEED;  // Walljump Geschwindigkeit
                jumpState = WALL_JUMPING;  // In den "WALL_JUMPING"-Zustand wechseln
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

void Players::update() {
    if (dashing) {
        dashTime--; // Dash-Timer herunterzählen
        if (dashTime <= 0) {
            dashing = false; // Dash beenden
            // Zurück in den vorherigen Zustand wechseln
           // jumpState = (vy < 0) ? JUMPING : DOUBLE_JUMPING;
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
    checkWallCollision();  // Wandkollision prüfen

    // Begrenzung der x-Position (Verhindern, dass der Spieler aus dem Fenster läuft)
    if (x < 0) {
        x = 0;
    }
    if (x + width > 640) { // Annahme: Fensterbreite 640
        x = 640 - width;
    }
}


void Players::checkGroundCollision(int screenHeight) {
    // Kollision mit dem Boden
    if (y + height >= screenHeight) {
        y = screenHeight - height; // Spieler auf den Boden setzen
        vy = 0;                    // Geschwindigkeit in Y-Richtung zurücksetzen
        jumpState = ON_GROUND;     // Zustand auf "ON_GROUND" setzen
        canDash = true;            // Dash zurücksetzen
    }
}

void Players::render(SDL_Renderer* renderer) const {
    SDL_Rect rect = { (int)x, (int)y, (int)width, (int)height };
    SDL_SetRenderDrawColor(renderer, dashing ? 0 : 255, dashing ? 255 : 0, 0, 255); // Grün bei Dash, Rot sonst
    SDL_RenderFillRect(renderer, &rect);
}

void Players::printState() const {
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
        std::cout << "State: DASHING" << std::endl;
        break;
    case WALL_JUMPING:
        std::cout << "State: WALLJUMPING" << std::endl;
        break;
    }
}
void Players::checkWallCollision() {
    // Hier prüfen wir, ob der Spieler eine Wand berührt. Das könnte eine Kollision mit der linken oder rechten Seite des Bildschirms oder mit einem Hindernis sein.
    // Wir setzen dies hier als Beispiel für eine einfache Wandkollision.
    if (x <= 0 || x + width >= 640) {  // Beispiel für Wandkollision mit Bildschirmgrenze
        touchingWall = true;
    }
    else {
        touchingWall = false;
    }
}

