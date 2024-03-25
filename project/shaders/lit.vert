#version 150

in vec3 in_Position;
in vec3 in_Normal;
in vec2 in_TexCoord;

out vec3 v_FragPos;
out vec3 v_Normal;
out vec2 v_TexCoord;

uniform mat4 modelMtx;
uniform mat4 viewMtx;
uniform mat4 projectionMtx;
uniform mat3 normalMtx; //modified modelMtx

void main(void)
{
	
	v_FragPos = (modelMtx * vec4(in_Position, 1.0)).xyz;
	v_Normal = normalMtx * in_Normal;
	v_TexCoord = in_TexCoord;

	vec4 pos = projectionMtx * viewMtx * modelMtx * vec4(in_Position, 1.0);
	gl_Position = pos;
}
