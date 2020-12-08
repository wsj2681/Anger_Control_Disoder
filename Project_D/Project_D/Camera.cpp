#include "framework.h"
#include "Camera.h"

Camera::Camera()
{
	this->xm4View = Matrix4x4::Identity();
	this->xm4Projection = Matrix4x4::Identity();
	this->Viewport = { 0, 0, FRAME_BUFFER_WIDTH , FRAME_BUFFER_HEIGHT, 0.0f, 1.0f };
	this->ScissorRect = { 0, 0, FRAME_BUFFER_WIDTH , FRAME_BUFFER_HEIGHT };

	// 월드 좌표
	this->xm3Position = XMFLOAT3(0.0f, 0.0f, 0.0f);

	//로컬 좌표
	this->xm3Right = XMFLOAT3(1.0f, 0.0f, 0.0f);
	this->xm3Look = XMFLOAT3(0.0f, 0.0f, 1.0f);
	this->xm3Up = XMFLOAT3(0.0f, 1.0f, 0.0f);

	//회전
	this->Pitch = 0.0f;
	this->Roll = 0.0f;
	this->Yaw = 0.0f;

	this->xm3Offset = XMFLOAT3(0.0f, 0.0f, 0.0f);
	this->xm3LookatWorld = XMFLOAT3(0.0f, 0.0f, 0.0f);

	RCamera = nullptr ;
	MappedCamera = nullptr ;

	//m_pPlayer = NULL;
}

Camera::Camera(const Camera* pCamera)
{
	if (pCamera)
	{
		*this = *pCamera;
	}
	else
	{
		this->xm4View = Matrix4x4::Identity();
		this->xm4Projection = Matrix4x4::Identity();
		this->Viewport = { 0, 0, FRAME_BUFFER_WIDTH , FRAME_BUFFER_HEIGHT, 0.0f, 1.0f };
		this->ScissorRect = { 0, 0, FRAME_BUFFER_WIDTH , FRAME_BUFFER_HEIGHT };

		// 월드 좌표
		this->xm3Position = XMFLOAT3(0.0f, 0.0f, 0.0f);

		//로컬 좌표
		this->xm3Right = XMFLOAT3(1.0f, 0.0f, 0.0f);
		this->xm3Look = XMFLOAT3(0.0f, 0.0f, 1.0f);
		this->xm3Up = XMFLOAT3(0.0f, 1.0f, 0.0f);

		//회전
		this->Pitch = 0.0f;
		this->Roll = 0.0f;
		this->Yaw = 0.0f;

		this->xm3Offset = XMFLOAT3(0.0f, 0.0f, 0.0f);
		this->xm3LookatWorld = XMFLOAT3(0.0f, 0.0f, 0.0f);

		RCamera = nullptr;
		MappedCamera = nullptr;

		//m_pPlayer = NULL;
	}
}




//카메라 변환행렬 생성
void Camera::GenerateViewMatrix()
{
	this->xm4View = Matrix4x4::LookAtLH(this->xm3Position, this->xm3LookatWorld, this->xm3Up);
}
void Camera::GenerateViewMatrix(XMFLOAT3 xm3Position, XMFLOAT3 xm3LookAt, XMFLOAT3 xm3Up)
{
	this->xm3Position = xm3Position;
	this->xm3LookatWorld = xm3LookAt;
	this->xm3Up = xm3Up;

	GenerateViewMatrix();
}


void Camera::RegenerateViewMatrix()
{
	this->xm3Look = Vector3::Normalize(this->xm3Look);
	this->xm3Right = Vector3::CrossProduct(this->xm3Up, this->xm3Look, true);
	this->xm3Up = Vector3::CrossProduct(this->xm3Look, this->xm3Right, true);

	this->xm4View._11 = this->xm3Right.x; this->xm4View._12 = this->xm3Up.x; this->xm4View._13 = this->xm3Look.x;
	this->xm4View._21 = this->xm3Right.y; this->xm4View._22 = this->xm3Up.y; this->xm4View._23 = this->xm3Look.y;
	this->xm4View._31 = this->xm3Right.z; this->xm4View._32 = this->xm3Up.z; this->xm4View._33 = this->xm3Look.z;
	this->xm4View._41 = -Vector3::DotProduct(this->xm3Position, this->xm3Right);
	this->xm4View._42 = -Vector3::DotProduct(this->xm3Position, this->xm3Up);
	this->xm4View._43 = -Vector3::DotProduct(this->xm3Position, this->xm3Look);
}

//투영 변환 행렬을 생성한다. 
void Camera::GenerateProjectionMatrix(float NearPlaneDistance, float FarPlaneDistance, float AspectRatio, float FOVAngle)
{
	this->xm4Projection = Matrix4x4::PerspectiveFovLH(XMConvertToRadians(FOVAngle), AspectRatio, NearPlaneDistance, FarPlaneDistance);

}

