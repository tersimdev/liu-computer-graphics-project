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
	//add some shading based on arbitary lightDir and normal
	//vec3 lightDir = vec3(0.8,-1.0,-1.0);
	//float atten = dot(lightDir, -v_Normal);
	//vec3 lightColor = vec3(0.4, 0.4, 0.2);

	//use base texture only for now
	out_Color = texture(texUnit, v_TexCoord);
}
