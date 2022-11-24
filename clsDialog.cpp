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
			/*GetDlgItemText(hwndDlg, IDC_Width, czIDC_Width, 80);
			GetDlgItemText(hwndDlg, IDC_Height, czIDC_Height, 80);
			GetDlgItemText(hwndDlg, IDC_TTL, czIDC_TTL, 80);
			GetDlgItemText(hwndDlg, IDC_DropRed, czIDC_DropRed, 80);
			GetDlgItemText(hwndDlg, IDC_DropGreen, czIDC_DropGreen, 80);
			GetDlgItemText(hwndDlg, IDC_DropBlue, czIDC_DropBlue, 80);
			GetDlgItemText(hwndDlg, IDC_TailRed, czIDC_TailRed, 80);
			GetDlgItemText(hwndDlg, IDC_TailGreen, czIDC_TailGreen, 80);
			GetDlgItemText(hwndDlg, IDC_TailBlue, czIDC_TailBlue, 80);
			GetDlgItemText(hwndDlg, IDC_BackRed, czIDC_BackRed, 80);
			GetDlgItemText(hwndDlg, IDC_BackGreen, czIDC_BackGreen, 80);
			GetDlgItemText(hwndDlg, IDC_BackBlue, czIDC_BackBlue, 80);
			GetDlgItemText(hwndDlg, IDC_REFRESH, czIDC_Refresh, 80);
			GetDlgItemText(hwndDlg, IDC_NewDrip, czIDC_NewDrip, 80);*/
			EndDialog(hwndDlg, wParam);
			return TRUE;
		case IDCANCEL:
			DoCancel(hwndDlg);
			EndDialog(hwndDlg, wParam);
			return TRUE;
		};
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
	SetTextParameters(hwndDlg, IDC_SCALE, 5, sScale);
	SetTextParameters(hwndDlg, IDC_OFFSETx, 5, sOffsetX);
	SetTextParameters(hwndDlg, IDC_OFFSETy, 5, sOffsetY);
	SetTextParameters(hwndDlg, IDC_OFFSETz, 5, sOffsetZ);
	SetTextParameters(hwndDlg, IDC_WINDOWwidth, 4, sSizeWidth);
	SetTextParameters(hwndDlg, IDC_WINDOWheight, 4, sSizeHeight);

	// move standard buttons to the bottem of order
	SetWindowPos(GetDlgItem(hwndDlg, IDOK), HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);			// remove topmost status
	SetWindowPos(GetDlgItem(hwndDlg, IDOK), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);			// move to bottom of "topmost' items
	SetWindowPos(GetDlgItem(hwndDlg, IDCANCEL), HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);			// remove topmost status
	SetWindowPos(GetDlgItem(hwndDlg, IDCANCEL), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);			// move to bottom of "topmost' items
	//

}

void clsDialog::DoCancel(HWND hwndDlg) {

}
void clsDialog::DoOk(HWND hwndDlg) {
	// copy dialog fields to local parameters
	sRefresh=GetTextValue(hwndDlg, IDC_REFRESH);
	sScale = GetTextValue(hwndDlg, IDC_SCALE);
	sOffsetX = GetTextValue(hwndDlg, IDC_OFFSETx);
	sOffsetY = GetTextValue(hwndDlg, IDC_OFFSETy);
	sOffsetZ = GetTextValue(hwndDlg, IDC_OFFSETz);
	sSizeWidth = GetTextValue(hwndDlg, IDC_WINDOWwidth);
	sSizeHeight = GetTextValue(hwndDlg, IDC_WINDOWheight);
}

void clsDialog::SetTextParameters(const HWND hwndDlg, const int IDdlgItem, const int LimitText, const std::string& InitValue){
	HWND hItem = GetDlgItem(hwndDlg, IDdlgItem);
	SendMessage(hItem, EM_LIMITTEXT, LimitText, 0);
	SetDlgItemText(hwndDlg, IDdlgItem, InitValue.c_str());
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

