#version 150

in vec3 v_Normal;
in vec2 v_TexCoord;

out vec4 out_Color;

uniform float elapsedTime;

void main(void)
{
	//maps time to o to 1 cycles
	float cycle = sin(elapsedTime  * 2) * 0.5f + 0.5f;
	vec3 uvColor = vec3(v_TexCoord.xy,0) * (cycle + 0.1);

	//add some shading based on arbitary lightDir and normal
	vec3 lightDir = vec3(0.8,-1.0,-1.0);
	float atten = dot(lightDir, -v_Normal);
	vec3 lightColor = vec3(0.4, 0.4, 0.2);
	//interpolate between light shading and color
	out_Color = vec4(mix(vec3(0.5) + atten * lightColor, uvColor, 0.8), 1);
}
