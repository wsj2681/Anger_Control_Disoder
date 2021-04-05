#include "stdafx.h"
#include "StandardObjectsShader.h"
#include "Object.h"

StandardObjectsShader::StandardObjectsShader()
{
}

StandardObjectsShader::~StandardObjectsShader()
{
}

void StandardObjectsShader::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, ModelInfo* pModel, void* pContext)
{
}

void StandardObjectsShader::ReleaseObjects()
{
	if (m_ppObjects)
	{
		for (int j = 0; j < m_nObjects; j++) if (m_ppObjects[j]) m_ppObjects[j]->Release();
		delete[] m_ppObjects;
	}
}

void StandardObjectsShader::AnimateObjects(float fTimeElapsed)
{
	m_fElapsedTime = fTimeElapsed;
}

void StandardObjectsShader::ReleaseUploadBuffers()
{
	for (int j = 0; j < m_nObjects; j++) if (m_ppObjects[j]) m_ppObjects[j]->ReleaseUploadBuffers();
}

void StandardObjectsShader::Render(ID3D12GraphicsCommandList* pd3dCommandList, Camera* pCamera)
{
	StandardShader::Render(pd3dCommandList, pCamera);

	for (int j = 0; j < m_nObjects; j++)
	{
		if (m_ppObjects[j])
		{
			m_ppObjects[j]->Animate(m_fElapsedTime);
			m_ppObjects[j]->UpdateTransform(NULL);
			m_ppObjects[j]->Render(pd3dCommandList, pCamera);
		}
	}
}