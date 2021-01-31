#include "stdafx.h"
#include "Object.h"
#include "Shader.h"
#include "FbxSceneContext.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CFbxRenderInfo
{
public:
	CFbxRenderInfo() { }
	~CFbxRenderInfo();

public:
	CShader 						*m_pShader = NULL;
	CMeshFromFbx					*m_pMesh = NULL;
};

CFbxRenderInfo::~CFbxRenderInfo()
{
	if (m_pShader) m_pShader->Release();
	if (m_pMesh) m_pMesh->Release();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
XMFLOAT4X4 FbxMatrixToXmFloat4x4Matrix(FbxAMatrix *pfbxmtxSource)
{
	FbxVector4 S = pfbxmtxSource->GetS();
//	FbxVector4 R = FbxVector4(0.0, 45.0, 0.0, 1.0);
	FbxVector4 R = pfbxmtxSource->GetR();
	FbxVector4 T = pfbxmtxSource->GetT();

	FbxAMatrix fbxmtxTransform = FbxAMatrix(T, R, S);

	XMFLOAT4X4 xmf4x4Result;
	for (int i = 0; i < 4; i++)
	{
//		for (int j = 0; j < 4; j++) xmf4x4Result.m[i][j] = (float)fbxmtxTransform[i][j];
		for (int j = 0; j < 4; j++) xmf4x4Result.m[i][j] = (float)(*pfbxmtxSource)[i][j];
	}

	XMFLOAT3 xmf3S = XMFLOAT3((float)S.mData[0], (float)S.mData[1], (float)S.mData[2]);
	XMFLOAT3 xmf3R = XMFLOAT3((float)R.mData[0], (float)R.mData[1], (float)R.mData[2]);
	XMFLOAT3 xmf3T = XMFLOAT3((float)T.mData[0], (float)T.mData[1], (float)T.mData[2]);
//	XMFLOAT4X4 xmf4x4Transform;
//	XMStoreFloat4x4(&xmf4x4Transform, XMMatrixAffineTransformation(XMLoadFloat3(&xmf3S), XMVectorZero(), XMQuaternionRotationRollPitchYaw(XMConvertToRadians(xmf3R.x), XMConvertToRadians(xmf3R.y), XMConvertToRadians(xmf3R.z)), XMLoadFloat3(&xmf3T)));

	//XMFLOAT4X4 xmf4x4Translation;
	//XMMATRIX xmT = XMMatrixTranslation(xmf3T.x, xmf3T.y, xmf3T.z);
	//XMStoreFloat4x4(&xmf4x4Translation, xmT);
	//XMFLOAT4X4 xmf4x4Rotation;
	//XMMATRIX xmR = XMMatrixRotationRollPitchYaw(XMConvertToRadians(xmf3R.x), XMConvertToRadians(xmf3R.y), XMConvertToRadians(xmf3R.z));
	//XMStoreFloat4x4(&xmf4x4Rotation, xmR);
	//XMStoreFloat4x4(&xmf4x4Rotation, ));
	//XMFLOAT4X4 xmf4x4Scale;
	//XMStoreFloat4x4(&xmf4x4Scale, XMMatrixScaling(xmf3S.x, xmf3S.y, xmf3S.z));
	
	XMMATRIX Rx = XMMatrixRotationX(XMConvertToRadians(xmf3R.x));
	XMMATRIX Ry = XMMatrixRotationY(XMConvertToRadians(xmf3R.y));
	XMMATRIX Rz = XMMatrixRotationZ(XMConvertToRadians(xmf3R.z));
	XMMATRIX xmR = XMMatrixMultiply(XMMatrixMultiply(Rx, Ry), Rz);
	XMFLOAT4X4 xmf4x4Multiply;
	XMStoreFloat4x4(&xmf4x4Multiply, XMMatrixMultiply(XMMatrixMultiply(XMMatrixScaling(xmf3S.x, xmf3S.y, xmf3S.z), xmR), XMMatrixTranslation(xmf3T.x, xmf3T.y, xmf3T.z)));

	return(xmf4x4Result);
}

FbxAMatrix XmFloat4x4MatrixToFbxMatrix(XMFLOAT4X4& xmf4x4Source)
{
	FbxAMatrix fbxmtxResult;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++) fbxmtxResult[i][j] = xmf4x4Source.m[i][j];
	}
	return(fbxmtxResult);
}

