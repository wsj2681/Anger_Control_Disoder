#pragma once
#include "Camera.h"

class SpaceShipCamera : public Camera
{
public:

	SpaceShipCamera() = default;
	SpaceShipCamera(Camera* camera);
	virtual ~SpaceShipCamera();

public:

	virtual void Rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f) override;

};

