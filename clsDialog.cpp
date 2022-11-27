/********************************************************************
Prog: clsDialog
Desc: General dialog routine

Log :
221123 - created

********************************************************************/

#include "clsDialog.h"

LRESULT CALLBACK clsDialogProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

	// look for user data
	clsDialog* ptrclsDialogWindow = (clsDialog*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

	if (message == WM_INITDIALOG) {					// lparam is only set with "this" on this message
		ptrclsDialogWindow = (clsDialog*)lParam;	// get pointer to window from lParam
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)ptrclsDialogWindow);	// Set user data; GWL_WNDPROC might be better
	};
	// if the pointer is still null assume it was not created by clsDialog
	if (!ptrclsDialogWindow) {
		return false;
	};
	
	return ptrclsDialogWindow->DoDialogProc(hWnd, message, wParam, lParam);
};

LRESULT clsDialog::DoDialogProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam){
	switch (message) {
	case WM_INITDIALOG:
		DoInitDialog(hwndDlg);
		return true;
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
			DoOk(hwndDlg);
			EndDialog(hwndDlg, wParam);
			return TRUE;
		case IDCANCEL:
			DoCancel(hwndDlg);
			EndDialog(hwndDlg, wParam);
			return TRUE;
		};
	default:
		return DoCommand(hwndDlg, wParam, lParam);
	};
	return FALSE;
}

INT_PTR clsDialog::CreateTheDialog(const HINSTANCE hInstance, int lpTemplate, HWND hWndParent) {
	LPARAM thisone = (LPARAM)this;
	return DialogBoxParam(hInstance, (LPCTSTR)lpTemplate, hWndParent, (DLGPROC)clsDialogProc, thisone);
	//	== IDOK
};

void clsDialog::DoInitDialog(HWND hwndDlg) {
	//// limit text box sizes
	//SendMessage(GetDlgItem(hwndDlg, IDC_Width), EM_LIMITTEXT, 5, 0);
	//// initilize text boxes
	//SetDlgItemText(hwndDlg, IDC_Width, czIDC_Width);
	//// order tabs: (implement backwards;  ie. last tab first)
	//SetWindowPos(GetDlgItem(hwndDlg, IDC_NewDrip), HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	//SetWindowPos(GetDlgItem(hwndDlg, IDC_REFRESH), HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	SetTextParameters(hwndDlg, IDC_REFRESH, 5, sRefresh);
	SetTextParameters(hwndDlg, IDC_FRAMES, 5, sFrames);
	SetTextParameters(hwndDlg, IDC_ZINC, 7, sZinc);
	SetTextParameters(hwndDlg, IDC_SCALE, 5, sScale);
	SetTextParameters(hwndDlg, IDC_OFFSETx, 5, sOffsetX);
	SetTextParameters(hwndDlg, IDC_OFFSETy, 5, sOffsetY);
	SetTextParameters(hwndDlg, IDC_OFFSETz, 5, sOffsetZ);
	SetTextParameters(hwndDlg, IDC_WINDOWwidth, 4, sSizeWidth);
	SetTextParameters(hwndDlg, IDC_WINDOWheight, 4, sSizeHeight);

	// list box
	HWND hItem = GetDlgItem(hwndDlg, IDC_COLOURS);
	SetWindowPos(hItem, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);			// remove topmost status
	SetWindowPos(hItem, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);			// move to bottom of "topmost' items
	for (int i = 0; i < sColourRamp.size(); i++) {
		int pos = (int)SendMessage(hItem, LB_ADDSTRING, 0, (LPARAM)sColourRamp[i].c_str());
	};

	PositionObject(hwndDlg, IDC_REMOVE);
	PositionObject(hwndDlg, IDC_ADD);

	SetTextParameters(hwndDlg, IDC_MAXVALUE, 8, "1.000000");
	SetTextParameters(hwndDlg, IDC_STARTRED, 3, "0");
	SetTextParameters(hwndDlg, IDC_STARTGREEN, 3, "0");
	SetTextParameters(hwndDlg, IDC_STARTBLUE, 3, "0");
	SetTextParameters(hwndDlg, IDC_ENDRED, 3, "255");
	SetTextParameters(hwndDlg, IDC_ENDGREEN, 3, "255");
	SetTextParameters(hwndDlg, IDC_ENDBLUE, 3, "255");


	PositionObject(hwndDlg, IDOK);
	PositionObject(hwndDlg, IDCANCEL);

	//
	// move standard buttons to the bottem of order
	//SetWindowPos(GetDlgItem(hwndDlg, IDOK), HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);			// remove topmost status
	//SetWindowPos(GetDlgItem(hwndDlg, IDOK), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);			// move to bottom of "topmost' items
	//SetWindowPos(GetDlgItem(hwndDlg, IDCANCEL), HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);			// remove topmost status
	//SetWindowPos(GetDlgItem(hwndDlg, IDCANCEL), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);			// move to bottom of "topmost' items
	//
	//// testing list box
	//HWND hItem = GetDlgItem(hwndDlg, IDC_COLOURS);
	//int pos = (int)SendMessage(hItem, LB_ADDSTRING, 0,(LPARAM)TEXT("0.000 255,255,255 255,255,255"));
	////SendMessage(hwndList, LB_SETITEMDATA, pos, (LPARAM)i);			// set item index
	//pos = (int)SendMessage(hItem, LB_ADDSTRING, 0, (LPARAM)"Line2");
	////
	sColourRamp.clear();

}

