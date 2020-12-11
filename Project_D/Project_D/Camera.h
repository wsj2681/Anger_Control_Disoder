#pragma once

//카메라 상수 버퍼를 위한 구조체
//struct VS_CB_CAMERA_INFO
//{
//	XMFLOAT4X4 xm4View;
//	XMFLOAT4X4 xm4Projection;
//	XMFLOAT3 xm3Position;
//};

class Camera
{
public:
	Camera();
	Camera(const Camera*);
	~Camera() = default;

protected:
	//카메의 월드좌표계
	XMFLOAT3 xm3Position;

	// 카메라의 로컬 좌표계
	XMFLOAT3	xm3Right{ 0.f, 0.f, 0.f };
	XMFLOAT3	xm3Up{ 0.f, 0.f, 0.f };
	XMFLOAT3	xm3Look{ 0.f, 0.f, 0.f };

	//카메라 회전 각도
	float       Pitch;
	float      	Roll;
	float      	Yaw;

	//바라보게될  위치 벡터
	XMFLOAT3	xm3LookatWorld{ 0.f, 0.f, 0.f };
	XMFLOAT3	xm3Offset{ 0.f, 0.f, 0.f };

	//카메라 변환 행렬(4x4)
	XMFLOAT4X4 xm4View = Matrix4x4::Identity();
	//투영 변환 행렬(4x4)
	XMFLOAT4X4 xm4Projection;

	//뷰포트와 씨저 사각형
	D3D12_VIEWPORT Viewport;
	D3D12_RECT ScissorRect;

	ID3D12Resource* RCamera;

	//VS_CB_CAMERA_INFO* MappedCamera

public:

	//카메라 변환 행렬을 생성한다. 
	void GenerateViewMatrix();
	void GenerateViewMatrix(XMFLOAT3, XMFLOAT3, XMFLOAT3);

	// 카메라의 로컬 x-축(Right), y-축(Up), z-축(Look)이 서로 직교하도록 만들어준다.
	void RegenerateViewMatrix();


	//투영 변환 행렬을 생성한다. 
	void GenerateProjectionMatrix(float, float, float, float);

	void SetViewport(int xTopLeft, int yTopLeft, int nWidth, int nHeight, float fMinZ, float fMaxZ);
	void SetScissorRect(LONG xLeft, LONG yTop, LONG xRight, LONG yBottom);

	//virtual void SetViewportsAndScissorRects(ID3D12GraphicsCommandList* pd3dCommandList);

	void SetPosition(XMFLOAT3);
	void SetLookAtPosition(XMFLOAT3);



	const XMFLOAT3& GetPosition();
	const XMFLOAT3& GetLookAtPosition();

	const XMFLOAT3& GetRightVector();
	const XMFLOAT3& GetUpVector();
	const XMFLOAT3& GetLookVector();

	const float& GetPitch();
	const float& GetRoll();
	const float& GetYaw();


	void SetOffset(XMFLOAT3);

	const XMFLOAT3& GetOffset();

	XMFLOAT4X4 GetViewMatrix();
	XMFLOAT4X4 GetProjectionMatrix();
	D3D12_VIEWPORT GetViewport();
	D3D12_RECT GetScissorRect();

	//카메라를 이동
	void Move(const XMFLOAT3&);
	// 카메라를 각 축으로 회전하는 함수
	void Rotate(float, float, float);

	//갱신 함수 
	void Update(XMFLOAT3&, float);

	//3인칭시 사용
	//virtual void SetLookAt(XMFLOAT3& xmf3LookAt) { }

	void CreateShaderVariables(ID3D12Device* Device, ID3D12GraphicsCommandList* CommandList);
	void ReleaseShaderVariables();
	void UpdateShaderVariables(ID3D12GraphicsCommandList* CommandList);

};