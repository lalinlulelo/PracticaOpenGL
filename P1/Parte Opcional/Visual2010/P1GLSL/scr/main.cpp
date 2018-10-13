#include "BOX.h"
#include "PYRAMID.h"
#include <IGL/IGlib.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>


//Idenficadores de los objetos de la escena
int objId =-1;
int objId2 =-2;
int objId3 =-3;
int objId4 = -4;

//DeclaraciÛn de CB
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

	//Se ajusta la c·mara
	//Si no se da valor se cogen valores por defecto
 

	view[3].z = -6.5f;//desplazamiento coordenada z en totalidad
	view[3].x = 0.0f;
	view[3].y = 0.0f;

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
	objId3 = IGlib::createObj(pyraNTriangleIndex, pyraNVertex, pyraTriangleIndex,
			pyraVertexPos, pyraVertexColor, pyraVertexNormal, pyraVertexTexCoord, pyraVertexTangent);
	objId4 = IGlib::createObj(cubeNTriangleIndex, cubeNVertex, cubeTriangleIndex, 
			cubeVertexPos, cubeVertexColor, cubeVertexNormal,cubeVertexTexCoord, cubeVertexTangent);		
		
	glm::mat4 modelMat = glm::mat4(1.0f);
	IGlib::setModelMat(objId, modelMat);
    glm::mat4 modelMat2 = glm::mat4(0.3f);
	IGlib::setModelMat(objId2, modelMat2);
	glm::mat4 modelMat3 = glm::mat4(0.5f);
	IGlib::setModelMat(objId3, modelMat3);
	IGlib::setModelMat(objId4, modelMat2);

	//Incluir texturas aquÅE
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
    float aspectratio = wi / he;
 
    glm::mat4 model = glm::perspective(70.0f, aspectratio, 1.0f, 50.0f); //angulo,aspect radio, near, far
 
    IGlib::setProjMat(model);
}

