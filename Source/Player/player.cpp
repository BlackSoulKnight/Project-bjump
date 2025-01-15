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
    SDL_Rect linksNebenPlayer;
    linksNebenPlayer.x = position.x+3;
    linksNebenPlayer.y = position.y+position.h;
    linksNebenPlayer.w = 10;
    linksNebenPlayer.h = max((float)1,vy);
    vector<GameObject*> objs = World::Instance->getCollisions(linksNebenPlayer);
    if (objs.size() > 0) {
        return true;
    }
    return false;
}

// Kollisionsprüfung (Wände)
bool Player::checkWallCollision() {
    if (position.x <= 0 || position.x + width >= 640) {
        touchingWall = false;
        return false;
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
    linksNebenPlayer.w = 1;
    linksNebenPlayer.x = position.x - linksNebenPlayer.w;
    linksNebenPlayer.y = position.y;
    linksNebenPlayer.h = 16;
    vector<GameObject*> objs = World::Instance->getCollisions(linksNebenPlayer);

    linksNebenPlayer.x = position.x +position.w;
    linksNebenPlayer.y = position.y;
    linksNebenPlayer.w = 1;
    linksNebenPlayer.h = 16;
    vector<GameObject*> objs2 = World::Instance->getCollisions(linksNebenPlayer);
    // Bewegung nach links und rechts (normale Bewegung)
    if (InputManager::checkInput("Left") && objs.size() == 0) {
        vx = (dashing ? MOVE_SPEED * 1.5f : MOVE_SPEED) * -1;  // Linksbewegung
    }
    else if (InputManager::checkInput("Right") && objs2.size()==0) {
        vx = (dashing ? MOVE_SPEED * 1.5f : MOVE_SPEED);  // Rechtsbewegung
    }
    else if (!dashing) {
        vx = 0;  // Keine Bewegung, wenn nicht gedasht wird
    }



    // Schwerkraft anwenden, wenn nicht im Dash
    if (playerState != PlayerState::DASHING) {
        vy += GRAVITY;
    }

    // Maximale Fallgeschwindigkeit begrenzen
    if (vy > FALL_SPEED) {
        vy = FALL_SPEED;
    }

    // Spieler-Zustände aktualisieren
    switch (playerState) {
    case PlayerState::ON_GROUND:
        if (!checkGroundCollision(0)) {
            playerState = PlayerState::FALLING;  // Vom Boden in "FALLING"
        }
        else {
            vy = 0;
        }
        if (InputManager::checkInput("Jump")) {
            vy = JUMP_FORCE;  // Vom Boden springen
            playerState = PlayerState::JUMPING;
        }
        canDash = true;  // Dash wieder verfügbar
        break;

    case PlayerState::JUMPING:
        if (checkGroundCollision(320)) {
            y = 320 - height;
            vy = 0;
            playerState = PlayerState::ON_GROUND;  // Zurück auf den Boden
        }
        else if (InputManager::checkInput("Dash") && canDash) {
            // Prüfen, ob Bewegungsinput vorhanden ist
            if (InputManager::checkInput("Left") || InputManager::checkInput("Right")) {
                // Dash aktivieren
                dashing = true;
                dashTime = 60;  
                playerState = PlayerState::DASHING;

                // Richtung des Dashs basierend auf Input
                if (InputManager::checkInput("Left")) {
                    vx = -DASH_SPEED;
                }
                else if (InputManager::checkInput("Right")) {
                    vx = DASH_SPEED;
                }
                vy = 0;  // Keine vertikale Bewegung beim Dash
                canDash = false;  // Dash deaktivieren
            }
        }
        if (vy > 0) {
            playerState = PlayerState::FALLING;  // In "FALLING", wenn nach unten
        }
        break;

    case PlayerState::DOUBLE_JUMPING:
        if (checkWallCollision()) {
            playerState = PlayerState::WALL_JUMPING;  // Wand-Sprung
        }
        else if (InputManager::checkInput("Dash") && canDash) {
            // Prüfen, ob Bewegungsinput vorhanden ist
            if (InputManager::checkInput("Left") || InputManager::checkInput("Right")) {
                // Dash aktivieren
                dashing = true;
                dashTime = 30;  
                playerState = PlayerState::DASHING;

                // Richtung des Dashs basierend auf Input
                if (InputManager::checkInput("Left")) {
                    vx = -DASH_SPEED;
                }
                else if (InputManager::checkInput("Right")) {
                    vx = DASH_SPEED;
                }
                vy = 0;  // Keine vertikale Bewegung beim Dash
                canDash = false;  // Dash deaktivieren
            }
        }
        if (vy > 0) {
            playerState = PlayerState::FALLING;  // In "FALLING", wenn nach unten
        }

        break;

    case PlayerState::DASHING:
        dashTime -= deltaT;  // Dash-Zeit abziehen

       

        if (dashTime <= 0) {
            // Dash beendet
            playerState = PlayerState::FALLING;  // Setze Zustand auf FALLING
            dashing = false;
        }
        break;

    case PlayerState::WALL_JUMPING:
        if (InputManager::checkInput("Jump")) {
            vy = WALL_JUMP_FORCE_Y;  // Wand-Sprung ausführen
            vx = (x < 320) ? WALL_JUMP_FORCE_X : -WALL_JUMP_FORCE_X;
            playerState = PlayerState::FALLING;  // Nach Wand-Sprung in FALLING
        }
        if (vy > 0) {
            playerState = PlayerState::FALLING;  // In "FALLING", wenn nach unten
        }
        break;

    case PlayerState::FALLING:
        if (InputManager::checkInput("Jump")) {
            vy = JUMP_FORCE;
            playerState = PlayerState::DOUBLE_JUMPING;  // Doppelsprung
        }
        if (checkGroundCollision(320)) {
            y = 320 - height;
            vy = 0;
            playerState = PlayerState::ON_GROUND;  // Landen
        }
        break;

    default:
        std::cerr << "Unbekannter Spielerzustand!\n";
        break;
    }

    // Bewegung und Kollision berechnen
    moveAndCollide(Vector2(vx, vy));
}














