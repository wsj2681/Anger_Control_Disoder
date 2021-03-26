#include "framework.h"
#include "Camera.h"

Camera::Camera()
{
	view = Matrix4x4::Identity();
	projection = Matrix4x4::Identity();
}

Camera::Camera(Camera* camera)
{
	if (camera)
	{
		*this = *camera;
	}
	else
	{
		view = Matrix4x4::Identity();
		projection = Matrix4x4::Identity();
	}
}

Camera::~Camera()
{
}

void Camera::CreateShaderVariables(ID3D12Device* device, ID3D12GraphicsCommandList* commandList)
{
	UINT elementbytes = ((sizeof(VS_CB_CAMERA_INFO) + 255) & ~255);
	cbCamera = ::CreateBufferResource(device, commandList, nullptr, elementbytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, nullptr);

	cbCamera->Map(0, nullptr, (void**)&mappedCamera);
}

void Camera::ReleaseShaderVariables()
{
	if (cbCamera)
	{
		cbCamera->Unmap(0, nullptr);
		cbCamera->Release();
	}
}

void Camera::UpdateShaderVariables(ID3D12GraphicsCommandList* commandList)
{
	XMFLOAT4X4 xmf4x4View;
	XMStoreFloat4x4(&xmf4x4View, XMMatrixTranspose(XMLoadFloat4x4(&view)));
	::memcpy(&mappedCamera->view, &xmf4x4View, sizeof(XMFLOAT4X4));

	XMFLOAT4X4 xmf4x4Projection;
	XMStoreFloat4x4(&xmf4x4Projection, XMMatrixTranspose(XMLoadFloat4x4(&projection)));
	::memcpy(&mappedCamera->projection, &xmf4x4Projection, sizeof(XMFLOAT4X4));

	::memcpy(&mappedCamera->position, &position, sizeof(XMFLOAT3));

	D3D12_GPU_VIRTUAL_ADDRESS d3dGpuVirtualAddress = cbCamera->GetGPUVirtualAddress();
	commandList->SetGraphicsRootConstantBufferView(0, d3dGpuVirtualAddress);

}

void Camera::GenerateProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance, float fAspectRatio, float fFOVAngle)
{
	projection = Matrix4x4::PerspectiveFovLH(XMConvertToRadians(fFOVAngle), fAspectRatio, fNearPlaneDistance, fFarPlaneDistance);
}

void Camera::GenerateViewMatrix()
{
	view = Matrix4x4::LookAtLH(position, lookAtWorld, up);
}

void Camera::GenerateViewMatrix(const XMFLOAT3& position, const XMFLOAT3& lookAt, const XMFLOAT3& up)
{
	this->position = position;
	lookAtWorld = lookAt;
	this->up = up;

	GenerateViewMatrix();
}

void Camera::RegenerateViewMatrix()
{
	look = Vector3::Normalize(look);
	right = Vector3::CrossProduct(up, look, true);
	up = Vector3::CrossProduct(look, right, true);

	view._11 = right.x; view._12 = up.x; view._13 = look.x;
	view._21 = right.y; view._22 = up.y; view._23 = look.y;
	view._31 = right.z; view._32 = up.z; view._33 = look.z;
	view._41 = -Vector3::DotProduct(position, right);
	view._42 = -Vector3::DotProduct(position, up);
	view._43 = -Vector3::DotProduct(position, look);
}

void Camera::SetViewportsAndScissorRects(ID3D12GraphicsCommandList* pd3dCommandList)
{
	pd3dCommandList->RSSetViewports(1, &viewport);
	pd3dCommandList->RSSetScissorRects(1, &scissorRect);
}

void Camera::SetViewport(int xTopLeft, int yTopLeft, int nWidth, int nHeight, float fMinZ, float fMaxZ)
{
	viewport.TopLeftX = float(xTopLeft);
	viewport.TopLeftY = float(yTopLeft);
	viewport.Width = float(nWidth);
	viewport.Height = float(nHeight);
	viewport.MinDepth = fMinZ;
	viewport.MaxDepth = fMaxZ;
}

void Camera::SetScissorRect(LONG xLeft, LONG yTop, LONG xRight, LONG yBottom)
{
	scissorRect.left = xLeft;
	scissorRect.top = yTop;
	scissorRect.right = xRight;
	scissorRect.bottom = yBottom;
}

Player* Camera::GetPlayer()
{
	return player;
}

void Camera::SetPlayer(Player* player)
{
	this->player = player;
}

DWORD Camera::GetMOde()
{
	return this->cameraMode;
}

void Camera::SetMode(DWORD mode)
{
	cameraMode = mode;
}

const XMFLOAT3& Camera::GetPosition()
{
	return this->position;
}

void Camera::SetPosition(const XMFLOAT3& position)
{
	this->position = position;
}

void Camera::SetPosition(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
}

const XMFLOAT3& Camera::GetRightVector()
{
	return this->right;
}

void Camera::SetRightVector(const XMFLOAT3& right)
{
	this->right = right;
}

void Camera::SetRightVector(float x, float y, float z)
{
	right.x = x;
	right.y = y;
	right.z = z;
}

const XMFLOAT3& Camera::GetLookVector()
{
	return this->look;
}

void Camera::SetLookVector(const XMFLOAT3& look)
{
	this->look = look;
}

void Camera::SetLookVector(float x, float y, float z)
{
	look.x = x;
	look.y = y;
	look.z = z;
}

const XMFLOAT3& Camera::GetUpVector()
{
	return this->up;
}

void Camera::GetUpVector(const XMFLOAT3& up)
{
	this->up = up;
}

void Camera::GetUpVector(float x, float y, float z)
{
	up.x = x;
	up.y = y;
	up.z = z;
}

const float& Camera::GetPitch()
{
	return this->pitch;
}

void Camera::SetPitch(float pitch)
{
	this->pitch = pitch;
}

const float& Camera::GetRoll()
{
	return this->roll;
}

void Camera::SetRoll(float roll)
{
	this->roll = roll;
}

const float& Camera::GetYaw()
{
	return this->yaw;
}

void Camera::SetYaw(float yaw)
{
	this->yaw = yaw;
}

const XMFLOAT3& Camera::GetOffset()
{
	this->offset;
}

void Camera::SetOffset(const XMFLOAT3& offset)
{
	this->offset = offset;
}

void Camera::SetOffset(float x, float y, float z)
{
	offset.x = x;
	offset.y = y;
	offset.z = z;
}

const float& Camera::GetTimeLag()
{
	return this->timeLag;
}

void Camera::SetTimeLag(float timeLag)
{
	this->timeLag = timeLag;
}

const XMFLOAT4X4& Camera::GetViewMatrix()
{
	return view;
}

const XMFLOAT4X4& Camera::GetProjectionMatrix()
{
	return projection;
}

const D3D12_VIEWPORT& Camera::GetViewport()
{
	return viewport;
}

const D3D12_RECT& Camera::GetScissorRect()
{
	return scissorRect;
}

void Camera::Move(const XMFLOAT3& xmf3Shift)
{
	position.x += xmf3Shift.x;
	position.y += xmf3Shift.y;
	position.z += xmf3Shift.z;
}

void Camera::Rotate(float fPitch, float fYaw, float fRoll)
{
}

void Camera::Update(const XMFLOAT3& xmf3LookAt, float fTimeElapsed)
{
}

void Camera::SetLookAt(const XMFLOAT3& xmf3LookAt)
{
}
