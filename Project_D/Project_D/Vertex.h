#pragma once
class Vertex
{
public:

	Vertex() = default;
	Vertex(const XMFLOAT3& position);
	virtual ~Vertex() = default;

protected:

	XMFLOAT3 position{ 0.f, 0.f, 0.f };

};

