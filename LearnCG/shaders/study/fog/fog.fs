#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 view_position;

uniform sampler2D texture_diffuse1;

struct FogInfo {
  float d_min;
  float d_max;
  vec3 color;
};
uniform FogInfo Fog;

void main()
{   
	float d = abs(view_position.z);
	float f_d = (d - Fog.d_min) / (Fog.d_max - Fog.d_min);	
	f_d = clamp(f_d, 0.0, 1.0);
	vec3 texture_color = vec3(texture(texture_diffuse1, TexCoords));
    FragColor = vec4(mix(texture_color, Fog.color, f_d), 1.0);
}