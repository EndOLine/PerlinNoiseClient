/********************************************************************
Prog: clsWindow
Desc: Modified windows class for specific program

Log : 221118 - created

********************************************************************/

#include "clsWindow.h"


bool clsWindow::DoCommand(int wmId, int wmEvent, LPARAM lParam) {
	// Parse the menu selections:
	//
	switch (wmId) {
	case ID_FILE_SAVEAS:
		{
		char filename[80] = "";
		GetSaveFile(filename, sizeof(filename), NULL);
		}
		return true;
		break;
	case ID_FILE_EXIT:
		DestroyWindow(hWnd);
		return true;
		break;
	case ID_EDIT_PROPERTIES:
		//
		// ... copy fields to PropertyDialog
		//
		if (PropertyDialog.CreateTheDialog(hAppInst, IDD_PropDlg, hWnd) == IDOK) {
		//
		//  ... copy fields from PropertyDialog
		//
		};
		return true;
		break;
	case ID_ACTION_TIMER:
		if (MenuItemIsChecked(GetMenu(hWnd), ID_ACTION_TIMER)) {
			CheckMenuItem(GetMenu(hWnd), ID_ACTION_TIMER, MF_UNCHECKED);
			SetupTimer(0);
		} else {
			CheckMenuItem(GetMenu(hWnd), ID_ACTION_TIMER, MF_CHECKED);
			SetupTimer(1);
		};
		return true;
		break;
	}
	
#ifdef _DEBUG
	std::string Message = "Command ID is: " + std::to_string(wmId);
	MsgBoxHGO("Menu Command Selected", Message.c_str());
#endif 

	// return false if message not handled by this route
	return false;
}

bool clsWindow::DoSize(WPARAM wParam, int wmHeight, int wmWidth){
	OutputDebugString("Enter Size\n");
	CreateSwapBuffer(hWnd, wmWidth, wmHeight);
	/*if ((wParam == SIZE_MAXIMIZED)) {
		UpdateSwapBuffer2();
	};*/
	if (!WindowSizing) {
		UpdateSwapBuffer2();
	};

	//UpdateSwapBuffer();

	return true;
}

void clsWindow::DoSizeMove(UINT message){
	/*
	* WM_ENTERSIZEMOVE:
	Sent one time to a window after it enters the moving or sizing modal loop. The window enters the moving or
	sizing modal loop when the user clicks the window's title bar or sizing border, or when the window passes
	the WM_SYSCOMMAND message to the DefWindowProc function and the wParam parameter of the message specifies
	the SC_MOVE or SC_SIZE value. The operation is complete when DefWindowProc returns.

	The system sends the WM_ENTERSIZEMOVE message regardless of whether the dragging of full windows is enabled.

	WM_EXITSIZEMOVE:
	Sent one time to a window, after it has exited the moving or sizing modal loop. The window enters the moving
	or sizing modal loop when the user clicks the window's title bar or sizing border, or when the window passes
	the WM_SYSCOMMAND message to the DefWindowProc function and the wParam parameter of the message specifies
	the SC_MOVE or SC_SIZE value. The operation is complete when DefWindowProc returns.

	message is either WM_ENTERSIZEMOVE or WM_EXITSIZEMOVE
	
	*/
	
	if (message == WM_ENTERSIZEMOVE) {
		OutputDebugString("Enter SizeMove\n");
		WindowSizing = true;
	} else if (message == WM_EXITSIZEMOVE) {
		OutputDebugString("Exit SizeMove\n");
		WindowSizing = false;
		UpdateSwapBuffer2();
		InvalidateRect(hWnd, NULL, false);
	};
}

bool clsWindow::DoTimer(UINT_PTR TimerID){
	//OutputDebugString("Timer:\n");
	static int iTimeCounter = 0;
	iTimeCounter++;
	iTimeCounter = iTimeCounter % 10000;
	UpdateSwapBuffer2((double)iTimeCounter / 10000.0f);
	InvalidateRect(hWnd, NULL, false);
	return true;
}

bool clsWindow::MenuItemIsChecked(HMENU hMenu, UINT iItem) {
	MENUITEMINFO mi = { 0 };
	bool bReturn = false;
	mi.cbSize = sizeof(MENUITEMINFO);
	mi.fMask = MIIM_STATE;
	bool result = GetMenuItemInfo(hMenu, iItem, false, &mi);
	bReturn = mi.fState & MFS_CHECKED;
	return bReturn;
}

