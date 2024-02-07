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
	

	//base texture color
	vec4 texColor = texture(texUnit, v_TexCoord);

	///constant for now
	vec3 lightDir = normalize(vec3(1, 0.5, 0)); //direction towards light
	vec3 lightColor = vec3(0.9, 0.9, 0.8);
	float lightStrength = 2.0f;
	
	//ambient lighting
	float ambientAmt = 0.3;

	//diffuse lighting
	float diffuseAmt = dot(v_Normal, lightDir);
	//diffuseAmt = 0.5 * diffuseAmt + 0.5f; //remap from -1,1 to 0,1, uses full range but not "realistic"
	diffuseAmt = clamp(diffuseAmt, 0, 1);

	//calc lighting
	//I = kd*Ia + kd*Il*max(0, s·n)
	vec3 totalLight = lightColor*ambientAmt +  lightColor*lightStrength*diffuseAmt;

	//final output
	out_Color = vec4(totalLight * texColor.xyz, 1);
}
