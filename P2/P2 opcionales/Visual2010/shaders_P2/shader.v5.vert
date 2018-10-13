#version 330 core

in vec3 inPos;	
in vec3 inColor;
in vec3 inNormal;
in vec2 inTexCoord;	
in vec3 inTangent;	
out vec2 texCoord;
out vec3 tangent;

uniform mat4 modelViewProj;
uniform mat4 normal;
uniform mat4 modelView;

out vec3 color;
out vec3 outPos;
out vec3 outNormal;
out mat3 TBN;

void main()
{    
    texCoord = inTexCoord;

	vec3 Tangent = normalize (vec3(inTangent));
	vec3 Normal = normalize (vec3(inNormal));
	vec3 Binormal = normalize (vec3 (cross(Normal,inTangent)));

	TBN[0].x = Tangent.x;
	TBN[0].y = Binormal.x;
	TBN[0].z = Normal.x;

	TBN[1].x = Tangent.y;
	TBN[1].y = Binormal.y;
	TBN[1].z = Normal.z;

	TBN[2].x = Tangent.z;
	TBN[2].y = Binormal.z;
	TBN[2].z = Normal.z;

    outPos = vec3(modelView * vec4(inPos, 1.0)); //Controla la duración del reflejo, a mayor valor menor tiempo
	outNormal = vec3 (normal * vec4(inNormal, 0.0));
	color = inColor;
	
	gl_Position =  modelViewProj * vec4 (inPos,1.0); //si aumentamos el valor de 1.0 el objeto se ve más lejos
}