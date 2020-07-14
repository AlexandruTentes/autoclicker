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

std::string mouse_pos_row = "";
std::string mouse_left_right_click_row = "";
std::string read_preset = "";
int load_i = 0;

int ClickerLogic::check_for_next_bookmark(int i)
{
    if(i >= ClickerLogic::max_size)
    {
        load_i++;
        ClickerLogic::app_preset_load();
        return 0;
    }

    return i;
}

void ClickerLogic::refocus()
{
    bool check = false;

    loop_in_app_wait:

    //Waits for the user to refocus on the desired window
    if(GetForegroundWindow() != ClickerLogic::hwnd)
    {
        check = true;
        Sleep(10);
        goto loop_in_app_wait;
    }
    
    if(check)
    {
        //SetWindowPos puts the window at top of the others
        SetWindowPos(ClickerLogic::hwnd, HWND_TOPMOST, 0, 0, 0, 0,
                    SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
        SetWindowPos(ClickerLogic::hwnd, HWND_NOTOPMOST, 0, 0, 0, 0,
                    SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

        //SetForegroundWindow sets the focus to the window
        SetForegroundWindow(ClickerLogic::hwnd);

        // Recalculate the window dimensions when refocusing
        GetClientRect(ClickerLogic::hwnd, &(ClickerLogic::rect));
        ClickerLogic::width = ClickerLogic::rect.right;
        ClickerLogic::height = ClickerLogic::rect.bottom;
        GetWindowRect(ClickerLogic::hwnd, &(ClickerLogic::rect));
    }
}

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

    ClickerLogic::refocus();

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
        GetClientRect(ClickerLogic::hwnd, &(ClickerLogic::rect));
        ClickerLogic::width = ClickerLogic::rect.right;
        ClickerLogic::height = ClickerLogic::rect.bottom;
        GetWindowRect(ClickerLogic::hwnd, &(ClickerLogic::rect));

        cout << "x: " << ClickerLogic::rect.right - ClickerLogic::rect.left << "---" << ClickerLogic::width << endl;
        cout << "y: " << ClickerLogic::rect.bottom - ClickerLogic::rect.top << "---" << ClickerLogic::height << endl;

        ClickerLogic::lastFocusedWindow = char_to_string(ClickerLogic::title);
        ClickerLogic::checkForWindowInStore();
    }
    catch(const std::exception& e)
    {
        std::string file_name(__FILE__);

        write("log", "ERROR IN FILE: " + file_name + ", CAUGHT ON LINE: " + std::to_string(__LINE__) + "\n\t\t" + e.what(), "app", true);
    }
}

//------------------------------------------------------//
//--- This function prepares the key recording logic ---//
//------------------------------------------------------//

void read_keys()
{
    //mingw_stdthread::thread read_keys_thread(read_keys_callback);
}

//-------------------------------//
//--- Thread callback function---//
//-------------------------------//

void read_keys_callback()
{

}

//----------------------------------------------//
//--- This function loads the preset feature ---//
//----------------------------------------------//

void ClickerLogic::app_preset_load()
{
    try
    {
        read_preset = read("preset\\load", "|", load_i + 1, '\n');

        if(read_preset == "") 
        {
            ClickerLogic::mouse_load[0].x = -1;
            ClickerLogic::mouse_load[0].y = -1;
            return;
        }

        string_manipulation::Array<std::string> mouze = split(read_preset, '-');
        string_manipulation::Array<std::string> mouze_poz = split(mouze.array[0], ';');
        string_manipulation::Array<std::string> mouze_click = split(mouze.array[1], ';');
        string_manipulation::Array<std::string> mouze_poz_xy;
        string_manipulation::Array<std::string> mouze_click_lr;

        for(int i = 0; i < mouze_poz.size; i++)
        {
            mouze_poz_xy = split(mouze_poz.array[i], ',');
            mouze_click_lr = split(mouze_click.array[i], ',');

            ClickerLogic::mouse_load[i].x = std::stol(mouze_poz_xy.array[0]);
            ClickerLogic::mouse_load[i].y = std::stol(mouze_poz_xy.array[1]);
            ClickerLogic::mouse_event_load_left[i] = std::stoi(mouze_click_lr.array[0]);
            ClickerLogic::mouse_event_load_right[i] = std::stoi(mouze_click_lr.array[1]);
        }

        for(int i = mouze_poz.size; i < ClickerLogic::max_size; i++)
        {
            ClickerLogic::mouse_load[i].x = -1;
            ClickerLogic::mouse_load[i].y = -1;
            ClickerLogic::mouse_event_load_left[i] = -1;
            ClickerLogic::mouse_event_load_right[i] = -1;
        }
    }
    catch(const std::exception& e)
    {
        std::string file_name(__FILE__);

        write("log", "ERROR IN FILE: " + file_name + ", CAUGHT ON LINE: " + std::to_string(__LINE__) + "\n\t\t" + e.what(), "app", true);
    }
}

