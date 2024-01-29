#version 150

in vec3 in_Position;
in vec3 in_Normal;
in vec2 in_TexCoord;

out vec3 v_Normal;
out vec2 v_TexCoord;
out vec3 v_ShadedColor;

uniform mat4 modelMtx;
uniform mat4 viewMtx;
uniform mat4 projectionMtx;

void main(void)
{
	vec4 pos = projectionMtx * viewMtx * modelMtx * vec4(in_Position, 1.0);
	vec3 normal = mat3(viewMtx) * mat3(modelMtx) * in_Normal; //strip translation
	normal = normalize(normal);

	///constant for now
	vec3 lightDir = normalize(vec3(1, 0.5, 0)); //direction towards light
	vec3 lightColor = vec3(0.9, 0.9, 0.8);
	float lightStrength = 2.0f;
	
	//ambient lighting
	float ambientAmt = 0.2;

	//diffuse lighting
	float diffuseAmt = dot(normal, lightDir);
	//diffuseAmt = 0.5 * diffuseAmt + 0.5f; //remap from -1,1 to 0,1, uses full range but not "realistic"
	diffuseAmt = clamp(diffuseAmt, 0, 1);

	//calc lighting
	//I = kd*Ia + kd*Il*max(0, s·n)
	vec3 totalLight = lightColor*ambientAmt +  lightColor*lightStrength*diffuseAmt;
	
	v_ShadedColor = totalLight;
	v_TexCoord = in_TexCoord;
	v_Normal = normalize(normal);
	gl_Position = pos;
}
