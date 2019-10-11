#define CONFIG_PATH "config.cfg"

#include "strmanip.h"
#include <windows.h>
#include <map>

using namespace std;
using namespace string_manipulation;

//Initializing the global variables
char title[256];
bool clickOnlyOnce = true;
bool only_once = true;
bool setCursorCoordsOnlyOnce = false;
string lastFocusedWindow = "";
BYTE latest_key = NULL;
HWND hwnd = NULL;
RECT rect;
POINT point;
DWORD dwPID = 0;
DWORD hThread;
int width;
int height;
int mouse_i = 0;
int mouse_set_i = 0;
int mouse_pos_i = 0;
int g_mouse_poz = 0;

Array<string> rows;
Array<string> config_buffer;
Array<string> spam_keys;
Array<string> all_macros;
Array<string> macro;
Array<string> mouse_poz;
Array<string> possitions;

map<string, string> mouse_commands_map;
map<string, string> keyboard_commands_map; 
map<string, char> map_mouse = 
    {
        { "mouse_absolute", MOUSEEVENTF_ABSOLUTE },
        { "mouse_left_down",  MOUSEEVENTF_LEFTDOWN },
        { "mouse_right_down", MOUSEEVENTF_RIGHTDOWN },
        { "mouse_left_up", MOUSEEVENTF_LEFTUP },
        { "mouse_right_up", MOUSEEVENTF_RIGHTUP }
    };

//The base functions
void read_configs();
void app();
void getCurrentWindowHandler();
void getWindow(int index);
void checkForWindowInStore();
void change_mouse_pos(string mode);
void close_app();

//The game script functions
void roblox_script();

//-------------------------------------------------//
//---- A loop which only detects the key inputs ---//
//-------------------------------------------------//

int main()
{
    ShowWindow( GetConsoleWindow(), 0);
    read_configs();

    loop_in_main:

    if(GetAsyncKeyState(VK_CONTROL) 
       && GetAsyncKeyState(VK_F5) && clickOnlyOnce)
    {
        clickOnlyOnce = false;
        read_configs();
    }

    if(GetAsyncKeyState(VK_CONTROL)
       && GetAsyncKeyState(VK_F6) && lastFocusedWindow != "")
        app();

    if(GetAsyncKeyState(VK_CONTROL)
       && GetAsyncKeyState(VK_F8) && clickOnlyOnce)
    {
        clickOnlyOnce = false;
        getCurrentWindowHandler();
    }

    if(GetAsyncKeyState(VK_CONTROL)
       && GetAsyncKeyState(VK_F2) && clickOnlyOnce)
    {
        clickOnlyOnce = false;
        close_app();
    }

    if(GetAsyncKeyState(VK_F8) == 0 
       && GetAsyncKeyState(VK_F5) == 0 && GetAsyncKeyState(VK_F2) == 0)
        clickOnlyOnce = true;

    Sleep(10);

    goto loop_in_main;
}

//-----------------------------------------------------------//
//--- This function is a loop which executes the 'script' ---//
//--- It sends certain key inputs to the specified window ---//
//-----------------------------------------------------------//

void app()
{
    loop_in_app:

    //Return back to main
    if(GetAsyncKeyState(VK_CONTROL) && GetAsyncKeyState(VK_F7))
        return;

    //Waits for the user to refocus on the desired window
    if(GetForegroundWindow() != hwnd)
    {
        setCursorCoordsOnlyOnce = false;
        Sleep(10);
        goto loop_in_app;
    }

    if(!setCursorCoordsOnlyOnce)
    {
        GetWindowRect(hwnd, &rect);
        width = rect.right - rect.left;
        height = rect.bottom - rect.top;

        SetCursorPos(rect.left + width / 2, rect.top + height / 2);
        setCursorCoordsOnlyOnce = true;
    }

    //SetWindowPos puts the window at top of the others
    SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0,
                 SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
    SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0,
                 SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

    //SetForegroundWindow sets the focus to the window
    SetForegroundWindow(hwnd);

    //Place the game script down here
    roblox_script();

    //Loop again
    Sleep(10);
    goto loop_in_app;
}

//----------------------------------------------------------------------//
//--- This function gets the handler of the currently focused window ---//
//----------------------------------------------------------------------//

