#include <iostream>
#include <stdlib.h>
#include <windows.h>
#include <vector>
#include <ShObjIdl.h>
#include <SDL.h>
#include <fstream>
#include <iterator>
#include <sstream>
#include "../../Source/engine/Utility.h"
#include "../../Source/engine/rendering/SpriteController.h"

#define FILE_BUFFER_SIZE 200

struct WorldState
{
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
    SpriteController sprite_controller;

    int selected_tile_x = 0;
    int selected_tile_y = 0;
    bool drag = false;
    bool repeat_placement = false;
    bool grid = false;
    std::vector<std::vector<int>> tiles;

    SDL_Rect camera = { 0, 0, 0, 0 };
    SDL_Rect mouse_position_to_highlight = { 0,0,0,0 };

    LPCWSTR editor_controlls =
        L"Hide controlls:\tSet WorldState.show_controlls=false\n\n"
        L"L_Click:\t\tTiles Platzieren\n"
        L"R_Click:\t\tKamera bewegen\n"
        L"S:\t\tSpeichern\n"
        L"G:\t\tGrid aktivieren/deaktivieren\n"
        L"UP/DOWN:\tZoom +/-\n";
    LPCWSTR editor_controlls_title = L"Editor Controlls";
} WS;

#pragma region GameLoop Function Declaration
void init_editor(char*);
void read_events();
void render_graphics();
#pragma endregion

#pragma region Event Function Declaration
void mouse_button_1(SDL_Event);
void set_tile_to_zero(SDL_Event);
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
char* converter_function(PWSTR);
#pragma endregion


