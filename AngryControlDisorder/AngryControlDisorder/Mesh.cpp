#include "framework.h"
#include "Mesh.h"

Mesh::Mesh()
{
	::ZeroMemory(meshName, sizeof(meshName));
	::ZeroMemory(&positionBuffer, sizeof(positionBuffer));
}

Mesh::Mesh(ID3D12Device* device, ID3D12GraphicsCommandList* commandList) : Mesh()
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
			DELETE_ARRAY(subSetIndices);
		}

		DELETE_ARRAY(subSetIndexBuffers);
		DELETE_ARRAY(subSetIndexBufferViews);
		DELETE_ARRAY(subSetIndexCount);
		DELETE_ARRAY(subSetIndices);
	}
	DELETE_ARRAY(positions);
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

	if ((subSetIndices > 0) && subSetIndexUploadBuffers)
	{
		for (int i = 0; i < subMeshes; ++i)
		{
			SAFE_RELEASE(subSetIndexUploadBuffers[i]);
		}
		DELETE_ARRAY(subSetIndexUploadBuffers);
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
		commandList->DrawIndexedInstanced(subSetIndexCount[nSubSet], 1, 0, 0, 0);
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

const char* Mesh::GetMeshName()
{
	return meshName;
}
