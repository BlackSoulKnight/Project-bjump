#pragma once
#include "../../engine/World.h"
#include "../../engine/rendering/SpriteController.h"	
#include <vector>
#include <string>
void renderGraphics(const World& wS, SDL_Renderer* renderer, std::vector<SpriteController> spriteControllers);
template <typename T> void renderEntity(const World& wS, SDL_Renderer* renderer, vector<T> entitiesObject, SpriteController entitySpriteController);
template <typename T> SDL_Rect scaledEntityPosition(T entity);