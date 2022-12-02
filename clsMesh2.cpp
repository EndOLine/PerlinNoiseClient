/*
* Program: clsMesh2
* Description: Handle and create meshes
*			   This is to replace clsMesh (the old one)
*	
* Notes:
*	integer returns can be 
		>=0 : OK , 
		== -1 : not found, 
		== -2 : error(vertexlist/facelist and colourlist not the same size)
		== -3 : error Colour list must be empty
		== -4 : index out of range
Log:
*	20221130: created
*/
#include "clsMesh2.h"

int clsMesh2::FindVertex(const clsCartCoord& VertexIn) {
	for (int i = 0; i < VertexList.size(); i++) {
		if (VertexList[i] == VertexIn) {
			return i;
		};
	};
	return -1;
}

int clsMesh2::FindVertex(const clsCartCoord& VertexIn, const clsRGBa& ColourIn){
	if (VertexList.size() != ColourList.size()) {
		return -2;							// Error
	};
	for (int i = 0; i < VertexList.size(); i++) {
		if (VertexList[i] == VertexIn) {
			if (ColourList[0] == ColourIn) {
				return i;
			}
		};
	};
	return -1;
}

int clsMesh2::FindFace(const int VertIndex1, const int VertIndex2, const int VertIndex3){
	for (int i = 0; i < FaceList.size(); i++) {
		if ((FaceList[i].i[0] == VertIndex1) && (FaceList[i].i[1] == VertIndex2) && (FaceList[i].i[2] == VertIndex3)) return i;
		if ((FaceList[i].i[0] == VertIndex2) && (FaceList[i].i[1] == VertIndex3) && (FaceList[i].i[2] == VertIndex1)) return i;
		if ((FaceList[i].i[0] == VertIndex3) && (FaceList[i].i[1] == VertIndex1) && (FaceList[i].i[2] == VertIndex2)) return i;
	};
	return -1;
}

int clsMesh2::FindFace(const int VertIndex1, const int VertIndex2, const int VertIndex3, const clsRGBa& ColourIn){
	if (FaceList.size() != ColourList.size()) {
		return -2;							// Error
	};
	for (int i = 0; i < FaceList.size(); i++) {
		if (ColourList[i] != ColourIn) continue;					// if colour does not match then continue
		if ((FaceList[i].i[0] == VertIndex1) && (FaceList[i].i[1] == VertIndex2) && (FaceList[i].i[2] == VertIndex3)) return i;
		if ((FaceList[i].i[0] == VertIndex2) && (FaceList[i].i[1] == VertIndex3) && (FaceList[i].i[2] == VertIndex1)) return i;
		if ((FaceList[i].i[0] == VertIndex3) && (FaceList[i].i[1] == VertIndex1) && (FaceList[i].i[2] == VertIndex2)) return i;
	};
	return -1;
}

void clsMesh2::Clear(){
	VertexList.clear();
	ColourList.clear();
	FaceList.clear();
}

int clsMesh2::AddVertex(const clsCartCoord& VertexIn){
	int vi = -1;
	//vi = FindVertex(VertexIn);
	if (vi < 0) {
		vi = VertexList.size();
		VertexList.push_back(VertexIn);
	};
	return vi;
}

int clsMesh2::AddVertex(const clsCartCoord& VertexIn, const clsRGBa& ColourIn){
	if (VertexList.size() != ColourList.size()) {		// colour list must be the same size as vertex
		return -2;							// Error
	};
	int vi = -1;
	//vi = FindVertex(VertexIn,ColourIn);
	if (vi < 0) {
		vi = VertexList.size();
		VertexList.push_back(VertexIn);
		ColourList.push_back(ColourIn);
	};
	return vi;
}

int clsMesh2::AddFace(const int VertIndex1, const int VertIndex2, const int VertIndex3){
	if ((VertIndex1 > VertexList.size()) || (VertIndex2 > VertexList.size()) || (VertIndex3 > VertexList.size())) {
		return -4;								// error
	};
	int index = -1;
	//index = FindFace(VertIndex1, VertIndex2, VertIndex3);
	if (index >= 0) {							// found
		return index;
	}
	index = FaceList.size();
	FaceList.push_back({ VertIndex1,VertIndex2,VertIndex3 });
	return index;
}

int clsMesh2::AddFace(const int VertIndex1, const int VertIndex2, const int VertIndex3, const clsRGBa& ColourIn){
	if (FaceList.size() != ColourList.size()) {		// colour list must be the same size as face list
		return -2;							// Error
	};
	int index = -1;
	//index = FindFace(VertIndex1, VertIndex2, VertIndex3,ColourIn);
	if (index >= 0) {							// found
		return index;
	};
	index = FaceList.size();
	FaceList.push_back({ VertIndex1,VertIndex2,VertIndex3 });
	ColourList.push_back(ColourIn);
	return 0;
}

