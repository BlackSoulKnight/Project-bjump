#include <iostream>
#include <stdlib.h>
#include <windows.h>
#include <wchar.h>
#include <vector>
#include <ShObjIdl.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <commdlg.h>
#include <fstream>
#include <iterator>
#include <sstream>
#include "../../Source/engine/rendering/SpriteController.h"
using namespace std;
#define FILE_BUFFER_SIZE 200

enum Layer
{
    FOREGROUND = 0,
    BACKGROUND = 1,
    SHOW_ALL = 2,
    PLACE_ENEMY = 3,
};

struct WorldState
{
    int FONT_SIZE = 16;
    TTF_Font* font;
    SDL_Surface* layer_msg;
    SDL_Surface* enemy_msg;
    char* workingDirectory;
    int vertical_size = 30;
    int horizontal_size = 30;

    bool show_controlls = true;

    float zoom = 2;
    float selected_tile_set_factor = 0.5;

    int WINDOW_SIZE_W = 400;
    int WINDOW_SIZE_H = 300;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    SDL_Texture* transparent_texture;
    SpriteController sprite_controller;

    int selected_tile_x = 0;
    int selected_tile_y = 0;
    bool drag = false;
    bool repeat_placement = false;
    bool grid = false;
    bool delete_mode = false;
    bool hide_hover_position = false;
    int current_layer = 0;
    int current_enemy = -2;
    std::vector<std::vector<int>> tiles;
    std::vector<std::vector<int>> background;

    SDL_Rect camera = { 0, 0, 0, 0 };
    SDL_Rect mouse_position_to_highlight = { 0,0,0,0 };

    LPCWSTR editor_controlls =
        L"F1:\t\tZeige Tastaturbelegung\n\n"
        L"L_Click:\t\tTiles Platzieren\n"
        L"R_Click:\t\tKamera bewegen\n"
        L"S:\t\tSpeichern\n"
        L"G:\t\tGrid aktivieren/deaktivieren\n"
        L"X:\t\tToggle Löschen\n"
        L"1 - 4:\t\tEbenen wechseln\n"
        L"\t\t┣ 1: Vordergrund\n"
        L"\t\t┣ 2: Hintergrund\n"
        L"\t\t┣ 3: Alle Ebenen\n"
        L"\t\t┗ 4: Enemy Placement\n\n"
        L"LArr/RArr:\t\tSwitch Enemy Type\n\n"
        L"MRad +/-:\tZoom +/-\n"
        L"UP/DOWN:\tTileset kleiner/größer machen\n";
    LPCWSTR editor_controlls_title = L"Editor Controlls";
} WS;

#pragma region GameLoop Function Declaration
void init_editor(char*);
void read_events();
void render_graphics();
#pragma endregion

#pragma region Event Function Declaration
void mouse_button_1(SDL_Event);
void increment_current_enemy();
void decrement_current_enemy();
#pragma endregion

#pragma region File Function Declaration
void start_editor(char*);
vector<vector<int>> load_map(const char* filePath);
void save_created_map();
void init_sprite_controller(char* filePath, int sizeX, int sizeY);
#pragma endregion

#pragma region WindowsAPI Functions
PWSTR WINAPI get_file_path();
int WINAPI showMessage(LPCWSTR, LPCWSTR);
char* convert_PWSTR_to_char(PWSTR);
#pragma endregion

#pragma region Helper Functions
void render_texture_auto(SDL_Renderer*, SDL_Rect*, SDL_Rect*, Layer);
void out_of_spritesheet(SDL_Event e);
#pragma endregion


