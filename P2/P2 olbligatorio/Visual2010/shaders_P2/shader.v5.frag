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
    float n = 300.0; //Controla el ancho del cono de luz

    vec3 N;
    vec3 pos;

    //fuente de luz
    vec3 Ia = vec3(0.01);
    vec3 Il = vec3(0.1);
    vec3 Pl = vec3(6.0,1.0,5.0);

	//fuente de luz 2
    vec3 Ia2 = vec3(0.1); //Intensidad ambiental
	vec3 Il2 = vec3(0.1); //Controla la intesidad de la luz de toda la escena
	vec3 Pl2 = vec3(-5.0,-1.0,2.0); //Controla la posición de la luz sobre el cubo

	//fuente de luz 3
    vec3 Ia3 = vec3(0.1); //Intensidad ambiental
	vec3 Il3 = vec3(0.1); //Controla la intesidad de la luz de toda la escena
	vec3 Pl3 = vec3(0.0,-5.0,5.0); //Controla la posición de la luz sobre el cubo

    vec3 shade();

void main()
{
    ka = texture (colorTex,texCoord).rgb;
	kd = ka;
	ks = vec3(16.0); //Es el brillo del reflejo de la luz
	
	//ks = texture (specularTex,texCoord).rgb;
	ke = texture (emiTex, texCoord).rgb;

	//ka = vec3(0.5);
	kd = ka;
	N = normalize(outNormal);
    pos = outPos;

	outColor = vec4(shade(), 0.1);
}

vec3 shade()
{
    
	vec3 color = vec3(0.0);
	color = Ia*ka;

	vec3 L = normalize(Pl - pos); //inPos pasar a coordenadas de la camara
	vec3 V = normalize(-pos);
	vec3 R = normalize(-reflect(L,N));
		
	float factor = clamp (dot(V,R), 0.0, 1.0); //clamp para poner un minimo
	factor = pow (factor,n); //solo permite n mayor que 0

	color += Il*kd*dot(N,L); //componenete difusa
	color += Il * ks * factor; //el sombreado de G solo brilla vertices
	color += ke;
	

	//Segunda luz
	vec3 color2 = vec3(0.0);
	vec3 L2 = normalize(Pl2 - pos); //inPos pasar a coordenadas de la camara
	vec3 V2 = normalize(-pos);
	vec3 R2 = normalize(-reflect(L2,N));
	
	float factor2 = clamp (dot(V2,R2), 0.0001, 1.0); //clamp para poner un minimo
	factor2 =+ pow (factor2,n); //solo permite n mayor que 0
	
	//color2 += Ia2*ka;
	color2 += Il2*kd*dot(N,L2); //componenete difusa
	color2 += Il2 * ks * factor2; //el sombreado de G solo brilla vertices
	//color2 += ke;

	//Tercera luz
	vec3 color3 = vec3(0.0,0.0,0.0);
	vec3 L3 = normalize(Pl3 - pos); //inPos pasar a coordenadas de la camara
	vec3 V3 = normalize(-pos);
	vec3 R3 = normalize(-reflect(L3,N));
	
	float factor3 = clamp (dot(V3,R3), 0.0001, 1.0); //clamp para poner un minimo
	factor3 =+ pow (factor3,n); //solo permite n mayor que 0
	
	//color3 += Ia2*ka;
	color3 += Il3*kd*dot(N,L3); //componenete difusa
	color3 += Il3 * ks * factor3; //el sombreado de G solo brilla vertices
	//color2 += ke;

	color += color2;
	color += color3;
    return color;
}