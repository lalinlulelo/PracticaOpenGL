#version 330 core

in vec3 inPos;	
in vec3 inColor;
in vec2 inTexCoord;
in vec3 inNormal;
in vec3 inTangent;

uniform mat4 modelViewProj;
uniform mat4 modelView;
uniform mat4 normal;

out vec3 color;
out vec3 pos;
out vec3 norm;
out vec2 texCoord;
out mat3 TBN;

void main()
{
	color = inColor;
	texCoord = inTexCoord;
	norm = (normal * vec4(inNormal, 0.0)).xyz;
	pos = (modelView * vec4(inPos, 1.0)).xyz;

	vec3 Tangent = normalize (inTangent);
	vec3 Normal = normalize (inNormal);
	vec3 Binormal = normalize (cross(Normal,inTangent));

	TBN[0].x = Tangent.x;
	TBN[0].y = Binormal.x;
	TBN[0].z = Normal.x;

	TBN[1].x = Tangent.y;
	TBN[1].y = Binormal.y;
	TBN[1].z = Normal.z;

	TBN[2].x = Tangent.z;
	TBN[2].y = Binormal.z;
	TBN[2].z = Normal.z;
	
	gl_Position =  modelViewProj * vec4 (inPos,1.0);
}