void clsWindow::CopySwapBuffer(HDC DsthDC, const int iWidth, const int iHeight){
	auto OrigBitmap = SelectObject(SwapDC, SwapBM);
	BOOL BitBltSuccess = BitBlt(DsthDC, 0, 0, iWidth,iHeight, SwapDC, 0, 0, SRCCOPY);
	DWORD error = GetLastError();
	if ((error != 0) || (!BitBltSuccess)) {
		char Message[80] = "";
		GetLastErrorMessage(error, Message, sizeof(Message));
		OutputDebugString(" Error: ");
		OutputDebugString(Message);
		OutputDebugString("\n");
	}
	SelectObject(SwapDC, OrigBitmap);
}


void clsWindow::CreateSwapBuffer(const HWND hWindow, const int iWidth, const int iHeight){
	// if previous object delete in reverse order of creation.
	DeleteSwapBuffer();

	HDC hDC = GetDC(hWindow);

	SwapDC = CreateCompatibleDC(hDC);
	SwapBM = CreateCompatibleBitmap(hDC, iWidth, iHeight);
	int iSize = iWidth * iHeight;
	if (iSize > 0) {										// dont create buffer if zero
		pSwapPixels = new DWORD[iWidth * iHeight];
	};
	//auto OrigBitmap=SelectObject(SwapDC, SwapBM);			// do this when "painting to it"

	ReleaseDC(hWnd, hDC);
}

void clsWindow::DeleteSwapBuffer(){
	if (SwapBM != NULL) {
		DeleteObject(SwapBM);
		SwapBM = NULL;
	};
	if (SwapDC != NULL) {
		DeleteDC(SwapDC);
		SwapDC = NULL;
	};
	if (pSwapPixels) {
		delete[] pSwapPixels;
		pSwapPixels = 0;
	}
}


bool clsWindow::DoPaint(HDC hdc) {
	//PaintTest(hdc);
	//OutputDebugString("Enter Paint\n");
	if (!WindowSizing) {
		BITMAP bm;
		GetObject(SwapBM, sizeof(BITMAP), &bm);
		CopySwapBuffer(hdc, bm.bmWidth, bm.bmHeight);
	};
	return true;
};


void clsWindow::GetLastErrorMessage(const DWORD LastError, char* poBuffer, int BufferSize) {
	if (BufferSize <= 0) {
		return;
	};
	DWORD CharCount;
	CharCount = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		LastError,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		poBuffer,
		BufferSize,
		NULL);
	if (CharCount == 0) {
		// I never know what to do when an error handling routine has an error.
		// at least during debug it will print a message.
		std::string Msg;
		DWORD FormatError = GetLastError();
		Msg = " REALLY!!!! another error: " + std::to_string(FormatError) + "\n";
		OutputDebugString(Msg.c_str());
	};
}

double clsWindow::Scale(double dInput, double dInputMin, double dInputMax, double dOutPutMin, double dOutputMax) {
	long double dScale = (dInput - dInputMin) / (dInputMin - dInputMax);
	return (dScale) * (dOutPutMin - dOutputMax) + dOutPutMin;
}

void clsWindow::UpdateSwapBuffer() {
	auto OrigBitmap = SelectObject(SwapDC, SwapBM);
	BITMAP bm;
	COLORREF Colour;
	GetObject(SwapBM, sizeof(BITMAP), &bm);
	
	for (int x = 0; x < bm.bmWidth; x++) {
		for (int y = 0; y < bm.bmHeight; y++) {
			double dx = (double)x / (double)bm.bmWidth;
			double dy = (double)y / (double)bm.bmHeight;
			double dz = 0.8f;
			double pn = 0.0;

			dx = (dx * 10.0f) + 0.0f;
			dy = (dy * 10.0f) + 0.0f;
			pn = ((Noise.noise(dx, dy, dz)) * 0.5) + 0.5;   // change from -1.0 -> 1.0; to 0.0 -> 1.0
			Colour = RGB((unsigned char)floor((255) * pn), (unsigned char)floor((255) * pn), (unsigned char)floor((255) * pn));
			SetPixelV(SwapDC, x, y, Colour);				// faster than SetPIxel()
		};
	};

	SelectObject(SwapDC, OrigBitmap);
}

