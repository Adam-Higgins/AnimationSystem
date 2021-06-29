#pragma once
#include "IndexBuffer.h"
#include "glad.h"

enum class DrawMode
{
	points,
	LineStrip,
	LineLoop,
	Lines,
	Triangles,
	TriangleStrip,
	TriangleFan
};

void Draw(IndexBuffer& inIndexBuffer, DrawMode mode);
void Draw(unsigned int vertexCount, DrawMode mode);

void DrawInstanced(IndexBuffer& inIndexBuffer, DrawMode mode, unsigned int instanceCount);
void DrawInstanced(unsigned int vertexCount, DrawMode mode, unsigned int numInstances);

