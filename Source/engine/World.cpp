#include "World.h"
#define MAP_SPRITE_SHEET "../resources/Tilemap/monochrome_tilemap_transparent_packed.png"

World* World::Instance = nullptr;
const int WINDOW_WIDTH = 16 * 40 * SCALE;
const int WINDOW_HEIGHT = 16 * 40 * SCALE;
World::World(int _windowSizeX, int _windowSizeY) {
    windowSizeX = _windowSizeX;
    windowSizeY = _windowSizeY;
    debug = true;
    deltaT = 0;
    targetFrameRate = 60;
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_CreateWindowAndRenderer(_windowSizeX, _windowSizeY, SDL_WINDOW_RESIZABLE, &window, &renderer);
    SDL_RenderSetLogicalSize(renderer, WINDOW_WIDTH, WINDOW_HEIGHT);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    //Load map Sprites
    World::Instance = this;
    debugWindow = DebugMenu();
    LoadGameAssets();
    //LoadFile(string("../Levels/dev-sandbox.bjump"));
    LoadFile(string("../Levels/map.bjump"));
    camera = { 0,0, WINDOW_WIDTH,WINDOW_HEIGHT };
    //camera.setCameraOffset(40, 40);
    debugWindow.DebugInit(window, renderer);
}




bool World::run() {
    chrono::time_point<chrono::system_clock> start = chrono::system_clock::now();
    if (deltaT <= 0)
        deltaT = 1;

    InputManager::pump();


    //Change WorldState

    
    for (auto& kv : gameObjectsNew) {
        for (int i = 0; i < kv.second.size(); i++) {
            if (kv.second[i]->isActive)
                kv.second[i]->update(deltaT);
        }
    }
    
    //Collide WorldState

    //Render
    //SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    //SDL_RenderClear(renderer);
    //SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    //for (int i = 0; i < gameObjects.size(); i++) {
    //    SDL_RenderFillRect(renderer, &gameObjects[i]->position);
    //}
    //SDL_RenderPresent(renderer);

    renderGraphics();
    

    
    deltaT = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now() - start).count();
    int frameRate = 1000 / targetFrameRate;
    if (frameRate > deltaT)
        SDL_Delay(frameRate-deltaT);
    deltaT = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now() - start).count();
    cout << deltaT << "\n";
    SDL_SetWindowTitle(window, to_string(deltaT).c_str());
    return running;
}

vector<tuple<GameObject*,SDL_Rect>> World::getCollisions(GameObject *origin, vector<string> tags) {
    vector<tuple<GameObject*, SDL_Rect>> collisions;
    for (int i = 0; i < tags.size(); i++) {
        if (gameObjectsNew.contains(tags[i])) {
            for (int n = 0; n < gameObjectsNew[tags[i]].size(); n++) {
                GameObject* obj = gameObjectsNew[tags[i]][n];
                SDL_Rect a;
                if (origin == obj)
                    continue;
                if (SDL_IntersectRect(&obj->position, &origin->position, &a))
                    collisions.push_back({ gameObjects[i],a });
            }
        }
    }
    return collisions;
}
vector<GameObject*> World::getCollisions(SDL_Rect area, vector<string> tags) {
    vector<GameObject*> result;

    for (int i = 0; i < tags.size(); i++) {
        if (gameObjectsNew.contains(tags[i])) {
            for (int n = 0; n < gameObjectsNew[tags[i]].size(); n++) {
                GameObject* obj = gameObjectsNew[tags[i]][n];
                if (SDL_HasIntersection(&obj->position, &area))
                    result.push_back(obj);
            }
        }
    }
    return result;
}

bool World::collideWithPlayer(GameObject* obj) {
    if (SDL_HasIntersection(&obj->position, &player->position))
        return true;
    return false;
}

bool World::createGameObject(GameObject* obj) {
    if (gameObjectsNew.contains(obj->tag)) {
        gameObjectsNew[obj->tag].push_back(obj);
    }
    else {
        gameObjectsNew[obj->tag] = vector<GameObject*>();
        gameObjectsNew[obj->tag].push_back(obj);
    }

    gameObjects.push_back(obj);
    return true;
}

bool World::destroyGameObject(GameObject* obj) {

    for (int i = 0; i < gameObjectsNew[obj->tag].size(); i++) {
        if (gameObjectsNew[obj->tag][i] == obj) {
            gameObjectsNew[obj->tag].erase(gameObjects.begin() + i - 1);
            return true;
        }
    }
    /*
    for (int i = 0; i < gameObjects.size(); i++) {
        if (gameObjects[i] == obj) {
            gameObjects.erase(gameObjects.begin()+i-1);
            return true;
        }
    }*/
    return false;
}

SDL_Renderer* World::getRenderer() {
    return renderer;
}

