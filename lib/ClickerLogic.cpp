#include "ClickerLogic.h"

using namespace std;
using namespace string_manipulation;

string_manipulation::Array<std::string> rows;
string_manipulation::Array<std::string> config_buffer;
string_manipulation::Array<std::string> spam_keys;
string_manipulation::Array<std::string> all_macros;
string_manipulation::Array<std::string> macro;
string_manipulation::Array<std::string> mouse_poz;
string_manipulation::Array<std::string> possitions;

//-----------------------------------------------------------//
//--- This function is a loop which executes the 'script' ---//
//--- It sends certain key inputs to the specified window ---//
//-----------------------------------------------------------//

void ClickerLogic::app()
{
    loop_in_app:
    
    //Return back to main
    if(GetAsyncKeyState(VK_CONTROL) && GetAsyncKeyState(VK_F7))
        return;

    //Waits for the user to refocus on the desired window
    if(GetForegroundWindow() != ClickerLogic::hwnd)
    {
        ClickerLogic::setCursorCoordsOnlyOnce = false;
        Sleep(10);
        goto loop_in_app;
    }

    if(!ClickerLogic::setCursorCoordsOnlyOnce)
    {
        GetWindowRect(ClickerLogic::hwnd, &(ClickerLogic::rect));
        ClickerLogic::width = ClickerLogic::rect.right - ClickerLogic::rect.left;
        ClickerLogic::height = ClickerLogic::rect.bottom - ClickerLogic::rect.top;

        SetCursorPos(ClickerLogic::rect.left + ClickerLogic::width / 2, ClickerLogic::rect.top + ClickerLogic::height / 2);
        ClickerLogic::setCursorCoordsOnlyOnce = true;
    }

    //SetWindowPos puts the window at top of the others
    SetWindowPos(ClickerLogic::hwnd, HWND_TOPMOST, 0, 0, 0, 0,
                 SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
    SetWindowPos(ClickerLogic::hwnd, HWND_NOTOPMOST, 0, 0, 0, 0,
                 SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

    //SetForegroundWindow sets the focus to the window
    SetForegroundWindow(ClickerLogic::hwnd);

    //Place the game script down here
    ClickerLogic::clicker();

    //Loop again
    Sleep(10);
    goto loop_in_app;
}

//----------------------------------------------------------------------//
//--- This function gets the handler of the currently focused window ---//
//----------------------------------------------------------------------//

void ClickerLogic::getCurrentWindowHandler()
{
    try
    {
        //Get the handler of the focused window
        ClickerLogic::hwnd = GetForegroundWindow();
        GetWindowTextA(ClickerLogic::hwnd, ClickerLogic::title, sizeof(ClickerLogic::title));

        //Get the dimensions of the focused window;
        GetWindowRect(ClickerLogic::hwnd, &(ClickerLogic::rect));
        ClickerLogic::width = ClickerLogic::rect.right - ClickerLogic::rect.left;
        ClickerLogic::height = ClickerLogic::rect.bottom - ClickerLogic::rect.top;

        ClickerLogic::lastFocusedWindow = char_to_string(ClickerLogic::title);
        ClickerLogic::checkForWindowInStore();
    }
    catch(const std::exception& e)
    {
        write("log", e.what(), "app", true);
    }
}

//------------------------------------------------------//
//--- This function prepares the key recording logic ---//
//------------------------------------------------------//

void read_keys()
{
    std::thread read_keys_thread(read_keys_callback);
}

//-------------------------------//
//--- Thread callback function---//
//-------------------------------//

auto read_keys_callback()
{

}

//----------------------------------------------//
//--- This function loads the preset feature ---//
//----------------------------------------------//

void ClickerLogic::app_preset_load()
{

}

//------------------------------------------------//
//--- This function unloads the preset feature ---//
//------------------------------------------------//

void ClickerLogic::app_preset_unload()
{

}

//-------------------------------------------//
//--- This function plays the preset file ---//
//-------------------------------------------//

void ClickerLogic::app_preset_play()
{

}

//----------------------------------------//
//--- This function records a new file ---//
//----------------------------------------//

void ClickerLogic::app_preset_record()
{

}

//--------------------------------------------------------------------//
//--- This function checks if the selected window is known already ---//
//--------------------------------------------------------------------//

void ClickerLogic::checkForWindowInStore()
{
    //coming soon

    /*

    */
}

//---------------------------------------------------------------------//
//--- Functions which will determine each action for different apps ---//
//---------------------------------------------------------------------//

void ClickerLogic::clicker()
{
    if( ClickerLogic::mouse_commands_map["mouse_coords"] == "true" && ClickerLogic::mouse_commands_map["mouse_script"] != "true" )
        if( ClickerLogic::mouse_commands_map["coords"] != "" )
            ClickerLogic::change_mouse_pos(ClickerLogic::mouse_commands_map["coords"]);

    if( ClickerLogic::mouse_commands_map["mouse_set"] != "" )
    {
        if( ClickerLogic::mouse_commands_map["mouse_script"] == "true" && ClickerLogic::mouse_commands_map["mouse_coords"] != "true")
        {
            macro.clear();
            macro = split(ClickerLogic::mouse_commands_map["mouse_set"], '=');

            if(ClickerLogic::latest_key != NULL && GetAsyncKeyState(ClickerLogic::latest_key) == 0)
                ClickerLogic::only_once = true;

            if((GetAsyncKeyState((BYTE) stoi(macro.array[0])) && ClickerLogic::only_once) || ClickerLogic::mouse_i < ClickerLogic::g_mouse_poz - 1)
            {
                ClickerLogic::only_once = false;
                ClickerLogic::latest_key = (BYTE) stoi(macro.array[0]);

                ClickerLogic::change_mouse_pos(macro.array[1]);
                Sleep(5);
            }
        }
    }

    if( ClickerLogic::keyboard_commands_map.size() != 0 )
    {
        if( ClickerLogic::keyboard_commands_map["spam_keys"] == "true")
        {
            if( ClickerLogic::keyboard_commands_map["keys"] != "" )
            {
                if( ClickerLogic::keyboard_commands_map["key_hold"] == "true" )
                {
                    spam_keys.clear();
                    spam_keys = split(ClickerLogic::keyboard_commands_map["keys"], ',');
                    
                    for(int i = 0; i < spam_keys.size; i++)
                    {
                        try
                        {
                            keybd_event((BYTE) stoi(spam_keys.array[i]), (BYTE) MapVirtualKey(stoi(spam_keys.array[i]), MAPVK_VK_TO_VSC), NULL, NULL);
                        }
                        catch(const std::exception& e)
                        {
                            write("log", e.what(), "app", true);
                            continue;
                        }
                    }
                }

                if( ClickerLogic::keyboard_commands_map["key_click"] == "true" )
                {
                    spam_keys.clear();
                    spam_keys = split(ClickerLogic::keyboard_commands_map["keys"], ',');
                    
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
                            write("log", e.what(), "app", true);
                            continue;
                        }
                    }
                }
            }
        }
    }

    if( ClickerLogic::keyboard_commands_map["define"] != "" )
    {
        if( ClickerLogic::keyboard_commands_map["macros"] == "true")
        {
            all_macros.clear();
            all_macros = split(ClickerLogic::keyboard_commands_map["define"], ';');

            for(int i = 0; i < all_macros.size; i++)
            {
                macro.clear();
                macro = split(all_macros.array[i], '=');

                if(ClickerLogic::latest_key != NULL && GetAsyncKeyState(ClickerLogic::latest_key) == 0)
                    ClickerLogic::only_once = true;

                if(GetAsyncKeyState((BYTE) stoi(macro.array[0])) && ClickerLogic::only_once)
                {
                    ClickerLogic::only_once = false;
                    ClickerLogic::latest_key = (BYTE) stoi(macro.array[0]);

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
                            write("log", e.what(), "app", true);
                            continue;
                        }
                    }
                }
            }
        }
    }

    if( ClickerLogic::mouse_commands_map.size() != 0 )
    {
        if( ClickerLogic::mouse_commands_map["mouse_hold"] == "true" )
        {
            mouse_event(ClickerLogic::map_mouse[(ClickerLogic::mouse_commands_map["mouse_left"] == "true" ? "mouse_left_down" : "mouse_absolute")], 
                        NULL, NULL, NULL, NULL);

            mouse_event(ClickerLogic::map_mouse[(ClickerLogic::mouse_commands_map["mouse_right"] == "true" ? "mouse_right_down" : "mouse_absolute")], 
                        NULL, NULL, NULL, NULL);           
        }

        if( ClickerLogic::mouse_commands_map["mouse_click"] == "true" )
        {
            mouse_event(ClickerLogic::map_mouse[(ClickerLogic::mouse_commands_map["mouse_left"] == "true" ? "mouse_left_down" : "mouse_absolute")] | 
                        ClickerLogic::map_mouse[(ClickerLogic::mouse_commands_map["mouse_left"] == "true" ? "mouse_left_up" : "mouse_absolute")], 
                        NULL, NULL, NULL, NULL);

            mouse_event(ClickerLogic::map_mouse[(ClickerLogic::mouse_commands_map["mouse_right"] == "true" ? "mouse_right_down" : "mouse_absolute")] | 
                        ClickerLogic::map_mouse[(ClickerLogic::mouse_commands_map["mouse_right"] == "true" ? "mouse_right_up" : "mouse_absolute")], 
                        NULL, NULL, NULL, NULL);
        }
    }

    Sleep(5);
}

