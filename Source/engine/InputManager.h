#pragma once
#include "Utility.h"
class InputManager
{
private:
	static map<string, bool> actionState;

public:
	map<string, int> keybinds;
	static map<string, bool> getInput() { return actionState; };
	static void pump();
};

