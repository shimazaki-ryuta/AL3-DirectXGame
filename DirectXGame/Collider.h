#pragma once

struct Vector3;

class Collider
{
public:

	virtual void OnCollision()=0;
	virtual Vector3 GetWorldPosition()=0;
	virtual float GetRadius() = 0;
};
