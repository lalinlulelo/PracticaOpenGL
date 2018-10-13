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
    vec3 Ia = vec3(0.3);
    vec3 Il = vec3(1.0);
    vec3 Pl = vec3(0.0,0.0,6.0);

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
    vec3 color = vec3(0.0);
	color = Ia*ka;

	vec3 L = normalize(Pl - pos); //inPos pasar a coordenadas de la camara
	color += Il*kd*dot(N,L); //componenete difusa

	vec3 V = normalize(-pos);
	vec3 R = normalize(reflect(-L,N));
	float factor = clamp (dot(V,R), 0.0001, 1.0); //clamp para poner un minimo
	factor = pow (factor,n); //solo permite n mayor que 0
	color += Il * ks * factor; //el sombreado de G solo brilla vertices

	color += ke;

    return color;
}


