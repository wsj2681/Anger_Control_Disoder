// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once
#define _WITH_SERVER_CONNECT			// 서버 연결및 해제
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C의 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <math.h>

#include <string>
#include <wrl.h>
#include <shellapi.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <random>
#include <thread>

/////////////SEVER///////////
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#pragma comment(lib, "ws2_32")

/////////////////////////////


using namespace std;

#include <d3d12.h>
#include <dxgi1_4.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>

#include <Mmsystem.h>

#ifdef _DEBUG
#include <dxgidebug.h>
#endif

using namespace DirectX;
using namespace DirectX::PackedVector;

using Microsoft::WRL::ComPtr;

extern HINSTANCE						ghAppInstance;

//#define _WITH_SWAPCHAIN_FULLSCREEN_STATE

#define FRAME_BUFFER_WIDTH		1280
#define FRAME_BUFFER_HEIGHT		720

//#define _WITH_CB_GAMEOBJECT_32BIT_CONSTANTS
//#define _WITH_CB_GAMEOBJECT_ROOT_DESCRIPTOR
#define _WITH_CB_WORLD_MATRIX_DESCRIPTOR_TABLE

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

#pragma comment(lib, "dxguid.lib")

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.

extern UINT	gnCbvSrvDescriptorIncrementSize;
extern UINT	gnRtvDescriptorIncrementSize;
extern UINT gnDsvDescriptorIncrementSize;


