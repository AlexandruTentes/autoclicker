#include "lib\\ClickerLogic.h"

//using namespace std;

//Initializing the global variables
ClickerLogic clicker;
bool clickOnlyOnce = true;

//Initializing main functions
void main_preset();
void close();
    
//-------------------------------------------------//
//---- A loop which only detects the key inputs ---//
//-------------------------------------------------//

int main()
{
    // Hide the cmd
    //ShowWindow(GetConsoleWindow(), 0);
    clicker.read_configs();

    loop_in_main:

    // Change the clicker mode from autoclicker to play/record preset
    if(GetAsyncKeyState(VK_CONTROL) && GetAsyncKeyState(VK_LSHIFT) 
       && GetAsyncKeyState(VK_F9) && clickOnlyOnce)
    {
        std::cout << "switch mode >> preset" << std::endl;
        clickOnlyOnce = false;
        clicker.app_preset_load();
        main_preset();
    }
    
    // Reload the config files
    if(GetAsyncKeyState(VK_CONTROL) && !GetAsyncKeyState(VK_LSHIFT)
    && GetAsyncKeyState(VK_F5) && clickOnlyOnce)
    {
        clickOnlyOnce = false;
        clicker.read_configs();
    }

    // Start the application
    if(GetAsyncKeyState(VK_CONTROL) && !GetAsyncKeyState(VK_LSHIFT)
    && GetAsyncKeyState(VK_F6) && clicker.lastFocusedWindow != "")
        clicker.app();

    // Save the currently focused window handler
    if(GetAsyncKeyState(VK_CONTROL) && !GetAsyncKeyState(VK_LSHIFT)
    && GetAsyncKeyState(VK_F8) && clickOnlyOnce)
    {
        clickOnlyOnce = false;
        clicker.getCurrentWindowHandler();
    }

    // Close the program
    close();

    if(!GetAsyncKeyState(VK_CONTROL))
        clickOnlyOnce = true;

    Sleep(10);    
    goto loop_in_main;
}

void main_preset()
{
    loop_in_main_preset:

    // Change the clicker mode from play/record preset to autoclicker
    if(GetAsyncKeyState(VK_CONTROL) && GetAsyncKeyState(VK_LSHIFT) 
       && GetAsyncKeyState(VK_F9) && clickOnlyOnce)
    {
        std::cout << "switch mode >> autoclicker" << std::endl;
        clickOnlyOnce = false;
        clicker.app_preset_unload();
        return;
    }

    // Change the preset mode to play
    if(GetAsyncKeyState(VK_CONTROL) && !GetAsyncKeyState(VK_LSHIFT)
    && GetAsyncKeyState(VK_F6) && clickOnlyOnce)
    {
        clickOnlyOnce = false;
        clicker.app_preset_play();
    }

    // Change the preset mode to record
    if(GetAsyncKeyState(VK_CONTROL) && !GetAsyncKeyState(VK_LSHIFT)
    && GetAsyncKeyState(VK_F8) && clickOnlyOnce)
    {
        clickOnlyOnce = false;
        clicker.app_preset_record();
    }

    close();

    if(!GetAsyncKeyState(VK_CONTROL))
        clickOnlyOnce = true;

    Sleep(10);
    goto loop_in_main_preset;
}

void close()
{
    if(GetAsyncKeyState(VK_CONTROL)
       && GetAsyncKeyState(VK_F2) && clickOnlyOnce)
    {
        clicker.close_app();
        clickOnlyOnce = false;
    }
}