void ClickerLogic::change_mouse_pos(string mode)
{
    GetWindowRect(ClickerLogic::hwnd, &(ClickerLogic::rect));
    ClickerLogic::width = ClickerLogic::rect.right - ClickerLogic::rect.left;
    ClickerLogic::height = ClickerLogic::rect.bottom - ClickerLogic::rect.top;

    mouse_poz.clear();
    possitions.clear();

    mouse_poz = split(mode, ';');
    possitions = split(mouse_poz.array[ClickerLogic::mouse_i], ',');

    try
    {
        if((ClickerLogic::rect.left + stoi(possitions.array[0]) < ClickerLogic::rect.right) && 
           (ClickerLogic::rect.left + stoi(possitions.array[0]) > ClickerLogic::rect.left) &&
           (ClickerLogic::rect.top + stoi(possitions.array[1]) < ClickerLogic::rect.bottom) &&
           (ClickerLogic::rect.top + stoi(possitions.array[1]) > ClickerLogic::rect.top))
            SetCursorPos(ClickerLogic::rect.left + stoi(possitions.array[0]), 
                         ClickerLogic::rect.top + stoi(possitions.array[1]));
    }
    catch(const std::exception& e)
    {
        write("log", e.what(), "app", true);
    }
    
    if(ClickerLogic::mouse_i < mouse_poz.size - 1)
        ClickerLogic::mouse_i++;
    else
        ClickerLogic::mouse_i = 0;

    ClickerLogic::g_mouse_poz = mouse_poz.size;
}