using namespace std;
int main(int argc, char **argv)
{
    SetConsoleTitle(TEXT("BJump Map Editor: v0.9.1"));
    WS.workingDirectory = argv[0];
    cout << "BJump Map Editor\n";
    cout << "Waehle das SpriteSheet aus.\n";
    PWSTR sprite_sheet_path = get_file_path();
    char* converted_sprite_sheet_path = convert_PWSTR_to_char(sprite_sheet_path);
    if (strlen(converted_sprite_sheet_path) == 0)
        converted_sprite_sheet_path = new char[100] {"../../resources/Tilemap/monochrome_tilemap_transparent_packed.png"};
    cout << "Datei: " << converted_sprite_sheet_path << endl << endl;
    cout << "Sprite groesse X: ";
    int sizeX = 0;
    cin >> sizeX;
    cout << "Sprite groesse Y: ";
    int sizeY = 0;
    cin >> sizeY;
    cout << "\n1: Neue Map erstellen\n2: Laden von vorhandener Datei\n\n";

    int input;
    cin >> input;

    init_sprite_controller(converted_sprite_sheet_path, sizeX, sizeY);
 
    
    PWSTR map_file_path = nullptr;
    char* converted_map_file_path = nullptr;
     
    switch (input)
    {
    case 1:
        cout << "Breite der Map: ";
        cin >> WS.horizontal_size;
        cout << "Hoehe der Map: ";
        cin >> WS.vertical_size;
        start_editor(converted_sprite_sheet_path);
        break;
    case 2:
        {
        cout << "Waehl die Map-Datei aus.";
        map_file_path = get_file_path();
        converted_map_file_path = convert_PWSTR_to_char(map_file_path);
        cout << converted_map_file_path;
        cin;

        WS.tiles = load_map(converted_map_file_path);
        int len = strlen(converted_map_file_path);
        char* tmp = new char[ len + 2 + 1];
        strcpy_s(tmp, sizeof(tmp[0]) * (len + 3), converted_map_file_path);
        tmp[len] = 'B', tmp[len + 1] = 'G', tmp[len + 2] = '\0';
        WS.background = load_map(tmp);
        start_editor(converted_sprite_sheet_path);
        }
        break;
    default:
        break;
    }

    return 0;    
}

#pragma region Gameloop Functions
void start_editor(char* sprite_sheet_path)
{
    init_editor(sprite_sheet_path);
    WS.camera.x += 250;
    WS.camera.y += 150;
    bool saveMap = false;
    while (!saveMap)
    {
        read_events();
        render_graphics();
    }
    SDL_DestroyRenderer(WS.renderer);
    SDL_DestroyWindow(WS.window);
}

