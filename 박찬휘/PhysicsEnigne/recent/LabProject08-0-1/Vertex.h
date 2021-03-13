#pragma once

class CVertex {
protected:
    //������ ��ġ �����̴�(��� ������ �ּ��� ��ġ ���͸� ������ �Ѵ�).
    XMFLOAT3 m_xmf3Position{ (XMFLOAT3(0.f, 0.f, 0.f)) };

public:
    CVertex() {}
    CVertex(XMFLOAT3 xmf3Position) { m_xmf3Position = xmf3Position; }
    ~CVertex() { }
};

class CDiffusedVertex : public CVertex {
protected:
    //������ �����̴�.
    XMFLOAT4 m_xmf4Diffuse{};
public:
    CDiffusedVertex() {
        m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
        m_xmf4Diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
    }
    CDiffusedVertex(float x, float y, float z, XMFLOAT4 xmf4Diffuse) {
        m_xmf3Position = XMFLOAT3(x, y, z);
        m_xmf4Diffuse = xmf4Diffuse;
    }
    CDiffusedVertex(XMFLOAT3 xmf3Position, XMFLOAT4 xmf4Diffuse) {
        m_xmf3Position = xmf3Position;
        m_xmf4Diffuse = xmf4Diffuse;
    }
    ~CDiffusedVertex() { }
};
