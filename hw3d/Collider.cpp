#include "Collider.h"


Collider::Collider(TransformParameters transform) : transform(transform) {}

void Collider::SetPos(DirectX::XMFLOAT3 pos) noexcept
{
	transform.x = pos.x;
	transform.y = pos.y;
	transform.z = pos.z;
}

void Collider::SetRotation(float rotX, float rotY, float rotZ) noexcept
{
	transform.xRot = rotX;
	transform.yRot = rotY;
	transform.zRot = rotZ;
}

void Collider::SetScaling(float scaleX, float scaleY, float scaleZ) noexcept
{
	transform.scaleX = scaleX;
	transform.scaleY = scaleY;
	transform.scaleZ = scaleZ;
}

Collider::~Collider() {}
