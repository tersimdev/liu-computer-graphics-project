#version 150

in vec3 v_Normal;
in vec2 v_TexCoord;

out vec4 out_Color;

uniform float elapsedTime;

void main(void)
{
	//maps time to o to 1 cycles
	float cycle = sin(elapsedTime  * 5) * 0.5f + 0.5f;
	vec3 uvColor = vec3(v_TexCoord.xy,0) * (cycle + 0.1);

	//from lecture + scrolling
	float a = sin(v_TexCoord.s*30 + elapsedTime * 10)*0.5+0.5;
 	float b = sin(v_TexCoord.t*30 + elapsedTime * 10)*0.5+0.5;
	vec3 procedualCol = vec3(a,b,1); 

	//mix glowing uvcolors with scrolling procedual lines
	//out_Color = vec4(procedualCol, 1);
	//out_Color = vec4(uvColor, 1);
	out_Color = vec4(mix(uvColor, procedualCol, 0.3), 1);
}
