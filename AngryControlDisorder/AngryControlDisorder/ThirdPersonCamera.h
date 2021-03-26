#pragma once
#include "Camera.h"

class ThirdPersonCamera : public Camera
{
public:

	ThirdPersonCamera() = default;
	ThirdPersonCamera(Camera * camera);
	virtual ~ThirdPersonCamera();

public:

	virtual void Rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f) override;

	virtual void Update(const XMFLOAT3& lookAt, float eTime) override;
	virtual void SetLookAt(const XMFLOAT3& lookAt) override;

};

