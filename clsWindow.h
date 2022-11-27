#pragma once
/********************************************************************
Prog: clsWindow
Desc: Modified windows class for specific program

Log : 221118 - created

To Do:
FPS display onscreen
write ppm stream
write ply object
do something with colours
********************************************************************/
#ifndef CLSWINDOW_H
#define CLSWINDOW_H
#define WIN32_LEAN_AND_MEAN	
#include <string>
#include <thread>
//#include <Gdiplusimaging.h>
//#include <Gdiplus.h>
#include <shobjidl.h>		// for SaveFile Dialog box
#include "..\Common\clsHGOwindow.h"
#include "resource.h"
#include "..\Common\clsPerlinNoise.h"
#include <wingdi.h>		// for SetDIBits()
#include "clsDialog.h"
#include "..\Common\clsPPM.h"

class clsWindow :public clsHGOWindow {
private:
	HDC SwapDC = 0;
	HBITMAP SwapBM = 0;
	DWORD *pSwapPixels = 0;
	clsPerlinNoise Noise;
	bool WindowSizing = false;					// is window in the middle of sizing it's self
	clsDialog PropertyDialog;
	double PerlinScale = 10.0f;
	double PerlinOffset[3] = { 0.0f };			// offset x,y,z; ie width,Height,z
	int RefreshMS = 10;
	int Frames = 10000;							// number of frames to generate
	double Zinc = 0.0001f;						// change in z value for each frame
	int iTimeCounter = 0;						// timer counter

public:
	virtual bool DoPaint(HDC hdc) override;
	virtual bool DoCommand(int wmId, int wmEvent, LPARAM lParam) override;
	virtual bool DoSize(WPARAM wParam, int wmHeight, int wmWidth) override;
	virtual void DoSizeMove(UINT message) override;
	virtual bool DoTimer(UINT_PTR TimerID) override;
	bool MenuItemIsChecked(HMENU hMenu, UINT iItem);

	void CopySwapBuffer(HDC DsthDC, const int iWidth, const int iHeight);
	void CreateSwapBuffer(const HWND hWindow, const int iWidth, const int iHeight);
	void DeleteSwapBuffer();
	void GetLastErrorMessage(const DWORD LastError, char* poBuffer, int BufferSize);
	double Scale(double dInput, double dInputMin, double dInputMax, double dOutPutMin, double dOutputMax);
	void UpdateSwapBuffer();
	void UpdatePixels(DWORD* pPixels, const int Width, const int Height, const int StartColumn, const int IncrementColumn, const double Zvalue);
	void UpdateSwapBuffer2(const double inputZ = 0.0);
	void WindowsFullScreenToggle();

	void SavePPM(HDC SwapDC, const char *Filename);
	bool GetFileSaveName(char* pFilePath, int iPathBufferSize);
	bool GetFolderSaveName(char* pFolderPath, int iPathBufferSize);
};
#endif