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
		static Ref<Mesh> GetCubeSmooth();
		// TODO: add more shapes
	};
}
