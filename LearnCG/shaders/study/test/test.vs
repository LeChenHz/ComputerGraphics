#version 330 core
layout (location = 0) in vec3 quadVertices;
layout (location = 1) in vec4 xyzs;
layout (location = 2) in vec4 color;

out vec4 ourColor;
out vec2 UV;
out float control;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 T;

void main()
{
	vec3 vertexPosition = xyzs.xyz;
	gl_Position = projection * view *T* model * vec4(vertexPosition + quadVertices, 1.0f);
	ourColor = color;
	control = xyzs.s;
	UV = quadVertices.xy + vec2(0.5,0.5);
}