void init_editor(char* sprite_sheet_path) {
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    SDL_Init(IMG_INIT_PNG);
    SDL_ShowCursor(true);
    WS.window = SDL_CreateWindow("Map Editor", 0, 20, 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    WS.renderer = SDL_CreateRenderer(WS.window, -1, SDL_RENDERER_PRESENTVSYNC);
    WS.font = TTF_OpenFont("fonts/monogram-extended.ttf", WS.FONT_SIZE);
    SDL_SetRenderDrawBlendMode(WS.renderer, SDL_BLENDMODE_BLEND);

    WS.texture = IMG_LoadTexture(WS.renderer, sprite_sheet_path);
    SDL_Texture* tmp = IMG_LoadTexture(WS.renderer, sprite_sheet_path);
    SDL_SetTextureAlphaMod(tmp, 0x7f);
    SDL_SetTextureColorMod(tmp, 0x7f, 0x7f, 0x7f);

    WS.transparent_texture = tmp;
    WS.camera = { 0,0,WS.sprite_controller.getSpriteSizeX(), WS.sprite_controller.getSpriteSizeY()};
    WS.mouse_position_to_highlight = WS.camera;

    if (WS.tiles.size() == 0)
        for (size_t i = 0; i < WS.vertical_size ; i++)
        {
            WS.tiles.push_back(std::vector<int>(WS.horizontal_size));
            std::fill(WS.tiles[i].begin(), WS.tiles[i].end(), -1);
        }
    if(WS.background.size() == 0)
        WS.background = WS.tiles;
    SDL_RenderSetLogicalSize(WS.renderer, 600, 400);

    if (WS.show_controlls)
        int msgboxID = showMessage(WS.editor_controlls, WS.editor_controlls_title);

}

void read_events() {
    SDL_PumpEvents();

    bool pressed = false;
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        switch (e.type)
        {
        case SDL_QUIT:
            exit(1);
            break;
        case SDL_KEYDOWN:
            pressed = e.type & 1;
            switch (e.key.keysym.scancode)
            {
            case SDL_SCANCODE_F1:
                showMessage(WS.editor_controlls, WS.editor_controlls_title);
                break;
            case SDL_SCANCODE_S:
                save_created_map();
                break;
            case SDL_SCANCODE_G:
                WS.grid = !WS.grid;
                break;
            case SDL_SCANCODE_SPACE:
                //set_tile_to_zero(e);
                break;
            case SDL_SCANCODE_UP:
                WS.selected_tile_set_factor += 0.25;
                //WS.camera = { WS.camera.x / WS.zoom, WS.camera.y / WS.zoom ,WS.camera.w ,WS.camera.h };
                break;
            case SDL_SCANCODE_DOWN:
                WS.selected_tile_set_factor -= 0.25;
                //WS.camera = { WS.camera.x * WS.zoom, WS.camera.y * WS.zoom ,WS.camera.w ,WS.camera.h };
                break;
            case SDL_SCANCODE_X:
                WS.delete_mode = !WS.delete_mode;
                break;
            case SDL_SCANCODE_1:
                WS.current_layer = 0; // Foreground
                break;
            case SDL_SCANCODE_2:
                WS.current_layer = 1; // Background
                break;
            case SDL_SCANCODE_3:
                WS.current_layer = 2; // All
                break;
            case SDL_SCANCODE_4:
                WS.current_layer = 3; // Enemy
                break;
            case SDL_SCANCODE_RIGHT:
                increment_current_enemy();
                break;
            case SDL_SCANCODE_LEFT:
                decrement_current_enemy();
                break;
            case SDL_SCANCODE_M:
                WS.camera = { 0,0, WS.camera.w, WS.camera.h };
                break;
            default:
                break;
            }
            break;

        case SDL_MOUSEBUTTONUP:
            switch (e.button.button)
            {
            case SDL_BUTTON_LEFT:
                break;
            case SDL_BUTTON_RIGHT:
                break;
            }
        case SDL_MOUSEBUTTONDOWN:
            switch (e.button.button)
            {
            case SDL_BUTTON_LEFT:
                mouse_button_1(e);
                
                WS.repeat_placement = !WS.repeat_placement;
                break;
            case SDL_BUTTON_RIGHT:
                WS.drag = !WS.drag;
                break;
            }
            break;
        case SDL_MOUSEWHEEL:   
            if (WS.zoom + (e.wheel.y * 0.05) < 0.55)
                return;
            WS.zoom += (e.wheel.y * 0.05);
            //cout << WS.zoom;
            break;
        case SDL_MOUSEMOTION:
            out_of_spritesheet(e);
            if (WS.drag) {
                //cout << e.motion.xrel << " " << e.motion.yrel << endl;
                WS.camera.x += e.motion.xrel;
                WS.camera.y += e.motion.yrel;
            }
            if (WS.repeat_placement) {
                mouse_button_1(e);
            }
            int xPos = (e.motion.x - WS.camera.x) / WS.sprite_controller.getSpriteSizeX();
            int yPos = (e.motion.y - WS.camera.y) / WS.sprite_controller.getSpriteSizeY();
            WS.mouse_position_to_highlight.x = xPos * WS.sprite_controller.getSpriteSizeX();
            WS.mouse_position_to_highlight.y = yPos * WS.sprite_controller.getSpriteSizeY();
            break;

        }
    }
}
void render_graphics() {
  
    SDL_SetRenderDrawColor(WS.renderer, 0, 0, 0, 255);
    SDL_RenderClear(WS.renderer);
    SDL_RenderSetScale(WS.renderer, WS.zoom, WS.zoom);

    SDL_Rect initial_cam_pos = WS.camera;
    int origin = initial_cam_pos.x;

    for (auto y : WS.background) {
        for (auto x : y) {
            if (x != -1) {
                render_texture_auto(WS.renderer, WS.sprite_controller.getSprite(x), &initial_cam_pos, Layer::BACKGROUND);
            }
            initial_cam_pos.x += WS.sprite_controller.getSpriteSizeX();
        }
        initial_cam_pos.y += WS.sprite_controller.getSpriteSizeY();
        initial_cam_pos.x = origin;
    }

    initial_cam_pos = WS.camera;
    origin = initial_cam_pos.x;

    Uint8 r = 0, g = 0, b = 0, a = 0;

    for (auto y : WS.tiles) {
        for (auto x : y) {
            if (x != -1) {
                if (x < 0) {
                    SDL_GetRenderDrawColor(WS.renderer, &r, &g, &b, &a);
                    SDL_SetRenderDrawColor(WS.renderer, 255, 127, 255, 255);
                    SDL_RenderDrawRect(WS.renderer, &initial_cam_pos);
                    SDL_Surface* enemyPosSurf = TTF_RenderText_Solid(WS.font, ('E' + to_string(-(x + 1))).c_str(), { 255, 127, 255 });
                    SDL_Texture* enemyPosBlock = SDL_CreateTextureFromSurface(WS.renderer, enemyPosSurf);
                    SDL_RenderCopy(WS.renderer, enemyPosBlock, NULL, &initial_cam_pos);
                    SDL_SetRenderDrawColor(WS.renderer, r, g, b, a);
                    SDL_FreeSurface(enemyPosSurf);
                    SDL_DestroyTexture(enemyPosBlock);
                }
                else if (x == 0) {
                    render_texture_auto(WS.renderer, WS.sprite_controller.getSprite(x), &initial_cam_pos, Layer::FOREGROUND);
                    SDL_GetRenderDrawColor(WS.renderer, &r, &g, &b, &a);
                    SDL_SetRenderDrawColor(WS.renderer, 255, 127, 127, 32);
                    SDL_RenderFillRect(WS.renderer, &initial_cam_pos);
                    SDL_RenderDrawRect(WS.renderer, &initial_cam_pos);
                    SDL_SetRenderDrawColor(WS.renderer, r, g, b, a);
                }
                else
                    render_texture_auto(WS.renderer, WS.sprite_controller.getSprite(x), &initial_cam_pos, Layer::FOREGROUND);
            }
            initial_cam_pos.x += WS.sprite_controller.getSpriteSizeX();
        }
        initial_cam_pos.y += WS.sprite_controller.getSpriteSizeY();
        initial_cam_pos.x = origin;
    }
    if (WS.grid)
    {
        SDL_SetRenderDrawColor(WS.renderer, 255, 255, 255, 30);
        SDL_BlendMode original_blend_mode = SDL_BLENDMODE_NONE;
        SDL_GetRenderDrawBlendMode(WS.renderer, &original_blend_mode);
        SDL_SetRenderDrawBlendMode(WS.renderer, SDL_BLENDMODE_BLEND);
        // Render Grid
        for (int y = 0; y <= WS.tiles.size() * WS.sprite_controller.getSpriteSizeY(); y += WS.sprite_controller.getSpriteSizeY())
        {
            if (y % (WS.sprite_controller.getSpriteSizeY() * 40) == 0)
                SDL_SetRenderDrawColor(WS.renderer, 100, 255, 255, 127);
            else if (y % (WS.sprite_controller.getSpriteSizeY() * 20) == 0)
                SDL_SetRenderDrawColor(WS.renderer, 255, 255, 127, 64);
            else 
                SDL_SetRenderDrawColor(WS.renderer, 255, 255, 255, 30);
            SDL_RenderDrawLine(WS.renderer, 0 + WS.camera.x, y + WS.camera.y, (WS.tiles[0].size() * WS.sprite_controller.getSpriteSizeX()) + WS.camera.x, y + WS.camera.y);
        }
        for (int x = 0; x <= WS.tiles[0].size() * WS.sprite_controller.getSpriteSizeX(); x += WS.sprite_controller.getSpriteSizeX())
        {
            if (x % (WS.sprite_controller.getSpriteSizeX() * 40) == 0)
                SDL_SetRenderDrawColor(WS.renderer, 100, 255, 255, 127);
            else if (x % (WS.sprite_controller.getSpriteSizeX() * 20 ) == 0)
                SDL_SetRenderDrawColor(WS.renderer, 255, 255, 127 , 64);
            else
                SDL_SetRenderDrawColor(WS.renderer, 255, 255, 255, 30);
            SDL_RenderDrawLine(WS.renderer, x + WS.camera.x, 0 + WS.camera.y, x + WS.camera.x, (WS.tiles.size() * WS.sprite_controller.getSpriteSizeY()) + WS.camera.y);
        }
        SDL_SetRenderDrawBlendMode(WS.renderer,original_blend_mode);
    }
    SDL_Rect to_place_position = WS.mouse_position_to_highlight;
    if (to_place_position.x < 0)
        to_place_position.x -= WS.sprite_controller.getSpriteSizeX();
    if (to_place_position.y < 0)
        to_place_position.y -= WS.sprite_controller.getSpriteSizeY();
    to_place_position.x += WS.camera.x;
    to_place_position.y += WS.camera.y;

    // Render highlight on Map
    if (!WS.hide_hover_position)
    if (WS.delete_mode)
        SDL_SetRenderDrawColor(WS.renderer, 255, 127, 127, 255);
    else
        if(WS.current_layer == Layer::FOREGROUND)
            SDL_SetRenderDrawColor(WS.renderer, 127, 255, 127, 127);
        else if (WS.current_layer == Layer::BACKGROUND)
            SDL_SetRenderDrawColor(WS.renderer, 127, 127, 255, 127);
        else if (WS.current_layer == Layer::PLACE_ENEMY)
            SDL_SetRenderDrawColor(WS.renderer, 255, 127, 255, 127);
        else
            SDL_SetRenderDrawColor(WS.renderer, 127, 127, 127, 127);
    SDL_RenderDrawRect(WS.renderer, &to_place_position);


    // Render Level Border
    SDL_FRect gameBorder = {
        0 + WS.camera.x,
        0 + WS.camera.y,
        WS.tiles[0].size() * WS.sprite_controller.getSpriteSizeX(),
        WS.tiles.size() * WS.sprite_controller.getSpriteSizeY()
    };
    SDL_SetRenderDrawColor(WS.renderer, (0 + WS.camera.x) % 256, (0 + WS.camera.y) % 256, 100, 255);
    SDL_RenderDrawRectF(WS.renderer, &gameBorder);
    SDL_SetRenderDrawColor(WS.renderer, 0, 0, 0, 255);

    SDL_FRect selectable_tiles = { 0.0,
                                0.0,
                                WS.sprite_controller.getSpriteSizeX() / (WS.zoom * WS.selected_tile_set_factor),
                                WS.sprite_controller.getSpriteSizeY() / (WS.zoom * WS.selected_tile_set_factor) };


    // Render selectable tileset
    for (int y = 0; y < WS.sprite_controller.getSpritesPerY(); y++)
    {
        int sprites_per_x = WS.sprite_controller.getSpritesPerX();
        for (int x = 0; x < sprites_per_x; x++)
        {
            SDL_RenderCopyF(WS.renderer, WS.texture, WS.sprite_controller.getSprite(x + (y * sprites_per_x)), &selectable_tiles);
            selectable_tiles.x += WS.sprite_controller.getSpriteSizeX() / (WS.zoom * WS.selected_tile_set_factor);
        }
        selectable_tiles.y += WS.sprite_controller.getSpriteSizeY() / (WS.zoom * WS.selected_tile_set_factor);
        selectable_tiles.x = 0;
    }

    // Render selected tile
    SDL_FRect selected_tile = {  WS.selected_tile_x * WS.sprite_controller.getSpriteSizeX() / (WS.zoom * WS.selected_tile_set_factor),
                                WS.selected_tile_y * WS.sprite_controller.getSpriteSizeY() / (WS.zoom * WS.selected_tile_set_factor),
                                WS.sprite_controller.getSpriteSizeX() / (WS.zoom * WS.selected_tile_set_factor),
                                WS.sprite_controller.getSpriteSizeY() / (WS.zoom * WS.selected_tile_set_factor) };
    SDL_SetRenderDrawColor(WS.renderer, 155, 0, 0, 255);
    
    SDL_RenderDrawRectF(WS.renderer, &selected_tile);
    SDL_FRect text_background = {
        (600 * 1.70 - WS.FONT_SIZE) / (WS.zoom) ,
        (10 / WS.zoom),
        (WS.FONT_SIZE + 48) / (WS.zoom ),
        (WS.FONT_SIZE + 48) / (WS.zoom )};
    SDL_SetRenderDrawColor(WS.renderer, 0, 0, 0, 255);
    SDL_RenderFillRectF(WS.renderer, &text_background);
    SDL_SetRenderDrawColor(WS.renderer, 255, 255, 255, 255);
    SDL_RenderDrawRectF(WS.renderer, &text_background);
    //Render Layer Text
    SDL_FRect layer_text_pos = {
        (600 * 1.73- WS.FONT_SIZE) / (WS.zoom) ,
        (10 / WS.zoom),
        (WS.FONT_SIZE * 2) / (WS.zoom), WS.FONT_SIZE * 2 / (WS.zoom) };
    switch (WS.current_layer)
    {
    case Layer::BACKGROUND:
        WS.layer_msg = TTF_RenderText_Solid(WS.font, "BG", { 127,127,255 });
        break;
    case Layer::SHOW_ALL:
        //layer_text_pos.x = (600 * 1.75 - WS.FONT_SIZE * 3) / (WS.zoom);
        WS.layer_msg = TTF_RenderText_Solid(WS.font, "ALL", { 127,127,127 });
        break;
    case Layer::PLACE_ENEMY:
        //layer_text_pos.x = (600 * 1.75 - WS.FONT_SIZE * 3) / (WS.zoom);
        WS.layer_msg = TTF_RenderText_Solid(WS.font, "PE", { 255,127,255 });
        break;
    default:
        WS.layer_msg = TTF_RenderText_Solid(WS.font, "FG", { 127,255,127 });
        break;
    }
    SDL_Texture* msgL = SDL_CreateTextureFromSurface(WS.renderer, WS.layer_msg);
    SDL_RenderCopyF(WS.renderer, msgL , NULL, &layer_text_pos);
    //Render Enemy Text
    SDL_FRect enemy_type_text_pos = {
        (600 * 1.73 - WS.FONT_SIZE) / (WS.zoom) ,
        (36 / WS.zoom),
        (WS.FONT_SIZE * 2) / (WS.zoom), WS.FONT_SIZE * 2 / (WS.zoom) };if( WS.current_layer == Layer::PLACE_ENEMY)
        WS.enemy_msg = TTF_RenderText_Solid(WS.font, ('E' + to_string(-(WS.current_enemy + 1))).c_str(), { 245,245,245 });
    else
        WS.enemy_msg = TTF_RenderText_Solid(WS.font, ('E' + to_string(-(WS.current_enemy + 1))).c_str(), { 245 / 6,245 / 6,245 / 6 });
    SDL_Texture *msgT = SDL_CreateTextureFromSurface(WS.renderer, WS.enemy_msg);
    SDL_RenderCopyF(WS.renderer, msgT, NULL, &enemy_type_text_pos);
    SDL_RenderPresent(WS.renderer);
    SDL_FreeSurface(WS.enemy_msg);
    SDL_DestroyTexture(msgT);
    SDL_FreeSurface(WS.layer_msg);
    SDL_DestroyTexture(msgL);

}

