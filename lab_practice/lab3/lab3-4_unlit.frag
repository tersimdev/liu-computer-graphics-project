#version 150

in vec3 v_Normal;
in vec2 v_TexCoord;

out vec4 out_Color;

uniform float elapsedTime;
uniform sampler2D texUnit;

void main(void)
{
	//maps time to o to 1 cycles
	//float cycle = sin(elapsedTime  * 2) * 0.5f + 0.5f;

	//base texture color
	vec4 texColor = texture(texUnit, v_TexCoord);

	//final output
	out_Color = texColor;
}