using namespace std;
int main(int argc, char **argv)
{
    SetConsoleTitle(TEXT("BJump Map Editor: v0.9.0"));
    cout << "BJump Map Editor\n";
    cout << "Waehle das SpriteSheet aus.\n";
    PWSTR sprite_sheet_path = get_file_path();
    char* converted_sprite_sheet_path = converter_function(sprite_sheet_path);
    cout << "Datei: " << converted_sprite_sheet_path << endl << endl;
    cout << "Sprite groesse X (Adve:8): ";
    int sizeX = 0;
    cin >> sizeX;
    cout << "Sprite groesse Y (Adve:8): ";
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
        cout << "Waehl die Map-Datei aus.";
        map_file_path = get_file_path();
        converted_map_file_path = converter_function(map_file_path);
        cout << converted_map_file_path;
        cin;

        WS.tiles = load_map(converted_map_file_path);
        start_editor(converted_sprite_sheet_path);
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
    SDL_Init(IMG_INIT_PNG);
    SDL_ShowCursor(true);
    WS.window = SDL_CreateWindow("Map Editor", 0, 20, 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    WS.renderer = SDL_CreateRenderer(WS.window, -1, SDL_RENDERER_PRESENTVSYNC);
    WS.texture = IMG_LoadTexture(WS.renderer, sprite_sheet_path);
    WS.camera = { 0,0,WS.sprite_controller.getSpriteSizeX(), WS.sprite_controller.getSpriteSizeY()};
    WS.mouse_position_to_highlight = WS.camera;

    if (WS.tiles.size() == 0)
        for (size_t i = 0; i <= WS.vertical_size ; i++)
        {
            WS.tiles.push_back(std::vector<int>(WS.horizontal_size));
            std::fill(WS.tiles[i].begin(), WS.tiles[i].end(), -1);
        }
        
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
                WS.zoom += 0.25;
                //WS.camera = { WS.camera.x / WS.zoom, WS.camera.y / WS.zoom ,WS.camera.w ,WS.camera.h };
                break;
            case SDL_SCANCODE_DOWN:
                if (WS.zoom > 0.25)
                    WS.zoom -= 0.25;
                //WS.camera = { WS.camera.x * WS.zoom, WS.camera.y * WS.zoom ,WS.camera.w ,WS.camera.h };
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
            WS.selected_tile_set_factor += e.wheel.y * 0.25;
            break;
        case SDL_MOUSEMOTION:
            if (WS.drag) {
                cout << e.motion.xrel << " " << e.motion.yrel << endl;
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
    // Render Map
    for (auto y : WS.tiles) {
        for (auto x : y) {
            if (x != -1) {
                SDL_RenderCopy(WS.renderer, WS.texture, WS.sprite_controller.getSprite(x), &initial_cam_pos);
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
            SDL_RenderDrawLine(WS.renderer, 0 + WS.camera.x, y + WS.camera.y, (WS.tiles[0].size() * WS.sprite_controller.getSpriteSizeX()) + WS.camera.x, y + WS.camera.y);
        }
        for (int x = 0; x <= WS.tiles[0].size() * WS.sprite_controller.getSpriteSizeX(); x += WS.sprite_controller.getSpriteSizeX())
        {
            SDL_RenderDrawLine(WS.renderer, x + WS.camera.x, 0 + WS.camera.y, x + WS.camera.x, (WS.tiles.size() * WS.sprite_controller.getSpriteSizeY()) + WS.camera.y);
        }
        SDL_SetRenderDrawBlendMode(WS.renderer,original_blend_mode);
    }
    SDL_Rect to_place_position = WS.mouse_position_to_highlight;
    to_place_position.x += WS.camera.x;
    to_place_position.y += WS.camera.y;
    // Render highlight on Map
    SDL_SetRenderDrawColor(WS.renderer, 0, 255, 0, 127);
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

    
    SDL_RenderPresent(WS.renderer);
}

#pragma endregion

#pragma region Event Functions
void set_at_position() {

}

void mouse_button_1(SDL_Event e) {
    SDL_MouseButtonEvent mbe = e.button;
    int xPos = mbe.x / (WS.sprite_controller.getSpriteSizeX() / (WS.zoom * WS.selected_tile_set_factor));
    int yPos = mbe.y / (WS.sprite_controller.getSpriteSizeY() / (WS.zoom * WS.selected_tile_set_factor));
    cout << "LMB x:" << xPos << " y: " << yPos << endl;
    if (xPos < WS.sprite_controller.getSpritesPerX() && xPos >= 0 && yPos < WS.sprite_controller.getSpritesPerY() && yPos >= 0) {
        WS.selected_tile_x = xPos;
        WS.selected_tile_y = yPos;
    }
    else {
        //Kamera offset mit einbeziehen beim platzieren
        xPos = (mbe.x - WS.camera.x) / WS.sprite_controller.getSpriteSizeX();
        yPos = (mbe.y - WS.camera.y) / WS.sprite_controller.getSpriteSizeY();
        cout << "INFO: outside of selectMap" << endl;
        if ((xPos < 0 || WS.tiles[0].size() <= xPos) || (yPos < 0 || WS.tiles.size() <= yPos))
            return;

        if (WS.tiles.at(yPos).at(xPos) == WS.selected_tile_x + (WS.selected_tile_y * WS.sprite_controller.getSpritesPerX()))
            return;
        WS.tiles.at(yPos).at(xPos) = WS.selected_tile_x + (WS.selected_tile_y * WS.sprite_controller.getSpritesPerX());
    }
}

void set_tile_to_zero(SDL_Event e) {

}
#pragma endregion

#pragma region File Functions
void save_created_map()
{
    ofstream saved_file("./map.bjump");
    ostream_iterator<int> o_iterator(saved_file, ",");
    for (auto row : WS.tiles) {
        copy(begin(row), end(row), o_iterator);
        saved_file << '\n';
    }
    showMessage((LPCWSTR)L"File was saved successfully", (LPCWSTR)L"Success Dialogue");
}

void init_sprite_controller(char* filePath, int sizeX, int sizeY)
{
    WS.sprite_controller = SpriteController::generateController(filePath, sizeX, sizeY, WS.renderer);
}

char* converter_function(PWSTR convert_from) {
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
                
            }
        }
        pFileOpen->Release();
    }
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