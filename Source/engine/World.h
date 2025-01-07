#pragma once
#include <SDL.h>
#include "GameObject.h"
#include <stdio.h>
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include "InputManager.h"
#include "rendering/SpriteController.h"

using namespace std;


class World
{
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	bool running = true;
	int windowSizeX, windowSizeY;
	int deltaT;

	vector<GameObject*> background;
	vector<GameObject*> gameObjects;
	GameObject* player;
	vector<GameObject*> Enemies;

	int targetFrameRate;
	map<int, vector<GameObject*>> scenes;

public:

	static World* Instance;
	map<string, SpriteController> spriteControllers;

	World(int _windowSizeX, int _windowSizeY);

	bool run();
	bool createGameObject(GameObject* obj);
	bool destroyGameObject(GameObject* obj);
	bool destroyGameObject(int pos);
	SDL_Renderer* getRenderer();
	vector<GameObject*> getCollisions(GameObject *origin);
	vector<GameObject*> getCollisions(SDL_Rect area);
	vector<GameObject*> getTraceCollisions(Vector2 &start,Vector2 &end, vector<GameObject*> ignoreList = vector<GameObject*>(), bool closest = false);
};

enum ElementType {
	Background,
	Object,
	Enemy,
};

struct SceneElement {
	GameObject obj;
	SDL_Rect position;
	bool flipH, flipV;
};