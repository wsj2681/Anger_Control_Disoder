#pragma once

class Camera
{
public:
	Camera() = default;

private:
	XMFLOAT3 position;

	XMFLOAT4X4 world;
	XMFLOAT4X4 projection;

	
public:

	void SetViewportsAndScissorRects(ID3D12GraphicsCommandList* pd3dCommandList);
	void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);

};