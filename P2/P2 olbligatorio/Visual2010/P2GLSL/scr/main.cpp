#include "BOX.h"
#include <IGL/IGlib.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>


//Idenficadores de los objetos de la escena
int objId =-1;

//Declaración de CB
void resizeFunc(int width, int height);
void idleFunc();
void keyboardFunc(unsigned char key, int x, int y);
void mouseFunc(int button, int state, int x, int y);

glm::mat4 view = glm::mat4(1.0); 

int main(int argc, char** argv)
{
	std::locale::global(std::locale("spanish"));// acentos ;)
	if (!IGlib::init("../shaders_P2/shader.v7.vert", "../shaders_P2/shader.v7.frag"))
		return -1;
   
	//CBs
	IGlib::setResizeCB(resizeFunc);
	IGlib::setIdleCB(idleFunc);
	IGlib::setKeyboardCB(keyboardFunc);
	IGlib::setMouseCB(mouseFunc);

	//Se ajusta la cámara
	//Si no se da valor se cojen valores por defecto

	view[3].z = -6.0f;
	view[3].x = -0.0f;
	view[3].y = -0.0f;

	glm::mat4 proj = glm::mat4(1.0);
	float f = 1.0f / tan(3.141592 / 6);
	float far = 10.0;
	float near = 0.1;

	proj[0].x = f;
	proj[1].y = f;
	proj[2].z = (far + near) / (near - far);
	proj[2].w = -1.0f;
	proj[3].z = (2.0f * far * near) / (near - far);
	proj[3].w = 0.0f;
	IGlib::setProjMat(proj);
	IGlib::setViewMat(view);

	//Creamos el objeto que vamos a visualizar
	objId = IGlib::createObj(cubeNTriangleIndex, cubeNVertex, cubeTriangleIndex, 
			cubeVertexPos, cubeVertexColor, cubeVertexNormal,cubeVertexTexCoord, cubeVertexTangent);
	IGlib::addColorTex(objId, "../img/color2.png");
	IGlib::addEmissiveTex(objId, "../img/emissive.png");
	IGlib::addSpecularTex(objId, "../img/specMap.png");
	IGlib::addNormalTex(objId, "../img/normal.png");
		
	glm::mat4 modelMat = glm::mat4(4.0f);
	IGlib::setModelMat(objId, modelMat);
	
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
	//Ajusta el aspect ratio al tamaño de la venta
	float wi = width;
    float he = height;
    float aspectratio = wi / he;
 
    glm::mat4 model = glm::perspective(70.0f, aspectratio, 1.0f, 50.0f); //angulo,aspect radio, near, far
 
    IGlib::setProjMat(model);
}

void idleFunc()
{
	glm::mat4 modelMat(1.0f);
	static float angle = 0.0f;
	angle = (angle > 3.141592f * 2.0f) ? 0 : angle + 0.0005f;

	modelMat = glm::rotate(modelMat, angle, glm::vec3(1.0f, 1.01f, 0.0f));

	IGlib::setModelMat(objId, modelMat);
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