void clsWindow::UpdatePixels(DWORD *pPixels, const int Width, const int Height,const int StartColumn, const int IncrementColumn, const double Zvalue) {
	if (!pPixels) {
		return;								// if no buffer space just exit
	};
	
	struct stLocalColour { unsigned char b, g, r, unused; };		// backwards; 32 bit for padding
	union uColour {												// to convert from structure to DWORD
		stLocalColour stColour;
		DWORD wColour;
	};

	uColour Colour{};
	for (int x = StartColumn; x < Width; x+= IncrementColumn) {
		for (int y = 0; y < Height; y++) {
			double dx = (double)x / (double)Width;
			double dy = (double)y / (double)Height;
			double dz = Zvalue;
			double pn = 0.0;
			double Scale = 10.0f;
			double Offset = 0.0f;
			dx = (dx * Scale) + Offset;
			dy = (dy * Scale) + Offset;
			dz = (dz * Scale) + Offset;
			pn = ((Noise.noise(dx, dy, dz)) * 0.5) + 0.5;   // change from -1.0 -> 1.0; to 0.0 -> 1.0
			if (pn < 0.1) {
				Colour.stColour.r = Colour.stColour.g = Colour.stColour.b = (unsigned char)floor((255) * pn);
			} else if (pn < 0.4) {
				Colour.stColour.b = (unsigned char)floor((255) * (pn + 0.4f));
				Colour.stColour.g = Colour.stColour.r = 0;
			} else if (pn < 0.9) {
				Colour.stColour.g = (unsigned char)floor((255) * pn);
				Colour.stColour.r = Colour.stColour.b = 0;
			} else {
				Colour.stColour.r = Colour.stColour.g = Colour.stColour.b = (unsigned char)floor((255) * pn);
			};

			pSwapPixels[y * Width + x] = Colour.wColour;
		};
	};
}

void clsWindow::UpdateSwapBuffer2(const double inputZ) {
	if (!pSwapPixels) {
		return;								// if no buffer space just exit
	};
	auto OrigBitmap = SelectObject(SwapDC, SwapBM);
	
	BITMAP bm{};
	GetObject(SwapBM, sizeof(BITMAP), &bm);
	
	/*
	struct stLocalColour { unsigned char b,g,r, unused; };		// backwards; 32 bit for padding
	union uColour {												// to convert from structure to DWORD
		stLocalColour stColour;
		DWORD wColour;
	};
	
	uColour Colour{};
	for (int x = 0; x < bm.bmWidth; x++) {
		for (int y = 0; y < bm.bmHeight; y++) {
			double dx = (double)x / (double)bm.bmWidth;
			double dy = (double)y / (double)bm.bmHeight;
			double dz = inputZ;
			double pn = 0.0;
			double Scale = 10.0f;
			double Offset = 0.0f;
			dx = (dx * Scale) + Offset;
			dy = (dy * Scale) + Offset;
			dz = (dz * Scale) + Offset;
			pn = ((Noise.noise(dx, dy, dz)) * 0.5) + 0.5;   // change from -1.0 -> 1.0; to 0.0 -> 1.0
			if (pn < 0.1) {
				Colour.stColour.r = Colour.stColour.g = Colour.stColour.b = (unsigned char)floor((255) * pn);
			} else if (pn < 0.4) {
				Colour.stColour.b = (unsigned char)floor((255) * (pn+0.4f));
				Colour.stColour.g = Colour.stColour.r = 0;
			} else if (pn < 0.9) {
				Colour.stColour.g = (unsigned char)floor((255) * pn);
				Colour.stColour.r = Colour.stColour.b = 0;
			} else {
				Colour.stColour.r = Colour.stColour.g = Colour.stColour.b = (unsigned char)floor((255) * pn);
			};

			pSwapPixels[y * bm.bmWidth + x] = Colour.wColour;
		};
	};
	*/
	std::thread T0(&clsWindow::UpdatePixels, this, pSwapPixels, bm.bmWidth, bm.bmHeight, 0, 4, inputZ);
	std::thread T1(&clsWindow::UpdatePixels, this, pSwapPixels, bm.bmWidth, bm.bmHeight, 1, 4, inputZ);
	std::thread T3(&clsWindow::UpdatePixels, this, pSwapPixels, bm.bmWidth, bm.bmHeight, 2, 4, inputZ);
	std::thread T4(&clsWindow::UpdatePixels, this, pSwapPixels, bm.bmWidth, bm.bmHeight, 3, 4, inputZ);
	T0.join();
	T1.join();
	T3.join();
	T4.join();
	//UpdatePixels(pSwapPixels, bm.bmWidth, bm.bmHeight, 0, 2, inputZ);
	//UpdatePixels(pSwapPixels, bm.bmWidth, bm.bmHeight, 1, 2, inputZ);

	BITMAPINFO info{ };
	info.bmiHeader.biSize = sizeof(info.bmiHeader);
	info.bmiHeader.biWidth = bm.bmWidth;
	// pay attention to the sign, you most likely want a
	// top-down pixel array as it's easier to use
	info.bmiHeader.biHeight = -bm.bmHeight;
	info.bmiHeader.biPlanes = 1;
	info.bmiHeader.biBitCount = 32;							// 32 to prevent padding at end of  scan line
	info.bmiHeader.biCompression = BI_RGB;

	// faster than SetPixel or SetPixelV
	int NbrScanLines = SetDIBits(SwapDC, SwapBM,0, bm.bmHeight, pSwapPixels,&info, DIB_RGB_COLORS);

	SelectObject(SwapDC, OrigBitmap);
}


