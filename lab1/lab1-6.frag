#version 150

in vec3 v_Normal;

out vec4 out_Color;

void main(void)
{
	vec3 light_dir = vec3(0.8,-1.0,-1.0);
	float atten = dot(light_dir, -v_Normal);
	vec3 base_col = vec3(0.15, 0.15, 0.2);
	vec3 light_col = vec3(0.7,0.1,0.4) * atten * 0.7;
	out_Color = vec4(base_col + light_col, 1);
}
