#version 150

in vec3 v_Normal;
in vec2 v_TexCoord;

out vec4 out_Color;

uniform float elapsedTime;
uniform sampler2D texUnit;

uniform vec3 albedo;

void main(void)
{
	//final output
	out_Color = vec4(albedo, 1);
}
