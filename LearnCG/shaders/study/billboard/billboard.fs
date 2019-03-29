#version 330 core

in vec2 UV;

out vec4 color;

uniform sampler2D myTextureSampler;

uniform float LifeLevel;

void main(){
	color = texture( myTextureSampler, UV );
	
	if (UV.x < LifeLevel && UV.y > 0.3 && UV.y < 0.7 && UV.x > 0.04 )
		color = vec4(0.2, 0.8, 0.2, 1.0); // бли╚
}