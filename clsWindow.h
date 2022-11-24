#pragma once
/********************************************************************
Prog: clsWindow
Desc: Modified windows class for specific program

Log : 221118 - created

********************************************************************/
#ifndef CLSWINDOW_H
#define CLSWINDOW_H
#define WIN32_LEAN_AND_MEAN	
#include <string>
#include <thread>
//#include <Gdiplusimaging.h>
//#include <Gdiplus.h>

#include "..\Common\clsHGOwindow.h"
#include "resource.h"
#include "..\Common\clsPerlinNoise.h"
#include <wingdi.h>		// for SetDIBits()
#include "clsDialog.h"

class clsWindow :public clsHGOWindow {
private:
	HDC SwapDC = 0;
	HBITMAP SwapBM = 0;
	DWORD *pSwapPixels = 0;
	clsPerlinNoise Noise;
	bool WindowSizing = false;					// is window in the middle of sizing it's self
	clsDialog PropertyDialog;

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
};
#endif