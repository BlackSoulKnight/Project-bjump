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
using namespace std;

class World
{
private:
	SDL_Rect camera;
	bool debug = true;
	SDL_Window* window;
	SDL_Renderer* renderer;
	bool running = true;
	int windowSizeX, windowSizeY;
	int deltaT;

	vector<GameObject*> background;
	vector<GameObject*> gameObjects;
	vector<GameObject*> Enemies;

	int targetFrameRate;
	map<int, vector<GameObject*>> scenes;
	GameObject* generateBlock(int, SDL_Rect, int, int);
	GameObject* generateDefaultBackground(SDL_Rect, int mapWidth, int mapHeight);


public:
	GameObject* player;
	void LoadGameAssets();
	static World* Instance;
	map<string, SpriteController> spriteControllers;
	void LoadFile(string);
	vector<vector<int>> load_map(const char* filePath);

	World(int _windowSizeX, int _windowSizeY);

	bool run();
	void renderGraphics();
	void renderEntity(GameObject&); 
	//SDL_Rect scaleEntityPosition(GameObject); <- Macht probleme, siehe funktion.
	bool createGameObject(GameObject* obj);
	bool destroyGameObject(GameObject* obj);
	bool destroyGameObject(int pos);
	SDL_Renderer* getRenderer();
	vector<tuple<GameObject*,SDL_Rect>> getCollisions(GameObject *origin);
	vector<GameObject*> getCollisions(SDL_Rect area);
	vector<GameObject*> getTraceCollisions(Vector2 &start,Vector2 &end, vector<GameObject*> ignoreList = vector<GameObject*>(), bool closest = false);
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