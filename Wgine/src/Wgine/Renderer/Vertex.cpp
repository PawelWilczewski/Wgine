#include "WginePCH.h"
#include "Vertex.h"

namespace Wgine
{
	const BufferLayout Vertex::s_Layout = {
		{ ShaderDataType::Float3, "in_Position" },
		{ ShaderDataType::Float4, "in_Color" },
		{ ShaderDataType::Float2, "in_TexCoord" },
		{ ShaderDataType::Float3, "in_Normal" },
	};
}
