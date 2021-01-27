#pragma once
#include "framework.h"
#include "Shader.h"
#include "Mesh.h"
#include "Shader.h"


class CFbxRenderInfo
{
public:
	CFbxRenderInfo() { }
	~CFbxRenderInfo();

public:
	Shader* m_pShader = NULL;
	MeshFromFbx* m_pMesh = NULL;
};

void AnimateFbxMesh(FbxMesh* pfbxMesh, FbxTime& fbxCurrentTime)
{
	int nVertices = pfbxMesh->GetControlPointsCount();
	if (nVertices > 0)
	{
		FbxVector4* pfbxv4Vertices = new FbxVector4[nVertices];
		::memcpy(pfbxv4Vertices, pfbxMesh->GetControlPoints(), nVertices * sizeof(FbxVector4));

		int nSkinDeformers = pfbxMesh->GetDeformerCount(FbxDeformer::eSkin);
		if (nSkinDeformers > 0) ::ComputeSkinDeformation(pfbxMesh, fbxCurrentTime, pfbxv4Vertices, nVertices);

		CFbxRenderInfo* pFbxRenderInfo = (CFbxRenderInfo*)pfbxMesh->GetUserDataPtr();
		if (pFbxRenderInfo->m_pMesh)
		{
			for (int i = 0; i < nVertices; i++) pFbxRenderInfo->m_pMesh->m_pxmf4MappedPositions[i] = XMFLOAT4((float)pfbxv4Vertices[i][0], (float)pfbxv4Vertices[i][1], (float)pfbxv4Vertices[i][2], 1.0f);
		}

		delete[] pfbxv4Vertices;
	}
}

void AnimateFbxNodeHierarchy(FbxNode* pfbxNode, FbxTime& fbxCurrentTime)
{
	FbxNodeAttribute* pfbxNodeAttribute = pfbxNode->GetNodeAttribute();
	if (pfbxNodeAttribute && (pfbxNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh))
	{
		FbxMesh* pfbxMesh = pfbxNode->GetMesh();
		AnimateFbxMesh(pfbxMesh, fbxCurrentTime);
	}

	int nChilds = pfbxNode->GetChildCount();
	for (int i = 0; i < nChilds; i++) AnimateFbxNodeHierarchy(pfbxNode->GetChild(i), fbxCurrentTime);
}

void RenderFbxMesh(ID3D12GraphicsCommandList* pd3dCommandList, FbxMesh* pfbxMesh, FbxAMatrix& fbxmtxNodeToRoot, FbxAMatrix& fbxmtxGeometryOffset, FbxAMatrix fbxmtxWorld)
{
	int nVertices = pfbxMesh->GetControlPointsCount();
	if (nVertices > 0)
	{
		FbxAMatrix fbxmtxTransform = fbxmtxWorld;
		int nSkinDeformers = pfbxMesh->GetDeformerCount(FbxDeformer::eSkin);
		if (nSkinDeformers == 0) fbxmtxTransform = fbxmtxWorld * fbxmtxNodeToRoot * fbxmtxGeometryOffset;

		Object::UpdateShaderVariable(pd3dCommandList, &fbxmtxTransform);

		CFbxRenderInfo* pFbxRenderInfo = (CFbxRenderInfo*)pfbxMesh->GetUserDataPtr();
		if (pFbxRenderInfo->m_pShader) pFbxRenderInfo->m_pShader->Render(pd3dCommandList, NULL);
		if (pFbxRenderInfo->m_pMesh) pFbxRenderInfo->m_pMesh->Render(pd3dCommandList);
	}
}

void RenderFbxNodeHierarchy(ID3D12GraphicsCommandList* pd3dCommandList, FbxNode* pfbxNode, const FbxTime& fbxCurrentTime, const FbxAMatrix& fbxmtxWorld)
{
	FbxNodeAttribute* pfbxNodeAttribute = pfbxNode->GetNodeAttribute();
	if (pfbxNodeAttribute && (pfbxNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh))
	{
		FbxAMatrix fbxmtxNodeToRoot = pfbxNode->EvaluateGlobalTransform(fbxCurrentTime);
		FbxAMatrix fbxmtxGeometryOffset = GetGeometricOffsetTransform(pfbxNode);

		FbxMesh* pfbxMesh = pfbxNode->GetMesh();
		RenderFbxMesh(pd3dCommandList, pfbxMesh, fbxmtxNodeToRoot, fbxmtxGeometryOffset, fbxmtxWorld);
	}

	int nChilds = pfbxNode->GetChildCount();
	for (int i = 0; i < nChilds; i++) RenderFbxNodeHierarchy(pd3dCommandList, pfbxNode->GetChild(i), fbxCurrentTime, fbxmtxWorld);
}

