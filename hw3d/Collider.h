#pragma once
#include "TransformParameters.h"
#include "CustomDirectXM.h"

class Collider
{
public:
	enum class Type {
		Default,
		Box,
	};
	Collider() : type(Type::Box) {};
	Collider(TransformParameters transform);
	void SetPos(DirectX::XMFLOAT3 pos) noexcept;
	void SetRotation(float roll, float pitch, float yaw) noexcept;
	void SetScaling(float scaleX, float scaleY, float scaleZ) noexcept;
	virtual bool Collide(Collider& other) const noexcept = 0;
	~Collider();
	Type type;
private:
	TransformParameters transform;
};
