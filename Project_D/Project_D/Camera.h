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

};