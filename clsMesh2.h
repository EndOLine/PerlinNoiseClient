/*
* Program: clsMesh2
* Description: Handle and create meshes
*			   This is to replace clsMesh (the old one)
*			   Duplicate Vertex plus Colour (if it exists) are not allow
* Notes:
*	If do not add colour to both vertex and faces. Pick one an stick to it for this mesh
* Log:
*	20221130: created
*
*/
#ifndef CLSMESH2_H
#define CLSMESH2_H
#define WIN32_LEAN_AND_MEAN
#include <vector>
#include <fstream>
#include "clsCartCoord.h"
#include "clsCLR.h"
class clsMesh2{
	
public:	//parameters
	struct stFace {
		int i[3] = {};
	};
	std::vector<clsCartCoord> VertexList;
	std::vector<clsRGBa> ColourList;
	std::vector<stFace> FaceList;
private:
	
public:	// functions
	void Clear();
	int AddVertex(const clsCartCoord& VertexIn);
	int AddVertex(const clsCartCoord& VertexIn, const clsRGBa& ColourIn);
	int AddFace(const int VertIndex1, const int VertIndex2, const int VertIndex3);
	int AddFace(const int VertIndex1, const int VertIndex2, const int VertIndex3, const clsRGBa& ColourIn);
	void AddCube(const clsCartCoord& CenterPoint, const float WidthIn, const clsRGBa& ColourIn);
	void AddCubeColourPoints(const clsCartCoord& CenterPoint, const float WidthIn, const clsRGBa& ColourIn);
private:
	int FindVertex(const clsCartCoord& VertexIn);
	int FindVertex(const clsCartCoord& VertexIn, const clsRGBa& ColourIn);
	int FindFace(const int VertIndex1, const int VertIndex2, const int VertIndex3);
	int FindFace(const int VertIndex1, const int VertIndex2, const int VertIndex3, const clsRGBa& ColourIn);
};


#endif