void CreateMeshFromFbxNodeHierarchy(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, FbxNode* pfbxNode)
{
	FbxNodeAttribute* pfbxNodeAttribute = pfbxNode->GetNodeAttribute();
	if (pfbxNodeAttribute && (pfbxNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh))
	{
		FbxMesh* pfbxMesh = pfbxNode->GetMesh();
		if (pfbxMesh)
		{
			int nVertices = pfbxMesh->GetControlPointsCount();

			int nIndices = 0;
			int nPolygons = pfbxMesh->GetPolygonCount();
			for (int i = 0; i < nPolygons; i++) nIndices += pfbxMesh->GetPolygonSize(i); //Triangle: 3, Triangulate(), nIndices = nPolygons * 3

			int* pnIndices = new int[nIndices];
			for (int i = 0, k = 0; i < nPolygons; i++)
			{
				int nPolygonSize = pfbxMesh->GetPolygonSize(i);
				for (int j = 0; j < nPolygonSize; j++) pnIndices[k++] = pfbxMesh->GetPolygonVertex(i, j);
			}

			CFbxRenderInfo* pFbxRenderInfo = new CFbxRenderInfo();
			pFbxRenderInfo->m_pMesh = new MeshFromFbx(pd3dDevice, pd3dCommandList, nVertices, nIndices, pnIndices);
			int nSkinDeformers = pfbxMesh->GetDeformerCount(FbxDeformer::eSkin);
			if (nSkinDeformers > 0)
				pFbxRenderInfo->m_pShader = new FbxSkinnedModelShader();
			else
				pFbxRenderInfo->m_pShader = new FbxModelShader();
			pFbxRenderInfo->m_pShader->CreateShader(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);

			pfbxMesh->SetUserDataPtr(pFbxRenderInfo);

			if (pnIndices) delete[] pnIndices;
		}
	}

	int nChilds = pfbxNode->GetChildCount();
	for (int i = 0; i < nChilds; i++) CreateMeshFromFbxNodeHierarchy(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, pfbxNode->GetChild(i));
}

void ReleaseMeshFromFbxNodeHierarchy(FbxNode* pfbxNode)
{
	FbxNodeAttribute* pfbxNodeAttribute = pfbxNode->GetNodeAttribute();
	if (pfbxNodeAttribute && (pfbxNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh))
	{
		FbxMesh* pfbxMesh = pfbxNode->GetMesh();
		if (pfbxMesh)
		{
			CFbxRenderInfo* pFbxRenderInfo = (CFbxRenderInfo*)pfbxMesh->GetUserDataPtr();
			if (pFbxRenderInfo->m_pMesh) pFbxRenderInfo->m_pMesh->Release();
			if (pFbxRenderInfo->m_pShader) pFbxRenderInfo->m_pShader->Release();
		}
	}

	int nChilds = pfbxNode->GetChildCount();
	for (int i = 0; i < nChilds; i++) ReleaseMeshFromFbxNodeHierarchy(pfbxNode->GetChild(i));
}

void ReleaseUploadBufferFromFbxNodeHierarchy(FbxNode* pfbxNode)
{
	FbxNodeAttribute* pfbxNodeAttribute = pfbxNode->GetNodeAttribute();
	if (pfbxNodeAttribute && (pfbxNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh))
	{
		FbxMesh* pfbxMesh = pfbxNode->GetMesh();
		if (pfbxMesh)
		{
			CFbxRenderInfo* pFbxRenderInfo = (CFbxRenderInfo*)pfbxMesh->GetUserDataPtr();
			if (pFbxRenderInfo->m_pMesh) pFbxRenderInfo->m_pMesh->ReleaseUploadBuffers();
			if (pFbxRenderInfo->m_pShader) pFbxRenderInfo->m_pShader->ReleaseUploadBuffers();
		}
	}

	int nChilds = pfbxNode->GetChildCount();
	for (int i = 0; i < nChilds; i++) ReleaseUploadBufferFromFbxNodeHierarchy(pfbxNode->GetChild(i));
}

FbxScene* LoadFbxSceneFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, FbxManager* pfbxSdkManager, const char* pstrFbxFileName)
{
	FbxScene* pfbxScene = NULL;

	int nSDKMajor, nSDKMinor, nSDKRevision;
	FbxManager::GetFileFormatVersion(nSDKMajor, nSDKMinor, nSDKRevision);

	FbxIOSettings* pfbxIOSettings = pfbxSdkManager->GetIOSettings();
	FbxImporter* pfbxImporter = FbxImporter::Create(pfbxSdkManager, " ");
	bool bImportStatus = pfbxImporter->Initialize(pstrFbxFileName, -1, pfbxIOSettings);

	int nFileFormatMajor, nFileFormatMinor, nFileFormatRevision;
	pfbxImporter->GetFileVersion(nFileFormatMajor, nFileFormatMinor, nFileFormatRevision);

	pfbxScene = FbxScene::Create(pfbxSdkManager, " ");
	bool bStatus = pfbxImporter->Import(pfbxScene);

	FbxGeometryConverter fbxGeomConverter(pfbxSdkManager);
	fbxGeomConverter.Triangulate(pfbxScene, true);
	//	fbxGeomConverter.SplitMeshesPerMaterial(pfbxScene, true);

	//	FbxAxisSystem fbxSceneAxisSystem = pfbxScene->GetGlobalSettings().GetAxisSystem();
	//	FbxAxisSystem fbxDirectXAxisSystem(FbxAxisSystem::eDirectX);
	//	if (fbxSceneAxisSystem != fbxDirectXAxisSystem) fbxDirectXAxisSystem.ConvertScene(pfbxScene);
	//	fbxSceneAxisSystem = pfbxScene->GetGlobalSettings().GetAxisSystem();

	FbxSystemUnit fbxSceneSystemUnit = pfbxScene->GetGlobalSettings().GetSystemUnit();
	if (fbxSceneSystemUnit.GetScaleFactor() != 1.0) FbxSystemUnit::cm.ConvertScene(pfbxScene);

	pfbxImporter->Destroy();

	return(pfbxScene);
}
