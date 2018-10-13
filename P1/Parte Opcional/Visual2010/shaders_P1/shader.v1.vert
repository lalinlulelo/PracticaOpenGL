#version 330 core

in vec3 inPos;
uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;
		

void main()
{
//para el paso 1 y matriz de proyeccion

    glm::mat4 view(1.0);// glm::mat4(1.0f);

	view[3].z = -6.0f; //desplazamiento coordenada z en totalidad

	glm::mat4 proj(0.0);// = glm::mat4(0.0f); //inicializa la diagonal principal en 0
	float near = 0.1;
	float far = 100.0;

	proj[0].x = 1.0f / (tan (3.14159/6.0f));
	proj[1].y = 1.0f / (tan (3.14159/6.0f));
	proj[2].z = -(far+near)/(far-near);
	proj[2].w = -1.0f;
	proj[3].z = -2.0f * far * near/(far-near);

	IGlib::setProjMat(proj);
	IGlib::setViewMat(view);

	gl_Position = proj * view * model * vec4(inPos,1.0);
}
