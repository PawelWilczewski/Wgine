#type "vertex"
#version 460 core

#include "include/Transform.glsl"

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Color;
layout(location = 2) in vec2 in_TexCoord;
layout(location = 3) in vec3 in_Normal;

layout (std430, binding = 0) buffer ss_MaterialIDs { int MaterialIDs[]; };
layout (std430, binding = 2) buffer ss_TransformIDs { int TransformIDs[]; };
layout (std430, binding = 3) buffer ss_Transforms { Transform Transforms[]; };

uniform mat4 u_ViewProjection;

out vec3 io_Color;
out vec2 io_TexCoord;
out vec3 io_Normal;
out flat int io_MaterialID;
out vec3 io_WorldPos;

#include "include/MatrixMath.glsl"

void main()
{
	io_Color = in_Color;
	io_TexCoord = in_TexCoord;
	io_MaterialID = MaterialIDs[gl_VertexID];

	Transform t = Transforms[TransformIDs[gl_VertexID]];
	mat4 model = translation(t.Location) * rotation3dZ(radians(t.Rotation[2])) * rotation3dY(radians(t.Rotation[1])) * rotation3dX(radians(t.Rotation[0])) * scale(t.Scale);

	vec4 worldPosition = model * vec4(in_Position, 1.0);

	io_Normal = mat3(transpose(inverse(model))) * in_Normal;
	io_WorldPos = vec3(worldPosition);
	gl_Position = u_ViewProjection * worldPosition;
}

#type "fragment"
#version 460 core

#include "include/Light.glsl"
#include "include/Material.glsl"

layout(location = 0) out vec4 out_Color;
layout (std430, binding = 1) buffer ss_Materials { Material Materials[]; };
layout (std430, binding = 4) buffer ss_PointLights { PointLight PointLights[]; };

uniform sampler2D u_Texture[32];
uniform vec2 u_Tiling; // TODO: tiling implemented per-texture (in material array of vec2d?)
uniform vec3 u_CameraLocation;

in vec3 io_Color;
in vec2 io_TexCoord;
in vec3 io_Normal;
in flat int io_MaterialID;
in vec3 io_WorldPos;

#include "include/SampleTexture.glsl"
#include "include/LightEval.glsl"

void main()
{
	Material mat = Materials[io_MaterialID];

	vec3 normal = normalize(io_Normal);

	// get textures if valid, otherwise constants
	vec3 matDiffuse = SampleTextureVec3(0U, io_TexCoord, mat.Diffuse);
	float matSpecular = SampleTextureScalar(1U, io_TexCoord, mat.Specular);

	vec3 light = vec3(0.0);
	for (int i = 0; i < 3; i++)
		light += EvaluatePointLight(PointLights[0], mat, matDiffuse, matSpecular, normal);

    out_Color = vec4(vec3(PointLights[0].Intensity) * light, 1.0);
}
