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

//material stuff
uniform vec3 albedo;
uniform vec4 specular;

void main(void)
{
	//base texture color
	vec4 texColor = texture(texUnit, v_TexCoord);
	//not used for now
	texColor = vec4(1);

	///for convenience
	vec3 lightColor = lightCol.rgb;
	float lightStrength = lightCol.a;
	vec3 viewDir = normalize(viewPos - v_FragPos);
	vec3 normal = normalize(v_Normal);

	//ambient lighting
	float ambientAmt = 0.3;
	//diffuse lighting
	float diffuseAmt = clamp(dot(normal, -lightDir), 0, 1);
	//specular lighting
	float specularAmt = dot(reflect(lightDir, normal), viewDir);
	specularAmt = pow(max(specularAmt, 0), specular.a);


	//calculate lighting
	//I = kd*Ia + kd*Il*max(0, s·n) + ks*Il*max(0, r·v)^a
	vec3 baseColor = albedo * texColor.xyz;
	vec3 totalLight = baseColor * lightColor * ambientAmt
		+ baseColor * lightColor * lightStrength * diffuseAmt
		+ specular.rgb * specularAmt;

	//final output
	out_Color = vec4(totalLight, texColor.a);
}
