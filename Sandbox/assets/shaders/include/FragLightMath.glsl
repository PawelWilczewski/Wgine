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
