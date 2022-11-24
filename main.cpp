/*
* File: Main.cpp
* copied as an existing file:
* clsApp.h
* clsWindow.h
* main.h
* clsApp.cpp
* clsWindows.cpp
* main.cpp
* *.rc renamed to PerlineNoiseClient.rc
* resource.h
*/

#include "main.h"

int APIENTRY WinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE hInstPrev, _In_ PSTR cmdline, _In_ int cmdshow){
    App.Run(hInst, cmdshow);
    DWORD dw = GetLastError();
    return 0;
}