#pragma once
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include <fbxsdk.h>

using namespace DirectX;

namespace Vector3
{

	XMFLOAT3 XMVectorToFloat3(const XMVECTOR& xmvVector)
	{
		XMFLOAT3 xmf3Result;
		XMStoreFloat3(&xmf3Result, xmvVector);
		return xmf3Result;
	}

	XMFLOAT3 ScalarProduct(const XMFLOAT3& xmf3Vector, float fScalar, bool bNormalize)
	{
		XMFLOAT3 xmf3Result;
		if (bNormalize) XMStoreFloat3(&xmf3Result, XMVector3Normalize(XMLoadFloat3(&xmf3Vector)) * fScalar);
		else XMStoreFloat3(&xmf3Result, XMLoadFloat3(&xmf3Vector) * fScalar);
		return xmf3Result;
	}

	XMFLOAT3 Add(const XMFLOAT3& xmf3Vector1, const XMFLOAT3& xmf3Vector2)
	{
		XMFLOAT3 xmf3Result;
		XMStoreFloat3(&xmf3Result, XMLoadFloat3(&xmf3Vector1) + XMLoadFloat3(&xmf3Vector2));
		return(xmf3Result);
	}

	XMFLOAT3 Add(const XMFLOAT3& xmf3Vector1, const XMFLOAT3& xmf3Vector2, float fScalar)
	{
		XMFLOAT3 xmf3Result;
		XMStoreFloat3(&xmf3Result, XMLoadFloat3(&xmf3Vector1) + (XMLoadFloat3(&xmf3Vector2) * fScalar));
		return(xmf3Result);
	}

	XMFLOAT3 Subtract(const XMFLOAT3& xmf3Vector1, const XMFLOAT3& xmf3Vector2)
	{
		XMFLOAT3 xmf3Result;
		XMStoreFloat3(&xmf3Result, XMLoadFloat3(&xmf3Vector1) - XMLoadFloat3(&xmf3Vector2));
		return(xmf3Result);
	}

	float DotProduct(const XMFLOAT3& xmf3Vector1, const XMFLOAT3& xmf3Vector2)
	{
		XMFLOAT3 xmf3Result;
		XMStoreFloat3(&xmf3Result, XMVector3Dot(XMLoadFloat3(&xmf3Vector1), XMLoadFloat3(&xmf3Vector2)));
		return(xmf3Result.x);
	}

	XMFLOAT3 CrossProduct(const XMFLOAT3& xmf3Vector1, const XMFLOAT3& xmf3Vector2, bool bNormalize)
	{
		XMFLOAT3 xmf3Result;
		if (bNormalize)
			XMStoreFloat3(&xmf3Result, XMVector3Normalize(XMVector3Cross(XMLoadFloat3(&xmf3Vector1), XMLoadFloat3(&xmf3Vector2))));
		else
			XMStoreFloat3(&xmf3Result, XMVector3Cross(XMLoadFloat3(&xmf3Vector1), XMLoadFloat3(&xmf3Vector2)));
		return(xmf3Result);
	}

	XMFLOAT3 Normalize(const XMFLOAT3& xmf3Vector)
	{
		XMFLOAT3 m_xmf3Normal;
		XMStoreFloat3(&m_xmf3Normal, XMVector3Normalize(XMLoadFloat3(&xmf3Vector)));
		return(m_xmf3Normal);
	}

	float Length(const XMFLOAT3& xmf3Vector)
	{
		XMFLOAT3 xmf3Result;
		XMStoreFloat3(&xmf3Result, XMVector3Length(XMLoadFloat3(&xmf3Vector)));
		return(xmf3Result.x);
	}

	float Angle(const XMVECTOR& xmvVector1, const XMVECTOR& xmvVector2)
	{
		XMVECTOR xmvAngle = XMVector3AngleBetweenNormals(xmvVector1, xmvVector2);
		return(XMConvertToDegrees(acosf(XMVectorGetX(xmvAngle))));
	}

	float Angle(const XMFLOAT3& xmf3Vector1, const XMFLOAT3& xmf3Vector2)
	{
		return(Angle(XMLoadFloat3(&xmf3Vector1), XMLoadFloat3(&xmf3Vector2)));
	}

