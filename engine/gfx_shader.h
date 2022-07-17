#ifndef GFX_SHADER_H
#define GFX_SHADER_H

#include "core_internal.h"
#include <map>

namespace gfx
{
	struct Shader
	{
		u32 id;
		std::map<const char*, u32> locations;

		Shader(const char *vertexPath, const char *fragmentPath);

		void Bind();
		void Delete();

		u32 GetLocation(const char *name);

		void SendInt(const char *name, int value);
		void SendFloat4(const char *name, f32 a, f32 b, f32 c, f32 d);
		void SendMat4(const char *name, float *elements);
	};
}

#endif