//----------------기본 애니메이션-------------------
constexpr UINT ANIMATION_IDLE_A = 0x00;             // Idle_A (서있는 IDLE)
constexpr UINT ANIMATION_IDLE_B = 0x01;             // Idle_B (딴짓하는 IDLE)
constexpr UINT ANIMATION_IDLE_COMBAT = 0x02;        // Idle_Combat (전투 준비)
constexpr UINT ANIMATION_COMBAT_IDLE = 0x03;        // Combat_Idle (전투 해제)
constexpr UINT ANIMATION_COMBAT_MODE_A = 0x04;      // Combat_Mode_A (전투 IDLE)
constexpr UINT ANIMATION_CLAP_LEG = 0x05;           // Clap_Leg (무릎 자학)
constexpr UINT ANIMATION_CLAP_ON_ARM = 0x06;        // Clap_On_Arm (팔 자학)
constexpr UINT ANIMATION_CLAP_ON_HEAD = 0x07;       // Clap_On_Head (머리 자학)
constexpr UINT ANIMATION_COME_HERE_1HAND = 0x08;    // Come_Here_1Hand (한손 도발)
constexpr UINT ANIMATION_COME_HERE_2HANDS = 0x09;   // Come_Here_2Hands (두손 도발)
constexpr UINT ANIMATION_COME_HERE_BRUCE_LI = 0x0A; // Come_Here_Bruce_Li (이소룡 도발)
constexpr UINT ANIMATION_KILL_YOU = 0x0B;           // Kill_You (킬유 도발)
constexpr UINT ANIMATION_RIZE_AUDITORY = 0x0C;      // Rize_Auditory (관객 호응 유도)
constexpr UINT ANIMATION_KARATE_FINISH = 0x0D;      // Karate_Finish (가라테 피니시)
constexpr UINT ANIMATION_OSSUUU = 0x0E;             // OSSUUU (OSSUUU)
constexpr UINT ANIMATION_CEREMONY = 0x0F;           // Ceremony (커스텀 세레머니)
constexpr UINT ANIMATION_WINN_BATTLE = 0x10;        // Winn_battle (승리 모션)
constexpr UINT ANIMATION_LOSTING_BATTLE = 0x11;     // Losting_Battle (패배 모션)
constexpr UINT ANIMATION_MOVE_FORWARD = 0x12;       // 1_Steps_Forward
constexpr UINT ANIMATION_MOVE_BACKWARD = 0x13;      // 1_Steps_Backward
constexpr UINT ANIMATION_MOVE_LEFT = 0x14;			// 1_Steps_Left 
constexpr UINT ANIMATION_MOVE_RIGHT = 0x15;			// 1_Steps_Right
constexpr UINT ANIMATION_KNOCKDOWN = 0x16;          // Knockdown
constexpr UINT ANIMATION_KNOCKDOWNED = 0x17;        // Knockdowned
constexpr UINT ANIMATION_GUARD_LEFT_HEAD = 0x18;	// Close_One_Block_L
constexpr UINT ANIMATION_GUARD_RIGHT_HEAD = 0x19;	// Close_One_Block_R
constexpr UINT ANIMATION_GUARD_BODY = 0x1A;			// Close_Both_Block
//----------------공격 애니메이션-------------------
constexpr UINT ANIMATION_HOOK_L = 0x1B; 			// Hook_L 
constexpr UINT ANIMATION_HOOK_R = 0x1C;				// Hook_R
constexpr UINT ANIMATION_JAB = 0x1D;				// Jab
constexpr UINT ANIMATION_CROSS = 0x1E;              // Cross
constexpr UINT ANIMATION_CROSS_BODY = 0x1F;         // Cross_Body
constexpr UINT ANIMATION_ONE_TWO = 0x20;            // One_Two
constexpr UINT ANIMATION_LEFT_BODY_HOOK = 0x21;     // Left Body Hook(liver punch)
constexpr UINT ANIMATION_RIGHT_BODY_HOOK = 0x22;    // Right_BodyHook(Spleen Punch)
constexpr UINT ANIMATION_UPPER_CUT_L = 0x23;        // UpperCut_L
constexpr UINT ANIMATION_UPPER_CUT_R = 0x24;        // UpperCut_R
constexpr UINT ANIMATION_1_2_3_KICK = 0x25;			// 1_2_3_Kick
constexpr UINT ANIMATION_1_2_KICK = 0x26;			// 1_2_Kick
constexpr UINT ANIMATION_CROSS_KICK = 0x27;			// Cross_Kick
constexpr UINT ANIMATION_DIAGONAL_ELBOW_L = 0x28;	// Diagonal_Elbow_L
constexpr UINT ANIMATION_DIAGONAL_ELBOW_R = 0x29;	// Diagonal_Elbow_R
constexpr UINT ANIMATION_DOWNWARD_ELBOW_R = 0x2A;	// Downward_Elbow_R
constexpr UINT ANIMATION_ELBOW_L = 0x2B;			// Elbow_L
constexpr UINT ANIMATION_ELBOW_R = 0x2C;			// Elbow_R
constexpr UINT ANIMATION_UPPER_ELBOW_L = 0x2D;		// Upper_Elbow_L
constexpr UINT ANIMATION_UPPER_ELBOW_R = 0x2E;		// Upper_Elbow_R
constexpr UINT ANIMATION_AXE_KICK_R = 0x2F;			// Axe_Kick_R
constexpr UINT ANIMATION_CRANE_KICK_R = 0x30;		// Crane_Kick_R
constexpr UINT ANIMATION_KICK_HIGH_L = 0x31;		// Kick_High_L
constexpr UINT ANIMATION_KICK_MID_L = 0x32;			// Kick_Mid_L
constexpr UINT ANIMATION_LOW_KICK_360_R = 0x33;		// Low_Kick_360_R
constexpr UINT ANIMATION_LOW_KICK_R = 0x34;			// Low_Kick_R
constexpr UINT ANIMATION_LOW_KICK_SL = 0x35;		// Low_Kick_Shortened (KickBoxing Style)_L
constexpr UINT ANIMATION_MAEGERI_BODY_R = 0x36;		// MaeGeri_Body(Karate Kick)_R
constexpr UINT ANIMATION_MAEGERI_HEAD_R = 0x37;		// MaeGeri_Head(Karate Kick)_R
constexpr UINT ANIMATION_MIKAZUKIGERI_R = 0x38;		// MikazukiGeri(Karate)_R
constexpr UINT ANIMATION_PUSH_KICK_R = 0x39;		// Push_Kick (Muat Thai)_R
constexpr UINT ANIMATION_SHORT_LOW_KICK_L = 0x3A;	// Short_Low_Kick_L
constexpr UINT ANIMATION_SIDE_KICK_L = 0x3B;		// Side_Kick_L
constexpr UINT ANIMATION_SIDE_KICK_R = 0x3C;		// Side_Kick_R
constexpr UINT ANIMATION_URAMIKAZUKIGERI_R = 0x3D;	// UraMikazukiGeri(Karate)_R
constexpr UINT ANIMATION_HIT_HEAD_STRIGHT_B = 0x3E; // Hit_Head_Stright_B
//----------------축 이동 애니메이션-------------------
constexpr UINT ANIMATION_JAB_KICK = 0x3F;			// Jab_Kick
constexpr UINT ANIMATION_KICK_COMBO = 0x40;			// Kick_Combo
constexpr UINT ANIMATION_KICK_COMBO_HEAD = 0x41;	// Kick_Combo_Head
constexpr UINT ANIMATION_KICK_COMBO_TORSO = 0x42;	// Kick_Combo_Torso
constexpr UINT ANIMATION_STEP_DOUBLEJAB_CROSS = 0x43;	// Step_DoubleJab_Cross
constexpr UINT ANIMATION_FAKE_CROSS_R_KICK_HIGH = 0x44;	// Fake_Cross_R_ Kick_High
constexpr UINT ANIMATION_STEP_HOOK_L = 0x45;		// Step_Hook_L
constexpr UINT ANIMATION_STEP_JAB = 0x46;			// Step_Jab
constexpr UINT ANIMATION_HIT_HEAD_LEFT_B = 0x47;	// Hit_Head_Left_B
constexpr UINT ANIMATION_HIT_HEAD_LEFT_C = 0x48;	// Hit_Head_Left_C
constexpr UINT ANIMATION_HIT_HEAD_LEG_LEFT_B = 0x49;	// Hit_Head_Leg_Left_B
constexpr UINT ANIMATION_HIT_HEAD_LEG_LEFT_C = 0x4A;	// Hit_Head_Leg_Left_C
constexpr UINT ANIMATION_HIT_HEAD_LEG_RIGHT_B = 0x4B;	// Hit_Head_Leg_Right_B
constexpr UINT ANIMATION_HIT_HEAD_LEG_RIGHT_C = 0x4C;	// Hit_Head_Leg_Right_C
constexpr UINT ANIMATION_HIT_HEAD_RIGHT_B = 0x4D;	// Hit_Head_Right_B
constexpr UINT ANIMATION_HIT_HEAD_RIGHT_C = 0x4E;	// Hit_Head_Right_C
constexpr UINT ANIMATION_HIT_HEAD_STRIGHT_C = 0x4F;	// Hit_Head_Stright_C
constexpr UINT ANIMATION_HIT_L_LEG_IN_B = 0x50;		// Hit_L_leg_In_B
constexpr UINT ANIMATION_HIT_L_LEG_IN_C = 0x51;		// Hit_L_leg_In_C
constexpr UINT ANIMATION_HIT_L_LEG_OUT_B = 0x52;	// Hit_L_leg_Out_B
constexpr UINT ANIMATION_HIT_L_LEG_OUT_C = 0x53;	// Hit_L_leg_Out_C
constexpr UINT ANIMATION_HIT_R_LEG_OUT_B = 0x54;	// Hit_R_leg_Out_B
constexpr UINT ANIMATION_HIT_R_LEG_OUT_C = 0x55;	// Hit_R_leg_Out_C
constexpr UINT ANIMATION_HIT_TORSO_LEFT_B = 0x56;	// Hit_Torso_Left_B
constexpr UINT ANIMATION_HIT_TORSO_LEFT_C = 0x57;	// Hit_Torso_Left_C
constexpr UINT ANIMATION_HIT_TORSO_RIGHT_B = 0x58;	// Hit_Torso_Right_B
constexpr UINT ANIMATION_HIT_TORSO_RIGHT_C = 0x59;	// Hit_Torso_Right_C
constexpr UINT ANIMATION_HIT_TORSO_STRIGHT_B = 0x5A;	// Hit_Torso_Stright_B
constexpr UINT ANIMATION_HIT_TORSO_STRIGHT_C = 0x5B;	// Hit_Torso_Stright_C
constexpr UINT ANIMATION_FUL_HIGH_KIK_L = 0x5C;		// Ful_High_Kik_L
constexpr UINT ANIMATION_FUL_LOW_KIK_L = 0x5D;		// Ful_Low_KIK_L
constexpr UINT ANIMATION_FUL_MID_KIK_L = 0x5E;		// Ful_Mid_Kik_L
constexpr UINT ANIMATION_HIGH_KICK_360_R = 0x5F;	// High_Kick_360_R
constexpr UINT ANIMATION_MIDDLE_KICK_360_R = 0x60;	// Middle_Kick_360_R
constexpr UINT ANIMATION_PUSH_KIK_HIGH_L = 0x61;	// Push_Kik_High_L
//----------------------------------------------------------
constexpr UINT DAMAGE_HOOK = 10;
constexpr UINT DAMAGE_HOOK_L = 10;
constexpr UINT DAMAGE_HOOK_R = 10;
constexpr UINT DAMAGE_JAB = 7;
constexpr UINT DAMAGE_STRAIGHT = 15;
//-----------------State-----------------------------
constexpr UINT IDLE = 1;
constexpr UINT ATTACK = 2;
constexpr UINT HIT = 3;
constexpr UINT DEAD = 3;

