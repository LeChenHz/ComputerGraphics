#version 330 core

layout(location = 0) in vec3 squareVertices;
//layout(location = 1) in vec4 xyzs; // 坐标和大小
//layout(location = 2) in vec4 color; // 颜色
layout(location = 1) in vec3 xyz; // 坐标

//out vec2 UV;
//out vec4 particlecolor;

uniform vec3 CameraRight_worldspace;
uniform vec3 CameraUp_worldspace;
uniform mat4 VP; // Model-View-Projection matrix, but without the Model (the position is in BillboardPos; the orientation depends on the camera)

void main()
{
	float particleSize = 0.02;
	vec3 particleCenter_wordspace = xyz;
	
	vec3 vertexPosition_worldspace = 
		particleCenter_wordspace
		+ CameraRight_worldspace * squareVertices.x * particleSize
		+ CameraUp_worldspace * squareVertices.y * particleSize;

	gl_Position = VP * vec4(vertexPosition_worldspace, 1.0f);

	//UV = squareVertices.xy + vec2(0.5, 0.5);
	//particlecolor = color;
}

