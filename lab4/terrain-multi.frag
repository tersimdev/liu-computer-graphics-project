#version 150

out vec4 outColor;
in vec2 texCoord;
in vec3 normal;
in float height;
uniform sampler2D tex0;
uniform sampler2D tex1;
uniform int multiTexture; 

//lighting stuff
uniform vec3 lightDir;
uniform vec4 lightCol;

void main(void)
{
	//add lighting to lab4-3
	//base texture color
	float maxHeight = 2.0;
	float waterHeight = 0.3;
	vec4 texColor;
	if (multiTexture == 1)
	{
		vec4 tex0Col = texture(tex0, texCoord); 
		//boost red and green to look like dirt
		tex0Col.r *= 0.9;
		tex0Col.g *= 0.7;
		tex0Col.b *= 0.6;
		vec4 tex1Col = texture(tex1, texCoord); 
		texColor = mix(tex0Col, tex1Col, height / maxHeight);
		if (height <= waterHeight)
		{
			vec4 waterColor = vec4(0.1,0.5,0.8,1);
			texColor = mix(tex0Col, waterColor, waterHeight - height + 0.2);
		}
	}
	else
		//texColor = texture(tex0, texCoord);
		texColor = vec4(abs(normal), 1);


	///for convenience
	vec3 lightColor = lightCol.rgb;
	float lightStrength = lightCol.a;
	
	//ambient lighting
	float ambientAmt = 0.4;

	//diffuse lighting
	float diffuseAmt = clamp(dot(normal, -lightDir), 0, 1);

	//calc lighting
	//I = kd*Ia + kd*Il*max(0, s·n) + ks*Il*max(0, r·v)^a
	vec3 totalLight = texColor.xyz*lightColor*ambientAmt 
		+ texColor.xyz*lightColor*lightStrength*diffuseAmt;

	//final output
	outColor = vec4(totalLight, 1);
	//outColor = vec4(vec3(height / maxHeight), 1);
}
