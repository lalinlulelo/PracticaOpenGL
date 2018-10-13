#version 330 core

out vec4 outColor;

in vec3 color;
in vec3 outNormal;
in vec3 outPos;
in vec2 texCoord;

uniform sampler2D colorTex;
uniform sampler2D emiTex;
uniform sampler2D specularTex;
uniform  mat4 view;

    //objeto
    vec3 ka;
    vec3 kd;
    vec3 ks;
	vec3 ke;
    float n = 100.0; 

    vec3 N;
    vec3 pos;

    //fuentes de luz
    vec3 Ia = vec3(0.2);
    vec3 Il = vec3(0.2);
    vec3 Pl = vec3(2.0,2.0,2.0);

	vec3 Il2 = vec3(0.2);
	vec3 Pl2 = vec3(-4.0,-4.0,4.0);

    vec3 shade();

void main()
{
    ka = texture (colorTex,texCoord).rgb;
	kd = ka;
	ks = vec3(16.0); //Es el brillo del reflejo de la luz
	
	ks = texture (specularTex,texCoord).rgb;
	ke = texture (emiTex, texCoord).rgb;

	//ka = vec3(0.5);
	//kd = ka;
	N = normalize(outNormal);
    pos = outPos;

	outColor = vec4(shade(), 0.1);
}

vec3 shade()
{
	vec3 color = vec3(0.0);
	color = Ia*ka;

	vec3 L = normalize(Pl - pos); //inPos pasar a coordenadas de la camara
	//Componentes para la atenuación c,d y fatt
	vec3 c = vec3(0.0, 0.1, 0.0);
	float d = length(Pl);
	float fatt = 1 / (c.x + c.y * d + c.z * d*d);
	vec3 V = normalize(-pos);
	vec3 R = normalize(-reflect(L,N));
		
	float factor = clamp (dot(V,R), 0.0, 1.0); //clamp para poner un minimo
	factor = pow (factor,n); //solo permite n mayor que 0

	color += fatt*Il*(kd*dot(N,L)+ks*factor); //componente difusa y especular
	color += ke; //Componente emisivo

    return color;
}