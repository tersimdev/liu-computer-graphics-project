#version 150

in vec3 v_FragPos;
in vec3 v_Normal;
in vec2 v_TexCoord;

out vec4 out_Color;

uniform float elapsedTime;
uniform sampler2D texUnit;

//lighting stuff
#define MAX_POINT_LIGHTS 4
#define kc 1.0
#define kl 0.22
#define kq 0.20

struct Light
{
    int type;       // 0 = disabled
    vec4 color;     // alpha for intensity/attenuation
    vec3 position;  // treated as direction for directional
};
uniform Light dLight;
uniform Light pLight[MAX_POINT_LIGHTS];
uniform vec3 viewPos;

//material stuff
uniform vec3 albedo;
uniform vec4 specular;
uniform float tileFactor;

vec3 calc_directional_light(vec3 baseColor, vec3 normal, vec3 viewDir)
{
	if (dLight.type == 0)
		return vec3(0);
	//for convenience
	vec3 lightColor = dLight.color.rgb;
	float lightStrength = dLight.color.a;
	vec3 lightDir = dLight.position;

	//ambient lighting
	float ambientAmt = 0.3;
	//diffuse lighting
	float diffuseAmt = clamp(dot(normal, -lightDir), 0, 1);
	//specular lighting
	float specularAmt = dot(reflect(lightDir, normal), viewDir);
	specularAmt = pow(max(specularAmt, 0), specular.a);

	//calculate lighting
	//I = kd*Ia + kd*Il*max(0, s·n) + ks*Il*max(0, r·v)^a
	vec3 totalLight = baseColor * lightColor * ambientAmt
		+ baseColor * lightColor * lightStrength * diffuseAmt
		+ specular.rgb * specularAmt;

	return totalLight;
}

vec3 calc_point_light(int i, vec3 baseColor, vec3 normal, vec3 viewDir)
{
	if (pLight[i].type == 0)
		return vec3(0);
	
	//for convenience
	vec3 lightColor = pLight[i].color.rgb;
	float lightFalloff = pLight[i].color.a;
	vec3 lightDir = v_FragPos - pLight[i].position;
	float distance = length(lightDir);
	lightDir /= distance;

	//ambient lighting
	float ambientAmt = 0.3;
	//diffuse lighting
	float diffuseAmt = clamp(dot(normal, -lightDir), 0, 1);
	//specular lighting
	float specularAmt = dot(reflect(lightDir, normal), viewDir);
	specularAmt = pow(max(specularAmt, 0), specular.a);
	//attenuation for point light
	float attenuation = lightFalloff / (kc + kl * distance + kq * distance * distance);

	//calculate lighting
	//I = kd*Ia + kd*Il*max(0, s·n) + ks*Il*max(0, r·v)^a
	vec3 totalLight = baseColor * lightColor * ambientAmt
		+ baseColor * lightColor * diffuseAmt
		+ specular.rgb * specularAmt;
	totalLight *= attenuation;
	return totalLight;
}

void main(void)
{
	//base texture color
	vec4 texColor = texture(texUnit, v_TexCoord * tileFactor);
	vec3 baseColor = albedo * texColor.xyz;

	vec3 viewDir = normalize(viewPos - v_FragPos);
	vec3 normal = normalize(v_Normal);

	vec3 outCol = calc_directional_light(baseColor, normal, viewDir);
	for (int i = 0; i < MAX_POINT_LIGHTS; ++i)
	{
		outCol += calc_point_light(i, baseColor, normal, viewDir);
	}

	//final output
	out_Color = vec4(outCol, texColor.a);
}
