#pragma once
#include "../../engine/World.h"
#include "../../engine/rendering/SpriteController.h"	
#include <vector>
#include <string>
#include <map>

void renderGraphics(const World& wS, SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	// Render Entities
	//renderEntity(wS, renderer, wS.worldObjects, wS.spriteControllers);


	SDL_RenderPresent(renderer);
}
template <typename T>
void renderEntity(const World& wS, SDL_Renderer* renderer, vector<T> entitiesObject, map<string,SpriteController> entitySpriteControllers) {
	for (T entity : entitiesObject)
	{
		if (entity.!isActive)
			continue;
		SDL_Rect entityPosition = scaledEntityPosition<T>(entity);
		
		// Flag: Wenn das Spritesheet noch nicht geladen wurde
		bool generated = false;
		if (!entitySpriteControllers.contains(entity.sprite.spriteSheetLocation))
		{
			//TODO: Generate SpriteSheet
			SDL_Texture* tmp = IMG_LoadTexture(renderer, entity.sprite.spriteSheetLocation);
			generated = true;
		}
		if (generated)
			SDL_RenderCopy(renderer, tmp, entitySpriteController.x(entity.sprite.x).y(entity.sprite.y).getSprite(), &entityPosition);
		else
			SDL_RenderCopy(renderer, wS.spriteControllers.at(wS.spriteControllers.find(entity.sprite.spriteSheetLocation)), entitySpriteController.x(entity.sprite.x).y(entity.sprite.y).getSprite(), &entityPosition);

	}
}

template <typename T>
SDL_Rect scaledEntityPosition(T entity) {
	return SDL_Rect{
			-entity.sprite.x,
			-entity.sprite.y,
			entity.sprite.w* entity.scale.x,
			entity.sprite.h* entity.scale.y
	};
}