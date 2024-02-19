#version 150

out vec4 outColor;
in vec2 texCoord;
in vec3 normal;
in float height;
uniform sampler2D tex;

//lighting stuff
uniform vec3 lightDir;
uniform vec4 lightCol;

void main(void)
{
	//add lighting to lab4-3
	//base texture color
	vec4 texColor = texture(tex, texCoord);

	///for convenience
	vec3 lightColor = lightCol.rgb;
	float lightStrength = lightCol.a;
	
	//ambient lighting
	float ambientAmt = 0.3;

	//diffuse lighting
	float diffuseAmt = clamp(dot(normal, -lightDir), 0, 1);

	//calc lighting
	//I = kd*Ia + kd*Il*max(0, s·n) + ks*Il*max(0, r·v)^a
	vec3 totalLight = texColor.xyz*lightColor*ambientAmt 
		+ texColor.xyz*lightColor*lightStrength*diffuseAmt;

	//final output
	outColor = vec4(totalLight, 1);
	//outColor = vec4(vec3(diffuseAmt), 1);
	//outColor = vec4(vec3(abs(normal)), 1);

	//outColor = texture(tex, texCoord); //uncomment for lab4-1, 4-2
}
