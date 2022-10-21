#type vertex
#version 460 core

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Color;
layout(location = 2) in vec2 in_TexCoord;
layout(location = 3) in vec3 in_Normal;

struct Transform
{
	vec3 Location;
	vec3 Rotation;
	vec3 Scale;
};

layout (std430, binding = 0) buffer ss_MaterialIDs
{
	int MaterialIDs[];
};

layout (std430, binding = 2) buffer ss_TransformIDs
{
	int TransformIDs[];
};

layout (std430, binding = 3) buffer ss_Transforms
{ 
	Transform Transforms[];
};

mat4 rotation3dX(float angle) {
  float s = sin(angle);
  float c = cos(angle);

  return mat4(
    1.0, 0.0, 0.0, 0.0,
    0.0,   c,   s, 0.0,
    0.0,  -s,   c, 0.0,
	0.0, 0.0, 0.0, 1.0
  );
}

mat4 rotation3dY(float angle) {
  float s = sin(angle);
  float c = cos(angle);

  return mat4(
      c, 0.0,  -s, 0.0,
    0.0, 1.0, 0.0, 0.0,
      s, 0.0,   c, 0.0,
	0.0, 0.0, 0.0, 1.0
  );
}

mat4 rotation3dZ(float angle) {
  float s = sin(angle);
  float c = cos(angle);

  return mat4(
      c,   s, 0.0, 0.0,
     -s,   c, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0,
	0.0, 0.0, 0.0, 1.0
  );
}

mat4 translation(vec3 delta)
{
    return mat4(
        vec4(1.0, 0.0, 0.0, 0.0),
        vec4(0.0, 1.0, 0.0, 0.0),
        vec4(0.0, 0.0, 1.0, 0.0),
        vec4(delta, 1.0)
	);
}

mat4 scale(vec3 s)
{
	return mat4(
        vec4(s[0],  0.0,  0.0, 0.0),
        vec4( 0.0, s[1],  0.0, 0.0),
        vec4( 0.0,  0.0, s[2], 0.0),
        vec4( 0.0,  0.0,  0.0, 1.0)
	);
}

uniform mat4 u_ViewProjection;

out vec3 io_Color;
out vec2 io_TexCoord;
out vec3 io_Normal;
out flat int io_MaterialID;
out vec3 io_WorldPos;

void main()
{
	io_Color = in_Color;
	io_TexCoord = in_TexCoord;
	io_Normal = in_Normal;
	io_MaterialID = MaterialIDs[gl_VertexID];

	Transform t = Transforms[TransformIDs[gl_VertexID]];
	mat4 transform = translation(t.Location) * rotation3dZ(radians(t.Rotation[2])) * rotation3dY(radians(t.Rotation[1])) * rotation3dX(radians(t.Rotation[0])) * scale(t.Scale);

	io_WorldPos = vec3(transform * vec4(t.Location, 1.0));
	gl_Position = u_ViewProjection * transform * vec4(in_Position, 1.0);
}

#type fragment
#version 460 core

layout(location = 0) out vec4 out_Color;

struct Material
{
	vec3 Diffuse;
	vec3 Specular;
	vec3 Ambient;
	uint Textures[8];
};

struct PointLight
{
	vec3 Location;
	vec3 Rotation;
	vec3 Scale;
	float Intensity;
	vec3 Color;
};

layout (std430, binding = 1) buffer ss_Materials
{ 
	Material Materials[];
};

layout (std430, binding = 4) buffer ss_PointLights
{
	PointLight PointLights[];
};

in vec3 io_Color;
in vec2 io_TexCoord;
in vec3 io_Normal;
in flat int io_MaterialID;
in vec3 io_WorldPos;

uniform sampler2D u_Texture[32];
uniform vec2 u_Tiling; // TODO: tiling implemented per-texture (in material array of vec2d?)

sampler2D TextureAt(uint index)
{
	uint indexInArray = index / 4;
	uint part = index % 4;

	return u_Texture[(Materials[io_MaterialID].Textures[indexInArray] >> (8U * part)) & 0xffU];
}

void main()
{
//	out_Color = vec4(io_MaterialID / 5.0, io_MaterialID / 5.0 , io_MaterialID / 5.0, 1.0);

	Material mat = Materials[io_MaterialID];

//	out_Color = texture(TextureAt(0), io_TexCoord /* * u_Tiling */);
//	out_Color = vec4(io_Normal, 1.0);
//
//	out_Color = vec4(PointLights[0].Intensity);
//	out_Color = vec4(PointLights[0].Color, 1.f);

    vec4 color = texture(TextureAt(1), io_TexCoord);

	float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * PointLights[0].Color;

	vec3 lightDir = normalize(PointLights[0].Location - io_WorldPos); 
	float diff = max(dot(io_Normal, lightDir), 0.0);
	vec3 diffuse = diff * PointLights[0].Color;

    out_Color = vec4(ambient + diffuse, 1.0) * color;


//	else
//		out_Color = vec4(mat.Diffuse, 1.0);
//
//	out_Color = vec4(vec3(Materials.length()), 1.0);

//	if (Materials.length() == 0)
//	{
//		out_Color = vec4(0.5, 0.7, 1.0, 1.0);
//	}
//	else
//	{
//		out_Color = vec4(1.0, 0.2, 0.3, 1.0);
//	}
}
