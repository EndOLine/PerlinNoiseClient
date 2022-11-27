/********************************************************************
Prog: clsWindow
Desc: Modified windows class for specific program

Log : 221118 - created

********************************************************************/

#include "clsWindow.h"

clsWindow::clsWindow() {
	//stColourRamp R = { 0.1f, {0,0,0},{255,255,255} };
	//ColourRamp.push_back({ 0.1f, {0,0,0},{255,255,255} });

	ColourRamp.push_back({ 0.1f, {0,0,0},{25,25,25} });
	ColourRamp.push_back({ 0.4f, {0,0,102},{0,0,204} });
	ColourRamp.push_back({ 0.9f, {0,150,0},{0,200,0} });
	ColourRamp.push_back({ 1.0f, {230,230,230},{255,255,255} });
	std::sort(ColourRamp.begin(), ColourRamp.end(), [](stColourRamp a, stColourRamp b) {return a.MaxValue < b.MaxValue; });
}

bool clsWindow::DoCommand(int wmId, int wmEvent, LPARAM lParam) {
	// Parse the menu selections:
	//
	BITMAP bm = {};
	char filename[80] = "";
	char foldername[80] = "";
	char Line[80] = "";

	switch (wmId) {
	case ID_FILE_SAVEAS:
	{
		if (GetFileSaveName(filename, sizeof(filename))) {
		//if (GetSaveFile(filename, sizeof(filename), "Portable Pixmap Format\0*.ppm\0All\0*.*\0")) {
			SavePPM(SwapDC, filename);
		};
	}
	return true;
	break;
	case ID_FILE_SAVESERIES:
		if (GetFolderSaveName(foldername, sizeof(foldername))) {
			for (int i = 0; i < Frames; i++) {
				UpdateSwapBuffer2((double)i * Zinc);
				sprintf_s(filename, sizeof(filename), "%s\\%s%04i.ppm", foldername, "Image", i);
				SavePPM(SwapDC, filename);
			};
		};
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
		PropertyDialog.sRefresh = std::to_string(RefreshMS);
		PropertyDialog.sFrames = std::to_string(Frames);
		PropertyDialog.RemoveTrailingZeros(PropertyDialog.sZinc = std::to_string(Zinc));
		PropertyDialog.RemoveTrailingZeros(PropertyDialog.sScale = std::to_string(PerlinScale));
		PropertyDialog.RemoveTrailingZeros(PropertyDialog.sOffsetX = std::to_string(PerlinOffset[0]));
		PropertyDialog.RemoveTrailingZeros(PropertyDialog.sOffsetY = std::to_string(PerlinOffset[1]));
		PropertyDialog.RemoveTrailingZeros(PropertyDialog.sOffsetZ = std::to_string(PerlinOffset[2]));
		GetObject(SwapBM, sizeof(BITMAP), &bm);
		PropertyDialog.sSizeWidth = std::to_string(bm.bmWidth);
		PropertyDialog.sSizeHeight = std::to_string(bm.bmHeight);
		PropertyDialog.sColourRamp.clear();
		for (int i = 0; i < ColourRamp.size(); i++) {
			sprintf_s(Line, sizeof(Line), "%f %i,%i,%i %i,%i,%i", ColourRamp[i].MaxValue,
				ColourRamp[i].StartColour.r, ColourRamp[i].StartColour.g, ColourRamp[i].StartColour.b,
				ColourRamp[i].EndColour.r, ColourRamp[i].EndColour.g, ColourRamp[i].EndColour.b);
			PropertyDialog.sColourRamp.push_back(Line);
		};
		if (PropertyDialog.CreateTheDialog(hAppInst, IDD_PropDlg, hWnd) == IDOK) {
			//
			//  ... copy fields from PropertyDialog
			//
			RefreshMS = std::stoi(PropertyDialog.sRefresh);
			Frames = std::stoi(PropertyDialog.sFrames);
			Zinc = std::stod(PropertyDialog.sZinc);
			PerlinScale = std::stod(PropertyDialog.sScale);
			PerlinOffset[0] = std::stod(PropertyDialog.sOffsetX);
			PerlinOffset[1] = std::stod(PropertyDialog.sOffsetY);
			PerlinOffset[2] = std::stod(PropertyDialog.sOffsetZ);

			ColourRamp.clear();
			for (int i = 0; i < PropertyDialog.sColourRamp.size(); i++) {
				stColourRamp Hold;
				int sr, sg, sb, er, eg, eb;
				float m;
				sscanf_s(PropertyDialog.sColourRamp[i].c_str(), "%f %d,%d,%d %d,%d,%d",
					&m, &sr, &sg, &sb,
					&er, &eg, &eb);
				Hold.MaxValue = m;
				Hold.StartColour.r = sr; Hold.StartColour.g = sg; Hold.StartColour.b = sb;
				Hold.EndColour.r = er; Hold.EndColour.g = eg; Hold.EndColour.b = eb;
				if (Hold.MaxValue < 0.0f) Hold.MaxValue = 0.0f;
				if (Hold.MaxValue > 1.0f) Hold.MaxValue = 1.0f;
				ColourRamp.push_back(Hold);
			};
			std::sort(ColourRamp.begin(), ColourRamp.end(), [](stColourRamp a, stColourRamp b) {return a.MaxValue < b.MaxValue; });

			//

			iTimeCounter = 0;
			SetupTimer(0);		// stop timer
			if (MenuItemIsChecked(GetMenu(hWnd), ID_ACTION_TIMER)) {		// restart timer
				SetupTimer(RefreshMS);
			} else {														// or update screen
				UpdateSwapBuffer2();
				InvalidateRect(hWnd, NULL, false);
			};
			int W = std::stoi(PropertyDialog.sSizeWidth);
			int H = std::stoi(PropertyDialog.sSizeHeight);
			if ((W > 0) && (H > 0)) {
				SetClientSize(W, H);
				//InvalidateRect(hWnd, NULL, FALSE);
			};
		};
		return true;
		break;
	case ID_ACTION_TIMER:
		if (MenuItemIsChecked(GetMenu(hWnd), ID_ACTION_TIMER)) {
			CheckMenuItem(GetMenu(hWnd), ID_ACTION_TIMER, MF_UNCHECKED);
			SetupTimer(0);
		} else {
			CheckMenuItem(GetMenu(hWnd), ID_ACTION_TIMER, MF_CHECKED);
			SetupTimer(RefreshMS);
		};
		return true;
		break;
	case ID_ACTION_FULLSCREEN:
		WindowsFullScreenToggle();
		return true;
		break;
	};
	
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
	static bool GoForward = true;
	if (GoForward) {
		iTimeCounter++;
		if (iTimeCounter > Frames) {
			GoForward = false;
			iTimeCounter--;
		};
	} else {
		iTimeCounter--;
		if (iTimeCounter < 0) {
			GoForward = true;
			iTimeCounter++;
		};
	};
	//iTimeCounter = iTimeCounter % 10000;
	UpdateSwapBuffer2((double)iTimeCounter * Zinc);
	//UpdateSwapBuffer2((double)iTimeCounter / 10000.0f);
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

double clsWindow::ScaleValue(double dInput, double dInputMin, double dInputMax, double dOutPutMin, double dOutputMax) {
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

void clsWindow::SavePPM(HDC SwapDC, const char* Filename){
	COLORREF Colour;
	clsPPM::stColour OutColour;
	auto OrigBitmap = SelectObject(SwapDC, SwapBM);
	BITMAP bm = {};
	GetObject(SwapBM, sizeof(BITMAP), &bm);
	clsPPM ppm(bm.bmHeight, bm.bmWidth);
	for (int x = 0; x < bm.bmWidth; x++) {
		for (int y = 0; y < bm.bmHeight; y++) {
			Colour = GetPixel(SwapDC, x, y);
			OutColour = { GetRValue(Colour), GetGValue(Colour), GetBValue(Colour) };
			ppm.GetPixel(y, x) = OutColour;
		};
	};
	ppm.SaveP6(Filename);
	SelectObject(SwapDC, OrigBitmap);
}

void clsWindow::UpdatePixels(DWORD *pPixels, const int Width, const int Height,const int StartColumn, const int IncrementColumn, const double Zvalue) {
	if (!pPixels) {
		return;								// if no buffer space just exit
	};
	
	struct stLocalColour { unsigned char b, g, r, unused; };		// backwards; 32 bit for padding
	union uColour {													// to convert from structure to DWORD
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
			double Scale = PerlinScale;
			//double Offset = PerlinOffset;
			dx = (dx * Scale) + PerlinOffset[0];
			dy = (dy * Scale) + PerlinOffset[1];
			dz = (dz * Scale) + PerlinOffset[2];
			pn = ((Noise.noise(dx, dy, dz)) * 0.5) + 0.5;   // change from -1.0 -> 1.0; to 0.0 -> 1.0


			Colour.stColour = { 0,0,0 };
			for (int i = 0; i < ColourRamp.size(); i++) {
				if (pn < ColourRamp[i].MaxValue) {
					//(i>0?ColourRamp[i-1].MaxValue:0.0f)
					//double bScaled = ScaleValue(pn, 0.0f, ColourRamp[i].MaxValue, ColourRamp[i].StartColour.r, ColourRamp[i].EndColour.r);
					Colour.stColour.r = floor(ScaleValue(pn, (i > 0 ? ColourRamp[i - 1].MaxValue : 0.0f), ColourRamp[i].MaxValue, ColourRamp[i].StartColour.r, ColourRamp[i].EndColour.r));
					Colour.stColour.g = floor(ScaleValue(pn, (i > 0 ? ColourRamp[i - 1].MaxValue : 0.0f), ColourRamp[i].MaxValue, ColourRamp[i].StartColour.g, ColourRamp[i].EndColour.g));
					Colour.stColour.b = floor(ScaleValue(pn, (i > 0 ? ColourRamp[i - 1].MaxValue : 0.0f), ColourRamp[i].MaxValue, ColourRamp[i].StartColour.b, ColourRamp[i].EndColour.b));
					break;
				};
			};
			/*if (pn < 0.1) {
				Colour.stColour.r = Colour.stColour.g = Colour.stColour.b = (unsigned char)floor((255) * pn);
			} else if (pn < 0.4) {
				Colour.stColour.b = (unsigned char)floor((255) * (pn + 0.4f));
				Colour.stColour.g = Colour.stColour.r = 0;
			} else if (pn < 0.9) {
				Colour.stColour.g = (unsigned char)floor((255) * pn);
				Colour.stColour.r = Colour.stColour.b = 0;
			} else {
				Colour.stColour.r = Colour.stColour.g = Colour.stColour.b = (unsigned char)floor((255) * pn);
			};*/

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
	
	
	std::thread T0(&clsWindow::UpdatePixels, this, pSwapPixels, bm.bmWidth, bm.bmHeight, 0, 4, inputZ);
	std::thread T1(&clsWindow::UpdatePixels, this, pSwapPixels, bm.bmWidth, bm.bmHeight, 1, 4, inputZ);
	std::thread T3(&clsWindow::UpdatePixels, this, pSwapPixels, bm.bmWidth, bm.bmHeight, 2, 4, inputZ);
	std::thread T4(&clsWindow::UpdatePixels, this, pSwapPixels, bm.bmWidth, bm.bmHeight, 3, 4, inputZ);
	T0.join();
	T1.join();
	T3.join();
	T4.join();


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

void clsWindow::WindowsFullScreenToggle() {
	// from https://devblogs.microsoft.com/oldnewthing/20100412-00/?p=14353
	static WINDOWPLACEMENT g_wpPrev = { sizeof(g_wpPrev) };
	static HMENU hMenu = {};
	DWORD dwStyle = GetWindowLong(hWnd, GWL_STYLE);
	if (dwStyle & WS_OVERLAPPEDWINDOW) {
		MONITORINFO mi = { sizeof(mi) };
		if (GetWindowPlacement(hWnd, &g_wpPrev) &&
			GetMonitorInfo(MonitorFromWindow(hWnd,
				MONITOR_DEFAULTTOPRIMARY), &mi)) {
			hMenu = GetMenu(hWnd);
			SetWindowLong(hWnd, GWL_STYLE,
				dwStyle & ~WS_OVERLAPPEDWINDOW);
			SetWindowPos(hWnd, HWND_TOP,
				mi.rcMonitor.left, mi.rcMonitor.top,
				mi.rcMonitor.right - mi.rcMonitor.left,
				mi.rcMonitor.bottom - mi.rcMonitor.top,
				SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
			SetMenu(hWnd, NULL);
		}
	} else {
		SetWindowLong(hWnd, GWL_STYLE,
			dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(hWnd, &g_wpPrev);
		SetWindowPos(hWnd, NULL, 0, 0, 0, 0,
			SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
			SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
		SetMenu(hWnd, hMenu);
	};
}

// example from "https://docs.microsoft.com/en-us/windows/win32/learnwin32/example--the-open-dialog-box"
//				"https://docs.microsoft.com/en-us/windows/win32/shell/common-file-dialog#ifiledialog-ifileopendialog-and-ifilesavedialog"
bool clsWindow::GetFileSaveName(char* pFilePath, int iPathBufferSize) {
	bool bPathFound = false;

	COMDLG_FILTERSPEC FileTypes[] = {
			{ L"Portable Pixmap Format", L"*.ppm" },
			{ L"All files", L"*.*" }
	};

	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
		COINIT_DISABLE_OLE1DDE);
	if (SUCCEEDED(hr))
	{
		IFileSaveDialog* pFileSave;
		// create the FileSave	CLSID_FileSaveDialog  IID_IFileSaveDialog
		hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL,
			IID_IFileSaveDialog, reinterpret_cast<void**>(&pFileSave));
	
		pFileSave->SetFileTypes(_countof(FileTypes), FileTypes);	// allow selection of these file types
		pFileSave->SetDefaultExtension(L"ppm");						// set default extension on return. can be multiple seperated by ; L"doc;docx"

		if (SUCCEEDED(hr))
		{
			// Show the Open dialog box.
			hr = pFileSave->Show(hWnd);								// setting handle to owner blocks use of menu. Timer still works.

			// Get the file name from the dialog box.
			if (SUCCEEDED(hr))
			{
				IShellItem* pItem;
				hr = pFileSave->GetResult(&pItem);
				if (SUCCEEDED(hr))
				{
					PWSTR pszFilePath;

					hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

					// Display the file name to the user.
					if (SUCCEEDED(hr))
					{
						size_t i;
						wcstombs_s(&i,pFilePath, iPathBufferSize, pszFilePath, iPathBufferSize-1);
						bPathFound = true;
						CoTaskMemFree(pszFilePath);
					}
					pItem->Release();
				}
			}
			pFileSave->Release();
		}
		CoUninitialize();
	}
	return bPathFound;
}

bool clsWindow::GetFolderSaveName(char* pFolderPath, int iPathBufferSize) {
	bool bPathFound = false;

	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
		COINIT_DISABLE_OLE1DDE);
	if (SUCCEEDED(hr))
	{
		IFileOpenDialog* pFileOpen = 0;

		// create the FileSave	CLSID_FileSaveDialog  IID_IFileSaveDialog
		hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
			IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));
		pFileOpen->SetOptions(FOS_PICKFOLDERS | FOS_PATHMUSTEXIST); // for picking folders only
		if (SUCCEEDED(hr))
		{
			// Show the Open dialog box.
			hr = pFileOpen->Show(hWnd);								// setting handle to owner blocks use of menu. Timer still works.

			// Get the file name from the dialog box.
			if (SUCCEEDED(hr))
			{
				IShellItem* pItem;
				hr = pFileOpen->GetResult(&pItem);
				if (SUCCEEDED(hr))
				{
					PWSTR pszFolderPath;

					hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFolderPath);

					// Display the file name to the user.
					if (SUCCEEDED(hr))
					{
						//std::wcstombs(pFolderPath, pszFolderPath, iPathBufferSize);
						size_t i;
						wcstombs_s(&i, pFolderPath, iPathBufferSize, pszFolderPath, iPathBufferSize - 1);
						bPathFound = true;
						CoTaskMemFree(pszFolderPath);
					}
					pItem->Release();
				}
			}
			pFileOpen->Release();
		}
		CoUninitialize();
	}
	return bPathFound;
}