void clsDialog::DoCancel(HWND hwndDlg) {

}
void clsDialog::DoOk(HWND hwndDlg) {
	// copy dialog fields to local parameters
	sRefresh=GetTextValue(hwndDlg, IDC_REFRESH);
	sFrames = GetTextValue(hwndDlg, IDC_FRAMES);
	sZinc = GetTextValue(hwndDlg, IDC_ZINC);
	sScale = GetTextValue(hwndDlg, IDC_SCALE);
	sOffsetX = GetTextValue(hwndDlg, IDC_OFFSETx);
	sOffsetY = GetTextValue(hwndDlg, IDC_OFFSETy);
	sOffsetZ = GetTextValue(hwndDlg, IDC_OFFSETz);
	sSizeWidth = GetTextValue(hwndDlg, IDC_WINDOWwidth);
	sSizeHeight = GetTextValue(hwndDlg, IDC_WINDOWheight);


	char Line[80] = "";
	int Count = (int)SendMessage(GetDlgItem(hwndDlg, IDC_COLOURS), LB_GETCOUNT, 0, 0);
	if (Count == LB_ERR) {
		return;
	};
	sColourRamp.clear();
	for (int i = 0; i < Count; i++) {
		SendMessage(GetDlgItem(hwndDlg, IDC_COLOURS), LB_GETTEXT, i, (LPARAM)Line);
		sColourRamp.push_back(Line);
	};

	// test list box
	// https://learn.microsoft.com/en-us/windows/win32/controls/create-a-simple-list-box
	//HWND hwndList = GetDlgItem(hDlg, IDC_LISTBOX_EXAMPLE);
	//// Get selected index.
	//int lbItem = (int)SendMessage(hwndList, LB_GETCURSEL, 0, 0);
	//// Get item data.
	//int i = (int)SendMessage(hwndList, LB_GETITEMDATA, lbItem, 0);
	//
}

bool clsDialog::DoCommand(HWND hwndDlg, WPARAM wParam, LPARAM lParam) {
	int lbItem = 0;
	HWND hwndList = 0;
	char Line[80] = "";
	

	switch (LOWORD(wParam)) {
	case IDC_REMOVE:
		hwndList = GetDlgItem(hwndDlg, IDC_COLOURS);
		lbItem = (int)SendMessage(hwndList, LB_GETCURSEL, 0, 0);
		if (lbItem != LB_ERR) {
			SendMessage(hwndList, LB_DELETESTRING, lbItem, 0);
		};
		return true;
		break;
	case IDC_ADD:
		auto MAX = GetTextValue(hwndDlg, IDC_MAXVALUE);
		RemoveNoneNumeric(MAX);
		sprintf_s(Line, sizeof(Line), "%s %s,%s,%s %s,%s,%s",
			MAX.c_str(),
			GetTextValue(hwndDlg, IDC_STARTRED).c_str(),
			GetTextValue(hwndDlg, IDC_STARTGREEN).c_str(),
			GetTextValue(hwndDlg, IDC_STARTBLUE).c_str(),
			GetTextValue(hwndDlg, IDC_ENDRED).c_str(),
			GetTextValue(hwndDlg, IDC_ENDGREEN).c_str(),
			GetTextValue(hwndDlg, IDC_ENDBLUE).c_str()
			);
		hwndList = GetDlgItem(hwndDlg, IDC_COLOURS);
		SendMessage(hwndList, LB_ADDSTRING, 0, (LPARAM)Line);
		return true;
		break;
	};

	return false;	// if not handled
}

void clsDialog::SetTextParameters(const HWND hwndDlg, const int IDdlgItem, const int LimitText, const std::string& InitValue){
	HWND hItem = GetDlgItem(hwndDlg, IDdlgItem);
	SendMessage(hItem, EM_LIMITTEXT, LimitText, 0);
	SetDlgItemText(hwndDlg, IDdlgItem, InitValue.c_str());
	SetWindowPos(hItem, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);			// remove topmost status
	SetWindowPos(hItem, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);			// move to bottom of "topmost' items
}

void clsDialog::PositionObject(const HWND hwndDlg, const int IDdlgItem){
	HWND hItem = GetDlgItem(hwndDlg, IDdlgItem);
	SetWindowPos(hItem, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);			// remove topmost status
	SetWindowPos(hItem, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);			// move to bottom of "topmost' items
}

std::string clsDialog::GetTextValue(const HWND hwndDlg, const int IDdlgItem){
	char czInput[80] = "";
	std::string Output;
	GetDlgItemText(hwndDlg, IDdlgItem, czInput, sizeof(czInput));
	Output = czInput;
	return Output;
}

// only remove trailing zeros after a '.'
// leave at least one trailing zero
void clsDialog::RemoveTrailingZeros(std::string& Value){
	size_t PeriodLocation = Value.find_first_of('.');
	if (PeriodLocation == std::string::npos) return;					// period not found
	Value.erase(Value.find_last_not_of('0') + 2, std::string::npos);	// leave one trailing zero
}

void clsDialog::RemoveNoneNumeric(std::string& Value) {
	//s.erase(std::remove_if(s.begin(), s.end(), isWhitespace), s.end());
	//[](unsigned char c) {return c == ' '; };
	//std::isdigit(c) || (c = '.') || (c='+') || (c='-')
	Value.erase(std::remove_if(Value.begin(), Value.end(), 
		[](unsigned char c) {return !(std::isdigit(c) || (c == '.') || (c == '+') || (c == '-')); }),
		Value.end());
}
