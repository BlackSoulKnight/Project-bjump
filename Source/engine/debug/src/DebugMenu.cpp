#include "../DebugMenu.h"


bool example = false;
void exampleButtonFunction(void) {
// Muss eine void funktion sein welche nichs returned damit addButton es akzeptiert.
example = !example;
}


#pragma once
#include <SDL.h>
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include <stdio.h>
#include "Utility.h"

#include <cstdarg>
#include <functional>

DebugMenu::DebugMenu() {
    showDebugMenu = false;
    showRendererMenu = false;
    showGameMenu = false;
};

void DebugMenu::DebugInit(SDL_Window* window, SDL_Renderer* renderer) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);;
    ImGui_ImplSDLRenderer2_Init(renderer);
    DebugSetSettings(renderer);
};

void DebugMenu::DebugStart() {
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
};

void DebugMenu::DebugEnd(SDL_Renderer* renderer) {
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    ImGui::Render();
    float xscale, yscale;
    SDL_RenderGetScale(renderer, &xscale, &yscale);
    SDL_RenderSetScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
    SDL_RenderSetScale(renderer, xscale, yscale);
};

void DebugMenu::DebugSetSettings(SDL_Renderer* renderer) {
    ImGuiIO& io = ImGui::GetIO();
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(500, 600));
    //ImGui::SetNextWindowSize(ImVec2(100, 250));
}

void DebugMenu::Menu() {
    //ImGui::SetNextWindowSize(ImVec2(200, 250));
    ImGui::Begin("Debug menu");//, &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
    ImGui::Text("Hello!");
    if (ImGui::Button("Render Menu"))
        showRendererMenu = !showRendererMenu;
    //if (ImGui::Button("Game Menu"))
    //    showGameMenu = !showGameMenu;
    if (ImGui::Button("Show Player"))
        showPlayerMenu = !showPlayerMenu;
    if (ImGui::Button("Show Examples"))
        showExamples = !showExamples;
    ImGui::End();

}

void DebugMenu::PlayerMenu(Player* player) {
    ImGui::Begin("Player Menu", &showPlayerMenu);
    ImGui::Text("Name: (%s)", player->name);
    ImGui::Text("X: (%d)\tY: (%d)", player->position.x, player->position.y);
    ImGui::Text("Is Active? (%s)", player->isActive ? "Yes" : "No");
    ImGui::Text("Layer: (%d)", player->layer);
    float GRAVITY = 0.5f * SCALE;
    float JUMP_FORCE = -10.0f * SCALE;
    float FALL_SPEED = 10.0f * SCALE;
    float MOVE_SPEED = 5.0f * SCALE;
    float DASH_SPEED = 200.0f * SCALE;
    float DASH_DURATION = 0.5 * SCALE;
    float WALL_JUMP_FORCE_Y = -8.0f * SCALE;
    float WALL_JUMP_FORCE_X = 5.0f * SCALE;
    ImGui::SliderFloat("Move Speed", &player->MOVE_SPEED, 5, 100);
    ImGui::SliderFloat("Fall Speed", &player->FALL_SPEED, 10, 200);
    ImGui::SliderFloat("Dash Speed", &player->DASH_SPEED, 200, 2000);
    ImGui::SliderFloat("Dash Duration", &player->DASH_DURATION, 0.5, 10.0);
    ImGui::SliderFloat("Gravity", &player->GRAVITY, 0.5,10.0);
    ImGui::SliderFloat("Jump Force", &player->JUMP_FORCE,-100.0, -10.0);
    ImGui::SliderFloat("Wall Jump Force X", &player->WALL_JUMP_FORCE_X, -80.0, -8.0);
    ImGui::SliderFloat("Wall Jump Force Y", &player->WALL_JUMP_FORCE_Y, 5.0, 100.0 );

    ImGui::End();
}