void Camera::SetViewport(int xTopLeft, int yTopLeft, int Width, int Height, float MinZ, float MaxZ)
{
	this->Viewport.TopLeftX = float(xTopLeft);
	this->Viewport.TopLeftY = float(yTopLeft);
	this->Viewport.Width = float(Width);
	this->Viewport.Height = float(Height);
	this->Viewport.MinDepth = MinZ;
	this->Viewport.MaxDepth = MaxZ;
}

void Camera::SetScissorRect(LONG xLeft, LONG yTop, LONG xRight, LONG yBottom)
{
	this->ScissorRect.left = xLeft;
	this->ScissorRect.top = yTop;
	this->ScissorRect.right = xRight;
	this->ScissorRect.bottom = yBottom;
}

///////////////////////////////////
//set함수
void Camera::SetPosition(XMFLOAT3 xm3position) {
	this->xm3Position = xm3position;
}
void Camera::SetLookAtPosition(XMFLOAT3 xm3LookatWorld) {
	this->xm3LookatWorld = xm3LookatWorld;
}
void Camera::SetOffset(XMFLOAT3 xm3Offset) {
	this->xm3Offset = xm3Offset;
	this->xm3Position.x += xm3Offset.x;
	this->xm3Position.y += xm3Offset.y;
	this->xm3Position.z += xm3Offset.z;
}
///////////////////////////////////

///////////////////////////////////
//get함수
XMFLOAT3& Camera::GetPosition() {
	return(this->xm3Position);
}

XMFLOAT3& Camera::GetLookAtPosition() {
	return(this->xm3LookatWorld);
}

XMFLOAT3& Camera::GetRightVector() {
	return(xm3Right);
}
XMFLOAT3& Camera::GetUpVector() {
	return(xm3Up);
}
XMFLOAT3& Camera::GetLookVector() {
	return(xm3Look);
}

float& Camera::GetPitch() {
	return(Pitch);
}
float& Camera::GetRoll() {
	return(Roll);
}
float& Camera::GetYaw() {
	return(Yaw);
}
XMFLOAT3& Camera::GetOffset() {
	return(xm3Offset);
}

XMFLOAT4X4 Camera::GetViewMatrix() {
	return(xm4View);
}
XMFLOAT4X4 Camera::GetProjectionMatrix() {
	return(xm4Projection);
}
D3D12_VIEWPORT Camera::GetViewport() {
	return(Viewport);
}
D3D12_RECT Camera::GetScissorRect() {
	return(ScissorRect);
}
///////////////////////////////////


///////////////////////////////////
//카메라 움직임 부분
//카메라를 xmf3Shift만큼 이동
void Camera::Move(const XMFLOAT3& xm3Shift) {
	this->xm3Position.x += xm3Shift.x;
	this->xm3Position.y += xm3Shift.y;
	this->xm3Position.z += xm3Shift.z;
}
// 카메라를 각 축으로 회전하는 함수
void Camera::Rotate(float Pitch = 0.0f, float Yaw = 0.0f, float Roll = 0.0f) { }

//갱신 함수 -> 필요한가???
void Camera::Update(XMFLOAT3& xm3LookAt, float fTimeElapsed) { }

//3인칭시 사용
//virtual void SetLookAt(XMFLOAT3& xmf3LookAt) { }
///////////////////////////////////


///////////////////////////////////
//세이더 부분
void Camera::CreateShaderVariables(ID3D12Device* Device, ID3D12GraphicsCommandList* CommandList)
{
	UINT ncbElementBytes = ((sizeof(VS_CB_CAMERA_INFO) + 255) & ~255); //256의 배수
	//TODO : stdafx.cpp에 있는 함수 적용
	//this->RCamera = ::CreateBufferResource(Device, CommandList, NULL, ncbElementBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	RCamera->Map(0, NULL, (void**)&(this->MappedCamera));
}

void Camera::UpdateShaderVariables(ID3D12GraphicsCommandList* CommandList)
{
	XMFLOAT4X4 xm4UpdateView{};
	XMStoreFloat4x4(&xm4UpdateView, XMMatrixTranspose(XMLoadFloat4x4(&xm4UpdateView)));
	::memcpy(&this->MappedCamera->xm4View, &xm4UpdateView, sizeof(XMFLOAT4X4));

	XMFLOAT4X4 xm4UpdateProjection{};
	XMStoreFloat4x4(&xm4UpdateProjection, XMMatrixTranspose(XMLoadFloat4x4(&xm4UpdateProjection)));
	::memcpy(&MappedCamera->xm4Projection, &xm4UpdateProjection, sizeof(XMFLOAT4X4));

	::memcpy(&MappedCamera->xm3Position, &xm4UpdateProjection, sizeof(XMFLOAT3));

	D3D12_GPU_VIRTUAL_ADDRESS GpuVirtualAddress = RCamera->GetGPUVirtualAddress();
	CommandList->SetGraphicsRootConstantBufferView(1, GpuVirtualAddress);
}

void Camera::ReleaseShaderVariables()
{
	if (RCamera)
	{
		RCamera->Unmap(0, NULL);
		RCamera->Release();
	}
}
///////////////////////////////////
