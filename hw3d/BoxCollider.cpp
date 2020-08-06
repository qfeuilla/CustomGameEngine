#include "BoxCollider.h"

BoxCollider::BoxCollider(TransformParameters transform) {
	type = Type::Box;
}

bool BoxCollider::Collide(Collider& other) const noexcept
{
	switch (other.type) {
	case Type::Box:
		return CollideBox(other);
	}
	return false;
}

bool BoxCollider::CollideBox(Collider& other) const noexcept
{
	int countIntersection;
	int i, iNormal;
	 
}
