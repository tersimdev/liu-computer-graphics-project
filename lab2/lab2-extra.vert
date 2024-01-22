#version 150

in vec3 in_Position;
in vec3 in_Normal;
in vec2 in_TexCoord;

out vec3 v_Normal;
out vec2 v_TexCoord;

uniform mat4 modelMtx;
uniform mat4 viewMtx;
uniform mat4 projectionMtx;

uniform float elapsedTime;

void main(void)
{
	vec4 pos = projectionMtx * viewMtx * modelMtx * vec4(in_Position, 1.0);
	vec3 normal = mat3(viewMtx) * mat3(modelMtx) * in_Normal; //strip translation
	
	//maps time to o to 1 cycles
	float cycle = sin(elapsedTime  * 2) * 0.5f + 0.5f;
	pos = pos + vec4(cycle,0,0,0) * cos(in_TexCoord.x + elapsedTime);
	pos = pos + vec4(0,cycle,0,0) * sin(in_TexCoord.y);
	pos = pos + vec4(0,0,cycle,0) * -cos(in_TexCoord.y * in_TexCoord.x);
	
	v_TexCoord = in_TexCoord;
	v_Normal = normalize(normal);
	gl_Position = pos;
}
