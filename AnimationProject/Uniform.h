#pragma once
#include <vector>
#include "glad.h"
#include "vec2.h"
#include "vec3.h"
#include "vec4.h"
#include "mat4.h"
#include "quat.h"


//Uniform Class used in OpenGL PipeLine
template <typename T>
class Uniform
{
private:
	Uniform();
	Uniform(const Uniform&);
	Uniform& operator=(const Uniform&);
	~Uniform();
public:
	static void Set(unsigned int slot, const T& value);
	static void Set(unsigned int slot, T* arr, unsigned int len);
	static void Set(unsigned int slot, std::vector<T>& arr);
};