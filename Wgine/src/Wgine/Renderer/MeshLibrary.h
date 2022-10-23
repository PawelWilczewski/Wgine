#pragma once

#include "Mesh.h"

namespace Wgine
{
	// TODO: should create some MeshGenerator class and thus Mesh will be read-only so that returned cube cant be messed up by the user etc.
	class MeshLibrary
	{
	public:
		static Ref<Mesh> Get(const std::string &filePath); // TODO: <- implement mesh file import

		static Ref<Mesh> GetQuad();
		static Ref<Mesh> GetCube(bool smooth = false);

		static Ref<Mesh> GetSphere(uint32_t slices = 32U, uint32_t stacks = 32U);
		// TODO: add more shapes
	};
}