void getCurrentWindowHandler()
{
    //Get the handler of the focused window
    hwnd = GetForegroundWindow();
    GetWindowTextA(hwnd, title, sizeof(title));

    //Get the dimensions of the focused window;
    GetWindowRect(hwnd, &rect);
    width = rect.right - rect.left;
    height = rect.bottom - rect.top;

    lastFocusedWindow = "";

    getWindow(0);

    checkForWindowInStore();
}

//------------------------------------------------------------//
//--- This function returns the title of the given handler ---//
//------------------------------------------------------------//

void getWindow(int index)
{
    if(title[index])
        lastFocusedWindow += title[index];
    else
        return;

    return getWindow(++index);
}

//--------------------------------------------------------------------//
//--- This function checks if the selected window is known already ---//
//--------------------------------------------------------------------//

void checkForWindowInStore()
{
    //coming soon

    /*

    */
}




//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//
//--- The functions which will determine each action for different apps ---//
//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//




void roblox_script()
{
    if( mouse_commands_map["mouse_coords"] == "true" && mouse_commands_map["mouse_script"] != "true" )
        if( mouse_commands_map["coords"] != "" )
            change_mouse_pos(mouse_commands_map["coords"]);

    if( mouse_commands_map["mouse_set"] != "" )
    {
        if(mouse_commands_map["mouse_script"] == "true" && mouse_commands_map["mouse_coords"] != "true")
        {
            macro.clear();
            macro = split(mouse_commands_map["mouse_set"], '=');

            if(latest_key != NULL && GetAsyncKeyState(latest_key) == 0)
                only_once = true;

            if((GetAsyncKeyState((BYTE) stoi(macro.array[0])) && only_once) || mouse_i < g_mouse_poz - 1)
            {
                only_once = false;
                latest_key = (BYTE) stoi(macro.array[0]);

                change_mouse_pos(macro.array[1]);
                Sleep(5);
            }
        }
    }

    if( keyboard_commands_map.size() != 0 )
    {
        if(keyboard_commands_map["spam_keys"] == "true")
        {
            if( keyboard_commands_map["keys"] != "" )
            {
                if( keyboard_commands_map["key_hold"] == "true" )
                {
                    spam_keys.clear();
                    spam_keys = split(keyboard_commands_map["keys"], ',');
                    
                    for(int i = 0; i < spam_keys.size; i++)
                    {
                        try
                        {
                            keybd_event((BYTE) stoi(spam_keys.array[i]), (BYTE) MapVirtualKey(stoi(spam_keys.array[i]), MAPVK_VK_TO_VSC), NULL, NULL);
                        }
                        catch(const std::exception& e)
                        {
                            write("log", e.what() + '\n', "app");
                            continue;
                        }
                    }
                }

                if( keyboard_commands_map["key_click"] == "true" )
                {
                    spam_keys.clear();
                    spam_keys = split(keyboard_commands_map["keys"], ',');
                    
                    for(int i = 0; i < spam_keys.size; i++)
                    {
                        try
                        {
                            keybd_event((BYTE) stoi(spam_keys.array[i]), (BYTE) MapVirtualKey(stoi(spam_keys.array[i]), MAPVK_VK_TO_VSC), NULL, NULL);
                            Sleep(1);
                            keybd_event((BYTE) stoi(spam_keys.array[i]), (BYTE) MapVirtualKey(stoi(spam_keys.array[i]), MAPVK_VK_TO_VSC), KEYEVENTF_KEYUP, NULL);
                        }
                        catch(const std::exception& e)
                        {
                            write("log", e.what() + '\n', "app");
                            continue;
                        }
                    }
                }
            }
        }
    }

    if( keyboard_commands_map["define"] != "" )
    {
        if(keyboard_commands_map["macros"] == "true")
        {
            all_macros.clear();
            all_macros = split(keyboard_commands_map["define"], ';');

            for(int i = 0; i < all_macros.size; i++)
            {
                macro.clear();
                macro = split(all_macros.array[i], '=');

                if(latest_key != NULL && GetAsyncKeyState(latest_key) == 0)
                    only_once = true;

                if(GetAsyncKeyState((BYTE) stoi(macro.array[0])) && only_once)
                {
                    only_once = false;
                    latest_key = (BYTE) stoi(macro.array[0]);

                    spam_keys.clear();
                    spam_keys = split(macro.array[1], ',');

                    for(int i = 0; i < spam_keys.size; i++)
                    {
                        try
                        {
                            keybd_event((BYTE) stoi(spam_keys.array[i]), (BYTE) MapVirtualKey(stoi(spam_keys.array[i]), MAPVK_VK_TO_VSC), NULL, NULL);
                            Sleep(1);
                            keybd_event((BYTE) stoi(spam_keys.array[i]), (BYTE) MapVirtualKey(stoi(spam_keys.array[i]), MAPVK_VK_TO_VSC), KEYEVENTF_KEYUP, NULL);
                        }
                        catch(const std::exception& e)
                        {
                            write("log", e.what() + '\n', "app");
                            continue;
                        }
                    }
                }
            }
        }
    }

    if( mouse_commands_map.size() != 0 )
    {
        if( mouse_commands_map["mouse_hold"] == "true" )
        {
            mouse_event(map_mouse[(mouse_commands_map["mouse_left"] == "true" ? "mouse_left_down" : "mouse_absolute")], 
                        NULL, NULL, NULL, NULL);

            mouse_event(map_mouse[(mouse_commands_map["mouse_right"] == "true" ? "mouse_right_down" : "mouse_absolute")], 
                        NULL, NULL, NULL, NULL);
        }

        if( mouse_commands_map["mouse_click"] == "true" )
        {
            mouse_event(map_mouse[(mouse_commands_map["mouse_left"] == "true" ? "mouse_left_down" : "mouse_absolute")] | 
                        map_mouse[(mouse_commands_map["mouse_left"] == "true" ? "mouse_left_up" : "mouse_absolute")], 
                        NULL, NULL, NULL, NULL);

            mouse_event(map_mouse[(mouse_commands_map["mouse_right"] == "true" ? "mouse_right_down" : "mouse_absolute")] | 
                        map_mouse[(mouse_commands_map["mouse_right"] == "true" ? "mouse_right_up" : "mouse_absolute")], 
                        NULL, NULL, NULL, NULL);
        }
    }

    Sleep(5);
}

