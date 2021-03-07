#include "framework.h"
#include "Camera.h"

void Camera::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	UINT elementBytes = ((sizeof(VS_CB_CAMERA_INFO) + 255) & ~255);
	cbCamera = ::CreateBufferResource(pd3dDevice, pd3dCommandList, nullptr, elementBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, nullptr);
	cbCamera->Map(0, nullptr, (void**)&cbMappedCamera);
}

void Camera::ReleaseShaderVariables()
{
	if (cbCamera)
	{
		cbCamera->Unmap(0, nullptr);
		cbCamera->Release();
	}
}

void Camera::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
	XMStoreFloat4x4(&cbMappedCamera->view, XMMatrixTranspose(XMLoadFloat4x4(&view)));
	XMStoreFloat4x4(&cbMappedCamera->projection, XMMatrixTranspose(XMLoadFloat4x4(&projection)));

	D3D12_GPU_VIRTUAL_ADDRESS gpuVirtualAddress = cbCamera->GetGPUVirtualAddress();
	pd3dCommandList->SetGraphicsRootConstantBufferView(1, gpuVirtualAddress);
}

void Camera::SetViewportsAndScissorRects(ID3D12GraphicsCommandList* pd3dCommandList)
{
	pd3dCommandList->RSSetViewports(1, &viewport);
	pd3dCommandList->RSSetScissorRects(1, &sissorRect);
}
