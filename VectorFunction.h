#pragma once
//#include "Matrix.h"
#include <Vector3.h>
Vector3 Add(const Vector3&, const Vector3&);
Vector3 Subtruct(const Vector3&, const Vector3&);
Vector3 Multiply(float, const Vector3&);
float Dot(const Vector3&, const Vector3&);
float Length(const Vector3&);
Vector3 Nomalize(const Vector3&);

Vector3 operator+(const Vector3&, const Vector3&);
Vector3 operator-(const Vector3&, const Vector3&);


Vector3 operator+=(Vector3&,Vector3&);
Vector3 operator-=(const Vector3&, const Vector3&);