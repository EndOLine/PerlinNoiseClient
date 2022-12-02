#pragma once
/*
* Program: clsOBJ
* Description:
*	Write  Wavefront Technologies's OBJ  image files
*
* Log:
*	20221115: created
*/
#ifndef CLSOBJ_H
#define CLSOBJ_H
#define WIN32_LEAN_AND_MEAN
#include <string>
#include <fstream>
//#include <stdio.h>
//#include <stdlib.h>

class clsOBJ{
public: // variables
	struct stVertex {
		float x, y, z;
	};
	struct stColour {
		unsigned char red, green, blue, alpha;
	};
	struct stFace3vertex {
		int i[3];
	};
private:
public: // functions
	bool SaveObj(const std::string FileName, const std::string Comments,
		const stVertex* stVertexList, const int iVertexCount,
		const stFace3vertex* stFaceList, const int iFaceCount,
		const stColour* stColourList = NULL, const int iColourCount = 0);
private:
};

#endif
