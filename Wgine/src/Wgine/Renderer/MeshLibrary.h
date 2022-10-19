#pragma once

#include "Mesh.h"

namespace Wgine
{
	class MeshLibrary
	{
	public:
		static Ref<Mesh> Get(const std::string &filePath); // TODO: <- implement mesh file import

		static Ref<Mesh> GetQuad();
		static Ref<Mesh> GetCube();
		// TODO: add more shapes
	};
}
