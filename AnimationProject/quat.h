//header file for quaternion structs
#pragma once
#ifndef _H_QUAT_
#define _H_QUAT_

#include "vec3.h"
#include "mat4.h"

#define QUAT_EPSILON 0.000001f

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

vec3 getAxis(const quat& quat);
float getAngle(const quat& quat);

quat angleAxis(float angle, const vec3& axis);
quat fromTo(const vec3& from, const vec3& to);

quat operator+(const quat& a, const quat& b);
quat operator-(const quat& a, const quat& b);
quat operator*(const quat& a, float b);
quat operator-(const quat& q);

bool operator==(const quat& left, const quat& right);
bool operator!=(const quat& left, const quat& right);

bool sameOrientation(const quat& l, const quat& r);

float dot(const quat& a, const quat& b);
#endif

