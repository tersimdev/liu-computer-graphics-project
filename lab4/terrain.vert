#version 150

in  vec3 inPosition;
in  vec3 inNormal;
in  vec2 inTexCoord;
out vec2 texCoord;

//lab4-3
out vec3 normal;
out float height;

// NY
uniform mat4 projMatrix;
uniform mat4 mdlMatrix;
uniform mat4 viewMatrix;
uniform mat3 normalMatrix;

void main(void)
{
	// lab4-3
	normal = normalMatrix * inNormal;
	height = inPosition.y;
	/////////////
	texCoord = inTexCoord;
	gl_Position = projMatrix * viewMatrix * mdlMatrix * vec4(inPosition, 1.0);
}
