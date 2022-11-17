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
    float d = max(length(light.Location - io_WorldPos) - light.Radius, 0.0); // distance respecting the light radius
    float t = d / light.Cutoff; // normalized distance
	float attenuation = light.Intensity / (t * t); // inverse square, div by 0 possible but the perf hit of removing that is not worth it

	return (ambient + diffuse + specular) * attenuation;
}
