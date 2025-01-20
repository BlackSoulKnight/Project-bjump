#pragma once
#include <SDL.h>
#include "../Utility.h"
#include "../../Player/player.h"
class Camera {
public: 
	int x, y;
	int w, h;
	int xOffset = 0, yOffset = 0;
	Vector2 gridLocation;

	Vector2 outOfScreen(const Player player) {
		return { ((player.position.x) / (w)), ((player.position.y) / (h)) };
	}
	bool entityOutOfScreen(const GameObject entity) {
		return !(((entity.position.x / w)) == gridLocation.x && (entity.position.y / h) == gridLocation.y);
	}
	void setCameraOffset(int xOff, int yOff) {
		xOffset = xOff;
		yOffset = yOff;
	}

	void calculatePositionBasedOnPlayer(const Player player) {
		gridLocation = outOfScreen(player);
		x = (w * -gridLocation.x) + xOffset;
		y = (h * -gridLocation.y) + yOffset;
	}
	void setCameraPosition(int xPos, int yPos) {
		xOffset = xPos;
		yOffset = yPos;
		gridLocation = { xPos / w, yPos / h };
		x = xPos;
		y = yPos;
	}
	void setCameraGridPosition(int xGridPos, int yGridPos) {
		xOffset = xGridPos * w;
		yOffset = yGridPos * h;
		gridLocation = { xGridPos, yGridPos };
		x = xGridPos * w;
		y = yGridPos * h;
	}
};

