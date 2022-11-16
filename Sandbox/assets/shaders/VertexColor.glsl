#type "vertex"
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

void main()
{
	io_Color = in_Color;

	Transform t = Transforms[TransformIDs[gl_VertexID]];
	mat4 model = translation(t.Location) * rotation3dZ(radians(t.Rotation[2])) * rotation3dY(radians(t.Rotation[1])) * rotation3dX(radians(t.Rotation[0])) * scale(t.Scale);
	vec4 worldPosition = model * vec4(in_Position, 1.0);
	gl_Position = u_ViewProjection * worldPosition;
}

#type "fragment"
#version 460 core

layout(location = 0) out vec4 out_Color;

struct Material
{
	vec3 Diffuse;
	float Specular;
	float Ambient;
	float Shininess;
	uint Textures[8];
};

struct PointLight
{
	vec3 Location;
	vec3 Rotation;
	vec3 Scale;
	vec3 Color;
	float Intensity;
	float Radius;
	float Cutoff;
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

void main()
{
    out_Color = vec4(io_Color, 1.f);
}
