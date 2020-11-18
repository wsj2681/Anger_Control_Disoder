#pragma once
#include "framework.h"

struct TextureVertex
{
	XMFLOAT3 Position;
	XMFLOAT3 Tangent;
	XMFLOAT3 Normal;
	XMFLOAT3 Binormal;
	XMFLOAT2 UV;

};

class FBXLoader
{
public:
	FBXLoader() = default;
	~FBXLoader() = default;


private:

	/* Vertex */
	vector<int> indices;
	vector<TextureVertex> TexVertex;
	vector<XMFLOAT3> CtrlPoints;
	unordered_map<int, TextureVertex> indexMapping;

	/* Material */
	FbxPropertyT<FbxDouble3> Double3;
	FbxPropertyT<FbxDouble> Double1;
	FbxColor color;



	FbxManager* Manager = nullptr;
	FbxIOSettings* IOsettings = nullptr;
	FbxScene* Scene = nullptr;

public:

	void Init(const char* fileName);
	void Destroy();

	bool LoadTexturefromFBX(const char* fileName);
	bool LoadMaterialfromFBX(FbxGeometry* geometry);
	void LoadAnimationfromFBX(const char* fileName);

	bool LoadControlPoints(FbxMesh* mesh);
	void LoadNode(FbxNode* node);


	XMFLOAT3 ReadNormal(const FbxMesh* mesh, int controlPointIndex, int vertexCounter);
	XMFLOAT3 ReadBinormal(const FbxMesh* mesh, int controlPointIndex, int vertexCounter);
	XMFLOAT3 ReadTangent(const FbxMesh* mesh, int controlPointIndex, int vertexCounter);
	XMFLOAT2 ReadUV(const FbxMesh* mesh, int TextureUVIndex);

	void InsertVertex(TextureVertex& vertex);

};