vector<GameObject*> World::getTraceCollisions(Vector2 &start, Vector2 &end,vector<GameObject*> ignoreList, bool closest) {
    vector<GameObject*> result;
    Vector2 _start, _end, closestPoint;
    for (int i = 0; i < gameObjects.size(); i++) {
        _start = start;
        _end = end;
        if (find(ignoreList.begin(),ignoreList.end(),gameObjects[i]) != ignoreList.end()) {
            continue;
        }
        if (SDL_IntersectRectAndLine(&gameObjects[i]->position, &_start.x, &_start.y, &_end.x, &_end.y))
        {
            if (closest) {

            }
            result.push_back(gameObjects[i]);
        }
    }
    return result;
}


void World::LoadFile(string fileName) {
    string file = fileName;
    if (file.size() == 0)
    {
        file = "..\\Levels\\test_map.bjump";
    }
    vector<vector<int>> loadedMap = load_map(file.c_str());

    // Eindimensionale Map beschreibung
    vector<int> reducedDimensionMap;
    vector<SDL_Rect> reducedDimensionTilePosition;

    // Generiere Daten über die map
    int xIndex = 0, yIndex = 0;
    int xMapMaxSize = 0, yMapMaxSize = 0;
    for(auto row : loadedMap)
    {
        for (auto tile : row) {
            if (xIndex >= xMapMaxSize)
                xMapMaxSize = xIndex;
            if (tile == -1) {
                xIndex++;
                continue;
            }
            //    8,8 Muss angepasst werden wenn wir andere tiles nutzen
            SDL_Rect posOfTile = { xIndex * 16 * SCALE, yIndex * 16 * SCALE, 16 * SCALE, 16 * SCALE };
            reducedDimensionMap.push_back(tile);
            reducedDimensionTilePosition.push_back(posOfTile);
            xIndex++;
        }
        xIndex = 0;
        yIndex++;
        yMapMaxSize++;
    }
    //Load Default BG
    for (int i = 0; i < reducedDimensionMap.size(); i++)
    {
        if (reducedDimensionMap[i] != '-1')
            background.push_back(generateDefaultBackground(
                reducedDimensionTilePosition[i],
                xMapMaxSize,
                yMapMaxSize
            ));
    }


    for (int i = 0; i < reducedDimensionMap.size(); i++) {
        if (reducedDimensionMap[i] != '-1')
        createGameObject(generateBlock(
            reducedDimensionMap[i],
            reducedDimensionTilePosition[i],
            xMapMaxSize,
            yMapMaxSize
        ));
    }
}
void World::renderGraphics() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    int w = windowSizeX, h = windowSizeY;
    SDL_RenderGetLogicalSize(renderer,&w, &h);

    //camera.xOffset = -(camera.w / 2);
    //camera.yOffset = -(camera.h / 2);
    
    camera.calculatePositionBasedOnPlayer(*player);
    //camera.setCameraPosition(1000, 300);
    debugWindow.DebugStart();
    // Render Default Background
    for (auto bg : background)
        renderEntity(*bg);

    // Render World elements
    for (auto block : gameObjects) {
        renderEntity(*block);
    }

    debugWindow.Menu();

    if (debugWindow.showRendererMenu) {
        debugWindow.RenderMenu(debug);
    }
    if (debugWindow.showGameMenu)
    {
        debugWindow.GameMenu();
    }
    if (debugWindow.showPlayerMenu)
    {
        debugWindow.PlayerMenu(player);
    }
    if (debugWindow.showCameraMenu)
        debugWindow.CameraMenu(&camera);
    if (debugWindow.showExamples)
        debugWindow.ShowExampleAddsToMenu();


    debugWindow.DebugEnd(renderer);
    SDL_RenderPresent(renderer);
}
void World::renderEntity(GameObject &entity) {

    if (!entity.isActive)
        return;

    // Scaling nicht an
    //SDL_Rect entityPosition = scaleEntityPosition(entity);

    if (!spriteControllers.contains(entity.sprite.spiteSheetLocation))
    {
        SpriteController tmpSpriteController = SpriteController::generateController(entity.sprite.spiteSheetLocation.c_str(), 16, 16, renderer);
        spriteControllers.insert({ entity.sprite.spiteSheetLocation, tmpSpriteController });
    }
    SpriteController entitySpriteController = spriteControllers.at(entity.sprite.spiteSheetLocation);

    // Transformieren des Sprites
    int flipDirection = 0;
    if (entity.sprite.flipV)
        flipDirection += SDL_FLIP_VERTICAL;
    if (entity.sprite.flipH)
        flipDirection += SDL_FLIP_HORIZONTAL;

    // Hardcoded 8,8 muss ausgewechselt werden!
    SDL_Rect spriteSheetLocation = { entity.getSprite().x, entity.getSprite().y , 16 ,16 };
    SDL_Rect actualPosition = {
        entity.position.x + (camera.x + camera.xOffset),
        entity.position.y + (camera.y + camera.yOffset),
        16 * SCALE, 16  * SCALE };

    /*
    int rowOverflow = 0;
    if (((entity.frameCount / 60) % entitySpriteController.getSpritesPerX()) + entity.sprite.x > entitySpriteController.getSpritesPerX())
        rowOverflow++;
    */
    SDL_RenderCopyEx(
        renderer,
        entitySpriteController.getTexture(),
        //Berechnung in XY in eigene funktion lagern
        entitySpriteController.x(entity.sprite.x).y(entity.sprite.y).getSprite(),
        &actualPosition,
        0,
        NULL,
        (SDL_RendererFlip)(flipDirection));

    if (debug) {
        SDL_Rect collisionbox = { 0,0,16 * SCALE,16 * SCALE };
        collisionbox.x = entity.position.x + (camera.x + camera.xOffset);
        collisionbox.y = entity.position.y + (camera.y + camera.yOffset);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 100);
        SDL_RenderFillRect(renderer, &collisionbox);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &collisionbox);
    }

    /*
    if (entity.sprite.animationSize == 0)
        return;
    entity.frameCount++;
    */

}