#pragma endregion

#pragma region Event Functions

void mouse_button_1(SDL_Event e) {
    SDL_MouseButtonEvent mbe = e.button;
    int xPos = mbe.x / (WS.sprite_controller.getSpriteSizeX() / (WS.zoom * WS.selected_tile_set_factor));
    int yPos = mbe.y / (WS.sprite_controller.getSpriteSizeY() / (WS.zoom * WS.selected_tile_set_factor));
    //cout << "LMB x:" << xPos << " y: " << yPos << endl;
    if (xPos < WS.sprite_controller.getSpritesPerX() && xPos >= 0 && yPos < WS.sprite_controller.getSpritesPerY() && yPos >= 0) {
        WS.selected_tile_x = xPos;
        WS.selected_tile_y = yPos;
        WS.delete_mode = false;
    }
    else {
        if (WS.current_layer == Layer::SHOW_ALL) 
            return; //Kein ausgewählter layer
        //Kamera offset mit einbeziehen beim platzieren
        xPos = (mbe.x - WS.camera.x) / WS.sprite_controller.getSpriteSizeX();
        yPos = (mbe.y - WS.camera.y) / WS.sprite_controller.getSpriteSizeY();
        //cout << "INFO: outside of selectMap" << endl;
        
        if ((xPos < 0 || WS.tiles[0].size() <= xPos) || (yPos < 0 || WS.tiles.size() <= yPos))
            return;
        vector<vector<int>>* layer = nullptr;
        switch (WS.current_layer)
        {
        case Layer::PLACE_ENEMY:
        case Layer::FOREGROUND:
            layer = &WS.tiles;
            break;
        case Layer::BACKGROUND:
            layer = &WS.background;
            break;
        }

        if (WS.delete_mode) {
            layer->at(yPos).at(xPos) = -1;
            return;
        }

        if (layer->at(yPos).at(xPos) == WS.selected_tile_x + (WS.selected_tile_y * WS.sprite_controller.getSpritesPerX()))
            return;
        if (WS.current_layer == Layer::PLACE_ENEMY) {
            layer->at(yPos).at(xPos) = WS.current_enemy;
            return;
        }
        layer->at(yPos).at(xPos) = WS.selected_tile_x + (WS.selected_tile_y * WS.sprite_controller.getSpritesPerX());
    }
}
#define MAX_ENEMY -10000
void increment_current_enemy() {
    if (WS.current_enemy == MAX_ENEMY) {

        WS.current_enemy = -2;
        return;
    }
    WS.current_enemy--;
}
void decrement_current_enemy() {
    if (WS.current_enemy == -2) {

        WS.current_enemy = MAX_ENEMY;
        return;
    }
    WS.current_enemy++;
}
#pragma endregion

