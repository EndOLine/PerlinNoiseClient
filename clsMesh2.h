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
//#include <fstream>
#include <functional>
#include <unordered_set>
#include "clsCartCoord.h"
#include "clsCLR.h"
class clsMesh2{
	
public:	//parameters
	const int NOT_FOUND = -1;
	const int COLOUR_SIZE = -2;
	const int INDEX_RANGE = -4;
	struct stFace {
		int i[3] = {};
	};
	std::vector<clsCartCoord> VertexList;
	std::vector<clsRGBa> ColourList;
	std::vector<int>     ColourIndex;				// list of indices to ColourList
	std::vector<stFace> FaceList;

	// for finding unique vertexes
	struct stUniqueVertex {
		clsCartCoord Vertex;
		int			 ColourIndex;
		int			 ExistingVertexIndex;			// where is the existing vertex in VertexList[]
		bool operator==(const stUniqueVertex& t) const { return ((this->Vertex == t.Vertex) && (this->ColourIndex == t.ColourIndex)); };
	};
	class SetHashFunction {
	public:
		// id is returned as hash function
		size_t operator()(const stUniqueVertex& t) const {
			std::size_t h1 = std::hash<float>{}(t.Vertex.x) ^ std::hash<float>{}(t.Vertex.y) ^ std::hash<float>{}(t.Vertex.z);
			return h1;
		};
	};
	std::unordered_set<stUniqueVertex, SetHashFunction> UniqueSet;
	bool CheckForUnique = false;

private:
	
public:	// functions
	void Clear();
	int AddVertex(const clsCartCoord& VertexIn);
	int AddVertex(const clsCartCoord& VertexIn, const clsRGBa& ColourIn);
	int AddFace(const int VertIndex1, const int VertIndex2, const int VertIndex3);
	int AddFace(const int VertIndex1, const int VertIndex2, const int VertIndex3, const clsRGBa& ColourIn);
	int AddColour(const clsRGBa& ColourIn);
	void AddCube(const clsCartCoord& CenterPoint, const float WidthIn, const clsRGBa& ColourIn);
	void AddCubeColourPoints(const clsCartCoord& CenterPoint, const float WidthIn, const clsRGBa& ColourIn);
	void UniqueBegin();
	void UniqueEnd();
	// assignment operator
	clsMesh2& operator=(const clsMesh2& rhs);
private:
	int FindVertex(const clsCartCoord& VertexIn);
	int FindVertex(const clsCartCoord& VertexIn, const clsRGBa& ColourIn);
	int FindFace(const int VertIndex1, const int VertIndex2, const int VertIndex3);
	int FindFace(const int VertIndex1, const int VertIndex2, const int VertIndex3, const clsRGBa& ColourIn);
	int FindColour(const clsRGBa& ColourIn);
};


#endif