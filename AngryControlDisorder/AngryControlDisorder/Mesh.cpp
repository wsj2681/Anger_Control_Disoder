#include "framework.h"
#include "Mesh.h"

Mesh::Mesh()
{
	memset(meshName, NULL, sizeof(meshName));
}

Mesh::Mesh(ID3D12Device* device, ID3D12GraphicsCommandList* commandList)
{

}

Mesh::~Mesh()
{
	SAFE_RELEASE(positionBuffer);
	
	if (subMeshes > 0)
	{
		for (int i = 0; i < subMeshes; ++i)
		{
			SAFE_RELEASE(subSetIndexBuffers[i]);
			if (subSetIndices[i])
			{
				delete[] subSetIndices[i];
			}
		}

		if (subSetIndexBuffers) delete[] subSetIndexBuffers;
		if (subSetIndexBufferViews) delete[] subSetIndexBufferViews;

		if (subSetIndiceCount) delete[] subSetIndiceCount;
		if (subSetIndices) delete[] subSetIndices;

	}

	if (positions) delete[] positions;
}

void Mesh::AddRef()
{
	this->references++;
}

void Mesh::Release()
{
	if (--references <= 0)
	{
		delete this;
	}
}

void Mesh::CreatShaderVariables(ID3D12Device* device, ID3D12GraphicsCommandList* commandList)
{
}

void Mesh::UpdateShaderVariables(ID3D12GraphicsCommandList* commandList)
{
}

void Mesh::ReleaseShaderVariables()
{
}

void Mesh::ReleaseUploadBuffers()
{
	SAFE_RELEASE(positionUploadBuffer);

	if ((subMeshes > 0) && subSetIndexUploadBuffers)
	{
		for (int i = 0; i < subMeshes; ++i)
		{
			SAFE_RELEASE(subSetIndexUploadBuffers[i]);
		}
		if (subSetIndexUploadBuffers)
		{
			delete[] subSetIndexUploadBuffers;
		}
		subSetIndexUploadBuffers = nullptr;
	}
}

void Mesh::OnPreRender(ID3D12GraphicsCommandList* commandList, void* pContext)
{
	commandList->IASetVertexBuffers(slot, 1, &positionBufferView);
}

void Mesh::Render(ID3D12GraphicsCommandList* commandList, int nSubSet)
{
	UpdateShaderVariables(commandList);

	OnPreRender(commandList, nullptr);

	if ((subMeshes > 0) && (nSubSet < subMeshes))
	{
		commandList->IASetIndexBuffer(&(subSetIndexBufferViews[nSubSet]));
		commandList->DrawIndexedInstanced(subSetIndiceCount[nSubSet], 1, 0, 0, 0);
	}
	else
	{
		commandList->DrawInstanced(vertices, 1, offset, 0);
	}
}

void Mesh::OnPostRender(ID3D12GraphicsCommandList* commandList, void* pContext)
{
}

const UINT Mesh::GetType()
{
	return this->type;
}
