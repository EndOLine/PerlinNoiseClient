/*****************************************************************************************
Program: clsPly.cpp
	Project : MeshMaker
	Author : Harold Otterdyks
	Description :
	save  "Polygon File Format" aka the "Stanford Triangle Format"
	http //paulbourke.net/dataformats/ply/
	https //github.com/ddiakopoulos/tinyply
Notes :
20200201 : created
20221116 : documentation modified
******************************************************************************************/
#include "clsPly.h"

clsPly::clsPly(){
};


clsPly::~clsPly(){
}


/* save ply file format */
void clsPly::Save(const char *pczLogFile, 
					const stVertex *stVertexList, const int iVertexCount, 
					const stFace3vertex *stFaceList, const int iFaceCount, 
					const clsRGBa *stColourList, const int iColourCount, 
					const stFace3Texture *stTextureList) {

	fFileHandle = _fsopen(pczLogFile, "wt", _SH_DENYWR);
	//fprintf(fFileHandle, "%s\n", pczMessage);
	

	fprintf(fFileHandle,"%s\n", czFileType);
	fprintf(fFileHandle,"%s\n", czFileFormat);
	fprintf(fFileHandle,"%s\n", czFileComment1);
	fprintf(fFileHandle,"%s\n", czFileComment2);
	// vertex list
	fprintf(fFileHandle,"%s %i\n", czVertexCount, iVertexCount);
	fprintf(fFileHandle,"%s\n", czVertexX);
	fprintf(fFileHandle,"%s\n", czVertexY);
	fprintf(fFileHandle,"%s\n", czVertexZ);
	
	if ((iColourCount != 0)&&(iColourCount== iVertexCount)) {						// only if colours are given and match vertex count
		fprintf(fFileHandle,"%s\n", czVertexColourRed);
		fprintf(fFileHandle,"%s\n", czVertexColourGreen);
		fprintf(fFileHandle,"%s\n", czVertexColourBlue);
	};

	// face list
	if ((stFaceList != NULL) || (iFaceCount > 0)) {
		fprintf(fFileHandle, "%s %i\n", czFaceCount, iFaceCount);
		fprintf(fFileHandle, "%s\n", czFacePropery);
	};
	if ((iColourCount != 0) && (iColourCount == iFaceCount)) {						// only if colours are given and match face count
		fprintf(fFileHandle, "%s\n", czVertexColourRed);
		fprintf(fFileHandle, "%s\n", czVertexColourGreen);
		fprintf(fFileHandle, "%s\n", czVertexColourBlue);
	};
	if (stTextureList != NULL) {
		fprintf(fFileHandle, "%s\n", czFaceTexture);
	};
	// end
	fprintf(fFileHandle,"%s\n", czEndHeader);

	// list vertexs
	if ((iColourCount != 0) && (iColourCount == iVertexCount)) {						// check if colours are given
		for (int i = 0; i < iVertexCount; i++) {
			fprintf(fFileHandle,"%f %f %f %i %i %i\n", stVertexList[i].x, stVertexList[i].y, stVertexList[i].z, stColourList[i].r , stColourList[i].g , stColourList[i].b );
		};
	} else {
		for (int i = 0; i < iVertexCount; i++) {
			fprintf(fFileHandle,"%f %f %f\n", stVertexList[i].x, stVertexList[i].y, stVertexList[i].z);
		};
	}
	// list faces
	if ((iColourCount != 0) && (iColourCount == iFaceCount)) {
		for (int i = 0; i < iFaceCount; i++) {
			fprintf(fFileHandle, "3 %i %i %i %i %i %i\n", stFaceList[i].i[0], stFaceList[i].i[1], stFaceList[i].i[2],
				stColourList[i].r, stColourList[i].g, stColourList[i].b);
		};
	} else if ((stTextureList != NULL) && (stFaceList != NULL)) {
		for (int i = 0; i < iFaceCount; i++) {
			fprintf(fFileHandle, "3 %i %i %i 6 %f %f %f %f %f %f\n", stFaceList[i].i[0], stFaceList[i].i[1], stFaceList[i].i[2],
					stTextureList[i].t[0].u, stTextureList[i].t[0].v, stTextureList[i].t[1].u, stTextureList[i].t[1].v, stTextureList[i].t[2].u, stTextureList[i].t[2].v );
		};
	} else {
		if ((stFaceList != NULL) || (iFaceCount > 0)) {
			for (int i = 0; i < iFaceCount; i++) {
				fprintf(fFileHandle, "3 %i %i %i\n", stFaceList[i].i[0], stFaceList[i].i[1], stFaceList[i].i[2]);
			};
		};
	};

	fclose(fFileHandle);
	return;
};