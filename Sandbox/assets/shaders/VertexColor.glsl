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

#include "include/MatrixMath.glsl"

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

#include "include/Light.glsl"
#include "include/Material.glsl"

layout(location = 0) out vec4 out_Color;
layout (std430, binding = 1) buffer ss_Materials { restrict readonly Material Materials[]; };
layout (std430, binding = 4) buffer ss_AmbientLights { restrict readonly AmbientLight AmbientLights[]; };
layout (std430, binding = 5) buffer ss_DirectionalLights { restrict readonly DirectionalLight DirectionalLights[]; };
layout (std430, binding = 6) buffer ss_PointLights { restrict readonly PointLight PointLights[]; };
layout (std430, binding = 7) buffer ss_SpotLights { restrict readonly SpotLight SpotLights[]; };

in vec3 io_Color;

void main()
{
    out_Color = vec4(io_Color, 1.f);
}
