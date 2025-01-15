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
    name = "Player";
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
   


    position.y += direction.y;
   
    
}

// Spielerzustand aktualisieren
void Player::update(int deltaT) {
    printState();

    SDL_Rect linksNebenPlayer;
    linksNebenPlayer.x = position.x - 1;
    linksNebenPlayer.y = position.y;
    linksNebenPlayer.w = 1;
    linksNebenPlayer.h = position.h;
    vector<GameObject*> objs = World::Instance->getCollisions(linksNebenPlayer);

    SDL_Rect rechtsNebenPlayer;
    rechtsNebenPlayer.x = position.x + position.w;
    rechtsNebenPlayer.y = position.y;
    rechtsNebenPlayer.w = 1;
    rechtsNebenPlayer.h = position.h;
    vector<GameObject*> objr = World::Instance->getCollisions(rechtsNebenPlayer);

    // Bewegung nach links und rechts (normale Bewegung)
    if (!dashing) { // Während des Dashs keine Richtungsänderung erlauben
        if (InputManager::checkInput("Left") && objs.size() == 0) {
            vx = MOVE_SPEED * -1;  // Linksbewegung
        }
        else if (InputManager::checkInput("Right")) {
            vx = MOVE_SPEED;  // Rechtsbewegung
        }
        else {
            vx = 0;  // Keine Bewegung
        }
    }

    // Maximale Fallgeschwindigkeit begrenzen
    if (vy > FALL_SPEED && !dashing) {
        vy = FALL_SPEED;
    }

    // Spieler-Zustände aktualisieren
    switch (playerState) {
    case PlayerState::ON_GROUND:
        canDoubleJump = true;  // Doppelsprung wieder erlauben
        canDash = true;        // Dash wieder erlauben
        if (!checkGroundCollision(320)) {
            playerState = PlayerState::FALLING;  // Vom Boden in "FALLING"
        }
        else if (InputManager::checkInput("Jump")) {
            vy = JUMP_FORCE;  // Vom Boden springen
            playerState = PlayerState::JUMPING;
        }
        break;

    case PlayerState::JUMPING:
        if (checkGroundCollision(320)) {
            position.y = 320 - height;
            vy = 0;
            playerState = PlayerState::ON_GROUND;  // Zurück auf den Boden
        }
        else if (InputManager::checkInput("Dash") && canDash) {
            dashing = true;
            dashTime = DASH_DURATION;  // Einheitliche Dauer
            dashDirection = InputManager::checkInput("Left") ? -1 : 1; // Richtung festlegen
            vx = DASH_SPEED * dashDirection;
            vy = 0;  // Vertikale Bewegung pausieren
            canDash = false;
            playerState = PlayerState::DASHING;
        }
        else if (vy > 0) {
            playerState = PlayerState::FALLING;  // Wechsel zu "FALLING", wenn nach unten
        }
        break;

    case PlayerState::DOUBLE_JUMPING:
        if (checkGroundCollision(320)) {
            position.y = 320 - height;
            vy = 0;
            playerState = PlayerState::ON_GROUND;  // Zurück auf den Boden
        }
        else if (InputManager::checkInput("Dash") && canDash) {
            dashing = true;
            dashTime = DASH_DURATION;  // Einheitliche Dauer
            dashDirection = InputManager::checkInput("Left") ? -1 : 1; // Richtung festlegen
            vx = DASH_SPEED * dashDirection;
            vy = 0;  // Vertikale Bewegung pausieren
            canDash = false;
            playerState = PlayerState::DASHING;
        }
        break;

    case PlayerState::FALLING:
        if (checkGroundCollision(320)) {
            position.y = 320 - height;
            vy = 0;
            playerState = PlayerState::ON_GROUND;  // Zurück auf den Boden
        }
        else if (InputManager::checkInput("Jump") && canDoubleJump) {
            vy = JUMP_FORCE;
            playerState = PlayerState::DOUBLE_JUMPING;
            canDoubleJump = false;  // Kein weiterer Doppelsprung möglich
        }
        break;

    case PlayerState::DASHING:
        dashTime -= deltaT;  // Dash-Zeit abziehen
        if (dashTime <= 0) {
            // Dash beendet
            dashing = false;
            dashDirection = 0;  // Richtung zurücksetzen
            playerState = PlayerState::FALLING;  // Setze Zustand auf FALLING
        }
        break;

    default:
        std::cerr << "Unbekannter Spielerzustand!\n";
        break;
    }

    // Schwerkraft anwenden, wenn nicht im Dash
    if (!dashing) {
        vy += GRAVITY;
    }

    // Bewegung und Kollision berechnen
    moveAndCollide(Vector2(vx, vy));
}













