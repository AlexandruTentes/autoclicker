#include "lib\\ClickerLogic.h"

//using namespace std;

//Initializing the global variables
ClickerLogic clicker;
bool clickOnlyOnce = true;
    

//-------------------------------------------------//
//---- A loop which only detects the key inputs ---//
//-------------------------------------------------//


int main()
{
    // Hide the cmd
    //ShowWindow(GetConsoleWindow(), 0);
    clicker.read_configs();

    loop_in_main:

    // Reload the config files
    if(GetAsyncKeyState(VK_CONTROL) 
       && GetAsyncKeyState(VK_F5) && clickOnlyOnce)
    {
        clickOnlyOnce = false;
        clicker.read_configs();
    }

    // Start the application
    if(GetAsyncKeyState(VK_CONTROL)
       && GetAsyncKeyState(VK_F6) && clicker.lastFocusedWindow != "")
        clicker.app();

    // Save the currently focused window handler
    if(GetAsyncKeyState(VK_CONTROL)
       && GetAsyncKeyState(VK_F8) && clickOnlyOnce)
    {
        clickOnlyOnce = false;
        clicker.getCurrentWindowHandler();
    }

    // Close the program
    if(GetAsyncKeyState(VK_CONTROL)
       && GetAsyncKeyState(VK_F2) && clickOnlyOnce)
    {
        clickOnlyOnce = false;
        clicker.close_app();
    }

    if(GetAsyncKeyState(VK_F8) == 0 
       && GetAsyncKeyState(VK_F5) == 0 && GetAsyncKeyState(VK_F2) == 0)
        clickOnlyOnce = true;

    Sleep(10);
    
    goto loop_in_main;
}