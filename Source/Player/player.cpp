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
    linksNebenPlayer.x = position.x;
    linksNebenPlayer.y = position.y+position.h;
    linksNebenPlayer.w = 16*SCALE;
    linksNebenPlayer.h = max((float)1,vy);
    vector<GameObject*> objs = World::Instance->getCollisions(linksNebenPlayer);
    if (objs.size() > 0) {
        return true;
    }
    return false;
}

// Kollisionsprüfung (Wände)
bool Player::checkWallCollision() {
    // Links von der Spielfigur
    SDL_Rect leftSide;
    leftSide.x = position.x - 1;  // Ein kleines Stück links vom Spieler
    leftSide.y = position.y + 3; // Um nicht die oberen oder unteren Pixel zu berücksichtigen
    leftSide.w = 1;              // Sehr schmale Breite
    leftSide.h = position.h - 6; // Nur der mittlere Bereich des Spielers

    // Rechts von der Spielfigur
    SDL_Rect rightSide;
    rightSide.x = position.x + position.w;  // Direkt rechts vom Spieler
    rightSide.y = position.y + 3;          // Um nicht die oberen oder unteren Pixel zu berücksichtigen
    rightSide.w = 1;                       // Sehr schmale Breite
    rightSide.h = position.h - 6;          // Nur der mittlere Bereich des Spielers

    // Prüfen, ob es eine Wand links oder rechts gibt
    std::vector<GameObject*> leftCollisions = World::Instance->getCollisions(leftSide);
    std::vector<GameObject*> rightCollisions = World::Instance->getCollisions(rightSide);

    // Status setzen
    if (!leftCollisions.empty()) {
        touchingWall = true;
        return true; // Wand links gefunden
    }
    if (!rightCollisions.empty()) {
        touchingWall = true;
        return true; // Wand rechts gefunden
    }

    // Keine Wand berührt
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
    CollisionCheck.h = 16*SCALE;
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
    CollisionCheck.w = 16*SCALE;
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
            if (newPos.y+position.h > objs[i]->position.y) {
                newPos.y = objs[i]->position.y - position.h;
            }
        }
    }
    position.y = newPos.y;
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
        //vy += GRAVITY;
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
            vy = 0;
            playerState = PlayerState::ON_GROUND;  // Zurück auf den Boden
        }
        else if (InputManager::checkInput("Dash") && canDash) {
            dashing = true;
            dashTime = DASH_DURATION;  // Einheitliche Dauer
            dashDirection = InputManager::checkInput("Left") ? -1 : 1; // Richtung festlegen
            vx = DASH_SPEED * dashDirection;
            vy = 0;  // Vertikale Bewegung pausieren
            canDash = false;  // Dash einmalig deaktivieren
            playerState = PlayerState::DASHING;  // Wechsel zu DASHING
        }
        // Prüfen, ob Spieler eine Wand berührt
        // Wechsel in WALL_JUMPING, wenn Wandkontakt hergestellt wird
        else if (checkWallCollision()) {
            vx = 0;            // Horizontaler Stopp beim Wandkontakt
            vy = 0;            // Vertikaler Stopp beim Wandkontakt
            wallStickTime = 0; // Haftzeit zurücksetzen
            playerState = PlayerState::WALL_JUMPING;
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
        else {
            // Während des Dashens die Bewegung kontinuierlich fortsetzen
            float dashSpeedFactor = (float)dashTime / DASH_DURATION; // Reduziert die Geschwindigkeit im Dash
            vx = DASH_SPEED * dashDirection * dashSpeedFactor;
            vy = 0;  // Keine vertikale Bewegung
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
        // Bleibt im WALL_JUMPING, solange die Wand berührt wird
        if (!checkWallCollision()) {
            playerState = PlayerState::FALLING;  // Verlasse Wandkontakt
            break;
        }

        // Wand-Haftzeit erhöhen, aber keine Bewegung zulassen
        wallStickTime += deltaT;

        // Wand-Sprung nach links oder rechts
        if (InputManager::checkInput("Left")) {
            vx = -WALL_JUMP_FORCE_X;
            vy = WALL_JUMP_FORCE_Y;
            playerState = PlayerState::JUMPING;  // Wechsel zu JUMPING
        }
        else if (InputManager::checkInput("Right")) {
            vx = WALL_JUMP_FORCE_X;
            vy = WALL_JUMP_FORCE_Y;
            playerState = PlayerState::JUMPING;  // Wechsel zu JUMPING
        }

        // Keine Bewegung, solange der Spieler an der Wand bleibt
        vx = 0;
        vy = 0;
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













