#pragma once
#ifndef _H_QUAT_
#define _H_QUAT_

#include "vec3.h"
#include "mat4.h"

struct quat {
	union {
		struct {
			float x;
			float y;
			float z;
			float w;
		};
		struct {
			vec3 vector;
			float scalar;
		};
		float v[4];
	};

	inline quat() : x(0), y(0), z(0), w(1) {}
	inline quat(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
};

quat angleAxis(float angle, const vec3& axis);
#endif

