#include "BOX.h"
#include <IGL/IGlib.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>


//Idenficadores de los objetos de la escena
int objId =-1;
int objId2 =-2;

//Declaración de CB
void resizeFunc(int width, int height);
void idleFunc();
void keyboardFunc(unsigned char key, int x, int y);
void mouseFunc(int button, int state, int x, int y);

glm::mat4 view(1.0);// glm::mat4(1.0f);

int main(int argc, char** argv)
{
	std::locale::global(std::locale("spanish"));// acentos ;)
	if (!IGlib::init("../shaders_P1/shader.v9.vert", "../shaders_P1/shader.v9.frag"))
		return -1;

	//CBs
	IGlib::setResizeCB(resizeFunc);
	IGlib::setIdleCB(idleFunc);
	IGlib::setKeyboardCB(keyboardFunc);
	IGlib::setMouseCB(mouseFunc); //que boton del raton se ha pulsado

	//Se ajusta la cámara
	//Si no se da valor se cogen valores por defecto
 
	view[3].z = -6.5f;//desplazamiento coordenada z en totalidad
	view[3].x = -0.0f;
	view[3].y = -0.0f;

	glm::mat4 proj(0.0);// = glm::mat4(0.0f); //inicializa la diagonal principal en 0
	float near = 0.1f;
	float far = 100.0f;

	proj[0].x = 1.0f / (tan (3.14159/6.0f));
	proj[1].y = 1.0f / (tan (3.14159/6.0f));
	proj[2].z = -(far+near)/(far-near);
	proj[2].w = -1.0f;
	proj[3].z = -2.0f * far * near/(far-near);

	IGlib::setProjMat(proj);
	IGlib::setViewMat(view);

	//Creamos el objeto que vamos a visualizar 
	objId = IGlib::createObj(cubeNTriangleIndex, cubeNVertex, cubeTriangleIndex, 
			cubeVertexPos, cubeVertexColor, cubeVertexNormal,cubeVertexTexCoord, cubeVertexTangent);
	objId2 = IGlib::createObj(cubeNTriangleIndex, cubeNVertex, cubeTriangleIndex, 
			cubeVertexPos, cubeVertexColor, cubeVertexNormal,cubeVertexTexCoord, cubeVertexTangent);		
		
	glm::mat4 modelMat = glm::mat4(1.0f);
	IGlib::setModelMat(objId, modelMat);
    glm::mat4 modelMat2 = glm::mat4(0.3f);
	IGlib::setModelMat(objId2, modelMat2);

	//Incluir texturas aquí.
	IGlib::addColorTex(objId,"..\\img\\color.png");

	//CBs
	IGlib::setIdleCB(idleFunc);
	IGlib::setResizeCB(resizeFunc);
	IGlib::setKeyboardCB(keyboardFunc);
	IGlib::setMouseCB(mouseFunc);
	
	//Mainloop
	IGlib::mainLoop();
	IGlib::destroy();
	return 0;
}

void resizeFunc(int width, int height)
{
    float wi = width;
    float he = height;
    float aspectratio = wi / he; //El aspect ratio se define por la division de ancho entre altura
 
    glm::mat4 model = glm::perspective(70.0f, aspectratio, 1.0f, 50.0f); //angulo,aspect radio, near, far
 
    IGlib::setProjMat(model);
}

void idleFunc()
{
	static float angle = 0.2f;
	angle = (angle < 2.0f * 3.14159)? angle + 0.0025f : 0.0f;
	glm::mat4 model (1.0f);
	model = glm::rotate (model,angle, glm::vec3(1.0f, 1.0f, 0.0f)); //Matriz que hace que rote sobre si mismo
	glm::mat4 escalado (1.0f);
	escalado = glm::scale (escalado, glm::vec3(0.7f, 0.7f, 0.7f));
	IGlib::setModelMat(objId,model*escalado);

	static float angle2 = 0.3f;
	angle2 = (angle2 < 2.0f * 3.14159)? angle2 + 0.0025f : 0.0f;
	glm::mat4 model2 (1.0f);
	model2 = glm::rotate (model2,angle2, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 traslacion2(2.0f);
	traslacion2 = glm::translate (traslacion2, glm::vec3(-2.5f,0.0f, 0.0f));
	glm::mat4 rotacion2 (1.0f);
	rotacion2 = glm::rotate(rotacion2,angle2, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 escalado2 (1.0f);
	escalado2 = glm::scale (escalado2, glm::vec3(0.5f, 0.5f, 0.5f));
	IGlib::setModelMat(objId2,model2*traslacion2*rotacion2*escalado2); //Hacemos que rote sobre si mismo entorno al eje Y y sobre el cubo principal
}

void keyboardFunc(unsigned char key, int x, int y)
{
	std::cout << "Se ha pulsado la tecla " << key << std::endl << std::endl;
	//Hacemos un switch para hacer una acción determinada dependiendo del teclado pulsado
   glm::mat4 rotacion(1.0f);
    
    switch (key){
        case 'j':
			rotacion = glm::rotate(rotacion, -0.1f, glm::vec3(0.0f, 1.0f, 0.0f));
            view = rotacion * view; //Movimiento de cámara hacia izquierda
            break;
        case 'k':
            rotacion = glm::rotate(rotacion, 0.1f, glm::vec3(1.0f, 0.0f, 0.0f));
            view = rotacion * view; //Movimiento de cámara hacia abajo
            break;
        case 'l':
			rotacion = glm::rotate(rotacion, 0.1f, glm::vec3(0.0f, 1.0f, 0.0f));
            view = rotacion * view; //Movimiento de cámara hacia derecha
            break;
        case 'i':
            rotacion = glm::rotate(rotacion, -0.1f, glm::vec3(1.0f, 0.0f, 0.0f));
            view = rotacion * view;  //Movimiento de cámara hacia arriba
            break;


        case 's':
            view[3].z -= 1.0f; //Movimiento hacia atrás
            break;
        case 'w':
            view[3].z += 1.0f; //Movimiento hacia adelante
            break;
        case 'a':
		    view[3].x += 1.0f; //Movimiento lateral izquierdo
            break;
        case 'd':
			view[3].x -= 1.0f; //Movimiento lateral derecho
            break;

		//Parte opcional
		case 'q':
        // Roto la cámara hacia la izquierda
            view = glm::rotate(view, 0.1f, glm::vec3(0.0f, 1.0f, 0.0f));
            break;
        case 'e':
        // Roto la cámara hacia la derecha
            view = glm::rotate(view, 0.1f, glm::vec3(0.0f, -1.0f, 0.0f));
            break;
    }
 
    IGlib::setViewMat(view);
}

void mouseFunc(int button, int state, int x, int y)
{
	if (state==0)
		std::cout << "Se ha pulsado el botón ";
	else
		std::cout << "Se ha soltado el botón ";
	
	if (button == 0) std::cout << "de la izquierda del ratón " << std::endl;
	if (button == 1) std::cout << "central del ratón " << std::endl;
	if (button == 2) std::cout << "de la derecha del ratón " << std::endl;

	std::cout << "en la posición " << x << " " << y << std::endl << std::endl;

}
