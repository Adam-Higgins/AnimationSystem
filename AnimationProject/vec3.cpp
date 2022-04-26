#include "vec3.h"
#include <math.h>

// + operator for the vector 3 struct
vec3 operator+(const vec3 & l, const vec3 & r)
{
	return vec3(l.x + r.x, l.y + r.y, l.z + r.z);
}

// - operator for the vector 3 struct
vec3 operator-(const vec3 & l, const vec3 & r)
{
	return vec3(l.x - r.x, l.y - r.y, l.z -r.z);
}

// * operator for multiplying a vector by a scalar float
vec3 operator*(const vec3 & v, float f)
{
	return vec3(v.x * f, v.y * f, v.z * f);
}

// * operator for multiplying a vector by another vector
vec3 operator*(const vec3 & l, const vec3 & r)
{
	return vec3(l.x * r.x, l.y * r.y, l.z * r.z);
}

// returns the dot product between 2 vectors
float dot(const vec3 & l, const vec3 & r)
{
	return l.x * r.x + l.y * r.y + l.z * r.z;
}

//returns the sqared length of a vector
float lenSq(const vec3 & v)
{
	return (v.x * v.x) + (v.y * v.y) + (v.z * v.z);
}

//returns the length of a vector. try to use lenSq when possible as it avoids sqrtf()
float len(const vec3 & v)
{
	float lenSq = (v.x * v.x) + (v.y * v.y) + (v.z * v.z);
	if (lenSq < VEC3_EPSILON)
	{
		return 0.0f;
	}
	return sqrtf(lenSq);
}

//normalizes a given vector
void normalize(vec3 & v) 
{
	float lenSq = (v.x * v.x) + (v.y * v.y) + (v.z * v.z);
	if (lenSq < VEC3_EPSILON) { return; }
	float invLen = 1.0f / sqrtf(lenSq);
	v.x *= invLen;
	v.y *= invLen;
	v.z *= invLen;
}

//returns the normalized version of a vector but doesn't change it
vec3 normalized(const vec3 &v)
{
	float lenSq = (v.x * v.x) + (v.y * v.y) + (v.z * v.z);
	if (lenSq < VEC3_EPSILON) { return v; }
	float invLen = 1.0f / sqrtf(lenSq);
	return vec3(v.x * invLen, v.y * invLen, v.z * invLen);
}

//returns the angle between two vectors in degrees
float angleDeg(const vec3 & l, const vec3 & r)
{
	float sqMagL = (l.x * l.x) + (l.y * l.y) + (l.z * l.z);
	float sqMagR = (r.x * r.x) + (r.y * r.y) + (r.z * r.z);

	if (sqMagL < VEC3_EPSILON || sqMagR < VEC3_EPSILON)
	{
		return 0.0f;
	}

	float dot = (l.x * r.x) + (l.y * r.y) + (l.z * r.z);
	float len = sqrtf(sqMagL) * sqrtf(sqMagR);
	
	return acosf(dot / len) * 57.2958f;
}

//returns the angle between 2 vectors in radians
float angleRad(const vec3 & l, const vec3 & r)
{
	float sqMagL = (l.x * l.x) + (l.y * l.y) + (l.z * l.z);
	float sqMagR = (r.x * r.x) + (r.y * r.y) + (r.z * r.z);

	if (sqMagL < VEC3_EPSILON || sqMagR < VEC3_EPSILON)
	{
		return 0.0f;
	}

	float dot = (l.x * r.x) + (l.y * r.y) + (l.z * r.z);
	float len = sqrtf(sqMagL) * sqrtf(sqMagR);

	return acosf(dot / len);
}

//project a vector onto another vector
vec3 project(const vec3 & a, const vec3 & b)
{
	float magBSq = len(b);
	if (magBSq < VEC3_EPSILON)
	{
		return vec3();
	}
	float scale = dot(a, b) / magBSq;
	return b * scale;
}

//reject a vector from another vector
vec3 reject(const vec3 & a, const vec3 & b)
{
	vec3 projection = project(a, b);
	return a - projection;
}

//reflect a vector through another vector
vec3 reflect(const vec3 & a, const vec3 & b)
{
	float magBSq = len(b);
	if (magBSq < VEC3_EPSILON)
	{
		return vec3();
	}
	float scale = dot(a, b) / magBSq;
	vec3 proj2 = b * (scale * 2);
	return a - proj2;
}

//returns the cross product between two vectors - a vector perpendicular to the two vectors
vec3 cross(const vec3 & l, const vec3 & r)
{
	return vec3(
	l.y * r.z - l.z * r.y,
	l.z * r.x - l.x * r.z,
	l.x * r.y - l.y * r.x
	);
}

//lerps a vector to another vector over the period t
vec3 lerp(const vec3 & s, const vec3 & e, float t)
{
	return vec3(
		s.x + (e.x - s.x) * t,
		s.y + (e.y - s.y) * t,
		s.z + (e.z - s.z) * t
	);
}

//smooth lerp for a vector
vec3 slerp(const vec3 & s, const vec3 & e, float t)
{
	if (t < 0.01f)
	{
		return lerp(s, e, t);
	}
	vec3 from = normalized(s);
	vec3 to = normalized(e);

	float theta = angleRad(from, to);
	float sin_theta = sinf(theta);

	float a = sinf((1.0f - t) * theta) / sin_theta;
	float b = sinf(t * theta) / sin_theta;

	return from * a + to * b;
}

//normalized lerp for a vector
vec3 nlerp(const vec3 & s, const vec3 & e, float t)
{
	vec3 linear(
		s.x + (e.x - s.x) * t,
		s.y + (e.y - s.y) * t,
		s.z + (e.z - s.z) * t
	);
	return normalized(linear);
}

// == operator to check if 2 vectors are equal
bool operator==(const vec3 & l, const vec3 & r)
{
	vec3 diff(l - r);
	return lenSq(diff) < VEC3_EPSILON;
}

//!= to do the opposite of the == operator
bool operator!=(const vec3 & l, const vec3 & r)
{
	return !(l == r);
}