#pragma region File Functions
void save_created_map()
{
    PWSTR toSaveTo = nullptr;
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (SUCCEEDED(hr)) {
        IFileDialog* pFileSave;
        hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL, IID_IFileSaveDialog, reinterpret_cast<void**>(&pFileSave));
        if (SUCCEEDED(hr)) {
            IShellItem* currfold = NULL;
            hr = pFileSave->Show(NULL);
            if (SUCCEEDED(hr)) {
                IShellItem* pItem;
                hr = pFileSave->GetResult(&pItem);
                if (SUCCEEDED(hr)) {
                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &toSaveTo);
                    pItem->Release();
                }
                else
                    return;
            }
            else
                return;
        }
        else
            return;
        pFileSave->Release();
    }
    else
        return;
    CoUninitialize();
    wprintf(toSaveTo);
    //return result;

    ofstream saved_file(toSaveTo);
    ostream_iterator<int> o_iterator(saved_file, ",");
    for (auto row : WS.tiles) {
        copy(begin(row), end(row), o_iterator);
        saved_file << '\n';
    }
    PWSTR toSaveToBG = toSaveTo;
    //wcscat_s(toSaveToBG, wcslen(toSaveTo), toSaveTo);
#pragma warning(suppress : 4996)
    wcscat(toSaveToBG, L"BG\0\0");
    ofstream saved_file_bg(toSaveToBG);
    ostream_iterator<int> o_iterator_bg(saved_file_bg, ",");
    for (auto row : WS.background) {
        copy(begin(row), end(row), o_iterator_bg);
        saved_file_bg << '\n';
    }
    saved_file.close();
    saved_file_bg.close();
    showMessage((LPWSTR)L"File was saved successfully", (LPCWSTR)L"Success Dialogue");
}