extern ID3D12Resource *CreateBufferResource(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, void *pData, UINT nBytes, D3D12_HEAP_TYPE d3dHeapType = D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATES d3dResourceStates = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, ID3D12Resource **ppd3dUploadBuffer = NULL);
extern ID3D12Resource *CreateTextureResourceFromDDSFile(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, wchar_t *pszFileName, ID3D12Resource **ppd3dUploadBuffer, D3D12_RESOURCE_STATES d3dResourceStates = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
extern ID3D12Resource *CreateTextureResourceFromWICFile(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, wchar_t *pszFileName, ID3D12Resource **ppd3dUploadBuffer, D3D12_RESOURCE_STATES d3dResourceStates = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

extern BYTE ReadStringFromFile(FILE *pInFile, char *pstrToken);
extern int ReadIntegerFromFile(FILE *pInFile);
extern float ReadFloatFromFile(FILE *pInFile);

#define RANDOM_COLOR			XMFLOAT4(rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX))

#define EPSILON					1.0e-10f

inline bool IsZero(float fValue) { return((fabsf(fValue) < EPSILON)); }
inline bool IsEqual(float fA, float fB) { return(::IsZero(fA - fB)); }
inline bool IsZero(float fValue, float fEpsilon) { return((fabsf(fValue) < fEpsilon)); }
inline bool IsEqual(float fA, float fB, float fEpsilon) { return(::IsZero(fA - fB, fEpsilon)); }
inline float InverseSqrt(float fValue) { return 1.0f / sqrtf(fValue); }
inline void Swap(float *pfS, float *pfT) { float fTemp = *pfS; *pfS = *pfT; *pfT = fTemp; }

namespace Vector3
{
	inline XMFLOAT3 XMVectorToFloat3(XMVECTOR& xmvVector)
	{
		XMFLOAT3 xmf3Result;
		XMStoreFloat3(&xmf3Result, xmvVector);
		return(xmf3Result);
	}

	inline XMFLOAT3 ScalarProduct(XMFLOAT3& xmf3Vector, float fScalar, bool bNormalize = true)
	{
		XMFLOAT3 xmf3Result;
		if (bNormalize)
			XMStoreFloat3(&xmf3Result, XMVector3Normalize(XMLoadFloat3(&xmf3Vector)) * fScalar);
		else
			XMStoreFloat3(&xmf3Result, XMLoadFloat3(&xmf3Vector) * fScalar);
		return(xmf3Result);
	}

	inline XMFLOAT3 Add(const XMFLOAT3& xmf3Vector1, const XMFLOAT3& xmf3Vector2)
	{
		XMFLOAT3 xmf3Result;
		XMStoreFloat3(&xmf3Result, XMLoadFloat3(&xmf3Vector1) + XMLoadFloat3(&xmf3Vector2));
		return(xmf3Result);
	}

	inline XMFLOAT3 Add(XMFLOAT3& xmf3Vector1, XMFLOAT3& xmf3Vector2, float fScalar)
	{
		XMFLOAT3 xmf3Result;
		XMStoreFloat3(&xmf3Result, XMLoadFloat3(&xmf3Vector1) + (XMLoadFloat3(&xmf3Vector2) * fScalar));
		return(xmf3Result);
	}

	inline XMFLOAT3 Subtract(XMFLOAT3& xmf3Vector1, XMFLOAT3& xmf3Vector2)
	{
		XMFLOAT3 xmf3Result;
		XMStoreFloat3(&xmf3Result, XMLoadFloat3(&xmf3Vector1) - XMLoadFloat3(&xmf3Vector2));
		return(xmf3Result);
	}

	inline float DotProduct(XMFLOAT3& xmf3Vector1, XMFLOAT3& xmf3Vector2)
	{
		XMFLOAT3 xmf3Result;
		XMStoreFloat3(&xmf3Result, XMVector3Dot(XMLoadFloat3(&xmf3Vector1), XMLoadFloat3(&xmf3Vector2)));
		return(xmf3Result.x);
	}

	inline XMFLOAT3 CrossProduct(XMFLOAT3& xmf3Vector1, XMFLOAT3& xmf3Vector2, bool bNormalize = true)
	{
		XMFLOAT3 xmf3Result;
		if (bNormalize)
			XMStoreFloat3(&xmf3Result, XMVector3Normalize(XMVector3Cross(XMLoadFloat3(&xmf3Vector1), XMLoadFloat3(&xmf3Vector2))));
		else
			XMStoreFloat3(&xmf3Result, XMVector3Cross(XMLoadFloat3(&xmf3Vector1), XMLoadFloat3(&xmf3Vector2)));
		return(xmf3Result);
	}

	inline XMFLOAT3 Normalize(XMFLOAT3& xmf3Vector)
	{
		XMFLOAT3 m_xmf3Normal;
		XMStoreFloat3(&m_xmf3Normal, XMVector3Normalize(XMLoadFloat3(&xmf3Vector)));
		return(m_xmf3Normal);
	}

	inline float Length(XMFLOAT3& xmf3Vector)
	{
		XMFLOAT3 xmf3Result;
		XMStoreFloat3(&xmf3Result, XMVector3Length(XMLoadFloat3(&xmf3Vector)));
		return(xmf3Result.x);
	}

	inline bool IsZero(XMFLOAT3& xmf3Vector)
	{
		XMFLOAT3 xmf3Result;
		XMStoreFloat3(&xmf3Result, XMVector3Length(XMLoadFloat3(&xmf3Vector)));
		return(::IsZero(xmf3Result.x));
	}

	inline float Angle(XMVECTOR& xmvVector1, XMVECTOR& xmvVector2)
	{
		XMVECTOR xmvAngle = XMVector3AngleBetweenNormals(xmvVector1, xmvVector2);
		return(XMConvertToDegrees(acosf(XMVectorGetX(xmvAngle))));
	}

	inline float Angle(XMFLOAT3& xmf3Vector1, XMFLOAT3& xmf3Vector2)
	{
		return(Angle(XMLoadFloat3(&xmf3Vector1), XMLoadFloat3(&xmf3Vector2)));
	}

	inline XMFLOAT3 TransformNormal(XMFLOAT3& xmf3Vector, XMMATRIX& xmmtxTransform)
	{
		XMFLOAT3 xmf3Result;
		XMStoreFloat3(&xmf3Result, XMVector3TransformNormal(XMLoadFloat3(&xmf3Vector), xmmtxTransform));
		return(xmf3Result);
	}

	inline XMFLOAT3 TransformCoord(XMFLOAT3& xmf3Vector, XMMATRIX& xmmtxTransform)
	{
		XMFLOAT3 xmf3Result;
		XMStoreFloat3(&xmf3Result, XMVector3TransformCoord(XMLoadFloat3(&xmf3Vector), xmmtxTransform));
		return(xmf3Result);
	}

	inline XMFLOAT3 TransformCoord(XMFLOAT3& xmf3Vector, XMFLOAT4X4& xmmtx4x4Matrix)
	{
		return(TransformCoord(xmf3Vector, XMLoadFloat4x4(&xmmtx4x4Matrix)));
	}

	inline bool Compare(XMFLOAT3& xmf3Vector1, XMFLOAT3& xmf3Vector2)
	{
		return (xmf3Vector1.x == xmf3Vector2.x) && (xmf3Vector1.y == xmf3Vector2.y) && (xmf3Vector1.z == xmf3Vector2.z);
	}
}

namespace Vector4
{
	inline XMFLOAT4 Add(XMFLOAT4& xmf4Vector1, XMFLOAT4& xmf4Vector2)
	{
		XMFLOAT4 xmf4Result;
		XMStoreFloat4(&xmf4Result, XMLoadFloat4(&xmf4Vector1) + XMLoadFloat4(&xmf4Vector2));
		return(xmf4Result);
	}

	inline XMFLOAT4 Multiply(XMFLOAT4& xmf4Vector1, XMFLOAT4& xmf4Vector2)
	{
		XMFLOAT4 xmf4Result;
		XMStoreFloat4(&xmf4Result, XMLoadFloat4(&xmf4Vector1) * XMLoadFloat4(&xmf4Vector2));
		return(xmf4Result);
	}

	inline XMFLOAT4 Multiply(float fScalar, XMFLOAT4& xmf4Vector)
	{
		XMFLOAT4 xmf4Result;
		XMStoreFloat4(&xmf4Result, fScalar * XMLoadFloat4(&xmf4Vector));
		return(xmf4Result);
	}
}

namespace Matrix4x4
{
	inline XMFLOAT4X4 Identity()
	{
		XMFLOAT4X4 xmf4x4Result;
		XMStoreFloat4x4(&xmf4x4Result, XMMatrixIdentity());
		return(xmf4x4Result);
	}

	inline XMFLOAT4X4 Zero()
	{
		XMFLOAT4X4 xmf4x4Result;
		XMStoreFloat4x4(&xmf4x4Result, XMMatrixSet(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f));
		return(xmf4x4Result);
	}

	inline XMFLOAT4X4 Multiply(XMFLOAT4X4& xmmtx4x4Matrix1, XMFLOAT4X4& xmmtx4x4Matrix2)
	{
		XMFLOAT4X4 xmf4x4Result;
		XMStoreFloat4x4(&xmf4x4Result, XMMatrixMultiply(XMLoadFloat4x4(&xmmtx4x4Matrix1), XMLoadFloat4x4(&xmmtx4x4Matrix2)));
		return(xmf4x4Result);
	}

	inline XMFLOAT4X4 Scale(XMFLOAT4X4& xmf4x4Matrix, float fScale)
	{
		XMFLOAT4X4 xmf4x4Result;
		XMStoreFloat4x4(&xmf4x4Result, XMLoadFloat4x4(&xmf4x4Matrix) * fScale);
/*
		XMVECTOR S, R, T;
		XMMatrixDecompose(&S, &R, &T, XMLoadFloat4x4(&xmf4x4Matrix));
		S = XMVectorScale(S, fScale);
		T = XMVectorScale(T, fScale);
		R = XMVectorScale(R, fScale);
		//R = XMQuaternionMultiply(R, XMVectorSet(0, 0, 0, fScale));
		XMStoreFloat4x4(&xmf4x4Result, XMMatrixAffineTransformation(S, XMVectorZero(), R, T));
*/
		return(xmf4x4Result);
	}

	inline XMFLOAT4X4 Add(XMFLOAT4X4& xmmtx4x4Matrix1, XMFLOAT4X4& xmmtx4x4Matrix2)
	{
		XMFLOAT4X4 xmf4x4Result;
		XMStoreFloat4x4(&xmf4x4Result, XMLoadFloat4x4(&xmmtx4x4Matrix1) + XMLoadFloat4x4(&xmmtx4x4Matrix2));
		return(xmf4x4Result);
	}

	inline XMFLOAT4X4 Multiply(XMFLOAT4X4& xmmtx4x4Matrix1, XMMATRIX& xmmtxMatrix2)
	{
		XMFLOAT4X4 xmf4x4Result;
		XMStoreFloat4x4(&xmf4x4Result, XMLoadFloat4x4(&xmmtx4x4Matrix1) * xmmtxMatrix2);
		return(xmf4x4Result);
	}

	inline XMFLOAT4X4 Multiply(XMMATRIX& xmmtxMatrix1, XMFLOAT4X4& xmmtx4x4Matrix2)
	{
		XMFLOAT4X4 xmf4x4Result;
		XMStoreFloat4x4(&xmf4x4Result, xmmtxMatrix1 * XMLoadFloat4x4(&xmmtx4x4Matrix2));
		return(xmf4x4Result);
	}

	inline XMFLOAT4X4 Interpolate(XMFLOAT4X4& xmf4x4Matrix1, XMFLOAT4X4& xmf4x4Matrix2, float t)
	{
		XMFLOAT4X4 xmf4x4Result;
		XMVECTOR S0, R0, T0, S1, R1, T1;
		XMMatrixDecompose(&S0, &R0, &T0, XMLoadFloat4x4(&xmf4x4Matrix1));
		XMMatrixDecompose(&S1, &R1, &T1, XMLoadFloat4x4(&xmf4x4Matrix2));
		XMVECTOR S = XMVectorLerp(S0, S1, t);
		XMVECTOR T = XMVectorLerp(T0, T1, t);
		XMVECTOR R = XMQuaternionSlerp(R0, R1, t);
		XMStoreFloat4x4(&xmf4x4Result, XMMatrixAffineTransformation(S, XMVectorZero(), R, T));
		return(xmf4x4Result);
	}

	inline XMFLOAT4X4 Inverse(XMFLOAT4X4& xmmtx4x4Matrix)
	{
		XMFLOAT4X4 xmf4x4Result;
		XMStoreFloat4x4(&xmf4x4Result, XMMatrixInverse(NULL, XMLoadFloat4x4(&xmmtx4x4Matrix)));
		return(xmf4x4Result);
	}

	inline XMFLOAT4X4 Transpose(XMFLOAT4X4& xmmtx4x4Matrix)
	{
		XMFLOAT4X4 xmf4x4Result;
		XMStoreFloat4x4(&xmf4x4Result, XMMatrixTranspose(XMLoadFloat4x4(&xmmtx4x4Matrix)));
		return(xmf4x4Result);
	}

	inline XMFLOAT4X4 PerspectiveFovLH(float FovAngleY, float AspectRatio, float NearZ, float FarZ)
	{
		XMFLOAT4X4 xmf4x4Result;
		XMStoreFloat4x4(&xmf4x4Result, XMMatrixPerspectiveFovLH(FovAngleY, AspectRatio, NearZ, FarZ));
		return(xmf4x4Result);
	}

	inline XMFLOAT4X4 LookAtLH(XMFLOAT3& xmf3EyePosition, XMFLOAT3& xmf3LookAtPosition, XMFLOAT3& xmf3UpDirection)
	{
		XMFLOAT4X4 xmf4x4Result;
		XMStoreFloat4x4(&xmf4x4Result, XMMatrixLookAtLH(XMLoadFloat3(&xmf3EyePosition), XMLoadFloat3(&xmf3LookAtPosition), XMLoadFloat3(&xmf3UpDirection)));
		return(xmf4x4Result);
	}
}

namespace Triangle
{
	inline bool Intersect(XMFLOAT3& xmf3RayPosition, XMFLOAT3& xmf3RayDirection, XMFLOAT3& v0, XMFLOAT3& v1, XMFLOAT3& v2, float& fHitDistance)
	{
		return(TriangleTests::Intersects(XMLoadFloat3(&xmf3RayPosition), XMLoadFloat3(&xmf3RayDirection), XMLoadFloat3(&v0), XMLoadFloat3(&v1), XMLoadFloat3(&v2), fHitDistance));
	}
}

namespace Plane
{
	inline XMFLOAT4 Normalize(XMFLOAT4& xmf4Plane)
	{
		XMFLOAT4 xmf4Result;
		XMStoreFloat4(&xmf4Result, XMPlaneNormalize(XMLoadFloat4(&xmf4Plane)));
		return(xmf4Result);
	}
}
