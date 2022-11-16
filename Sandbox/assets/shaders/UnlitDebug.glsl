#type "vertex"
#version 330 core

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Color;
layout(location = 2) in vec2 in_TexCoord;
layout(location = 3) in vec3 in_Normal;

uniform mat4 u_ViewProjection;

out vec3 io_Color;

void main()
{
	io_Color = in_Color;
	gl_Position = u_ViewProjection * vec4(in_Position, 1.0);
}

#type "fragment"
#version 330 core

layout(location = 0) out vec4 out_Color;

in vec3 io_Color;

void main()
{
	out_Color = vec4(io_Color, 1.0);
}