	XMFLOAT3 TransformNormal(const XMFLOAT3& xmf3Vector, const XMMATRIX& xmmtxTransform)
	{
		XMFLOAT3 xmf3Result;
		XMStoreFloat3(&xmf3Result, XMVector3TransformNormal(XMLoadFloat3(&xmf3Vector), xmmtxTransform));
		return(xmf3Result);
	}

	XMFLOAT3 TransformCoord(const XMFLOAT3& xmf3Vector, const XMMATRIX& xmmtxTransform)
	{
		XMFLOAT3 xmf3Result;
		XMStoreFloat3(&xmf3Result, XMVector3TransformCoord(XMLoadFloat3(&xmf3Vector), xmmtxTransform));
		return(xmf3Result);
	}

	XMFLOAT3 TransformCoord(const XMFLOAT3& xmf3Vector, const XMFLOAT4X4& xmmtx4x4Matrix)
	{
		return(TransformCoord(xmf3Vector, XMLoadFloat4x4(&xmmtx4x4Matrix)));
	}
}

namespace Vector4
{
	XMFLOAT4 Add(const XMFLOAT4& xmf4Vector1, const XMFLOAT4& xmf4Vector2)
	{
		XMFLOAT4 xmf4Result;
		XMStoreFloat4(&xmf4Result, XMLoadFloat4(&xmf4Vector1) + XMLoadFloat4(&xmf4Vector2));
		return(xmf4Result);
	}

	XMFLOAT4 Multiply(const XMFLOAT4& xmf4Vector1, const XMFLOAT4& xmf4Vector2)
	{
		XMFLOAT4 xmf4Result;
		XMStoreFloat4(&xmf4Result, XMLoadFloat4(&xmf4Vector1) * XMLoadFloat4(&xmf4Vector2));
		return(xmf4Result);
	}

	XMFLOAT4 Multiply(float fScalar, const XMFLOAT4& xmf4Vector)
	{
		XMFLOAT4 xmf4Result;
		XMStoreFloat4(&xmf4Result, fScalar * XMLoadFloat4(&xmf4Vector));
		return(xmf4Result);
	}
}

namespace Matrix4x4
{
	XMFLOAT4X4 Identity()
	{
		XMFLOAT4X4 xmmtx4x4Result;
		XMStoreFloat4x4(&xmmtx4x4Result, XMMatrixIdentity());
		return(xmmtx4x4Result);
	}

	XMFLOAT4X4 Multiply(const XMFLOAT4X4& xmmtx4x4Matrix1, const XMFLOAT4X4& xmmtx4x4Matrix2)
	{
		XMFLOAT4X4 xmmtx4x4Result;
		XMStoreFloat4x4(&xmmtx4x4Result, XMLoadFloat4x4(&xmmtx4x4Matrix1) * XMLoadFloat4x4(&xmmtx4x4Matrix2));
		return(xmmtx4x4Result);
	}

	XMFLOAT4X4 Multiply(const XMFLOAT4X4& xmmtx4x4Matrix1, const XMMATRIX& xmmtxMatrix2)
	{
		XMFLOAT4X4 xmmtx4x4Result;
		XMStoreFloat4x4(&xmmtx4x4Result, XMLoadFloat4x4(&xmmtx4x4Matrix1) * xmmtxMatrix2);
		return(xmmtx4x4Result);
	}

	XMFLOAT4X4 Multiply(const XMMATRIX& xmmtxMatrix1, const XMFLOAT4X4& xmmtx4x4Matrix2)
	{
		XMFLOAT4X4 xmmtx4x4Result;
		XMStoreFloat4x4(&xmmtx4x4Result, xmmtxMatrix1 * XMLoadFloat4x4(&xmmtx4x4Matrix2));
		return(xmmtx4x4Result);
	}

	XMFLOAT4X4 Inverse(const XMFLOAT4X4& xmmtx4x4Matrix)
	{
		XMFLOAT4X4 xmmtx4x4Result;
		XMStoreFloat4x4(&xmmtx4x4Result, XMMatrixInverse(NULL, XMLoadFloat4x4(&xmmtx4x4Matrix)));
		return(xmmtx4x4Result);
	}

