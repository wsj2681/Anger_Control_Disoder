#pragma once
#include "Object.h"
#include "BillboardAnimationShader.h"
#include "BillboardMesh.h"

class EffectBillboard : public Object
{
public:
	EffectBillboard(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature);
	virtual ~EffectBillboard();

private:

public:

	//frame++ % 25
	int keyFrame = 0;
	
	//elapsedTime >= 0.1f
	float elapsedTime = 0.f;

public:

	virtual void Update(float fTimeElapsed);
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, Camera* pCamera);
};

