#include "stdafx.h"
#include "Mesh.h"

typedef struct _Sphere
{
	XMFLOAT3 xmfCenter;
	float fRadius;
} Sphere;

typedef struct _AABB {
	XMFLOAT3 xmfMin;
	XMFLOAT3 xmfMax;
} AABB;

class BoundingBox {
public:
private:
	Sphere sphere;
	AABB aabb;
protected:
	CMesh boxMesh;
	CMesh sphereMesh;
};
	//BYTE* boundingVolume;;

