#version 150

in  vec3 ex_Color;
out vec4 out_Color;
uniform sampler2D texUnit;

void main(void)
{
	out_Color = texture(texUnit, vec2(ex_Color));
	out_Color.a = 1.0;
//	out_Color = vec4(ex_Color,1.0);
}
