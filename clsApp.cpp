/********************************************************************
Prog: clsApp
Desc: Modified application class for specific program

Log : 221118 - created

********************************************************************/

#include "clsApp.h"


bool clsApp::DoInitilize() {
	SetWindowClass("Perlin Noise Client Class Name");
	SetWindowTitle("Perlin Noise Client Title");
	SetAcceleratorID(IDR_MAINACCELERATOR);
	return true;
};
bool clsApp::DoBegin(int nCmdShow) {
	Window.CreateTheWindow(this, "Perlin Noise Client Window", IDR_MAINMENU);
	Window.SetClientSize(500, 500);
	Window.ShowTheWindow(nCmdShow);
	return true;
};
bool clsApp::DoRunning() {
	return true;
}
void clsApp::DoEnd() {
};