#pragma once
/********************************************************************
Prog: clsApp
Desc: Modified application class for specific program

Log : 221118 - created
	  
********************************************************************/
#ifndef CLSAPP_H
#define CLSAPP_H
#define WIN32_LEAN_AND_MEAN	
#include "..\Common\clsHGOapplication.h"
#include "clsWindow.h"
#include "resource.h"
class clsApp : public clsHGOapplication {
public:
	clsWindow Window;
public:
	virtual bool DoInitilize() override;
	virtual bool DoBegin(int nCmdShow) override;
	virtual bool DoRunning() override;
	virtual void DoEnd() override;
};

#endif