GameObject* World::generateBlock(int tile, SDL_Rect tilePos, int mapWidth, int mapHeight) {
    GameObject* block = new GameObject();
    Sprite spriteData;
    SpriteController blockSprites = spriteControllers["../resources/Tilemap/monochrome_tilemap_transparent_packed.png"];
    spriteData.flipH = false, spriteData.flipV = false;
    spriteData.animationSize = 0;
    // Adve hardcoded für Blöcke der Welt
    spriteData.spiteSheetLocation = "../resources/Tilemap/monochrome_tilemap_transparent_packed.png";

    // Berechnen der XY position durch umkehren der Tile berechnung
    //  spriteSheetWidth * posY + posX = tile
    //  posX = tile % spriteSheetWidth
    //  posY = (tile - posX) / spriteSheetWidth
    spriteData.x = tile % blockSprites.getSpritesPerX();
    spriteData.y = (tile - spriteData.x) / blockSprites.getSpritesPerX();
    /**
    *   block.collisions; // vector<gameObject> ?
    *   block.layer;      // int          worldBlocks = 1 maybe change to enum?
    *   block.scale;      // Vector2f     x:1 y:1
    *   block.position;   // SDL_Rect     xyIndex
    *   block.name;       // String       ?
    *   block.sprite;     // Sprite       animsize = 1; flip false, spritesheet location xyIndex on tilemap;
    *   block.isActive;   // bool         true
    */
    block->layer = 1;
    block->scale.x = 1, block->scale.y = 1;
    block->position = tilePos;
    block->name = "Block";   // ??
    block->tag = "World";
    block->sprite = spriteData;
    if (tile == -1) {
        block->isActive = false;
    } else
        block->isActive = true;

    return block;
}

GameObject* World::generateDefaultBackground(SDL_Rect bgPos, int mapWidth, int mapHeight) {
    GameObject* block = new GameObject();
    Sprite spriteData;
    SpriteController blockSprites = spriteControllers["../resources/Tilemap/monochrome_tilemap_transparent_packed.png"];
    spriteData.flipH = false, spriteData.flipV = false;
    spriteData.animationSize = 0;
    // Adve hardcoded für Blöcke der Welt
    spriteData.spiteSheetLocation = "../resources/Tilemap/monochrome_tilemap_transparent_packed.png";

    // Berechnen der XY position durch umkehren der Tile berechnung
    //  spriteSheetWidth * posY + posX = tile
    //  posX = tile % spriteSheetWidth
    //  posY = (tile - posX) / spriteSheetWidth
    spriteData.x = 0;
    spriteData.y = 0;
    /**
    *   block.collisions; // vector<gameObject> ?
    *   block.layer;      // int          worldBlocks = 1 maybe change to enum?
    *   block.scale;      // Vector2f     x:1 y:1
    *   block.position;   // SDL_Rect     xyIndex
    *   block.name;       // String       ?
    *   block.sprite;     // Sprite       animsize = 1; flip false, spritesheet location xyIndex on tilemap;
    *   block.isActive;   // bool         true
    */
    block->layer = 1;
    block->scale.x = 1, block->scale.y = 1;
    block->position = bgPos;
    block->name = "Block";   // ??
    block->sprite = spriteData;

    return block;
}


/*
SDL_Rect scaleEntityPosition(GameObject entity) {
    return SDL_Rect{
            entity.sprite.x,
            entity.sprite.y,
            entity.sprite.w * entity.scale.x, <- macht probleme sprite hat keine W / H !
            entity.sprite.h * entity.scale.y
    };
}
*/

vector<vector<int>> World::load_map(const char* filePath)
{
    vector<vector<int>> mapData;
    ifstream level;
    level.open(filePath);
    string line, tile;
    if (!level.is_open())
        exit(1);
    while (level) {
        getline(level, line);
        stringstream line_stream(line);
        vector<int> tmp;
        while (getline(line_stream, tile, ',')) {
            tmp.push_back(atoi(tile.c_str()));
        }
        if (tmp.size() == 0)
            break;
        mapData.push_back(tmp);
    }
    level.close();
    return mapData;
}

void World::LoadGameAssets() {
    SpriteController tmpSpriteController;
    if (!spriteControllers.contains(MAP_SPRITE_SHEET));
    {
        tmpSpriteController = SpriteController::generateController(MAP_SPRITE_SHEET, 16,16, renderer);

        spriteControllers.insert({ string(MAP_SPRITE_SHEET), tmpSpriteController });
    }
}