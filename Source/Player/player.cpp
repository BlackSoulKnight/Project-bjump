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
    position.w = width * SCALE;
    position.h = height * SCALE;
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
    Vector2 newPos = { position.x + direction.x,position.y + direction.y};
    SDL_Rect CollisionCheck;
    CollisionCheck.w = abs(direction.x);
    CollisionCheck.w = max((int)1, abs(direction.x));
    CollisionCheck.x = (direction.x<0)? position.x - CollisionCheck.w : position.x + position.w;
    CollisionCheck.y = position.y;
    CollisionCheck.h = 16;
    vector<GameObject*> objs = World::Instance->getCollisions(CollisionCheck);
    if (direction.x < 0) {
        for (int i = 0; i < objs.size(); i++) {
            if (newPos.x < objs[i]->position.x + objs[i]->position.w) {
                newPos.x = objs[i]->position.x + objs[i]->position.w;
            }
        }
    }
    else {
        for (int i = 0; i < objs.size(); i++) {
            if (newPos.x + position.w > objs[i]->position.x) {
                newPos.x = objs[i]->position.x - position.w;
            }
        }
    }
    position.x = newPos.x;


    CollisionCheck.x = position.x;
    CollisionCheck.h = max(1, abs(direction.y));
    CollisionCheck.y = (direction.y <0 )? position.y - CollisionCheck.h : position.y + position.h;
    CollisionCheck.w = 16;
    objs = World::Instance->getCollisions(CollisionCheck);
    if (direction.y < 0) {
        for (int i = 0; i < objs.size(); i++) {
            if (newPos.y < objs[i]->position.y + objs[i]->position.h) {
                newPos.y = objs[i]->position.y + objs[i]->position.h;
            }
        }
    }
    else {
        for (int i = 0; i < objs.size(); i++) {
            if (newPos.y > objs[i]->position.y) {
                newPos.y = objs[i]->position.y;
            }
        }
    }
    position.y = newPos.y;
    /*
    vector<tuple<GameObject*, SDL_Rect>> t = World::Instance->getCollisions(this);
    for (int i = 0; i < t.size(); i++) {

         Vector2 playerPos = { position.x + position.w / 2,position.y + position.h / 2 };
        SDL_Rect target = get<1>(t[i]);

        if (true) {
            auto renderer = World::Instance->getRenderer();
            auto camera = World::Instance->camera;
            SDL_Rect collisionbox = target;
            collisionbox.x = target.x + camera.x;
            collisionbox.y = target.y + camera.y;
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 100);
            SDL_RenderFillRect(renderer, &collisionbox);
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            SDL_RenderDrawRect(renderer, &collisionbox);
            SDL_RenderPresent(renderer);
        };

        Vector2 targetPos = { target.x + target.w / 2,target.y + target.h / 2 };
        if (abs(playerPos.x - target.x) > abs(playerPos.y - targetPos.y)) {
            playerPos.x -= targetPos.x - playerPos.x;
        }
        else {
            playerPos.y += targetPos.y - playerPos.y;
        }
        position.x = playerPos.x - position.w / 2;
        position.y = playerPos.y - position.h / 2;
    }
    */
}

// Spielerzustand aktualisieren
void Player::update(int deltaT) {
    printState();
    int moveXSpeed = (dashing ? MOVE_SPEED * 1.5f : MOVE_SPEED);


    // Bewegung nach links und rechts (normale Bewegung)
    if (InputManager::checkInput("Left")) {
        vx = moveXSpeed * -1;  // Linksbewegung
    }
    else if (InputManager::checkInput("Right")) {
        vx = moveXSpeed;  // Rechtsbewegung
    }
    else if (!dashing) {
        vx = 0;  // Keine Bewegung, wenn nicht gedasht wird
    }



    // Schwerkraft anwenden, wenn nicht im Dash
    if (playerState != PlayerState::DASHING) {
        vy += GRAVITY;
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
        if (!checkGroundCollision(320)) { // früher 0 (test)

            playerState = PlayerState::FALLING;  // Vom Boden in "FALLING"
        }
        else {
            vy = 0;
        }
        if (InputManager::checkInput("Jump")) {
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
        /*if (checkWallCollision() && false) {
            playerState = PlayerState::WALL_JUMPING;  // Wand-Sprung */
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
        if (InputManager::checkInput("Dash") && canDash) {
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
        break;

    case PlayerState::WALL_JUMPING:
   /*     wallStickTime += deltaT;  // Wand-Klebezeit erhöhen
        if (wallStickTime > MAX_WALL_STICK_TIME || !touchingWall) {
            playerState = PlayerState::FALLING;  // Von der Wand fallen
        }
        // Bewegung nach links/rechts, um sich von der Wand zu lösen
        if (InputManager::checkInput("Left") && objsLeft.empty()) {
            vx = -moveXSpeed;
            playerState = PlayerState::FALLING;
        }
        else if (InputManager::checkInput("Right") && objsRight.empty()) {
            vx = moveXSpeed;
            playerState = PlayerState::FALLING;
        }
        // Walljump ausführen
        if (InputManager::checkInput("Jump") && canWallJump) {
            vy = JUMP_FORCE;
            vx = objsLeft.empty() ? -JUMP_FORCE / 2 : JUMP_FORCE / 2;  // Abstoßrichtung
            canWallJump = false;  // Walljump blockieren
            playerState = PlayerState::JUMPING;
        }
        break;
        */

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













