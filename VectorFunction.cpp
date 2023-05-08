#include "VectorFunction.h"
#include <math.h>

Vector3 Add(const Vector3& v1, const Vector3& v2) {
	Vector3 v;
	v.x = v1.x + v2.x;
	v.y = v1.y + v2.y;
	v.z = v1.z + v2.z;
	return v;
}

Vector3 Subtruct(const Vector3& v1, const Vector3& v2) {
	Vector3 v;
	v.x = v1.x - v2.x;
	v.y = v1.y - v2.y;
	v.z = v1.z - v2.z;
	return v;
}

Vector3 Multiply(float scalar, const Vector3& v) {
	Vector3 returnV;
	returnV.x = v.x * scalar;
	returnV.y = v.y * scalar;
	returnV.z = v.z * scalar;
	return returnV;
}

float Dot(const Vector3& v1, const Vector3& v2) { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; }

float Length(const Vector3& v) { return sqrtf(Dot(v, v)); }

Vector3 Nomalize(const Vector3& v) {
	float length = Length(v);
	if (length == 0) {
		return v;
	}
	return Multiply((1.0f / length), v);
}

Vector3 operator+(const Vector3& v1, const Vector3& v2) { return Add(v1,v2); }
Vector3 operator-(const Vector3& v1, const Vector3& v2) { return Subtruct(v1, v2); }

Vector3 operator+=(Vector3& v1,Vector3& v2) { return v1 = Add(v1,v2); }

Vector3 operator-=(const Vector3& v1, const Vector3& v2) { return Subtruct(v1, v2); }