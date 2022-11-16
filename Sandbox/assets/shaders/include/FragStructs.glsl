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
