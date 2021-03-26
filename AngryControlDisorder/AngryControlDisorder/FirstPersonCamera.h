#pragma once
#include "Camera.h"

class FirstPersonCamera : public Camera
{
public:

	FirstPersonCamera() = default;
	FirstPersonCamera(Camera * camera);
	virtual ~FirstPersonCamera();

public:

	virtual void Rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f) override;
};

