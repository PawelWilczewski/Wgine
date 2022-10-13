#type vertex
#version 460 core

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec4 in_Color;
layout(location = 2) in vec2 in_TexCoord;
layout(location = 3) in vec3 in_Normal;

layout (std430, binding = 0) buffer ss_MaterialIDs
{
	int MaterialIDs[];
};

uniform mat4 u_ViewProjection;

out vec4 io_Color;
out vec2 io_TexCoord;
out vec3 io_Normal;
out flat int io_MaterialID;

void main()
{
	io_Color = in_Color;
	io_TexCoord = in_TexCoord;
	io_Normal = in_Normal;
	io_MaterialID = MaterialIDs[gl_VertexID];

//	gl_Position = u_ViewProjection * a_Transform * vec4(a_Position, 1.0);
	gl_Position = u_ViewProjection * vec4(in_Position, 1.0);
}

#type fragment
#version 460 core

struct Material
{
	vec3 Diffuse;
	vec3 Specular;
	vec3 Ambient;
	int DiffuseTex;
	int SpecularTex;
};

layout (std430, binding = 1) buffer ss_Materials
{ 
	Material Materials[];
};

layout(location = 0) out vec4 out_Color;

in vec4 io_Color;
in vec2 io_TexCoord;
in vec3 io_Normal;
in flat int io_MaterialID;

uniform sampler2D u_Texture[32];
uniform vec2 u_Tiling; // TODO: tiling implemented per-texture (how?) (in material per texture?)

void main()
{
	// out_Color = vec4(in_MaterialID / 512.0, in_MaterialID / 512.0 , in_MaterialID / 512.0, 1.0);
	// out_Color = vec4(Materials[io_MaterialID].Specular, 1.0);
//	if (Data[io_MaterialID].DiffuseTex < 0)
//		out_Color = vec4(1.f);
//	else
//		out_Color = vec4(0.f);
	Material mat = Materials[io_MaterialID];
	if (mat.DiffuseTex >= 0)
		out_Color = texture(u_Texture[Materials[io_MaterialID].DiffuseTex], io_TexCoord * u_Tiling) * vec4(Materials[io_MaterialID].Diffuse, 1.0);
	else
		out_Color = vec4(Materials[io_MaterialID].Diffuse, 1.0);
		
//	if (!bool(io_MaterialID))
//		out_Color = vec4(1.0, 0.0, 1.0, 1.0);
//	out_Color = vec4(vec3(Materials.length()), 1.0);
//	if (Materials.length() == 0)
//	{
//		out_Color = vec4(0.5, 0.7, 1.0, 1.0);
//	}
//	else
//	{
//		out_Color = vec4(1.0, 0.2, 0.3, 1.0);
//	}
//	out_Color = vec4(vec3(io_MaterialID / 1000.0), 1.0);
//	out_Color = vec4(vec3(io_MaterialID / 100.0), 1.0);
	// out_Color = vec4(in_TexCoord, 0.0, 1.0);
	// out_Color = in_Color;
}
