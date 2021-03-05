#pragma once
class Camera
{
public:
	Camera() = default;
	Camera(const Camera&) = delete;
	Camera& operator=(const Camera&) = delete;
	virtual ~Camera() = default;

protected:

public:
};

