#include "quat.h"
#include <cmath>

quat angleAxis(float angle, const vec3& axis)
{
	vec3 norm = normalized(axis);
	float s = sinf(angle * 0.5f);

	return quat(norm.x * s, norm.y * s, norm.z * s, cosf(angle * 0.5f));
}