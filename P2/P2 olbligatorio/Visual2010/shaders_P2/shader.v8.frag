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
    float n = 30.0; //nos controla el brillo

    vec3 N;
    vec3 pos;

    //fuente de luz

	//iluminacion ambiental
    vec3 Ia = vec3(0.3);

	//iluminacion difusa 1 y 2
    vec3 Il = vec3(0.0, 0.0, 0.2);
    vec3 Pl = vec3(-10.0, 0.0, 0.0);

	vec3 Il2 = vec3(0.2, 0.0, 0.0);
    vec3 Pl2 = vec3(10.0, 0.0, 0.0);

	//iluminación focal (?)
	vec3 Il3 = vec3(1.0, 1.0, 1.0);
	vec3 Pl3 = vec3(0.0, 0.0, 10.0);

	//iluminación direccional
	vec3 Il4 = vec3(1.0, 1.0, 1.0);
	vec3 Pl4 = vec3(0.0, 10.0, 10.0);

    vec3 shade();

void main()
{
    ka = texture (colorTex,texCoord).rgb;
	kd = ka;
	//ks = vec3(1.0);
	ks = texture (specularTex,texCoord).rgb;
	ke = texture (emiTex, texCoord).rgb;

	//ka = vec3(1.0);
	//kd = ka;
	N = normalize(outNormal);
    pos = outPos;

	outColor = vec4(shade(), 1.0);
}

vec3 shade()
{
	//iluminacion ambiental
    vec3 color = vec3(0.0);
	color = Ia*ka;

	//iluminacion difusa
	vec3 L = normalize(Pl- pos); //inPos pasar a coordenadas de la camara
	vec3 c = vec3(0.0, 0.1, 0.0);
	float d = length(Pl);
	float ate = 1 / (c.x + c.y * d + c.z * d*d);
	//color += Il*kd*dot(N,L)*ate; //componente difusa

	vec3 L2 = normalize(Pl2 - pos);
	//color += Il2*kd*dot(N,L2);

	//iluminación focal
	vec3 Dl = normalize(vec3(vec4(-Pl3, 1.0))); //Dirección en la que apunta el foco
	float angAper = 0.02; //Apertura del foco
	float e = 0.5; //Factor de atenuación por extremos

	vec3 L3 = normalize(vec3(vec4(Pl3,1.0)) - pos);

	if (dot(vec3(Dl), vec3(-L3)) > cos(angAper)){
	float foco = pow ((dot(vec3(Dl),vec3(-L)) - cos(angAper))/(1 - cos(angAper)),e);
	color += foco*Il3*kd*dot(N,L3);
	}
	else
	color += vec3 (0.0);

	//iluminación direccional
	vec3 L4 = normalize(vec3(vec4(-Pl4,0.0)) - pos);
	color += Il4*kd*dot(N,L4);

	//iluminacion especular
	vec3 V = normalize(-pos);
	vec3 R = normalize(reflect(-L,N));
	float factor = clamp (dot(V,R), 0.0001, 1.0); //clamp para poner un minimo
	factor = pow (factor,n); //solo permite n mayor que 0
	color += Il * ks * factor; //el sombreado de G solo brilla vertices

	color += ke;

    return color;
}
