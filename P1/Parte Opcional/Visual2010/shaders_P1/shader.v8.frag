#version 330 core

out vec4 outColor;

in vec3 normal;
uniform sampler2D colorTex;

void main()
{
	  outColor = abs(vec4(normal,1)); 
}