void idleFunc()
{
	static float angle = 0.1f;
	angle = (angle < 2.0f * 3.14159)? angle + 0.0015f : 0.0f;
	glm::mat4 model (1.0f);
	model = glm::rotate (model,angle, glm::vec3(1.0f, 1.0f, 0.0f));
	glm::mat4 escalado (1.0f);
	escalado = glm::scale (escalado, glm::vec3(0.7f, 0.7f, 0.7f));
	IGlib::setModelMat(objId,model*escalado);

	static float angle2 = 0.2f;
	angle2 = (angle2 < 2.0f * 3.14159)? angle2 + 0.0015f : 0.0f;
	glm::mat4 model2 (1.0f);
	model2 = glm::rotate (model2,angle2, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 traslacion2(2.0f);
	traslacion2 = glm::translate (traslacion2, glm::vec3(-2.5f, 0.0f, 0.0f));
	glm::mat4 rotacion2 (1.0f);
	rotacion2 = glm::rotate(rotacion2,angle2, glm::vec3(0.0f, 0.5f, 0.0f));
	glm::mat4 escalado2 (1.0f);
	escalado2 = glm::scale (escalado2, glm::vec3(0.5f, 0.5f, 0.5f));
	IGlib::setModelMat(objId2,model2*traslacion2*rotacion2*escalado2);

	static float angle3 = 0.3f;
	angle3 = (angle3 < 2.0f * 3.14159)? angle3 + 0.0025f : 0.0f;
	glm::mat4 model3 (1.0f);
	model3 = glm::rotate (model3,angle2, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 traslacion3(2.3f);
	traslacion3 = glm::translate (traslacion3, glm::vec3(0.0f, 2.7f, 0.0f));
	glm::mat4 escalado3 (1.0f);
	escalado3 = glm::scale (escalado3, glm::vec3(0.5f, 0.5f, 0.5f));
	IGlib::setModelMat(objId3,model3*traslacion3*escalado3);
	
	//P0 = (0,2,0); P1 = (0,0,10); P2 = (0,-8,0); P3 = (0,0,-10);
	static float t = 0.0f;
	t = (t < 1.0f)? t + 0.0002f : 0.0f;
	float y = ((1.0f - t) * (1.0f - t) * (1.0f - t) * (1.0f - t)) * 2.0f + ((1.0f - t) * (1.0f - t)) * 6 * (t*t) * (-8.0f) + (t*t*t*t) * 2.0f;
	float z =  ((1.0f - t) * (1.0f - t) * (1.0f - t)) * 4 * t * 10.0f + (1.0f - t) * 4 * (t*t*t) * (-10.0f);
	glm::mat4 model4 (1.0f);
	glm::mat4 traslacion4(1.0f);
	traslacion4 = glm::translate (traslacion4, glm::vec3(0.0f, y, z));
	glm::mat4 escalado4 (1.0f);
	escalado4 = glm::scale (escalado4, glm::vec3(0.2f, 0.2f, 0.2f));
	IGlib::setModelMat(objId4, model4*traslacion4*escalado4);
}

void keyboardFunc(unsigned char key, int x, int y)
{
	std::cout << "Se ha pulsado la tecla " << key << std::endl << std::endl;
    
    glm::mat4 rotacion(1.0f);
    
    switch (key){
        case 'j':
			rotacion = glm::rotate(rotacion, -0.1f, glm::vec3(0.0f, 1.0f, 0.0f));
            view = rotacion * view; //Movimiento de c·mara hacia izquierda
            break;
        case 'k':
            rotacion = glm::rotate(rotacion, 0.1f, glm::vec3(1.0f, 0.0f, 0.0f));
            view = rotacion * view; //Movimiento de c·mara hacia abajo
            break;
        case 'l':
			rotacion = glm::rotate(rotacion, 0.1f, glm::vec3(0.0f, 1.0f, 0.0f));
            view = rotacion * view; //Movimiento de c·mara hacia derecha
            break;
        case 'i':
            rotacion = glm::rotate(rotacion, -0.1f, glm::vec3(1.0f, 0.0f, 0.0f));
            view = rotacion * view;  //Movimiento de c·mara hacia arriba
            break;


        case 's':
            view[3].z -= 1.0f; //Movimiento hacia atr·s
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
        // Roto la c·mara hacia la izquierda
            view = glm::rotate(view, 0.1f, glm::vec3(0.0f, 1.0f, 0.0f));
            break;
        case 'e':
        // Roto la c·mara hacia la derecha
            view = glm::rotate(view, 0.1f, glm::vec3(0.0f, -1.0f, 0.0f));
            break;
    }
 
    IGlib::setViewMat(view);
}

//Nos creamos las variables para la funcion mouseFunc
float xAhora = 0.0f;
float yAhora = 0.0f;
float xDiferencia;
float yDiferencia;
float angulo = 0.2;

void mouseFunc(int button, int state, int x, int y)
{
    glm::mat4 matAux (1.0); //Matriz auxiliar para trasladar y rotar
    
	if (state==0){
		std::cout << "Se ha pulsado el botÛn ";
        xAhora = x;  //Posicion inicial de coordenada x
        yAhora = y;  //Posicion inicial de coordenada y
	}else{
		std::cout << "Se ha soltado el botÛn ";
		//coordenadas inciales - actuales
        xDiferencia = xAhora - x; 
        yDiferencia = yAhora - y;
	}
	
    if (button == 0){
        std::cout << "de la izquierda del ratÛn " << std::endl;
        if (xDiferencia <= 40){
            matAux = glm::rotate(matAux, angulo*(yDiferencia/100.0f),glm::vec3(2.0f, 0.0f ,0.0f));//matriz para rotar en el eje X
            view = matAux*view;
        }
    }
    
    if (button == 1){
        std::cout << "central del ratÛn " << std::endl;
        matAux = glm::translate(matAux,glm::vec3(0.0f, 0.0f, yDiferencia/100.0f)); //Sirve para avanzar o retroceder con la c·mara
        view = matAux*view;
	}
    
    if (button == 2){
        std::cout << "de la derecha del ratÛn " << std::endl;
        if (yDiferencia <= 40){
        matAux = glm::rotate(matAux, angulo*(xDiferencia/100.0f),glm::vec3(0.0f, 2.0f ,0.0f));//matriz para rotar en el eje Y
        view = matAux*view;
        }
    }

	std::cout << "en la posiciÛn " << x << " " << y << std::endl << std::endl;
	IGlib::setViewMat(view);
}
