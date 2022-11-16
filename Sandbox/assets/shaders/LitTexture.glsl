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

// TODO: ALLOW FOR INCLUDES IN GLSL (#include similar thing to <hash> type) then include the below functions this way

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
	io_MaterialID = MaterialIDs[gl_VertexID];

	Transform t = Transforms[TransformIDs[gl_VertexID]];
	mat4 model = translation(t.Location) * rotation3dZ(radians(t.Rotation[2])) * rotation3dY(radians(t.Rotation[1])) * rotation3dX(radians(t.Rotation[0])) * scale(t.Scale);

	vec4 worldPosition = model * vec4(in_Position, 1.0);

	io_Normal = mat3(transpose(inverse(model))) * in_Normal;
	io_WorldPos = vec3(worldPosition);
	gl_Position = u_ViewProjection * worldPosition;
}

#type fragment
#version 460 core

// invalid texture index is uint8(-1) = 255
#define INVALID_TEXTURE_INDEX 255U

layout(location = 0) out vec4 out_Color;

struct Material
{
	vec3 Diffuse;
	float Specular;
	float Ambient;
	float Shininess;
	// this is actually an array of 32, 2-byte uints. stored like this to be more memory efficient (only 32 texture slots in opengl so 8 bits is enough)
	// in order to access the appropriate index, pass the desired slot (0-32) to ActualTextureIndex and then access u_Texture using that slot
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

uniform vec3 u_CameraLocation;

in vec3 io_Color;
in vec2 io_TexCoord;
in vec3 io_Normal;
in flat int io_MaterialID;
in vec3 io_WorldPos;

uniform sampler2D u_Texture[32];
uniform vec2 u_Tiling; // TODO: tiling implemented per-texture (in material array of vec2d?)

// converts given slot to index to be used in u_Texture (conversion from local texture index in range <0-32> to global texture index in range <x-y>)
uint ActualTextureIndex(uint slot)
{
	uint indexInArray = slot / 4;
	uint part = slot % 4;

	return (Materials[io_MaterialID].Textures[indexInArray] >> (8U * part)) & 0xffU;
}

vec4 SampleTexture(uint slot, vec2 coords, vec4 invalidAlternative)
{
	uint index = ActualTextureIndex(slot);
	if (index == INVALID_TEXTURE_INDEX)
		return invalidAlternative;
	else
		return texture(u_Texture[index], coords);
}

vec3 SampleTextureVec3(uint slot, vec2 coords, vec3 invalidAlternative)
{
	return vec3(SampleTexture(slot, coords, vec4(invalidAlternative, 1.0))); // TODO: maybe inefficient this casting, probably can just implement this separately
}

float SampleTextureScalar(uint slot, vec2 coords, float invalidAlternative)
{
	return SampleTexture(slot, coords, vec4(invalidAlternative)).x; // TODO: maybe inefficient this casting, probably can just implement this separately
}

vec3 EvaluatePointLight(PointLight light, Material mat, vec3 matDiffuse, float matSpecular, vec3 normal)
{
	// ambient lighting
    vec3 ambient = matDiffuse * mat.Ambient * light.Color;

	// diffuse lighting
	vec3 lightDir = normalize(light.Location - io_WorldPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = matDiffuse * diff * light.Color;

	// specular lighting
	vec3 viewDir = normalize(u_CameraLocation - io_WorldPos);
	vec3 reflectDir = reflect(-lightDir, normal); 
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.Shininess);
	vec3 specular = matSpecular * spec * light.Color;

	// attenuation
	// calculate normalized light vector and distance to sphere light surface
    vec3 L = light.Location - io_WorldPos;
    float dist = length(L);
    float d = max(dist - light.Radius, 0.0);
    L /= dist;
     
    // calculate basic attenuation
//    float denom = d / max(light.Radius, 0.00001) + 1.0;
//    float attenuation = 1.0 / (denom * denom);
	float denominv = light.Radius / (d + light.Radius);
	float attenuation = denominv * denominv;
     
    // scale and bias attenuation such that:
    //   attenuation == 0 at extent of max influence
    //   attenuation == 1 when d == 0
    attenuation = light.Intensity * max((attenuation - light.Cutoff) / (1.0 - light.Cutoff), 0.0);

	return (ambient + diffuse + specular) * attenuation;
}

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