void DebugMenu::ShowExampleAddsToMenu() {
    //Muss ich noch fixen irgendwie
    std::function<void(void)> functionToCall = exampleButtonFunction;
    const char* windowName = "Example Window";

    ImGui::Begin(windowName, &showExamples);
    ImGui::Text("So fügt man sachen ein!");
    ImGui::End(); // Hier wird das frame geschlossen
    addText(windowName, "PS: DIE MEISTEN SACHEN SETZTEN SICH WIEDER AUF NULL");
    addText(windowName, "WEIL ICH ZU FAUL WAR DIE VARIABLEN AUSSERHALB DER");
    addText(windowName, "EXAMPLE FUNKTION ANZULEGEN");


    addText(windowName, "");

    addText(windowName, "So wird der Text hinzugefügt. Man muss den Namen des Fensters hinzufügen");
    addText(windowName, "");

    addText(windowName, "Um einen Toggle hinzuzufügen benötigt man einen Bool, bei dem ");
    addText(windowName, "'addToggle' wird dann die Addresse des Bools angegeben.");
    bool exampleCheckbox = false;
    addToggle(windowName, "Checkbox added", &exampleCheckbox);
    addText(windowName, "");
    addText(windowName, "Ein Button ist kompliziert, man benötigt:");
    addText(windowName, "1. Fenster Name");
    addText(windowName, "2. Label des Buttons");
    addText(windowName, "2. Funktion welcher der Button ausführen soll");
    addText(windowName, "");

    //const char* listOfComplexStuff[] = { "1. Fenster Name", "2.Label des Buttons", "3. Funktion welcher der Button ausführen soll" };
    //addList(windowName, &currentlySelected, *listOfComplexStuff, 3);
    //addText(windowName, "Wenn du die Liste haben willst guck dir den Code an.");
    
    addButton(
        windowName,
        "Klick Mich!",
        functionToCall
    );
    if (example)
    {
        int intSlider = 0;
        float floatSlider = 0.0f;
        addText(windowName, "Slider sind recht einfach: FensterName, SliderLabel, addresseVonVariable,min, max");
        addSlider(windowName, "intSliderLabel", &intSlider, 1, 20);
        addSlider(windowName, "floatSliderLabel", &floatSlider, 1.0f, 2.0f);
    }
};

void DebugMenu::RenderMenu(bool& debugOn) {
    //ImGui::SetNextWindowSize(ImVec2(300, 250));
    ImGui::Begin("Render Menu", &showRendererMenu);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
    ImGui::Text("Render Menu!");
    ImGui::Checkbox("Debug Collisions", &debugOn);
    ImGui::End();
};
void DebugMenu::GameMenu() {
    //ImGui::SetNextWindowSize(ImVec2(100, 250));
    ImGui::Begin("Game Menu", &showGameMenu);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
    ImGui::Text("Game Menu!");
    ImGui::End();
    ImGui::Begin("Game Menu");

    ImGui::Text("Game Menu!");
    ImGui::End();

};
//Funktionen zum erstellen von menüpunkten
// SLIDER, BUTTON, TOGGLE
template <typename T>
void DebugMenu::addSlider(const char* windowName, const char* sliderLabel, T* variableToAdd, T vMin, T vMax) {
    ImGui::Begin(windowName);
    if (typeid(*variableToAdd).name() == typeid(float).name())
        ImGui::SliderFloat(sliderLabel, (float*)variableToAdd, vMin, vMax);
    else
        ImGui::SliderInt(sliderLabel, (int*)variableToAdd, vMin, vMax);
    ImGui::End();
};

void DebugMenu::addToggle(const char* windowName, const char* label, bool* variableToAdd) {
    ImGui::Begin(windowName);
    ImGui::Checkbox(label, variableToAdd);
    ImGui::End();
};
void DebugMenu::addButton(const char* windowName, const char* buttonLabel, std::function<void(void)> passedFunction) {
    ImGui::Begin(windowName);
    if (ImGui::Button(buttonLabel))
    {
        passedFunction();
    }
    ImGui::End();
}

void DebugMenu::addText(const char* windowName, const char* textToDisplay) {
    ImGui::Begin(windowName);
    ImGui::Text(textToDisplay);
    ImGui::End();
}
/*
void DebugMenu::addList(const char* windowName, int* selectedIndex, const char* listOfStuff[], int displaySize) {
    ImGui::Begin(windowName);
    const char* listOfComplexStuff[] = { "1. Fenster Name", "2.Label des Buttons", "3. Funktion welcher der Button ausführen soll" };
    static int selectedItem = 0;
    ImGui::ListBox("listBox", selectedIndex, listOfStuff, IM_ARRAYSIZE(listOfStuff), 3);
    ImGui::End();
}
*/
