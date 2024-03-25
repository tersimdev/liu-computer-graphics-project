#version 150

in  vec3 in_Position;
in  vec3 in_Color;
out vec3 ex_Color;
uniform float dx;
uniform float dy;

void main(void)
{
	ex_Color = in_Color;
	gl_Position = vec4(in_Position, 1.0) + vec4(dx, dy, 0,0);
}
