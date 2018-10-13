#version 330 core

out vec4 outColor;

in vec3 color;
in vec3 outNormal;
in vec3 outPos;
in vec2 texCoord;
in vec3 tangent;
in mat3 TBN;	

uniform sampler2D colorTex;
uniform sampler2D emiTex;
uniform sampler2D specularTex;
uniform sampler2D normalTex;
uniform sampler2D auxiliarTex;
uniform mat4 model;
uniform mat4 normal;
uniform mat4 modelView;
uniform  mat4 view;

    //objeto
    vec3 ka;
    vec3 kd;
    vec3 ks;
	vec3 ke;
    float n = 300.0; //Controla el ancho del cono de luz

    vec3 N;
    vec3 pos;
	vec4 Normal;	

    //fuente de luz
    vec3 Ia = vec3(0.3);
    vec3 Il = vec3(0.2);
    vec3 Pl = vec3(7.0,10.0,7.0);

	//fuente de luz 2 
    vec3 Ia2 = vec3(0.2); //Intensidad ambiental
	vec3 Il2 = vec3(0.3); //Controla la intesidad de la luz de toda la escena
	vec3 Pl2 = vec3(5.0,5.0,5.0); //Controla la posición de la luz sobre el cubo

    vec3 shade();

void main()
{
    ka = texture (colorTex,texCoord).rgb;
	kd = ka;
	ks = vec3(16.0); //Es el brillo del reflejo de la luz
	
	ks = texture (specularTex, texCoord).rgb;
	ke = texture (emiTex, texCoord).rgb;

	Normal = texture (normalTex, texCoord);

	//Bump Mapping
	vec3 coordlocal = 2.0* Normal.rgb - vec3(1.0);
	N = normalize(TBN*coordlocal*mat3(modelView));

	//ka = vec3(0.5);
	//kd = ka;
	//N = normalize(outNormal);
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

	color += color2;
    return color;
}

