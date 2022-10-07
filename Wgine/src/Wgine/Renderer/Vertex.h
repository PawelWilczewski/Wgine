#pragma once

#include "Buffer.h"

#include <string>
#include <sstream>
#include <glm/glm.hpp>

namespace Wgine
{
	// TODO: rename to static vertex (in contrast to skeletal vertex with bone id and bone weights etc.?)
	class Vertex
	{
	public:
		Vertex()
		{}

		Vertex(glm::vec3 position, glm::vec4 color = glm::vec4(1.f), glm::vec2 texCoord = glm::vec2(0.f), glm::vec3 normal = glm::vec3(0.f))
			: Position(position), Color(color), TexCoord(texCoord), Normal(normal)
		{}

		operator std::string() const {
			std::ostringstream ss;
			ss << "Vertex: pos(" << Position.x << ", " << Position.y << ", " << Position.z <<
				")\tcolor(" << Color.x << ", " << Color.y << ", " << Color.z << ", " << Color.w <<
				")\ttex(" << TexCoord.x << ", " << TexCoord.y <<
				")\tnormal(" << Normal.x << ", " << Normal.y << ", " << Normal.z << ")";
			return ss.str();
		}

		glm::vec3 Position = glm::vec3(0.f);
		glm::vec4 Color = glm::vec4(1.f);
		glm::vec2 TexCoord = glm::vec2(0.f);
		glm::vec3 Normal = glm::vec3(0.f);
		//uint32_t transformID; // (set automatically later on)
		// TODO: lighting stuff?

	public:
		static const BufferLayout &GetLayout() { return s_Layout; }

	private:
		static const BufferLayout s_Layout;
	};
}
