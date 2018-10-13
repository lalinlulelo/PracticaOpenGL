#version 330 core

in vec3 inPos;	
in vec3 inColor;
in vec3 inNormal;
in vec2 inTexCoord;	
out vec2 texCoord;

uniform mat4 modelViewProj;
uniform mat4 normal;
uniform mat4 modelView;

out vec3 color;
out vec3 outPos;
out vec3 outNormal;

void main()
{    
    texCoord = inTexCoord;

    outPos = vec3(modelView * vec4(inPos, 1.0));
	outNormal = vec3 (normal * vec4(inNormal, 0.0));
    color = inColor;
	gl_Position =  modelViewProj * vec4 (inPos,1.0);
}

