#include "Player.h"
#include "../engine/World.h"

// Konstruktor
Player::Player()
    : playerState(PlayerState::ON_GROUND),
    spacePressed(false),
    spaceJustPressed(false),  // Neue Variable für einmalige Tastendrücke
    canDash(true),
    dashing(false),
    dashTime(0),
    vx(0),
    vy(0),
    width(16),
    height(16) {
    position.x = 50;
    position.y = 200;
    position.w = width;
    position.h = height;
    name = "Quatsch";
    sprite.spiteSheetLocation = "../resources/Tilemap/monochrome_tilemap_transparent_packed.png";
    sprite.x = 0;
    sprite.y = 12;
    sprite.animationSize = 4;
}


// Spielerzustand ausgeben
void Player::printState() const {
    switch (playerState) {
    case PlayerState::ON_GROUND: std::cout << "State: ON_GROUND\n"; break;
    case PlayerState::JUMPING: std::cout << "State: JUMPING\n"; break;
    case PlayerState::DOUBLE_JUMPING: std::cout << "State: DOUBLE_JUMPING\n"; break;
    case PlayerState::DASHING: std::cout << "State: DASHING\n"; break;
    case PlayerState::WALL_JUMPING: std::cout << "State: WALL_JUMPING\n"; break;
    case PlayerState::FALLING: std::cout << "State: FALLING\n"; break;
    default: std::cout << "Unknown State\n"; break;
    }
}

// Kollisionsprüfung (Boden)
bool Player::checkGroundCollision(int screenHeight) {
    if (position.y + height >= screenHeight) {
        position.y = screenHeight - height;
        vy = 0;
        return true;
    }
    return false;
}

// Kollisionsprüfung (Wände)
bool Player::checkWallCollision() {
    if (position.x <= 0 || position.x + width >= 640) {
        touchingWall = true;
        return true;
    }
    touchingWall = false;
    return false;
}

// Bewegung und Kollision berechnen
void Player::moveAndCollide(Vector2 direction) {
    std::vector<std::tuple<GameObject*, SDL_Rect>> collisions;

    position.x += direction.x;
    while (!(collisions = getCollision()).empty()) {
        if (position.x + width / 2 > std::get<1>(collisions[0]).x) {
            position.x -= std::get<1>(collisions[0]).w;
        }
        else {
            position.x += std::get<1>(collisions[0]).w;
        }
    }

    position.y += direction.y;
    while (!(collisions = getCollision()).empty()) {
        if (position.y + height / 2 > std::get<1>(collisions[0]).y) {
            position.y -= std::get<1>(collisions[0]).h;
        }
        else {
            position.y += std::get<1>(collisions[0]).h;
        }
    }
}

// Spielerzustand aktualisieren
void Player::update(int deltaT) {
    printState();
    SDL_Rect linksNebenPlayer;
    linksNebenPlayer.x = position.x - 16;
    linksNebenPlayer.y = position.y;
    linksNebenPlayer.w = 16;
    linksNebenPlayer.h = 16;
    vector<GameObject*> objs = World::Instance->getCollisions(linksNebenPlayer);
    // Bewegung nach links und rechts
    if (InputManager::checkInput("Left") && objs.size() == 0) {
        vx = (dashing ? MOVE_SPEED * 1.5f : MOVE_SPEED) * -1;  // Wenn Dash, multiplizieren mit 1.5
    }
    else if (InputManager::checkInput("Right")) {
        vx = (dashing ? MOVE_SPEED * 1.5f : MOVE_SPEED);  // Wenn Dash, multiplizieren mit 1.5
    }
    else {
        vx = 0;
    }

    // Maximale Fallgeschwindigkeit begrenzen
    if (vy > FALL_SPEED) {
        vy = FALL_SPEED;
    }

    // Spieler-Zustände aktualisieren
    switch (playerState) {
    case PlayerState::ON_GROUND:
        if (!checkGroundCollision(320)) {
            // Vom Boden in den "FALLING"-Zustand wechseln
            playerState = PlayerState::FALLING;
        }
        else if (InputManager::checkInput("Jump")) {  // Wenn Leertaste gedrückt wird
            // Vom Boden springen
            vy = JUMP_FORCE;
            playerState = PlayerState::JUMPING;
        }
        // Dash ist nur verfügbar, wenn der Spieler den Boden berührt
        canDash = true;
        break;

    case PlayerState::JUMPING:
        if (checkGroundCollision(320)) {
            // Landen: Zurück zum "ON_GROUND"-Zustand
            y = 320 - height;
            vy = 0;
            playerState = PlayerState::ON_GROUND;
        }
        else if (InputManager::checkInput("Dash") && canDash && !dashing) {
            // Dash aktivieren
            dashing = true;
            dashTime = 500;  // Dash für eine halbe Sekunde (500ms)
            playerState = PlayerState::DASHING;
            vy = 0;  // Verhindert das Absinken oder Aufsteigen während des Dashs
            canDash = false;
        }
        if (vy > 0) {
            playerState = PlayerState::FALLING;

        }
        break;

    case PlayerState::DOUBLE_JUMPING:
        if (checkWallCollision()) {
            // An der Wand springen
            playerState = PlayerState::WALL_JUMPING;
        }
        else if (InputManager::checkInput("Dash") && canDash && !dashing) {
            // Dash aktivieren
            dashing = true;
            dashTime = 500;  // Dash für eine halbe Sekunde (500ms)
            playerState = PlayerState::DASHING;
            vy = 0;  // Verhindert das Absinken oder Aufsteigen während des Dashs
            canDash = false;
        }
        break;

    case PlayerState::DASHING:
        dashTime -= deltaT;  // Dash-Zeit abziehen
        if (dashTime <= 0) {
            // Dash beendet: Zurück in den normalen Zustand
            playerState = PlayerState::FALLING;  // Setze den Spieler zurück auf FALLING, wenn er in der Luft ist
            dashing = false;
        }
        break;

    case PlayerState::WALL_JUMPING:
        if (spaceJustPressed) {  // Wenn Leertaste gedrückt wird
            // Wand-Sprung ausführen
            vy = WALL_JUMP_FORCE_Y;
            vx = (x < 320) ? WALL_JUMP_FORCE_X : -WALL_JUMP_FORCE_X;
            playerState = PlayerState::FALLING;
        }
        break;

    case PlayerState::FALLING:
        if (InputManager::checkInput("Jump")) {
            vy = JUMP_FORCE;
            playerState = PlayerState::DOUBLE_JUMPING;
            
        }
        if (checkGroundCollision(320)) {
            // Landen: Zurück zum "ON_GROUND"-Zustand
            y = 320 - height;
            vy = 0;
            playerState = PlayerState::ON_GROUND;
        }
        break;

    default:
        std::cerr << "Unbekannter Spielerzustand!\n";
        break;
    }

    // Schwerkraft anwenden, wenn nicht im Dash
    if (playerState != PlayerState::DASHING) {
        vy += GRAVITY;
    }

    // Bewegung und Kollision berechnen
    moveAndCollide(Vector2(vx, vy));
}











