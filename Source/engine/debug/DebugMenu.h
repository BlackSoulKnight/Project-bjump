
#pragma once
#include <SDL.h>
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include <stdio.h>
#include "Utility.h"

#include <cstdarg>


class DebugMenu
{
public:
    bool showDebugMenu = true;
    bool showRendererMenu = false;
    bool showGameMenu = false;

    DebugMenu() {
        showDebugMenu = false;
        showRendererMenu = false;
        showGameMenu = false;
    };

    void DebugInit(SDL_Window* window, SDL_Renderer* renderer) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);;
        ImGui_ImplSDLRenderer2_Init(renderer);
        DebugSetSettings(renderer);
    };
    void DebugStart() {
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
    };
    void DebugEnd(SDL_Renderer* renderer) {
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

    void DebugSetSettings(SDL_Renderer* renderer) {
        ImGuiIO& io = ImGui::GetIO();
        ImGui::SetNextWindowPos(ImVec2(0,0));
    }

    void Menu() {
        ImGui::Begin("Another Window");//, &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("Hello!");
        ImGui::End();
	};

	//Funktionen zum erstellen von menüpunkten
	// SLIDER, BUTTON, TOGGLE



};