//------------------------------------------------//
//--- This function unloads the preset feature ---//
//------------------------------------------------//

void ClickerLogic::app_preset_unload()
{

}

//----------------------------------------------------------------------------------//
//--- This function plays the preset file with applied data smoothing algorithms ---//
//----------------------------------------------------------------------------------//

void ClickerLogic::smoothing(std::string mode)
{
    try
    {
        int i = 0;
        ClickerLogic::mouse_smooth[0].x = ClickerLogic::mouse_load[0].x;
        ClickerLogic::mouse_smooth[0].y = ClickerLogic::mouse_load[0].y;

        loop_smoothing:

        //Return back to main
        if(GetAsyncKeyState(VK_CONTROL) && GetAsyncKeyState(VK_F7))
            return;

        ClickerLogic::refocus();

        if(ClickerLogic::mouse_load[i].x == -1) return;

        i = check_for_next_bookmark(i);

        if(mode == "untouched")
        {
            if(ClickerLogic::mouse_load[i + 1].x == -1) return;

            i = check_for_next_bookmark(i + 1) != 0 ? i : 0;

            ClickerLogic::mouse_smooth[i + 1].x = ClickerLogic::mouse_load[i + 1].x;
            ClickerLogic::mouse_smooth[i + 1].y = ClickerLogic::mouse_load[i + 1].y;

            i++;
        }
        else if(mode == "average")
        {
            if(ClickerLogic::mouse_load[i + 1].x == -1) return;

            i = check_for_next_bookmark(i + 1) != 0 ? i : 0;

            ClickerLogic::mouse_smooth[i + 1].x = (ClickerLogic::mouse_load[i].x + ClickerLogic::mouse_load[i + 1].x) / 2;
            ClickerLogic::mouse_smooth[i + 1].y = (ClickerLogic::mouse_load[i].y + ClickerLogic::mouse_load[i + 1].y) / 2;

            i++;
        }

        SetCursorPos(ClickerLogic::rect.left + (double) ((double)  ClickerLogic::mouse_smooth[i].x / ClickerLogic::mouse_accuracy) * ClickerLogic::width,
                    ClickerLogic::rect.top + (double) ((double)  ClickerLogic::mouse_smooth[i].y / ClickerLogic::mouse_accuracy) * ClickerLogic::height);

        mouse_event((ClickerLogic::mouse_event_load_left[i] == 0 ? MOUSEEVENTF_ABSOLUTE : ClickerLogic::mouse_event_load_left[i]) | 
                    (ClickerLogic::mouse_event_load_right[i] == 0 ? MOUSEEVENTF_ABSOLUTE : ClickerLogic::mouse_event_load_right[i]), 
                            NULL, NULL, NULL, NULL);

        Sleep(ClickerLogic::mouse_record_delay * 2);
        goto loop_smoothing;
    }
    catch(const std::exception& e)
    {
        std::string file_name(__FILE__);

        write("log", "ERROR IN FILE: " + file_name + ", CAUGHT ON LINE: " + std::to_string(__LINE__) + "\n\t\t" + e.what(), "app", true);
    }
}

//------------------------------------------------------//
//--- This function prepares to play the preset file ---//
//------------------------------------------------------//

void ClickerLogic::app_preset_play()
{
    // Recalculate the window dimensions when refocusing
    GetClientRect(ClickerLogic::hwnd, &(ClickerLogic::rect));
    ClickerLogic::width = ClickerLogic::rect.right;
    ClickerLogic::height = ClickerLogic::rect.bottom;
    GetWindowRect(ClickerLogic::hwnd, &(ClickerLogic::rect));

    smoothing("untouched");

    load_i = 0;

    mouse_event(MOUSEEVENTF_LEFTUP, 
                NULL, NULL, NULL, NULL);

    mouse_event(MOUSEEVENTF_RIGHTUP, 
                NULL, NULL, NULL, NULL); 
}

//----------------------------------------//
//--- This function records a new file ---//
//----------------------------------------//

