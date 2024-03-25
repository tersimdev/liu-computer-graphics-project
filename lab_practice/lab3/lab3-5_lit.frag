#version 150

in vec3 v_Normal;
in vec2 v_TexCoord;

out vec4 out_Color;

uniform float elapsedTime;
uniform sampler2D texUnit;

//lighting stuff
uniform vec3 lightDir;
uniform vec4 lightCol;

void main(void)
{
	//maps time to o to 1 cycles
	//float cycle = sin(elapsedTime  * 2) * 0.5f + 0.5f;
	

	//base texture color
	vec4 texColor = texture(texUnit, v_TexCoord);

	///constant for now
	vec3 lightColor = lightCol.rgb;
	float lightStrength = lightCol.a;
	
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