void clsMesh2::AddCubeColourPoints(const clsCartCoord& CenterPointIn, const float WidthIn, const clsRGBa& ColourIn) {
	/* Cube nodes:
	*
	*      5 ------------ 4
	*     /.             /|
	*    / .            / |
	*   0 ------------ 1  |
	*   |  .           |  |
	*   |  .           |  |
	*   |  .           |  |
	*   |  6 ..........|. 7
	*   | .            | /
	*   |.             |/
	*   3 -------------2
	*/
	//unsigned int iStartIdx = Points.size();
	//ClockWise = true;
	//ZintoScreen = true;
	float halfWidth = WidthIn / 2;
	int iStartIdx[8] = {};
	iStartIdx[0] = AddVertex({ CenterPointIn.x - halfWidth, CenterPointIn.y + halfWidth, CenterPointIn.z - halfWidth },ColourIn);
	iStartIdx[1] = AddVertex({ CenterPointIn.x + halfWidth, CenterPointIn.y + halfWidth, CenterPointIn.z - halfWidth }, ColourIn);
	iStartIdx[2] = AddVertex({ CenterPointIn.x + halfWidth, CenterPointIn.y - halfWidth, CenterPointIn.z - halfWidth }, ColourIn);
	iStartIdx[3] = AddVertex({ CenterPointIn.x - halfWidth, CenterPointIn.y - halfWidth, CenterPointIn.z - halfWidth }, ColourIn);
	iStartIdx[4] = AddVertex({ CenterPointIn.x + halfWidth, CenterPointIn.y + halfWidth, CenterPointIn.z + halfWidth }, ColourIn);
	iStartIdx[5] = AddVertex({ CenterPointIn.x - halfWidth, CenterPointIn.y + halfWidth, CenterPointIn.z + halfWidth }, ColourIn);
	iStartIdx[6] = AddVertex({ CenterPointIn.x - halfWidth, CenterPointIn.y - halfWidth, CenterPointIn.z + halfWidth }, ColourIn);
	iStartIdx[7] = AddVertex({ CenterPointIn.x + halfWidth, CenterPointIn.y - halfWidth, CenterPointIn.z + halfWidth }, ColourIn);
	AddFace(iStartIdx[0], iStartIdx[1], iStartIdx[2]);
	AddFace(iStartIdx[0], iStartIdx[2], iStartIdx[3]);
	AddFace(iStartIdx[0], iStartIdx[3], iStartIdx[6]);
	AddFace(iStartIdx[0], iStartIdx[6], iStartIdx[5]);
	AddFace(iStartIdx[4], iStartIdx[5], iStartIdx[6]);
	AddFace(iStartIdx[4], iStartIdx[6], iStartIdx[7]);
	AddFace(iStartIdx[1], iStartIdx[4], iStartIdx[7]);
	AddFace(iStartIdx[1], iStartIdx[7], iStartIdx[2]);
	AddFace(iStartIdx[0], iStartIdx[5], iStartIdx[1]);
	AddFace(iStartIdx[1], iStartIdx[5], iStartIdx[4]);
	AddFace(iStartIdx[2], iStartIdx[6], iStartIdx[3]);
	AddFace(iStartIdx[2], iStartIdx[7], iStartIdx[6]);
}

void clsMesh2::AddCube(const clsCartCoord& CenterPointIn, const float WidthIn, const clsRGBa& ColourIn) {
	/* Cube nodes:
	*
	*      5 ------------ 4
	*     /.             /|
	*    / .            / |
	*   0 ------------ 1  |
	*   |  .           |  |
	*   |  .           |  |
	*   |  .           |  |
	*   |  6 ..........|. 7
	*   | .            | /
	*   |.             |/
	*   3 -------------2
	*/
	//unsigned int iStartIdx = Points.size();
	//ClockWise = true;
	//ZintoScreen = true;
	float halfWidth = WidthIn / 2;
	int iStartIdx[8] = {};
	iStartIdx[0] = AddVertex({ CenterPointIn.x - halfWidth, CenterPointIn.y + halfWidth, CenterPointIn.z - halfWidth });
	iStartIdx[1] = AddVertex({ CenterPointIn.x + halfWidth, CenterPointIn.y + halfWidth, CenterPointIn.z - halfWidth });
	iStartIdx[2] = AddVertex({ CenterPointIn.x + halfWidth, CenterPointIn.y - halfWidth, CenterPointIn.z - halfWidth });
	iStartIdx[3] = AddVertex({ CenterPointIn.x - halfWidth, CenterPointIn.y - halfWidth, CenterPointIn.z - halfWidth });
	iStartIdx[4] = AddVertex({ CenterPointIn.x + halfWidth, CenterPointIn.y + halfWidth, CenterPointIn.z + halfWidth });
	iStartIdx[5] = AddVertex({ CenterPointIn.x - halfWidth, CenterPointIn.y + halfWidth, CenterPointIn.z + halfWidth });
	iStartIdx[6] = AddVertex({ CenterPointIn.x - halfWidth, CenterPointIn.y - halfWidth, CenterPointIn.z + halfWidth });
	iStartIdx[7] = AddVertex({ CenterPointIn.x + halfWidth, CenterPointIn.y - halfWidth, CenterPointIn.z + halfWidth });
	AddFace(iStartIdx[0], iStartIdx[1], iStartIdx[2], ColourIn);
	AddFace(iStartIdx[0], iStartIdx[2], iStartIdx[3], ColourIn);
	AddFace(iStartIdx[0], iStartIdx[3], iStartIdx[6], ColourIn);
	AddFace(iStartIdx[0], iStartIdx[6], iStartIdx[5], ColourIn);
	AddFace(iStartIdx[4], iStartIdx[5], iStartIdx[6], ColourIn);
	AddFace(iStartIdx[4], iStartIdx[6], iStartIdx[7], ColourIn);
	AddFace(iStartIdx[1], iStartIdx[4], iStartIdx[7], ColourIn);
	AddFace(iStartIdx[1], iStartIdx[7], iStartIdx[2], ColourIn);
	AddFace(iStartIdx[0], iStartIdx[5], iStartIdx[1], ColourIn);
	AddFace(iStartIdx[1], iStartIdx[5], iStartIdx[4], ColourIn);
	AddFace(iStartIdx[2], iStartIdx[6], iStartIdx[3], ColourIn);
	AddFace(iStartIdx[2], iStartIdx[7], iStartIdx[6], ColourIn);
}