void ClickerLogic::app_preset_record()
{
    try
    {
        POINT mouse;
        POINT mouse_previous;
        bool stop = false;
        
        int i = 0;
        cout << "check" << endl;
        mouse_previous.x = -1;
        mouse_previous.y = -1;

        double fullscreen_client_ratio_x = (double) (ClickerLogic::rect.right - ClickerLogic::rect.left) / ClickerLogic::width;
        double fullscreen_client_ratio_y = (double) (ClickerLogic::rect.bottom - ClickerLogic::rect.top) / ClickerLogic::height;

        // Recalculate the window dimensions when refocusing
        GetClientRect(ClickerLogic::hwnd, &(ClickerLogic::rect));
        ClickerLogic::width = ClickerLogic::rect.right;
        ClickerLogic::height = ClickerLogic::rect.bottom;
        GetWindowRect(ClickerLogic::hwnd, &(ClickerLogic::rect));

        loop_in_app_preset_record:

        //Return back to main
        if(GetAsyncKeyState(VK_CONTROL) && GetAsyncKeyState(VK_F7))
        {
            write("preset\\load", "|\n" +  mouse_pos_row + "-" + mouse_left_right_click_row, "app", true);
            mouse_pos_row = "";
            mouse_left_right_click_row = "";
            return;
        }

        if(i >= ClickerLogic::max_size)
        {
            write("preset\\load", "|\n" +  mouse_pos_row + "-" + mouse_left_right_click_row, "app", true);
            mouse_pos_row = "";
            mouse_left_right_click_row = "";
            i = 0;
        }

        ClickerLogic::refocus();

        if(ClickerLogic::hwnd == NULL) return;

        GetCursorPos(&mouse);
        ScreenToClient(ClickerLogic::hwnd, &mouse);
        
        if(mouse.x >= 0 && mouse.x <= ClickerLogic::width &&
           mouse.y >= 0 && mouse.y <= ClickerLogic::height)
        {
            mouse_previous.x = mouse.x;
            mouse_previous.y = mouse.y;

            ClickerLogic::mouse_mod[i].x = (int) ((double) ((double) (mouse.x * fullscreen_client_ratio_x) / ClickerLogic::width) * ClickerLogic::mouse_accuracy);
            ClickerLogic::mouse_mod[i].y = (int) ((double) ((double) (mouse.y * fullscreen_client_ratio_y) / ClickerLogic::height) * ClickerLogic::mouse_accuracy);

            cout << ClickerLogic::mouse_mod[i].x << " --- " << ClickerLogic::mouse_mod[i].y << " size: " << i << endl;

            if (GetAsyncKeyState(VK_LBUTTON) < 0)
                ClickerLogic::mouse_event_left[i] = MOUSEEVENTF_LEFTDOWN;
            else if( i > 0 && (ClickerLogic::mouse_event_left[i - 1] == MOUSEEVENTF_LEFTUP || ClickerLogic::mouse_event_left[i - 1] == 0))
                ClickerLogic::mouse_event_left[i] = 0;
            else
                ClickerLogic::mouse_event_left[i] = MOUSEEVENTF_LEFTUP;

            if (GetAsyncKeyState(VK_RBUTTON) < 0)
                ClickerLogic::mouse_event_right[i] = MOUSEEVENTF_RIGHTDOWN;
            else if(i > 0 && (ClickerLogic::mouse_event_right[i - 1] == MOUSEEVENTF_RIGHTUP || ClickerLogic::mouse_event_right[i - 1] == 0))
                ClickerLogic::mouse_event_right[i] = 0;
            else
                ClickerLogic::mouse_event_right[i] = MOUSEEVENTF_RIGHTUP;

            mouse_pos_row += std::to_string(ClickerLogic::mouse_mod[i].x) + "," + std::to_string(ClickerLogic::mouse_mod[i].y) + ";";
            mouse_left_right_click_row += std::to_string(ClickerLogic::mouse_event_left[i]) + "," + std::to_string(ClickerLogic::mouse_event_right[i]) + ";";

            i++;
        }

        ClickerLogic::mouse_mod_size = i;

        Sleep(ClickerLogic::mouse_record_delay);
        goto loop_in_app_preset_record;
    }
    catch(const std::exception& e)
    {
        std::string file_name(__FILE__);

        write("log", "ERROR IN FILE: " + file_name + ", CAUGHT ON LINE: " + std::to_string(__LINE__) + "\n\t\t" + e.what(), "app", true);
    }
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
                            std::string file_name(__FILE__);

        write("log", "ERROR IN FILE: " + file_name + ", CAUGHT ON LINE: " + std::to_string(__LINE__) + "\n\t\t" + e.what(), "app", true);
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
                            std::string file_name(__FILE__);

        write("log", "ERROR IN FILE: " + file_name + ", CAUGHT ON LINE: " + std::to_string(__LINE__) + "\n\t\t" + e.what(), "app", true);
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
                            std::string file_name(__FILE__);

        write("log", "ERROR IN FILE: " + file_name + ", CAUGHT ON LINE: " + std::to_string(__LINE__) + "\n\t\t" + e.what(), "app", true);
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
        std::string file_name(__FILE__);

        write("log", "ERROR IN FILE: " + file_name + ", CAUGHT ON LINE: " + std::to_string(__LINE__) + "\n\t\t" + e.what(), "app", true);
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
        std::string file_name(__FILE__);

        write("log", "ERROR IN FILE: " + file_name + ", CAUGHT ON LINE: " + std::to_string(__LINE__) + "\n\t\t" + e.what(), "app", true);
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
        std::string file_name(__FILE__);

        write("log", "ERROR IN FILE: " + file_name + ", CAUGHT ON LINE: " + std::to_string(__LINE__) + "\n\t\t" + e.what(), "app", true);
        exit(EXIT_FAILURE);
    }
}