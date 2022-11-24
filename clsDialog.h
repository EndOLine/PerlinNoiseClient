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
class clsDialog{
public:
	//
	// ... list fields here
	//
public:
	virtual LRESULT DoDialogProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	INT_PTR CreateTheDialog(const HINSTANCE hInstance, int lpTemplate, HWND hWndParent);

	virtual void DoInitDialog(HWND hWnd);
	virtual void DoCancel(HWND hWnd);
	virtual void DoOk(HWND hWnd);
};

#endif
