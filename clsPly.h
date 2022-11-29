#pragma once
/*****************************************************************************************
Program: clsPly.h
Project: MeshMaker
Author: Harold Otterdyks
Description:
	save  "Polygon File Format" aka the "Stanford Triangle Format"
Notes:
20200201: created
20221116: documentation modified
******************************************************************************************/
#define WIN32_LEAN_AND_MEAN
#include <stdio.h> 
#include <share.h>

class clsPly
{
public:
	struct stVertex {
		float x, y, z;
	};
	struct stColour {
		unsigned char red, green, blue;
	};

	struct stFace3vertex {
		int i[3];
	};
	/*
	Textures: 0,0 is lower left corner
		      u is to right (x coord) 0.0->1.0
			  v is up (y coord)		  0.0->1.0

	*/
	struct stTexture {
		float u, v;
	};
	struct stFace3Texture {
		stTexture t[3];
	};

private:
	FILE *fFileHandle=0;

	// list of ply elements
	// ply elements
	const char* czFileType = "ply";
	const char* czFileFormat = "format ascii 1.0";				// ascii/binary, format version number "binary_little_endian 1.0" "binary_big_endian 1.0"
	const char* czFileComment1 = "comment Generate by HGO clsPly";	// comments keyword specified, like all lines 
	const char* czFileComment2 = "comment by Harold Otterdyks";
	// vertex list description
	const char* czVertexCount = "element vertex ";				// define "vertex" element, concatinate the number of elements "element vertex 8"
	const char* czVertexX = "property float x";					// vertex contains float "x" coordinate
	const char* czVertexY = "property float y";					// vertex contains float "y" coordinate
	const char* czVertexZ = "property float z";					// vertex contains float "z" coordinate
																// optional colours in vertex
	const char* czVertexColourRed = "property uchar red";			// start of vertex color (optional)
	const char* czVertexColourGreen = "property uchar green";		// start of vertex color (optional)
	const char* czVertexColourBlue = "property uchar blue";			// start of vertex color (optional)

																	// face list description
	const char* czFaceCount = "element face ";				// define "face" element, concatinate the number of elements "element face 6"
	const char* czFacePropery = "property list uchar int vertex_index";	// "vertex_indices" is a list of ints
	const char* czFaceTexture = "property list uchar float texcoord";	// texture u v coordinates are attached to face
	// end header
	const char* czEndHeader = "end_header";						// delimits the end of the header

	// vertix list "x y z"
	//			   "x y z red green blue"			optional
	// face list   "n i1 i2 i3 ..." n- number of vertexs in list, i? - index of vertex
	// texture list follows face list n - of textures in list u v values

	/*	scalar types
	name        type        number of bytes
	---------------------------------------
	char       character                 1
	uchar      unsigned character        1
	short      short integer             2
	ushort     unsigned short integer    2
	int        integer                   4
	uint       unsigned integer          4
	float      single-precision float    4
	double     double-precision float    8
	*/


public:
	clsPly();
	~clsPly();
	void Save(const char *pczLogFile, const stVertex *stVertexList,int iVertexCount, const stFace3vertex *stFaceList,int iFaceCount, 
		      const stColour *stColourList=NULL, const stFace3Texture *stTextureList = NULL);
};