void MatrixScale(FbxAMatrix& fbxmtxSrcMatrix, double pValue)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++) fbxmtxSrcMatrix[i][j] *= pValue;
	}
}

void MatrixAddToDiagonal(FbxAMatrix& fbxmtxSrcMatrix, double pValue)
{
	fbxmtxSrcMatrix[0][0] += pValue;
	fbxmtxSrcMatrix[1][1] += pValue;
	fbxmtxSrcMatrix[2][2] += pValue;
	fbxmtxSrcMatrix[3][3] += pValue;
}

void MatrixAdd(FbxAMatrix& fbxmtxDstMatrix, FbxAMatrix& fbxmtxSrcMatrix)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++) fbxmtxDstMatrix[i][j] += fbxmtxSrcMatrix[i][j];
	}
}

FbxAMatrix GetGeometricOffsetTransform(FbxNode *pfbxNode)
{
	const FbxVector4 T = pfbxNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	const FbxVector4 R = pfbxNode->GetGeometricRotation(FbxNode::eSourcePivot);
	const FbxVector4 S = pfbxNode->GetGeometricScaling(FbxNode::eSourcePivot);

	return(FbxAMatrix(T, R, S));
}

FbxAMatrix ComputeClusterDeformation(FbxMesh *pfbxMesh, FbxCluster *pfbxCluster, FbxCluster::ELinkMode nClusterMode, FbxTime& fbxCurrentTime)
{
	FbxAMatrix fbxmtxVertexTransform;

	if (nClusterMode == FbxCluster::eNormalize) 
	{
		FbxAMatrix fbxmtxGeometryOffset = GetGeometricOffsetTransform(pfbxMesh->GetNode());

		FbxAMatrix fbxmtxBindPoseMeshToRoot; //Cluster Transform
		pfbxCluster->GetTransformMatrix(fbxmtxBindPoseMeshToRoot);

		FbxAMatrix fbxmtxBindPoseBoneToRoot; //Cluster Link Transform
		pfbxCluster->GetTransformLinkMatrix(fbxmtxBindPoseBoneToRoot);

		FbxAMatrix fbxmtxAnimatedBoneToRoot = pfbxCluster->GetLink()->EvaluateGlobalTransform(fbxCurrentTime); //Cluster Link Node Global Transform

		fbxmtxVertexTransform = fbxmtxAnimatedBoneToRoot * fbxmtxBindPoseBoneToRoot.Inverse() * fbxmtxBindPoseMeshToRoot * fbxmtxGeometryOffset;
	}
	else
	{ //FbxCluster::eAdditive
		if (pfbxCluster->GetAssociateModel())
		{
			FbxAMatrix fbxmtxAssociateModel;
			pfbxCluster->GetTransformAssociateModelMatrix(fbxmtxAssociateModel);

			FbxAMatrix fbxmtxAssociateGeometryOffset = GetGeometricOffsetTransform(pfbxCluster->GetAssociateModel());
			fbxmtxAssociateModel *= fbxmtxAssociateGeometryOffset;

			FbxAMatrix fbxmtxAssociateModelGlobal = pfbxCluster->GetAssociateModel()->EvaluateGlobalTransform(fbxCurrentTime);

			FbxAMatrix fbxmtxClusterTransform;
			pfbxCluster->GetTransformMatrix(fbxmtxClusterTransform);

			FbxAMatrix fbxmtxGeometryOffset = GetGeometricOffsetTransform(pfbxMesh->GetNode());
			fbxmtxClusterTransform *= fbxmtxGeometryOffset;

			FbxAMatrix fbxmtxClusterLinkTransform;
			pfbxCluster->GetTransformLinkMatrix(fbxmtxClusterLinkTransform);

			FbxAMatrix fbxmtxLinkGeometryOffset = GetGeometricOffsetTransform(pfbxCluster->GetLink());
			fbxmtxClusterLinkTransform *= fbxmtxLinkGeometryOffset;

			FbxAMatrix fbxmtxClusterLinkToRoot = pfbxCluster->GetLink()->EvaluateGlobalTransform(fbxCurrentTime);

			fbxmtxVertexTransform = fbxmtxClusterTransform.Inverse() * fbxmtxAssociateModel * fbxmtxAssociateModelGlobal.Inverse() * fbxmtxClusterLinkToRoot * fbxmtxClusterLinkTransform.Inverse() * fbxmtxClusterTransform;
		}
	}
	return(fbxmtxVertexTransform);
}

