#include "Player.h"

float Player::getX() const { return position.x; }
float Player::getY() const { return position.y; }
float Player::getWidth() const { return position.w; }
float Player::getHeight() const { return position.h; }

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



void Player::checkGroundCollision(int screenHeight) {
    // Kollision mit dem Boden
    if (y + height >= screenHeight) {
        y = screenHeight - height; // Spieler auf den Boden setzen
        vy = 0;                    // Geschwindigkeit in Y-Richtung zurücksetzen
        jumpState = ON_GROUND;     // Zustand auf "ON_GROUND" setzen
        canDash = true;            // Dash zurücksetzen
    }
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
        std::cout << "State: DASHING" << std::endl;
        break;
    case WALL_JUMPING:
        std::cout << "State: WALLJUMPING" << std::endl;
        break;
    }
}
void Player::checkWallCollision() {
    // Hier prüfen wir, ob der Spieler eine Wand berührt. Das könnte eine Kollision mit der linken oder rechten Seite des Bildschirms oder mit einem Hindernis sein.
    // Wir setzen dies hier als Beispiel für eine einfache Wandkollision.
    if (x <= 0 || x + width >= 640) {  // Beispiel für Wandkollision mit Bildschirmgrenze
        touchingWall = true;
    }
    else {
        touchingWall = false;
    }
}
/*
void Player::update() {
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
*/

void Player::update(int deltaT) {
    switch (playerState) {
    case PlayerState::ON_GROUND:
        if (getInputState("Left"))
            vx = -MOVE_SPEED;
        else if (getInputState("Right"))
            vx = MOVE_SPEED;
        else 
            vx = 0;
        if (getInputState("Jump")) {
            playerState = JUMPING;
        }
        break;
    case JUMPING:
        
        break;

    }
    moveAndCollide(Vector2(vx,vy));
}

Player::Player():jumpState(ON_GROUND), spacePressed(false), canDash(true), dashing(false), dashTime(0)
{
    position.x = 50;
    position.w = 100;
    position.h = 100;
    name = "player";
}

void Player::moveAndCollide(Vector2 direction) {

    vector<tuple<GameObject*, SDL_Rect>> collisions = getCollision();
    //x
    position.x += direction.x;
    while ((collisions = getCollision()).size() > 0) {

        if (position.x + position.w / 2 > get<1>(collisions[0]).x)
        {
            position.x += get<1>(collisions[0]).w;
        }
        else {
            position.x -= get<1>(collisions[0]).w;
        }
    }

    //y
    position.y += direction.y;
    while ((collisions = getCollision()).size() > 0) {

        if (position.y + position.h / 2 > get<1>(collisions[0]).y)
        {
            position.y += get<1>(collisions[0]).h;
        }
        else {
            position.y -= get<1>(collisions[0]).h;
        }
    }
}