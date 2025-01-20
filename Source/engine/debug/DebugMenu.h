
#pragma once
#include <SDL.h>
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include <stdio.h>
#include "Utility.h"
#include "../GameObject.h"
#include "../Player/player.h"
#include "../rendering/Camera.h"

#include <cstdarg>
#include <functional>



class DebugMenu
{
private:
    int currentlySelected = 1;


public:
    bool showExamples = false;
    bool showDebugMenu = true;
    bool showRendererMenu = false;
    bool showGameMenu = false;
    bool showPlayerMenu = false;
    bool showCameraMenu = false;
    const char* DEBUGMENU = "Debug menu";
    const char* GAMEMENU = "Game Menu";
    const char* RENDERMENU = "Render Menu";
    DebugMenu();

    void DebugInit(SDL_Window* window, SDL_Renderer* renderer);

    void DebugStart();

    void DebugEnd(SDL_Renderer* renderer);

    void DebugSetSettings(SDL_Renderer* renderer);

    void Menu();
    void RenderMenu(bool& debugOn);
    void GameMenu();
    void PlayerMenu(Player* player);
    void CameraMenu(Camera* cam);

    void ShowExampleAddsToMenu();
    template <typename T>
    void addSlider(const char* windowName, const char* sliderLabel, T* variableToAdd, T vMin, T vMax);

    void addToggle(const char* windowName, const char* label, bool* variableToAdd);
    void addButton(const char* windowName, const char* buttonLabel, std::function<void(void)> passedFunction);

    void addText(const char* windowName, const char* textToDisplay);

    //void addList(const char* windowName, int* selectedIndex, const char* listOfStuff[], int displaySize);

};

