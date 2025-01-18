#pragma once
#include "Utility.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
class InputManager
{
private:
	static map<string, bool> actionState;

public:
	map<string, int> keybinds;
	static map<string, bool> getInput() { return actionState; };
	static void pump();
	static bool checkInput(string Action) {
		return actionState.contains(Action) ? actionState[Action]:false;
	};
};

