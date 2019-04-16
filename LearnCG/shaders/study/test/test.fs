#version 330 core
out vec4 FragColor;

in vec4 ourColor;
in vec2 UV;
in float control;

// texture samplers
uniform sampler2D texture1;

void main()
{
	vec4 texColor =  texture(texture1, UV) * ourColor;

	FragColor = texColor;
}