//bool clsWindow::UpdateSwapBuffer() {
//	bool SomeThingToDraw = false;
//	// clear swap buffer
//	HBRUSH hbrush = CreateSolidBrush(RGB(Back.Red, Back.Green, Back.Blue));		// window colour -- dont forget to delete hbrush
//	RECT rect = { 0,0,PixelXsize,PixelYsize };									// set rectangle to window's dimensions
//
//	if (BackGroundDC == 0) {
//		FillRect(SwapDC, &rect, hbrush);					// fill window with colour
//		SetBkColor(SwapDC, RGB(Back.Red, Back.Green, Back.Blue));
//		SetBkMode(SwapDC, OPAQUE);
//	} else {
//		BitBlt(SwapDC, 0, 0, PixelXsize, PixelYsize, BackGroundDC, 0, 0, SRCCOPY);
//		SetBkMode(SwapDC, TRANSPARENT);
//	}
//
//
//
//	// get text dimensions
//	TEXTMETRIC tm;
//	GetTextMetrics(SwapDC, &tm);
//
//	// add new drip 
//	if (!StopRain) {
//		for (int x = 0; x < LineWidth; ++x) {
//			if (pTextMatrix[x] == 0) {
//				int temp = rng.RNG() % 1000;
//				if (temp < NewDripChance) {			// Chance in a thousand
//					pTextMatrix[x] = (rng.RNG() % ('~' - ' ')) + 1 + ' ';
//					pTextMatrixTTL[x] = StartTTL;
//				};
//			};
//		};
//	};
//
//	// update swap DC from the text matrix
//	unsigned char Temp[2] = " ";
//	for (int x = 0; x < LineWidth; ++x) {
//		for (int y = 0; y < NbrLines; ++y) {
//			if ((Index(x, y) >= BufferSize) || (Index(x, y) < 0)) {
//				break; // error
//			}
//			Temp[0] = pTextMatrix[Index(x, y)];
//			if (Temp[0] != 0) {
//				if (pTextMatrixTTL[Index(x, y)] == StartTTL) {
//					SetTextColor(SwapDC, RGB(Drop.Red, Drop.Green, Drop.Blue));
//				} else {
//					float fScale = 1.0f - ((float)pTextMatrixTTL[Index(x, y)] / (float)StartTTL);
//					stColour ScaledColour = { Tail.Red - ((Tail.Red - Back.Red) * fScale),
//											Tail.Green - ((Tail.Green - Back.Green) * fScale),
//											Tail.Blue - ((Tail.Blue - Back.Blue) * fScale) };
//					SetTextColor(SwapDC, RGB(ScaledColour.Red, ScaledColour.Green, ScaledColour.Blue));
//				};
//				TextOut(SwapDC, tm.tmMaxCharWidth * x, tm.tmHeight * y, (LPCSTR)&Temp, 1);
//				SomeThingToDraw = true;
//				--pTextMatrixTTL[Index(x, y)];
//				if (pTextMatrixTTL[Index(x, y)] == 0) {
//					pTextMatrix[Index(x, y)] = 0;
//				};
//			};
//		};
//	};
//
//	// move drip down screen
//	for (int x = 0; x < LineWidth; ++x) {
//		for (int y = NbrLines - 1; y > 0; --y) {
//			if (pTextMatrix[Index(x, y)] == 0) {
//				if (pTextMatrix[Index(x, y - 1)] != 0) {
//					pTextMatrix[Index(x, y)] = (rng.RNG() % ('~' - ' ')) + 1 + ' ';
//					pTextMatrixTTL[Index(x, y)] = StartTTL;
//				}
//			}
//		}
//	};
//	DeleteObject(hbrush);		// make sure to delete brush when not used
//	return SomeThingToDraw;
//}