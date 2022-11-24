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

LRESULT clsDialog::DoDialogProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
	switch (message) {
	case WM_INITDIALOG:
		DoInitDialog(hWnd);
		//// limit text box sizes
		//SendMessage(GetDlgItem(hwndDlg, IDC_Width), EM_LIMITTEXT, 5, 0);
		//SendMessage(GetDlgItem(hwndDlg, IDC_Height), EM_LIMITTEXT, 5, 0);
		//SendMessage(GetDlgItem(hwndDlg, IDC_TTL), EM_LIMITTEXT, 5, 0);
		//SendMessage(GetDlgItem(hwndDlg, IDC_DropRed), EM_LIMITTEXT, 3, 0);
		//SendMessage(GetDlgItem(hwndDlg, IDC_DropGreen), EM_LIMITTEXT, 3, 0);
		//SendMessage(GetDlgItem(hwndDlg, IDC_DropBlue), EM_LIMITTEXT, 3, 0);
		//SendMessage(GetDlgItem(hwndDlg, IDC_TailRed), EM_LIMITTEXT, 3, 0);
		//SendMessage(GetDlgItem(hwndDlg, IDC_TailGreen), EM_LIMITTEXT, 3, 0);
		//SendMessage(GetDlgItem(hwndDlg, IDC_TailBlue), EM_LIMITTEXT, 3, 0);
		//SendMessage(GetDlgItem(hwndDlg, IDC_BackRed), EM_LIMITTEXT, 3, 0);
		//SendMessage(GetDlgItem(hwndDlg, IDC_BackGreen), EM_LIMITTEXT, 3, 0);
		//SendMessage(GetDlgItem(hwndDlg, IDC_BackBlue), EM_LIMITTEXT, 3, 0);
		//SendMessage(GetDlgItem(hwndDlg, IDC_REFRESH), EM_LIMITTEXT, 4, 0);
		//SendMessage(GetDlgItem(hwndDlg, IDC_NewDrip), EM_LIMITTEXT, 4, 0);
		//// initilize text boxes
		//SetDlgItemText(hwndDlg, IDC_Width, czIDC_Width);
		//SetDlgItemText(hwndDlg, IDC_Height, czIDC_Height);
		//SetDlgItemText(hwndDlg, IDC_TTL, czIDC_TTL);
		//SetDlgItemText(hwndDlg, IDC_DropRed, czIDC_DropRed);
		//SetDlgItemText(hwndDlg, IDC_DropGreen, czIDC_DropGreen);
		//SetDlgItemText(hwndDlg, IDC_DropBlue, czIDC_DropBlue);
		//SetDlgItemText(hwndDlg, IDC_TailRed, czIDC_TailRed);
		//SetDlgItemText(hwndDlg, IDC_TailGreen, czIDC_TailGreen);
		//SetDlgItemText(hwndDlg, IDC_TailBlue, czIDC_TailBlue);
		//SetDlgItemText(hwndDlg, IDC_BackRed, czIDC_BackRed);
		//SetDlgItemText(hwndDlg, IDC_BackGreen, czIDC_BackGreen);
		//SetDlgItemText(hwndDlg, IDC_BackBlue, czIDC_BackBlue);
		//SetDlgItemText(hwndDlg, IDC_REFRESH, czIDC_Refresh);
		//SetDlgItemText(hwndDlg, IDC_NewDrip, czIDC_NewDrip);
		//// order tabs: (implement backwards;  ie. last tab first)
		//SetWindowPos(GetDlgItem(hwndDlg, IDC_NewDrip), HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		//SetWindowPos(GetDlgItem(hwndDlg, IDC_REFRESH), HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		//SetWindowPos(GetDlgItem(hwndDlg, IDC_BackBlue), HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		//SetWindowPos(GetDlgItem(hwndDlg, IDC_BackGreen), HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		//SetWindowPos(GetDlgItem(hwndDlg, IDC_BackRed), HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		//SetWindowPos(GetDlgItem(hwndDlg, IDC_TailBlue), HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		//SetWindowPos(GetDlgItem(hwndDlg, IDC_TailGreen), HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		//SetWindowPos(GetDlgItem(hwndDlg, IDC_TailRed), HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		//SetWindowPos(GetDlgItem(hwndDlg, IDC_DropBlue), HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		//SetWindowPos(GetDlgItem(hwndDlg, IDC_DropGreen), HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		//SetWindowPos(GetDlgItem(hwndDlg, IDC_DropRed), HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		//SetWindowPos(GetDlgItem(hwndDlg, IDC_TTL), HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		//SetWindowPos(GetDlgItem(hwndDlg, IDC_Height), HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		//SetWindowPos(GetDlgItem(hwndDlg, IDC_Width), HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		//
		return true;
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
			DoOk(hWnd);
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
			EndDialog(hWnd, wParam);
			return TRUE;
		case IDCANCEL:
			DoCancel(hWnd);
			EndDialog(hWnd, wParam);
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

void clsDialog::DoInitDialog(HWND hWnd) {
	//// limit text box sizes
	//SendMessage(GetDlgItem(hwndDlg, IDC_Width), EM_LIMITTEXT, 5, 0);
	//// initilize text boxes
	//SetDlgItemText(hwndDlg, IDC_Width, czIDC_Width);
	//// order tabs: (implement backwards;  ie. last tab first)
	//SetWindowPos(GetDlgItem(hwndDlg, IDC_NewDrip), HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	//SetWindowPos(GetDlgItem(hwndDlg, IDC_REFRESH), HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

void clsDialog::DoCancel(HWND hWnd) {

}
void clsDialog::DoOk(HWND hWnd) {
	// copy dialog fields to local parameters
}

