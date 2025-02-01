#pragma once
#include <SDL.h>
#include <stdio.h>
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <chrono>
#include <filesystem>
#include "GameObject.h"
#include "InputManager.h"
#include "Utility.h"
#include "rendering/SpriteController.h"
#include "rendering/Camera.h"
#include "debug/DebugMenu.h"
#include "../Player/player.h"
#include "../Enemy/Enemy.h"

using namespace std;

class World
{
private:
	//SDL_Rect camera;
	bool debug = true;
	SDL_Window* window;
	SDL_Renderer* renderer;
	bool running = true;
	int windowSizeX, windowSizeY;
	int deltaT;
	vector<GameObject*> background;
	vector<GameObject*>** WorldObjectBG;
	vector<GameObject*> gameObjects;
	vector<GameObject*>** WorldObject;
	unordered_map<string, vector<GameObject*>> gameObjectsNew;
	vector<GameObject*> Enemies;
	int targetFrameRate;
	GameObject* generateBlock(int, SDL_Rect, int, int);
	Enemy* generateEnemy(int, SDL_Rect, int, int);
	
	GameObject* generateDefaultBackground(int tile, SDL_Rect tilePos, int mapWidth, int mapHeight);
	void testFunction(GameObject* obj);

public:
	Camera camera;
	DebugMenu debugConsole;
	Player* player;
	void LoadGameAssets();
	static World* Instance;
	DebugMenu debugWindow;
	map<string, SpriteController> spriteControllers;
	void LoadFile(string, bool);
	vector<vector<int>> load_map(const char* filePath);
	int test = 0;
	World(int _windowSizeX, int _windowSizeY);
	int mapSizeX, mapSizeY;
	bool run();
	void renderGraphics();
	void renderEntity(GameObject&);

	//SDL_Rect scaleEntityPosition(GameObject); <- Macht probleme, siehe funktion.
	bool createGameObject(GameObject* obj);
	bool destroyGameObject(GameObject* obj);
	SDL_Renderer* getRenderer();
	vector<tuple<GameObject*,SDL_Rect>> getCollisions(GameObject *origin, vector<string> IgnoreTags = vector<string>());
	vector<GameObject*> getCollisions(SDL_Rect area, vector<string> IgnoreTags = vector<string>());
	vector<GameObject*> getTraceCollisions(Vector2 &start,Vector2 &end, vector<GameObject*> ignoreList = vector<GameObject*>(), bool closest = false);
	bool collideWithPlayer(GameObject* obj);
};

enum ElementType {
	Background,
	Object,
	EnemyE,
};

struct SceneElement {
	GameObject obj;
	SDL_Rect position;
	bool flipH, flipV;
};