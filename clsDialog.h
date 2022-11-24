#pragma once
/********************************************************************
Prog: clsDialog
Desc: General dialog routine

Log : 
221123 - created

********************************************************************/
#ifndef CLSDIALOG_H
#define CLSDIALOG_H
#define WIN32_LEAN_AND_MEAN	
#include <windows.h>
#include <string>
#include "resource.h"
class clsDialog{
public:
	//
	// ... list fields here
	//
	std::string sRefresh = "";
	std::string sScale = "";
	std::string sOffsetX = "";
	std::string sOffsetY = "";
	std::string sOffsetZ = "";
	std::string sSizeWidth = "";
	std::string sSizeHeight = "";
public:
	virtual LRESULT DoDialogProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);
	INT_PTR CreateTheDialog(const HINSTANCE hInstance, int lpTemplate, HWND hWndParent);

	virtual void DoInitDialog(HWND hwndDlg);
	virtual void DoCancel(HWND hwndDlg);
	virtual void DoOk(HWND hwndDlg);

	void SetTextParameters(const HWND hwndDlg, const int IDdlgItem, const int LimitText, const std::string &InitValue);
	std::string GetTextValue(const HWND hwndDlg, const int IDdlgItem);
	void RemoveTrailingZeros(std::string &Value);
};

#endif
