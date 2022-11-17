// invalid texture index is uint8(-1) = 255
#define INVALID_TEXTURE_INDEX 255U

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
