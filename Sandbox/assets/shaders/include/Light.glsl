struct AmbientLight
{
	vec3 Color;
	float Intensity;
};

struct DirectionalLight
{
	vec3 Rotation;
	vec3 Color;
	float Intensity;
};

struct PointLight
{
	vec3 Location;
	vec3 Color;
	float Intensity;
	float Radius;
	float CutoffRadius;
};

struct SpotLight
{
	vec3 Location;
	vec3 Rotation;
	vec3 Color;
	float Intensity;
	float Radius;
	float CutoffRadius;
	float CutoffAngle;
};