void change_mouse_pos(string mode)
{
    GetWindowRect(hwnd, &rect);
    width = rect.right - rect.left;
    height = rect.bottom - rect.top;

    mouse_poz.clear();
    possitions.clear();

    mouse_poz = split(mode, ';');
    possitions = split(mouse_poz.array[mouse_i], ',');

    SetCursorPos(rect.left + stoi(possitions.array[0]), rect.top + stoi(possitions.array[1]));
    
    if(mouse_i < mouse_poz.size - 1)
        mouse_i++;
    else
        mouse_i = 0;

    g_mouse_poz = mouse_poz.size;
}

void read_configs()
{
    try
    {
        mouse_commands_map.clear();
        keyboard_commands_map.clear();
        rows.clear();

        string config_file = replace(read(CONFIG_PATH), " ", "");
        rows = split(config_file, '\n');

        cout << "LOADING CONFIGS FILE ..." << endl;

        bool mouse_check = false;
        bool keyboard_check = false; 

        for(int i = 0; i < rows.size; i++)
        {
            if(grep(rows.array[i], "mouse:"))
            {
                keyboard_check = false;
                mouse_check = true;

                if(split(rows.array[i], ':').array[1] != "true")
                    mouse_check = false;

                string_manipulation::clear();
            }

            if(grep(rows.array[i], "keyboard:"))
            {
                mouse_check = false;
                keyboard_check = true;

                if(split(rows.array[i], ':').array[1] != "true")
                    keyboard_check = false;

                string_manipulation::clear();
            }

            if(mouse_check)
            {
                config_buffer = split(rows.array[i], ':');
                mouse_commands_map[config_buffer.array[0]] = ( config_buffer.size > 1 ? config_buffer.array[1] : "" );

                config_buffer.clear();
            }
            else if(keyboard_check)
            {
                config_buffer = split(rows.array[i], ':');
                keyboard_commands_map[config_buffer.array[0]] = ( config_buffer.size > 1 ? config_buffer.array[1] : "" );

                config_buffer.clear();
            }
        }
    }
    catch(const std::exception& e)
    {
        write("log", e.what() + '\n', "app");
    }
}

void close_app()
{
    try
    {
        mouse_commands_map.clear();
        keyboard_commands_map.clear();
        map_mouse.clear();

        rows.clear();
        config_buffer.clear();
        spam_keys.clear();
        all_macros.clear();
        macro.clear();
        mouse_poz.clear();
        possitions.clear();

        exit(EXIT_SUCCESS);
    }
    catch(const std::exception& e)
    {
        write("log", e.what() + '\n', "app");
    }
}
