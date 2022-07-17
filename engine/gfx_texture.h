#ifndef GFX_TEXTURE_H
#define GFX_TEXTURE_H

#include "core_internal.h"

namespace gfx
{
	struct Texture
	{
		u32 id;
		int width, height;

		Texture(const char *path);

		void Bind();
		void Delete();
	};
}

#endif