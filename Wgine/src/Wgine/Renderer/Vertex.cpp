#include "WginePCH.h"
#include "Vertex.h"

namespace Wgine
{
	const BufferLayout Vertex::s_Layout = {
		{ ShaderDataType::Float3, "a_Position" },
		{ ShaderDataType::Float4, "a_Color" },
		{ ShaderDataType::Float2, "a_TexCoord" },
		{ ShaderDataType::Float3, "a_Normal" },
		// TODO: transform id?...
	};


}
