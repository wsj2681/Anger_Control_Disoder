#pragma once

class Camera {
public:
	Camera() = default;
	Camera(const Camera&) = delete;
	Camera& operator=(const Camera&) = delete;
	~Camera() = default;
protected:
	// 뭘 받아와야 할지 자료 보고 고민을 해보자
};