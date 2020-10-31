#include "quat.h"
#include <cmath>

vec3 getAxis(const quat & quat)
{
	return normalized(vec3(quat.x, quat.y, quat.z));
}

float getAngle(const quat & quat)
{
	return 2.0f * acosf(quat.w);
}

//returns the quaternion of an angle of rotation around an axis
quat angleAxis(float angle, const vec3& axis) 
{
	vec3 norm = normalized(axis);
	float s = sinf(angle * 0.5f);

	return quat(norm.x * s, norm.y * s, norm.z * s, cosf(angle * 0.5f));
}

quat fromTo(const vec3 & from, const vec3 & to)
{
	vec3 f = normalized(from);
	vec3 t = normalized(t);
	if (f == t)
	{
		return quat();
	}
	else if (f == t * -1.0f)
	{
		vec3 ortho = vec3(1, 0, 0);
		if (fabsf(f.y) < fabsf(f.x))
		{
			ortho = vec3(0, 1, 0);
		}
		if (fabsf(f.z) < fabs(f.y) && fabs(f.z) < fabsf(f.x))
		{
			ortho = vec3(0, 0, 1);
		}
		vec3 axis = normalized(cross(f, ortho));
	}
	vec3 half = normalized(f + t);
	vec3 axis = cross(f, half);
	return quat(axis.x, axis.y, axis.z, dot(f, half));
}

quat operator+(const quat & a, const quat & b)
{
	return quat(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

quat operator-(const quat & a, const quat & b)
{
	return quat(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

quat operator*(const quat & a, float b)
{
	return quat(a.x * b, a.y * b, a.z * b, a.w * b);
}

quat operator-(const quat & q)
{
	return quat(-q.x, -q.y, -q.z, -q.w); 
}

bool operator==(const quat & left, const quat & right)
{
	return (fabsf(left.x - right.x) <= QUAT_EPSILON &&
			fabsf(left.y - right.y) <= QUAT_EPSILON &&
			fabsf(left.z - right.z) <= QUAT_EPSILON &&
			fabsf(left.w - right.w) <= QUAT_EPSILON);
}

bool operator!=(const quat & left, const quat & right)
{
	return !(left == right);
}

bool sameOrientation(const quat & left, const quat & right)
{
	return (fabsf(left.x - right.x) <= QUAT_EPSILON &&
			fabsf(left.y - right.y) <= QUAT_EPSILON &&
			fabsf(left.z - right.z) <= QUAT_EPSILON &&
			fabsf(left.w - right.w) <= QUAT_EPSILON) ||
		   (fabsf(left.x + right.x) <= QUAT_EPSILON &&
			fabsf(left.y + right.y) <= QUAT_EPSILON &&
			fabsf(left.z + right.z) <= QUAT_EPSILON &&
			fabsf(left.w + right.w) <= QUAT_EPSILON);
}

float dot(const quat & a, const quat & b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}
