#pragma once

class Camera {
public:
	Camera() = default;
	Camera(const Camera&) = delete;
	Camera& operator=(const Camera&) = delete;
	~Camera() = default;
protected:
	// �� �޾ƿ;� ���� �ڷ� ���� ����� �غ���
};