	XMFLOAT4X4 Transpose(const XMFLOAT4X4& xmmtx4x4Matrix)
	{
		XMFLOAT4X4 xmmtx4x4Result;
		XMStoreFloat4x4(&xmmtx4x4Result, XMMatrixTranspose(XMLoadFloat4x4(&xmmtx4x4Matrix)));
		return(xmmtx4x4Result);
	}

	XMFLOAT4X4 PerspectiveFovLH(float FovAngleY, float AspectRatio, float NearZ, float FarZ)
	{
		XMFLOAT4X4 xmmtx4x4Result;
		XMStoreFloat4x4(&xmmtx4x4Result, XMMatrixPerspectiveFovLH(FovAngleY, AspectRatio, NearZ, FarZ));
		return(xmmtx4x4Result);
	}

	XMFLOAT4X4 LookAtLH(const XMFLOAT3& xmf3EyePosition, const XMFLOAT3& xmf3LookAtPosition, const XMFLOAT3& xmf3UpDirection)
	{
		XMFLOAT4X4 xmmtx4x4Result;
		XMStoreFloat4x4(&xmmtx4x4Result, XMMatrixLookAtLH(XMLoadFloat3(&xmf3EyePosition), XMLoadFloat3(&xmf3LookAtPosition), XMLoadFloat3(&xmf3UpDirection)));
		return(xmmtx4x4Result);
	}
}

namespace Triangle
{
	bool Intersect(const XMFLOAT3& xmf3RayPosition, const XMFLOAT3& xmf3RayDirection, const XMFLOAT3& v0, const XMFLOAT3& v1, const XMFLOAT3& v2, float& fHitDistance)
	{
		return(TriangleTests::Intersects(XMLoadFloat3(&xmf3RayPosition), XMLoadFloat3(&xmf3RayDirection), XMLoadFloat3(&v0), XMLoadFloat3(&v1), XMLoadFloat3(&v2), fHitDistance));
	}
}

namespace Plane
{
	XMFLOAT4 Normalize(const XMFLOAT4& xmf4Plane)
	{
		XMFLOAT4 xmf4Result;
		XMStoreFloat4(&xmf4Result, XMPlaneNormalize(XMLoadFloat4(&xmf4Plane)));
		return(xmf4Result);
	}
}

//FBX Math

XMFLOAT4X4 FbxMatrixToXmFloat4x4Matrix(FbxAMatrix* pfbxmtxSource)
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

	return xmf4x4Result;
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

FbxAMatrix GetGeometricOffsetTransform(FbxNode* pfbxNode)
{
	const FbxVector4 T = pfbxNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	const FbxVector4 R = pfbxNode->GetGeometricRotation(FbxNode::eSourcePivot);
	const FbxVector4 S = pfbxNode->GetGeometricScaling(FbxNode::eSourcePivot);

	return(FbxAMatrix(T, R, S));
}

FbxAMatrix ComputeClusterDeformation(FbxMesh* pfbxMesh, FbxCluster* pfbxCluster, FbxCluster::ELinkMode nClusterMode, FbxTime& fbxCurrentTime)
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

