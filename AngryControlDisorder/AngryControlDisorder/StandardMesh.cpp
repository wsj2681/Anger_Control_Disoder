#include "framework.h"
#include "StandartMesh.h"

StandartMesh::StandartMesh()
{
}

StandartMesh::StandartMesh(ID3D12Device* device, ID3D12GraphicsCommandList* commandList)
{
	SAFE_RELEASE(textureCoord0Buffer);
	SAFE_RELEASE(normalBuffer);
	SAFE_RELEASE(tangentBuffer);
	SAFE_RELEASE(biTangentBuffer);

	DELETE_ARRAY(colors);
	DELETE_ARRAY(normals);
	DELETE_ARRAY(tangents);
	DELETE_ARRAY(biTangents);
	DELETE_ARRAY(textureCoords0);
	DELETE_ARRAY(textureCoords1);
}

StandartMesh::~StandartMesh()
{
}

void StandartMesh::LoadMeshFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, FILE* pInFile)
{
	char pstrToken[64] = { '\0' };
	int nPositions = 0, nColors = 0, nNormals = 0, nTangents = 0, nBiTangents = 0, nTextureCoords = 0, nIndices = 0, nSubMeshes = 0, nSubIndices = 0;

	UINT nReads = (UINT)::fread(&vertices, sizeof(int), 1, pInFile);

	::ReadStringFromFile(pInFile, meshName);

	for (; ; )
	{
		::ReadStringFromFile(pInFile, pstrToken);
		if (!strcmp(pstrToken, "<Bounds>:"))
		{
			nReads = (UINT)::fread(&AABBCenter, sizeof(XMFLOAT3), 1, pInFile);
			nReads = (UINT)::fread(&AABBExtents, sizeof(XMFLOAT3), 1, pInFile);
		}
		else if (!strcmp(pstrToken, "<Positions>:"))
		{
			nReads = (UINT)::fread(&nPositions, sizeof(int), 1, pInFile);
			if (nPositions > 0)
			{
				type |= VERTEXT_POSITION;
				positions = new XMFLOAT3[nPositions];
				nReads = (UINT)::fread(positions, sizeof(XMFLOAT3), nPositions, pInFile);

				positionBuffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList, positions, sizeof(XMFLOAT3) * vertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &positionUploadBuffer);

				positionBufferView.BufferLocation = positionBuffer->GetGPUVirtualAddress();
				positionBufferView.StrideInBytes = sizeof(XMFLOAT3);
				positionBufferView.SizeInBytes = sizeof(XMFLOAT3) * vertices;
			}
		}
		else if (!strcmp(pstrToken, "<Colors>:"))
		{
			nReads = (UINT)::fread(&nColors, sizeof(int), 1, pInFile);
			if (nColors > 0)
			{
				type |= VERTEXT_COLOR;
				colors = new XMFLOAT4[nColors];
				nReads = (UINT)::fread(colors, sizeof(XMFLOAT4), nColors, pInFile);
			}
		}
		else if (!strcmp(pstrToken, "<TextureCoords0>:"))
		{
			nReads = (UINT)::fread(&nTextureCoords, sizeof(int), 1, pInFile);
			if (nTextureCoords > 0)
			{
				type |= VERTEXT_TEXTURE_COORD0;
				textureCoords0 = new XMFLOAT2[nTextureCoords];
				nReads = (UINT)::fread(textureCoords0, sizeof(XMFLOAT2), nTextureCoords, pInFile);

				textureCoord0Buffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList, textureCoords0, sizeof(XMFLOAT2) * vertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &textureCoord0UploadBuffer);

				textureCoord0BufferView.BufferLocation = textureCoord0Buffer->GetGPUVirtualAddress();
				textureCoord0BufferView.StrideInBytes = sizeof(XMFLOAT2);
				textureCoord0BufferView.SizeInBytes = sizeof(XMFLOAT2) * vertices;
			}
		}
		else if (!strcmp(pstrToken, "<TextureCoords1>:"))
		{
			nReads = (UINT)::fread(&nTextureCoords, sizeof(int), 1, pInFile);
			if (nTextureCoords > 0)
			{
				type |= VERTEXT_TEXTURE_COORD1;
				textureCoords1 = new XMFLOAT2[nTextureCoords];
				nReads = (UINT)::fread(textureCoords1, sizeof(XMFLOAT2), nTextureCoords, pInFile);

				textureCoord1Buffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList, textureCoords1, sizeof(XMFLOAT2) * vertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &textureCoord1UploadBuffer);

				textureCoord1BufferView.BufferLocation = textureCoord1Buffer->GetGPUVirtualAddress();
				textureCoord1BufferView.StrideInBytes = sizeof(XMFLOAT2);
				textureCoord1BufferView.SizeInBytes = sizeof(XMFLOAT2) * vertices;
			}
		}
		else if (!strcmp(pstrToken, "<Normals>:"))
		{
			nReads = (UINT)::fread(&nNormals, sizeof(int), 1, pInFile);
			if (nNormals > 0)
			{
				type |= VERTEXT_NORMAL;
				normals = new XMFLOAT3[nNormals];
				nReads = (UINT)::fread(normals, sizeof(XMFLOAT3), nNormals, pInFile);

				normalBuffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList, normals, sizeof(XMFLOAT3) * vertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &normalUploadBuffer);

				normalBufferView.BufferLocation = normalBuffer->GetGPUVirtualAddress();
				normalBufferView.StrideInBytes = sizeof(XMFLOAT3);
				normalBufferView.SizeInBytes = sizeof(XMFLOAT3) * vertices;
			}
		}
		else if (!strcmp(pstrToken, "<Tangents>:"))
		{
			nReads = (UINT)::fread(&nTangents, sizeof(int), 1, pInFile);
			if (nTangents > 0)
			{
				type |= VERTEXT_TANGENT;
				tangents = new XMFLOAT3[nTangents];
				nReads = (UINT)::fread(tangents, sizeof(XMFLOAT3), nTangents, pInFile);

				tangentBuffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList, tangents, sizeof(XMFLOAT3) * vertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &tangentUploadBuffer);

				tangentBufferView.BufferLocation = tangentBuffer->GetGPUVirtualAddress();
				tangentBufferView.StrideInBytes = sizeof(XMFLOAT3);
				tangentBufferView.SizeInBytes = sizeof(XMFLOAT3) * vertices;
			}
		}
		else if (!strcmp(pstrToken, "<BiTangents>:"))
		{
			nReads = (UINT)::fread(&nBiTangents, sizeof(int), 1, pInFile);
			if (nBiTangents > 0)
			{
				biTangents = new XMFLOAT3[nBiTangents];
				nReads = (UINT)::fread(biTangents, sizeof(XMFLOAT3), nBiTangents, pInFile);

				biTangentBuffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList, biTangents, sizeof(XMFLOAT3) * vertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &biTangentUploadBuffer);

				biTangentBufferView.BufferLocation = biTangentBuffer->GetGPUVirtualAddress();
				biTangentBufferView.StrideInBytes = sizeof(XMFLOAT3);
				biTangentBufferView.SizeInBytes = sizeof(XMFLOAT3) * vertices;
			}
		}
		else if (!strcmp(pstrToken, "<SubMeshes>:"))
		{
			nReads = (UINT)::fread(&(subSetIndices), sizeof(int), 1, pInFile);
			if (subMeshes > 0)
			{
				subSetIndexCount = new int[subMeshes];
				subSetIndices = new UINT * [subMeshes];

				subSetIndexBuffers = new ID3D12Resource * [subMeshes];
				subSetIndexUploadBuffers = new ID3D12Resource * [subMeshes];
				subSetIndexBufferViews = new D3D12_INDEX_BUFFER_VIEW[subMeshes];

				for (int i = 0; i < subMeshes; i++)
				{
					::ReadStringFromFile(pInFile, pstrToken);
					if (!strcmp(pstrToken, "<SubMesh>:"))
					{
						int nIndex = 0;
						nReads = (UINT)::fread(&nIndex, sizeof(int), 1, pInFile); //i
						nReads = (UINT)::fread(&(subSetIndices[i]), sizeof(int), 1, pInFile);
						if (subSetIndexCount[i] > 0)
						{
							subSetIndices[i] = new UINT[subSetIndexCount[i]];
							nReads = (UINT)::fread(subSetIndices[i], sizeof(UINT), subSetIndexCount[i], pInFile);

							subSetIndexBuffers[i] = ::CreateBufferResource(pd3dDevice, pd3dCommandList, subSetIndices[i], sizeof(UINT) * subSetIndexCount[i], D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_INDEX_BUFFER, &subSetIndexUploadBuffers[i]);

							subSetIndexBufferViews[i].BufferLocation = subSetIndexBuffers[i]->GetGPUVirtualAddress();
							subSetIndexBufferViews[i].Format = DXGI_FORMAT_R32_UINT;
							subSetIndexBufferViews[i].SizeInBytes = sizeof(UINT) * subSetIndexCount[i];
						}
					}
				}
			}
		}
		else if (!strcmp(pstrToken, "</Mesh>"))
		{
			break;
		}
	}
}

void StandartMesh::ReleaseUploadBuffers()
{
	SAFE_RELEASE(textureCoord0UploadBuffer);
	SAFE_RELEASE(normalUploadBuffer);
	SAFE_RELEASE(tangentUploadBuffer);
	SAFE_RELEASE(biTangentUploadBuffer);
}

void StandartMesh::OnPreRender(ID3D12GraphicsCommandList* pd3dCommandList, void* pContext)
{
}
