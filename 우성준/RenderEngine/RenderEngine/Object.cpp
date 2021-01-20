#include "framework.h"
#include "Object.h"
#include "AnimationController.h"
#include "Camera.h"
#include "FbxSceneContext.h"

void Object::Animate(float eTime)
{
	if (animationController)
	{
		animationController->AdvanceTime(eTime);
		FbxTime fbxCurrentTime = animationController->GetCurrentTime();
		::AnimateFbxNodeHierarchy(fbxScene->GetRootNode(), fbxCurrentTime);
	}
}

void Object::Render(ID3D12GraphicsCommandList* commandList, Camera* pCamera)
{
	OnPrepareRender();

	FbxAMatrix fbxWorld = ::XmFloat4x4MatrixToFbxMatrix(this->world);
	if (fbxScene) ::RenderFbxNodeHierarchy(commandList, fbxScene->GetRootNode(), animationController->GetCurrentTime(), fbxWorld);

}

void Object::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void Object::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{

}

void Object::UpdateShaderVariable(ID3D12GraphicsCommandList* commandList, XMFLOAT4X4* pxmf4x4World)
{
	XMFLOAT4X4 xmf4x4World;
	XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(pxmf4x4World)));
	commandList->SetGraphicsRoot32BitConstants(1, 16, &xmf4x4World, 0);
}

void Object::UpdateShaderVariable(ID3D12GraphicsCommandList* commandList, FbxAMatrix* pfbxf4x4World)
{
	XMFLOAT4X4 xmf4x4World = ::FbxMatrixToXmFloat4x4Matrix(pfbxf4x4World);
	XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(&xmf4x4World)));
	commandList->SetGraphicsRoot32BitConstants(1, 16, &xmf4x4World, 0);
}

void Object::ReleaseShaderVariables()
{

}

void Object::ReleaseUploadBuffers()
{
#ifndef _WITH_FBX_SCENE_INSTANCING
	if (fbxScene) ::ReleaseUploadBufferFromFbxNodeHierarchy(fbxScene->GetRootNode());
#endif

}

XMFLOAT3 Object::GetPosition()
{
	return XMFLOAT3(world._41, world._42, world._43);
}

XMFLOAT3 Object::GetLook()
{
	return Vector3::Normalize(XMFLOAT3(world._31, world._32, world._33));
}

XMFLOAT3 Object::GetUp()
{
	return Vector3::Normalize(XMFLOAT3(world._21, world._22, world._23));
}

XMFLOAT3 Object::GetRight()
{
	return Vector3::Normalize(XMFLOAT3(world._11, world._12, world._13));
}

void Object::SetPosition(float x, float y, float z)
{
	world._41 = x;
	world._42 = y;
	world._43 = z;
}

void Object::SetPosition(XMFLOAT3 position)
{
	SetPosition(position.x, position.y, position.z);
}

void Object::SetScale(float x, float y, float z)
{
	XMMATRIX mtxScale = XMMatrixScaling(x, y, z);
	world = Matrix4x4::Multiply(mtxScale, world);
}

void Object::MoveStrafe(float fDistance)
{
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3Right = GetRight();
	xmf3Position = Vector3::Add(xmf3Position, xmf3Right, fDistance);
	Object::SetPosition(xmf3Position);
}

void Object::MoveUp(float fDistance)
{
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3Up = GetUp();
	xmf3Position = Vector3::Add(xmf3Position, xmf3Up, fDistance);
	Object::SetPosition(xmf3Position);
}

void Object::MoveForward(float fDistance)
{
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3Look = GetLook();
	xmf3Position = Vector3::Add(xmf3Position, xmf3Look, fDistance);
	Object::SetPosition(xmf3Position);
}

void Object::Rotate(float fPitch, float fYaw, float fRoll)
{
	XMMATRIX mtxRotate = XMMatrixRotationRollPitchYaw(XMConvertToRadians(fPitch), XMConvertToRadians(fYaw), XMConvertToRadians(fRoll));
	world = Matrix4x4::Multiply(mtxRotate, world);
}

void Object::Rotate(XMFLOAT3* pxmf3Axis, float fAngle)
{
	XMMATRIX mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(pxmf3Axis), XMConvertToRadians(fAngle));
	world = Matrix4x4::Multiply(mtxRotate, world);
}

void Object::Rotate(XMFLOAT4* pxmf4Quaternion)
{
	XMMATRIX mtxRotate = XMMatrixRotationQuaternion(XMLoadFloat4(pxmf4Quaternion));
	world = Matrix4x4::Multiply(mtxRotate, world);
}

void Object::SetAnimationStack(int nAnimationStack)
{
	this->animationController->SetAnimationStack(this->fbxScene, nAnimationStack);
}
