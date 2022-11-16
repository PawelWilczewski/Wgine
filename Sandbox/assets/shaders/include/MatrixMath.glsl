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
