#version 330 core

in vec3 inPos;	
in vec3 inColor;
in vec3 inNormal;


uniform mat4 modelViewProj;
uniform mat4 normal;
uniform mat4 modelView;

out vec3 color;

//objeto
vec3 ka;
vec3 kd;
vec3 ks;
float n = 30.0; //nos controla el brillo

vec3 N;
vec3 pos;

//fuente de luz
vec3 Ia = vec3(0.3);
vec3 Il = vec3(1.0);
vec3 Pl = vec3(0.0);

vec3 shade();

void main()
{
    ka = inColor;
	kd = inColor;
	ks = vec3(1.0);

	//ka = vec3(1.0);
	//kd = ka;
	N = normalize(vec3(normal * vec4(inNormal,0.0)));

	pos = vec3(modelView * vec4(inPos,1.0));

	color = shade();
	gl_Position =  modelViewProj * vec4 (inPos,1.0);

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

    return color;
}
