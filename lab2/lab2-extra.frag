#version 150

in vec3 v_Normal;
in vec2 v_TexCoord;

out vec4 out_Color;

uniform sampler2D texUnit;

void main(void)
{
	//ambient lighting
	float ambientAmt = 0.1;

	//diffuse lighting
	vec3 lightDir = normalize(vec3(1, 0.5, 0)); //direction towards light
	float diffuseAmt = dot(v_Normal, lightDir);
	diffuseAmt = 0.5 * diffuseAmt + 0.5f; //remap from -1,1 to 0,1

	//calc lighting
	vec3 lightColor = vec3(0.9, 0.9, 0.8);
	float lightStrength = 2.0f;
	vec3 totalLight = ambientAmt * lightColor + diffuseAmt * lightColor * lightStrength;

	//base texture color
	vec4 texColor = texture(texUnit, v_TexCoord * 10);

	//final output
	out_Color = vec4(totalLight * texColor.xyz, 1);
}
