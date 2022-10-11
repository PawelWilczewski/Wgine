#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in vec3 a_Normal;
//layout(location = 4) in mat4 a_Transform;

uniform mat4 u_ViewProjection;
uniform int u_MaterialID[512];

out vec4 v_Color;
out vec2 v_TexCoord;
out vec3 v_Normal;
out float v_MaterialID;

void main()
{
	v_Color = a_Color;
	v_TexCoord = a_TexCoord;
	v_Normal = a_Normal;
	v_MaterialID = u_MaterialID[gl_VertexID];

//	gl_Position = u_ViewProjection * a_Transform * vec4(a_Position, 1.0);
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core

struct Material
{
	vec3 Diffuse;
	vec3 Specular;
	vec3 Ambient;
	int DiffuseTex;
	int SpecularTex;
};

layout(location = 0) out vec4 Color;

in vec2 v_TexCoord;
in vec4 v_Color;
in vec3 v_Normal;
in float v_MaterialID;

uniform sampler2D u_Texture[32];
uniform vec2 u_Tiling; // TODO: tiling implemented per-texture (how?) (in material per texture?)
uniform Material u_Material;

void main()
{
	// Color = vec4(v_MaterialID / 512.0, v_MaterialID / 512.0 , v_MaterialID / 512.0, 1.0);
	 Color = texture(u_Texture[int(round(v_MaterialID / 100.0))], v_TexCoord * u_Tiling) * v_Color;
	// Color = vec4(v_TexCoord, 0.0, 1.0);
	// Color = v_Color;
}