void ComputeLinearDeformation(FbxMesh *pfbxMesh, FbxTime& fbxCurrentTime, FbxVector4 *pfbxv4Vertices, int nVertices)
{
	FbxAMatrix *pfbxmtxClusterDeformations = new FbxAMatrix[nVertices];
	::memset(pfbxmtxClusterDeformations, 0, nVertices * sizeof(FbxAMatrix));

	double *pfSumOfClusterWeights = new double[nVertices];
	::memset(pfSumOfClusterWeights, 0, nVertices * sizeof(double));

	FbxCluster::ELinkMode nClusterMode = ((FbxSkin *)pfbxMesh->GetDeformer(0, FbxDeformer::eSkin))->GetCluster(0)->GetLinkMode();
	if (nClusterMode == FbxCluster::eAdditive)
	{
		for (int i = 0; i < nVertices; ++i) pfbxmtxClusterDeformations[i].SetIdentity();
	}

	int nSkinDeformers = pfbxMesh->GetDeformerCount(FbxDeformer::eSkin);
	for (int i = 0; i < nSkinDeformers; i++)
	{
		FbxSkin *pfbxSkinDeformer = (FbxSkin *)pfbxMesh->GetDeformer(i, FbxDeformer::eSkin);	
		int nClusters = pfbxSkinDeformer->GetClusterCount();

		for (int j = 0; j < nClusters; j++)
		{
			FbxCluster *pfbxCluster = pfbxSkinDeformer->GetCluster(j);
			if (!pfbxCluster->GetLink()) continue;

			FbxAMatrix fbxmtxClusterDeformation = ComputeClusterDeformation(pfbxMesh, pfbxCluster, nClusterMode, fbxCurrentTime);

			int *pnIndices = pfbxCluster->GetControlPointIndices();
			double *pfWeights = pfbxCluster->GetControlPointWeights();

			int nIndices = pfbxCluster->GetControlPointIndicesCount();
			for (int k = 0; k < nIndices; k++)
			{            
				int nIndex = pnIndices[k];
				double fWeight = pfWeights[k];
				if ((nIndex >= nVertices) || (fWeight == 0.0)) continue;

				FbxAMatrix fbxmtxInfluence = fbxmtxClusterDeformation;
				MatrixScale(fbxmtxInfluence, fWeight);

				if (nClusterMode == FbxCluster::eAdditive)
				{    
					MatrixAddToDiagonal(fbxmtxInfluence, 1.0 - fWeight);
					pfbxmtxClusterDeformations[nIndex] = fbxmtxInfluence * pfbxmtxClusterDeformations[nIndex];
					pfSumOfClusterWeights[nIndex] = 1.0;
				}
				else 
				{
					MatrixAdd(pfbxmtxClusterDeformations[nIndex], fbxmtxInfluence);
					pfSumOfClusterWeights[nIndex] += fWeight;
				}
			}			
		}
	}

	for (int i = 0; i < nVertices; i++) 
	{
		if (pfSumOfClusterWeights[i] != 0.0)
		{
			FbxVector4 fbxv4Vertex = pfbxv4Vertices[i];
			pfbxv4Vertices[i] = pfbxmtxClusterDeformations[i].MultT(fbxv4Vertex);
			if (nClusterMode == FbxCluster::eNormalize)
			{
				pfbxv4Vertices[i] /= pfSumOfClusterWeights[i];
			}
			else if (nClusterMode == FbxCluster::eTotalOne)
			{
				fbxv4Vertex *= (1.0 - pfSumOfClusterWeights[i]);
				pfbxv4Vertices[i] += fbxv4Vertex;
			}
		} 
	}

	delete[] pfbxmtxClusterDeformations;
	delete[] pfSumOfClusterWeights;
}

