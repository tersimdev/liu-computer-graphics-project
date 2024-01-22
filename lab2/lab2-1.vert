#version 150

in vec3 in_Position;
in vec3 in_Normal;

out vec3 v_Normal;

uniform mat4 myMatrix;

void main(void)
{
	gl_Position = myMatrix * vec4(in_Position, 1.0);

	v_Normal = in_Normal;
}
