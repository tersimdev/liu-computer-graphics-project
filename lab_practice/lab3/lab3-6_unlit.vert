#version 150

in vec3 in_Position;
in vec3 in_Normal;
in vec2 in_TexCoord;

out vec3 v_Normal;
out vec2 v_TexCoord;

uniform mat4 modelMtx;
uniform mat4 viewMtx;
uniform mat4 projectionMtx;
uniform mat3 normalMtx; //modified modelMtx

void main(void)
{
	vec4 pos = projectionMtx * viewMtx * modelMtx * vec4(in_Position, 1.0);
	vec3 normal = normalMtx * in_Normal;
	
	v_TexCoord = in_TexCoord;
	v_Normal = normalize(normal);
	gl_Position = pos;
}
