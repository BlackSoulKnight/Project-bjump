#include "Player.h"

// Konstruktor
Player::Player()
    : playerState(PlayerState::ON_GROUND),
    spacePressed(false),
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

// Eingaben behandeln
void Player::handleInput() {
    const Uint8* state = SDL_GetKeyboardState(NULL);

    // Links/Rechts-Bewegung
    if (state[SDL_SCANCODE_A]) {
        vx = dashing ? -DASH_SPEED : -MOVE_SPEED;
    }
    else if (state[SDL_SCANCODE_D]) {
        vx = dashing ? DASH_SPEED : MOVE_SPEED;
    }
    else {
        vx = 0;
    }

    // Springen
    if (state[SDL_SCANCODE_SPACE]) {
        if (!spacePressed) {
            spacePressed = true;
            if (playerState == PlayerState::ON_GROUND) {
                vy = JUMP_FORCE;
                playerState = PlayerState::JUMPING;
            }
            else if (playerState == PlayerState::JUMPING) {
                vy = JUMP_FORCE;
                playerState = PlayerState::DOUBLE_JUMPING;
            }
            else if (touchingWall) {
                vy = WALL_JUMP_FORCE_Y;
                vx = (position.x < 320) ? WALL_JUMP_FORCE_X : -WALL_JUMP_FORCE_X;
                playerState = PlayerState::WALL_JUMPING;
            }
        }
    }
    else {
        spacePressed = false;
    }

    // Dash
    if (state[SDL_SCANCODE_LSHIFT] && canDash && !dashing) {
        if (playerState == PlayerState::JUMPING || playerState == PlayerState::DOUBLE_JUMPING) {
            dashing = true;
            canDash = false;
            vy = 0;
            dashTime = DASH_DURATION;
            playerState = PlayerState::DASHING;
        }
    }
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
    // Bewegung nach links und rechts
    if (getInputState("Left")) {
        vx = -MOVE_SPEED;
    }
    else if (getInputState("Right")) {
        vx = MOVE_SPEED;
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
        else if (getInputState("Jump")) {
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
        else if (getInputState("Jump")) {
            // Doppelsprung
            vy = JUMP_FORCE;
            playerState = PlayerState::DOUBLE_JUMPING;
        }
        else if (getInputState("Dash") && canDash) {
            // Dash aktivieren
            vx = (vx > 0) ? DASH_SPEED : -DASH_SPEED;
            dashTime = DASH_DURATION;
            canDash = false;
            playerState = PlayerState::DASHING;
        }
        break;

    case PlayerState::DOUBLE_JUMPING:
        if (checkWallCollision()) {
            // An der Wand springen
            playerState = PlayerState::WALL_JUMPING;
        }
        else if (getInputState("Dash") && canDash) {
            // Dash aktivieren
            vx = (vx > 0) ? DASH_SPEED : -DASH_SPEED;
            dashTime = DASH_DURATION;
            canDash = false;
            playerState = PlayerState::DASHING;
        }
        break;

    case PlayerState::DASHING:
        dashTime -= deltaT;
        if (dashTime <= 0) {
            // Dash beendet: In den "FALLING"-Zustand wechseln
            playerState = PlayerState::FALLING;
        }
        break;

    case PlayerState::WALL_JUMPING:
        if (getInputState("Jump")) {
            // Wand-Sprung ausführen
            vy = WALL_JUMP_FORCE_Y;
            vx = (x < 320) ? WALL_JUMP_FORCE_X : -WALL_JUMP_FORCE_X;
            playerState = PlayerState::FALLING;
        }
        break;

    case PlayerState::FALLING:
        if (checkGroundCollision(320)) {
            // Landen: Zurück zum "ON_GROUND"-Zustand
            y = 320 - height;
            vy = 0;
            playerState = PlayerState::ON_GROUND;
        }
        else if (getInputState("Dash") && canDash) {
            // Dash aktivieren
            vx = (vx > 0) ? DASH_SPEED : -DASH_SPEED;
            dashTime = DASH_DURATION;
            canDash = false;
            playerState = PlayerState::DASHING;
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



