#version 330 core

in vec2 TexCoords;

uniform sampler2D tex;
uniform sampler2D hitmap;

void main()
{
	vec3 result = vec3(0.0, 0.0, 0.0);
    result += texture2D(tex, TexCoords).rgb;
    gl_FragColor = vec4(result, 1.0);
}