void ComputeLinearDeformation(FbxMesh* pfbxMesh, FbxTime& fbxCurrentTime, FbxVector4* pfbxv4Vertices, int nVertices)
{
	FbxAMatrix* pfbxmtxClusterDeformations = new FbxAMatrix[nVertices];
	::memset(pfbxmtxClusterDeformations, 0, nVertices * sizeof(FbxAMatrix));

	double* pfSumOfClusterWeights = new double[nVertices];
	::memset(pfSumOfClusterWeights, 0, nVertices * sizeof(double));

	FbxCluster::ELinkMode nClusterMode = ((FbxSkin*)pfbxMesh->GetDeformer(0, FbxDeformer::eSkin))->GetCluster(0)->GetLinkMode();
	if (nClusterMode == FbxCluster::eAdditive)
	{
		for (int i = 0; i < nVertices; ++i) pfbxmtxClusterDeformations[i].SetIdentity();
	}

	int nSkinDeformers = pfbxMesh->GetDeformerCount(FbxDeformer::eSkin);
	for (int i = 0; i < nSkinDeformers; i++)
	{
		FbxSkin* pfbxSkinDeformer = (FbxSkin*)pfbxMesh->GetDeformer(i, FbxDeformer::eSkin);
		int nClusters = pfbxSkinDeformer->GetClusterCount();

		for (int j = 0; j < nClusters; j++)
		{
			FbxCluster* pfbxCluster = pfbxSkinDeformer->GetCluster(j);
			if (!pfbxCluster->GetLink()) continue;

			FbxAMatrix fbxmtxClusterDeformation = ComputeClusterDeformation(pfbxMesh, pfbxCluster, nClusterMode, fbxCurrentTime);

			int* pnIndices = pfbxCluster->GetControlPointIndices();
			double* pfWeights = pfbxCluster->GetControlPointWeights();

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

void ComputeDualQuaternionDeformation(FbxMesh* pfbxMesh, FbxTime& fbxCurrentTime, FbxVector4* pfbxv4Vertices, int nVertices)
{
	FbxDualQuaternion* pfbxDQClusterDeformations = new FbxDualQuaternion[nVertices];
	memset(pfbxDQClusterDeformations, 0, nVertices * sizeof(FbxDualQuaternion));
	double* pfClusterWeights = new double[nVertices];
	memset(pfClusterWeights, 0, nVertices * sizeof(double));

	FbxCluster::ELinkMode nClusterMode = ((FbxSkin*)pfbxMesh->GetDeformer(0, FbxDeformer::eSkin))->GetCluster(0)->GetLinkMode();
	int nSkinDeformers = pfbxMesh->GetDeformerCount(FbxDeformer::eSkin);
	for (int i = 0; i < nSkinDeformers; i++)
	{
		FbxSkin* pfbxSkinDeformer = (FbxSkin*)pfbxMesh->GetDeformer(i, FbxDeformer::eSkin);
		int nClusters = pfbxSkinDeformer->GetClusterCount();
		for (int j = 0; j < nClusters; j++)
		{
			FbxCluster* pfbxCluster = pfbxSkinDeformer->GetCluster(j);
			if (!pfbxCluster->GetLink()) continue;

			FbxAMatrix fbxmtxCluster = ComputeClusterDeformation(pfbxMesh, pfbxCluster, nClusterMode, fbxCurrentTime);

			FbxQuaternion Q = fbxmtxCluster.GetQ();
			FbxVector4 T = fbxmtxCluster.GetT();
			FbxDualQuaternion fbxDualQuaternion(Q, T);

			int nIndices = pfbxCluster->GetControlPointIndicesCount();
			int* pnControlPointIndices = pfbxCluster->GetControlPointIndices();
			double* pfControlPointWeights = pfbxCluster->GetControlPointWeights();
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
						if (fSign >= 0.0)
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

void ComputeSkinDeformation(FbxMesh* pfbxMesh, FbxTime& fbxCurrentTime, FbxVector4* pfbxv4Vertices, int nVertices)
{
	FbxSkin* pfbxSkinDeformer = (FbxSkin*)pfbxMesh->GetDeformer(0, FbxDeformer::eSkin);
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
		FbxVector4* pfbxv4LinearVertices = new FbxVector4[nVertices];
		memcpy(pfbxv4LinearVertices, pfbxMesh->GetControlPoints(), nVertices * sizeof(FbxVector4));
		ComputeLinearDeformation(pfbxMesh, fbxCurrentTime, pfbxv4LinearVertices, nVertices);

		FbxVector4* pfbxv4DQVertices = new FbxVector4[nVertices];
		memcpy(pfbxv4DQVertices, pfbxMesh->GetControlPoints(), nVertices * sizeof(FbxVector4));
		ComputeDualQuaternionDeformation(pfbxMesh, fbxCurrentTime, pfbxv4DQVertices, nVertices);

		int nBlendWeights = pfbxSkinDeformer->GetControlPointIndicesCount();
		double* pfControlPointBlendWeights = pfbxSkinDeformer->GetControlPointBlendWeights();
		for (int i = 0; i < nBlendWeights; i++)
		{
			pfbxv4Vertices[i] = pfbxv4DQVertices[i] * pfControlPointBlendWeights[i] + pfbxv4LinearVertices[i] * (1 - pfControlPointBlendWeights[i]);
		}

		delete[] pfbxv4LinearVertices;
		delete[] pfbxv4DQVertices;
	}
}
