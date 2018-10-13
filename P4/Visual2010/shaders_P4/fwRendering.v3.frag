#version 330 core

layout(location = 0) out vec4 outColor; //Location es identificador de la salida
layout(location = 1) out vec4 outVertex;


in vec3 color;
in vec3 pos;
in vec3 norm;
in vec2 texCoord;
in mat3 TBN;

uniform sampler2D colorTex;
uniform sampler2D emiTex;
uniform sampler2D specularTex;
uniform sampler2D normalTex;
uniform mat4 model;
uniform mat4 normal;
uniform mat4 modelView;
uniform mat4 view;

//Propiedades del objeto
vec3 Ka;
vec3 Kd;
vec3 Ks;
vec3 N;
float alpha = 5000.0;
vec3 Ke;
vec4 Normal;

//Propiedades de la luz
vec3 Ia = vec3 (0.3);
vec3 Id = vec3 (1.0);
vec3 Is = vec3 (1.0);
vec3 lpos = vec3 (0.0); 

vec3 shade();

void main()
{
	Ka = texture(colorTex, texCoord).rgb;
	Kd = texture(colorTex, texCoord).rgb;
	Ke = texture(emiTex, texCoord).rgb;
	Ks = texture(specularTex, texCoord).rgb;

	Normal = texture (normalTex, texCoord);

	//Bump Mapping
	vec3 coordlocal = 2.0 * Normal.rgb - vec3(1.0);
	N = normalize(TBN*coordlocal*mat3(modelView));

	//N = normalize (norm);

	outColor = vec4(shade(), 1.0); 
	outVertex = vec4(pos,1.0);  
}

vec3 shade()
{
	vec3 c = vec3(0.0);
	c = Ia * Ka;

	//iluminacion difusa segun distancia
	vec3 L = normalize (lpos - pos);
	vec3 c2 = vec3(0.2);
	float d = length(lpos);
	float ate = 1 / (c2.x + c2.y * d + c2.z * d*d);
	vec3 diffuse = Id * Kd * dot (L,N) * ate;
	c += clamp(diffuse, 0.0, 1.0);
	
	//luz especular
	vec3 V = normalize (-pos);
	vec3 R = normalize (reflect (-L,N));
	float factor = max (dot (R,V), 0.01);
	vec3 specular = Is*Ks*pow(factor,alpha);
	c += clamp(specular, 0.0, 1.0);

	c+=Ke;
	
	return c;
}