void ComputeDualQuaternionDeformation(FbxMesh *pfbxMesh, FbxTime& fbxCurrentTime, FbxVector4 *pfbxv4Vertices, int nVertices)
{
	FbxDualQuaternion *pfbxDQClusterDeformations = new FbxDualQuaternion[nVertices];
	memset(pfbxDQClusterDeformations, 0, nVertices * sizeof(FbxDualQuaternion));
	double *pfClusterWeights = new double[nVertices];
	memset(pfClusterWeights, 0, nVertices * sizeof(double));

	FbxCluster::ELinkMode nClusterMode = ((FbxSkin *)pfbxMesh->GetDeformer(0, FbxDeformer::eSkin))->GetCluster(0)->GetLinkMode();
	int nSkinDeformers = pfbxMesh->GetDeformerCount(FbxDeformer::eSkin);
	for (int i = 0; i < nSkinDeformers; i++)
	{
		FbxSkin *pfbxSkinDeformer = (FbxSkin *)pfbxMesh->GetDeformer(i, FbxDeformer::eSkin);
		int nClusters = pfbxSkinDeformer->GetClusterCount();
		for (int j = 0; j < nClusters; j++)
		{
			FbxCluster *pfbxCluster = pfbxSkinDeformer->GetCluster(j);
			if (!pfbxCluster->GetLink()) continue;

			FbxAMatrix fbxmtxCluster = ComputeClusterDeformation(pfbxMesh, pfbxCluster, nClusterMode, fbxCurrentTime);

			FbxQuaternion Q = fbxmtxCluster.GetQ();
			FbxVector4 T = fbxmtxCluster.GetT();
			FbxDualQuaternion fbxDualQuaternion(Q, T);

			int nIndices = pfbxCluster->GetControlPointIndicesCount();
			int *pnControlPointIndices = pfbxCluster->GetControlPointIndices();
			double *pfControlPointWeights = pfbxCluster->GetControlPointWeights();
			for (int k = 0; k < nIndices; ++k) 
			{ 
				int nIndex = pnControlPointIndices[k];
				if (nIndex >= nVertices) continue;

				double fWeight = pfControlPointWeights[k];
				if (fWeight == 0.0) continue;

				FbxDualQuaternion fbxmtxInfluence = fbxDualQuaternion * fWeight;
				if (nClusterMode == FbxCluster::eAdditive)
				{    
					pfbxDQClusterDeformations[nIndex] = fbxmtxInfluence;
					pfClusterWeights[nIndex] = 1.0;
				}
				else // FbxCluster::eNormalize || FbxCluster::eTotalOne
				{
					if (j == 0)
					{
						pfbxDQClusterDeformations[nIndex] = fbxmtxInfluence;
					}
					else
					{
						double fSign = pfbxDQClusterDeformations[nIndex].GetFirstQuaternion().DotProduct(fbxDualQuaternion.GetFirstQuaternion());
						if (fSign >= 0.0 )
						{
							pfbxDQClusterDeformations[nIndex] += fbxmtxInfluence;
						}
						else
						{
							pfbxDQClusterDeformations[nIndex] -= fbxmtxInfluence;
						}
					}
					pfClusterWeights[nIndex] += fWeight;
				}
			}
		}
	}

	for (int i = 0; i < nVertices; i++) 
	{
		FbxVector4 fbxv4SrcVertex = pfbxv4Vertices[i];
		double fWeightSum = pfClusterWeights[i];

		if (fWeightSum != 0.0) 
		{
			pfbxDQClusterDeformations[i].Normalize();
			pfbxv4Vertices[i] = pfbxDQClusterDeformations[i].Deform(pfbxv4Vertices[i]);

			if (nClusterMode == FbxCluster::eNormalize)
			{
				pfbxv4Vertices[i] /= fWeightSum;
			}
			else if (nClusterMode == FbxCluster::eTotalOne)
			{
				fbxv4SrcVertex *= (1.0 - fWeightSum);
				pfbxv4Vertices[i] += fbxv4SrcVertex;
			}
		} 
	}

	delete[] pfbxDQClusterDeformations;
	delete[] pfClusterWeights;
}