void init_sprite_controller(char* filePath, int sizeX, int sizeY)
{
    WS.sprite_controller = SpriteController::generateController(filePath, sizeX, sizeY, WS.renderer);
}

char* convert_PWSTR_to_char(PWSTR convert_from) {
    size_t i;
    char* converted_string = (char*)malloc(sizeof(char) * FILE_BUFFER_SIZE);
    wcstombs_s(&i, converted_string, (size_t)FILE_BUFFER_SIZE, convert_from, (size_t)(FILE_BUFFER_SIZE - 1));
    return converted_string;
}

PWSTR WINAPI get_file_path()
{
    PWSTR toWriteTo = nullptr;
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (SUCCEEDED(hr)) {
        IFileDialog* pFileOpen;
        hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));
        if (SUCCEEDED(hr)) {
            hr = pFileOpen->Show(NULL);
            if (SUCCEEDED(hr)) {
                IShellItem* pItem;
                hr = pFileOpen->GetResult(&pItem);
                if (SUCCEEDED(hr)) {
                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &toWriteTo);
                    pItem->Release();
                }
                else
                    return PWSTR("");
            }
            else
                return PWSTR("");
        }
        pFileOpen->Release();
    }
    else return PWSTR("");
    CoUninitialize();
    //return result;
    return toWriteTo;
}