void ClickerLogic::read_configs()
{
    try
    {
        ClickerLogic::mouse_commands_map.clear();
        ClickerLogic::keyboard_commands_map.clear();
        rows.clear();

        string config_file = replace(read(ClickerLogic::CONFIG_PATH), " ", "");
        rows = split(config_file, '\n');

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
                ClickerLogic::mouse_commands_map[config_buffer.array[0]] = ( config_buffer.size > 1 ? config_buffer.array[1] : "" );

                config_buffer.clear();
            }
            else if(keyboard_check)
            {
                config_buffer = split(rows.array[i], ':');
                ClickerLogic::keyboard_commands_map[config_buffer.array[0]] = ( config_buffer.size > 1 ? config_buffer.array[1] : "" );

                config_buffer.clear();
            }
        }
    }
    catch(const std::exception& e)
    {
        write("log", e.what(), "app", true);
        close_app();
    }
}

void ClickerLogic::close_app()
{
    try
    {
        ClickerLogic::mouse_commands_map.clear();
        ClickerLogic::keyboard_commands_map.clear();
        ClickerLogic::map_mouse.clear();

        rows.clear();
        config_buffer.clear();
        spam_keys.clear();
        all_macros.clear();
        mouse_poz.clear();
        possitions.clear();
        
        exit(EXIT_SUCCESS);
    }
    catch(const std::exception& e)
    {
        write("log", e.what(), "app", true);
        exit(EXIT_FAILURE);
    }
}