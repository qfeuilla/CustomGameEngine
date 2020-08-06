#pragma once
#include "Collider.h"

class BoxCollider : public Collider
{
	BoxCollider(TransformParameters transform);
	bool Collide(Collider& other) const noexcept override;
	bool CollideBox(Collider& other) const noexcept;
};