void ComputeSkinDeformation(FbxMesh *pfbxMesh, FbxTime& fbxCurrentTime, FbxVector4 *pfbxv4Vertices, int nVertices)
{
	FbxSkin *pfbxSkinDeformer = (FbxSkin *)pfbxMesh->GetDeformer(0, FbxDeformer::eSkin);
	FbxSkin::EType nSkinningType = pfbxSkinDeformer->GetSkinningType();

	if ((nSkinningType == FbxSkin::eLinear) || (nSkinningType == FbxSkin::eRigid))
	{
		ComputeLinearDeformation(pfbxMesh, fbxCurrentTime, pfbxv4Vertices, nVertices);
	}
	else if (nSkinningType == FbxSkin::eDualQuaternion)
	{
		ComputeDualQuaternionDeformation(pfbxMesh, fbxCurrentTime, pfbxv4Vertices, nVertices);
	}
	else if (nSkinningType == FbxSkin::eBlend)
	{
		FbxVector4 *pfbxv4LinearVertices = new FbxVector4[nVertices];
		memcpy(pfbxv4LinearVertices, pfbxMesh->GetControlPoints(), nVertices * sizeof(FbxVector4));
		ComputeLinearDeformation(pfbxMesh, fbxCurrentTime, pfbxv4LinearVertices, nVertices);

		FbxVector4 *pfbxv4DQVertices = new FbxVector4[nVertices];
		memcpy(pfbxv4DQVertices, pfbxMesh->GetControlPoints(), nVertices * sizeof(FbxVector4));
		ComputeDualQuaternionDeformation(pfbxMesh, fbxCurrentTime, pfbxv4DQVertices, nVertices);

		int nBlendWeights = pfbxSkinDeformer->GetControlPointIndicesCount();
		double *pfControlPointBlendWeights = pfbxSkinDeformer->GetControlPointBlendWeights();
		for (int i = 0; i < nBlendWeights; i++)
		{
			pfbxv4Vertices[i] = pfbxv4DQVertices[i] * pfControlPointBlendWeights[i] + pfbxv4LinearVertices[i] * (1 - pfControlPointBlendWeights[i]);
		}

		delete[] pfbxv4LinearVertices;
		delete[] pfbxv4DQVertices;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
void AnimateFbxMesh(FbxMesh *pfbxMesh, FbxTime& fbxCurrentTime)
{
	int nVertices = pfbxMesh->GetControlPointsCount();
	if (nVertices > 0)
	{
		FbxVector4 *pfbxv4Vertices = new FbxVector4[nVertices];
		::memcpy(pfbxv4Vertices, pfbxMesh->GetControlPoints(), nVertices * sizeof(FbxVector4));

		int nSkinDeformers = pfbxMesh->GetDeformerCount(FbxDeformer::eSkin);
		if (nSkinDeformers > 0) ::ComputeSkinDeformation(pfbxMesh, fbxCurrentTime, pfbxv4Vertices, nVertices);

		CFbxRenderInfo *pFbxRenderInfo = (CFbxRenderInfo *)pfbxMesh->GetUserDataPtr();
		if (pFbxRenderInfo->m_pMesh)
		{
			for (int i = 0; i < nVertices; i++) pFbxRenderInfo->m_pMesh->m_pxmf4MappedPositions[i] = XMFLOAT4((float)pfbxv4Vertices[i][0], (float)pfbxv4Vertices[i][1], (float)pfbxv4Vertices[i][2], 1.0f);
		}

		delete[] pfbxv4Vertices;
	}
}

void AnimateFbxNodeHierarchy(FbxNode *pfbxNode, FbxTime& fbxCurrentTime)
{
	FbxNodeAttribute *pfbxNodeAttribute = pfbxNode->GetNodeAttribute();
	if (pfbxNodeAttribute && (pfbxNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh))
	{
		FbxMesh *pfbxMesh = pfbxNode->GetMesh();
		AnimateFbxMesh(pfbxMesh, fbxCurrentTime);
	}

	int nChilds = pfbxNode->GetChildCount();
	for (int i = 0; i < nChilds; i++) AnimateFbxNodeHierarchy(pfbxNode->GetChild(i), fbxCurrentTime);
}

void RenderFbxMesh(ID3D12GraphicsCommandList *pd3dCommandList, FbxMesh *pfbxMesh, FbxAMatrix& fbxmtxNodeToRoot, FbxAMatrix& fbxmtxGeometryOffset, FbxAMatrix fbxmtxWorld)
{
	int nVertices = pfbxMesh->GetControlPointsCount();
	if (nVertices > 0)
	{
		FbxAMatrix fbxmtxTransform = fbxmtxWorld;
		int nSkinDeformers = pfbxMesh->GetDeformerCount(FbxDeformer::eSkin);
		if (nSkinDeformers == 0) fbxmtxTransform = fbxmtxWorld * fbxmtxNodeToRoot * fbxmtxGeometryOffset;

		CGameObject::UpdateShaderVariable(pd3dCommandList, &fbxmtxTransform);

		CFbxRenderInfo *pFbxRenderInfo = (CFbxRenderInfo *)pfbxMesh->GetUserDataPtr();
		if (pFbxRenderInfo->m_pShader) pFbxRenderInfo->m_pShader->Render(pd3dCommandList, NULL);
		if (pFbxRenderInfo->m_pMesh) pFbxRenderInfo->m_pMesh->Render(pd3dCommandList);
	}
}

void RenderFbxNodeHierarchy(ID3D12GraphicsCommandList *pd3dCommandList, FbxNode *pfbxNode, FbxTime& fbxCurrentTime, FbxAMatrix& fbxmtxWorld)
{
	FbxNodeAttribute *pfbxNodeAttribute = pfbxNode->GetNodeAttribute();
	if (pfbxNodeAttribute && (pfbxNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh))
	{
		FbxAMatrix fbxmtxNodeToRoot = pfbxNode->EvaluateGlobalTransform(fbxCurrentTime);
		FbxAMatrix fbxmtxGeometryOffset = GetGeometricOffsetTransform(pfbxNode);

		FbxMesh *pfbxMesh = pfbxNode->GetMesh();
		RenderFbxMesh(pd3dCommandList, pfbxMesh, fbxmtxNodeToRoot, fbxmtxGeometryOffset, fbxmtxWorld);
	}

	int nChilds = pfbxNode->GetChildCount();
	for (int i = 0; i < nChilds; i++) RenderFbxNodeHierarchy(pd3dCommandList, pfbxNode->GetChild(i), fbxCurrentTime, fbxmtxWorld);
}

void CreateMeshFromFbxNodeHierarchy(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, FbxNode *pfbxNode)
{
	FbxNodeAttribute *pfbxNodeAttribute = pfbxNode->GetNodeAttribute();
	if (pfbxNodeAttribute && (pfbxNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh))
	{
		FbxMesh *pfbxMesh = pfbxNode->GetMesh();
		if (pfbxMesh)
		{
			int nVertices = pfbxMesh->GetControlPointsCount();

			int nIndices = 0;
			int nPolygons = pfbxMesh->GetPolygonCount();
			for (int i = 0; i < nPolygons; i++) nIndices += pfbxMesh->GetPolygonSize(i); //Triangle: 3, Triangulate(), nIndices = nPolygons * 3

			int *pnIndices = new int[nIndices];
			for (int i = 0, k = 0; i < nPolygons; i++)
			{
				int nPolygonSize = pfbxMesh->GetPolygonSize(i); 
				for (int j = 0; j < nPolygonSize; j++) pnIndices[k++] = pfbxMesh->GetPolygonVertex(i, j);
			}

			CFbxRenderInfo *pFbxRenderInfo = new CFbxRenderInfo();
			pFbxRenderInfo->m_pMesh = new CMeshFromFbx(pd3dDevice, pd3dCommandList, nVertices, nIndices, pnIndices);
			int nSkinDeformers = pfbxMesh->GetDeformerCount(FbxDeformer::eSkin);
			if (nSkinDeformers > 0)
				pFbxRenderInfo->m_pShader = new CFbxSkinnedModelShader();
			else
				pFbxRenderInfo->m_pShader = new CFbxModelShader();
			pFbxRenderInfo->m_pShader->CreateShader(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature);

			pfbxMesh->SetUserDataPtr(pFbxRenderInfo);

			if(pnIndices) delete[] pnIndices;
		}
	}

	int nChilds = pfbxNode->GetChildCount();
	for (int i = 0; i < nChilds; i++) CreateMeshFromFbxNodeHierarchy(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, pfbxNode->GetChild(i));
}

void ReleaseMeshFromFbxNodeHierarchy(FbxNode *pfbxNode)
{
	FbxNodeAttribute *pfbxNodeAttribute = pfbxNode->GetNodeAttribute();
	if (pfbxNodeAttribute && (pfbxNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh))
	{
		FbxMesh *pfbxMesh = pfbxNode->GetMesh();
		if (pfbxMesh) 
		{
			CFbxRenderInfo *pFbxRenderInfo = (CFbxRenderInfo *)pfbxMesh->GetUserDataPtr();
			if (pFbxRenderInfo->m_pMesh) pFbxRenderInfo->m_pMesh->Release();
			if (pFbxRenderInfo->m_pShader) pFbxRenderInfo->m_pShader->Release();
		}
	}

	int nChilds = pfbxNode->GetChildCount();
	for (int i = 0; i < nChilds; i++) ReleaseMeshFromFbxNodeHierarchy(pfbxNode->GetChild(i));
}

void ReleaseUploadBufferFromFbxNodeHierarchy(FbxNode *pfbxNode)
{
	FbxNodeAttribute *pfbxNodeAttribute = pfbxNode->GetNodeAttribute();
	if (pfbxNodeAttribute && (pfbxNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh))
	{
		FbxMesh *pfbxMesh = pfbxNode->GetMesh();
		if (pfbxMesh) 
		{
			CFbxRenderInfo *pFbxRenderInfo = (CFbxRenderInfo *)pfbxMesh->GetUserDataPtr();
			if (pFbxRenderInfo->m_pMesh) pFbxRenderInfo->m_pMesh->ReleaseUploadBuffers();
			if (pFbxRenderInfo->m_pShader) pFbxRenderInfo->m_pShader->ReleaseUploadBuffers();
		}
	}

	int nChilds = pfbxNode->GetChildCount();
	for (int i = 0; i < nChilds; i++) ReleaseUploadBufferFromFbxNodeHierarchy(pfbxNode->GetChild(i));
}

FbxScene *LoadFbxSceneFromFile(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, FbxManager *pfbxSdkManager, char *pstrFbxFileName)
{
	FbxScene *pfbxScene = NULL;

	int nSDKMajor, nSDKMinor, nSDKRevision;
	FbxManager::GetFileFormatVersion(nSDKMajor, nSDKMinor, nSDKRevision);

	FbxIOSettings *pfbxIOSettings = pfbxSdkManager->GetIOSettings();
	FbxImporter *pfbxImporter = FbxImporter::Create(pfbxSdkManager, " ");
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
