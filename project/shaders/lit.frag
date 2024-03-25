#version 150

in vec3 v_FragPos;
in vec3 v_Normal;
in vec2 v_TexCoord;

out vec4 out_Color;

uniform float elapsedTime;
uniform sampler2D texUnit;

//lighting stuff
uniform vec3 viewPos;
uniform vec3 lightDir;
uniform vec4 lightCol;
uniform float specularExp;

void main(void)
{
	//base texture color
	vec4 texColor = texture(texUnit, v_TexCoord);

	///for convenience
	vec3 lightColor = lightCol.rgb;
	float lightStrength = lightCol.a;
	vec3 viewDir = normalize(viewPos - v_FragPos);
	vec3 normal = normalize(v_Normal);

	//ambient lighting
	float ambientAmt = 0.3;

	//diffuse lighting
	float diffuseAmt = dot(normal, -lightDir);
	//diffuseAmt = 0.5 * diffuseAmt + 0.5f; //remap from -1,1 to 0,1, uses full range but not "realistic"
	diffuseAmt = clamp(diffuseAmt, 0, 1);

	//specular lighting
	float specularAmt = dot(reflect(lightDir, normal), viewDir);
	specularAmt = pow(max(specularAmt, 0), specularExp);
	if (specularExp <= 1)
		specularAmt = 0; //better done using material


	//calc lighting
	//I = kd*Ia + kd*Il*max(0, s·n) + ks*Il*max(0, r·v)^a
	vec3 totalLight = texColor.xyz*lightColor*ambientAmt 
		+ texColor.xyz*lightColor*lightStrength*diffuseAmt 
		+ lightColor*20*specularAmt;
	//totalLight = clamp(totalLight, vec3(0), vec3(1));

	//final output
	out_Color = vec4(totalLight, 1);
}