int WINAPI showMessage(LPCWSTR msg, LPCWSTR windowTitle){
    return MessageBox(
        NULL,
        msg,
        windowTitle,
        0);
}

#pragma endregion

vector<vector<int>> load_map(const char* filePath)
{
    vector<vector<int>> mapData;
    ifstream level;
    level.open(filePath);
    string line, tile;
    if (!level.is_open())
        exit(1);
    while (level) {
        getline(level, line);
        stringstream line_stream(line);
        vector<int> tmp;
        while (getline(line_stream, tile, ',')) {
            tmp.push_back(atoi(tile.c_str()));
        }
        if (tmp.size() == 0)
            break;
        mapData.push_back(tmp);
    }
    return mapData;
}

void render_texture_auto(SDL_Renderer* renderer, SDL_Rect* sprite, SDL_Rect* position, Layer mode) {
    if (WS.current_layer == Layer::SHOW_ALL)
    {
        SDL_RenderCopy(WS.renderer, WS.texture, sprite, position);
        return;
    }
    if (WS.current_layer == Layer::PLACE_ENEMY)
    {
        SDL_RenderCopy(WS.renderer, WS.texture, sprite, position);
        return;
    }
    if (WS.current_layer != mode)
        SDL_RenderCopy(WS.renderer, WS.transparent_texture, sprite, position);
    else
        SDL_RenderCopy(WS.renderer, WS.texture, sprite, position);

}

void out_of_spritesheet(SDL_Event e) {
    SDL_MouseButtonEvent mbe = e.button;
    int xPos = mbe.x / (WS.sprite_controller.getSpriteSizeX() / (WS.zoom * WS.selected_tile_set_factor));
    int yPos = mbe.y / (WS.sprite_controller.getSpriteSizeY() / (WS.zoom * WS.selected_tile_set_factor));
    //cout << "LMB x:" << xPos << " y: " << yPos << endl;
    if (xPos < WS.sprite_controller.getSpritesPerX() && xPos >= 0 && yPos < WS.sprite_controller.getSpritesPerY() && yPos >= 0)
        WS.hide_hover_position = true;
    else
        WS.hide_hover_position = false;
}