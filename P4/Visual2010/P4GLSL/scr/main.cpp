#include "BOX.h"
#include "auxiliar.h"
#include "PLANE.h"

#include <windows.h>

#include <gl/glew.h>
#include <gl/gl.h>
#define SOLVE_FGLUT_WARNING
#include <gl/freeglut.h> 
#include <iostream>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <cstdlib>

#define RAND_SEED 31415926
#define SCREEN_SIZE 500,500

//////////////////////////////////////////////////////////////
// Datos que se almacenan en la memoria de la CPU
//////////////////////////////////////////////////////////////

//Matrices
glm::mat4	proj = glm::mat4(1.0f);
glm::mat4	view = glm::mat4(1.0f);
glm::mat4	model = glm::mat4(1.0f);
glm::mat4   model3 = glm::mat4(1.0f);

//vector de 4 coordenadas que se usara para el motion blur
glm::vec4   glcolor = glm::vec4(0.3f, 0.3f, 0.3f, 0.8f);

//Distancia focal y desenfoque maximo
float  dFocal = -15.0f;
float  maxD = 1.0f/5.0f;

//Valores Near y Far
float  Far = 50.0f;
float  Near = 1.0f;

//////////////////////////////////////////////////////////////
// Variables que nos dan acceso a Objetos OpenGL
//////////////////////////////////////////////////////////////
float angle = 0.0f;

//VAO
unsigned int vao;

//VBOs que forman parte del objeto
unsigned int posVBO;
unsigned int colorVBO;
unsigned int normalVBO;
unsigned int texCoordVBO;
unsigned int triangleIndexVBO;
unsigned int tangentVBO;

unsigned int colorTexId;
unsigned int emiTexId;
unsigned int normalTexId;
unsigned int specularTexId;
unsigned int depthTexId;

//Define las variables que nos darán acceso al objeto cuadrado
unsigned int planeVAO;
unsigned int planeVertexVBO;

//Por definir
unsigned int vshader;
unsigned int fshader;
unsigned int program;

//Variables Uniform 
int uModelViewMat;
int uModelViewProjMat;
int uNormalMat;

//Texturas Uniform
int uColorTex;
int uEmiTex;
int uNormalTex;
int uSpecularTex;
int ufocalDistance;
int umaxDistanceFactor;
int uNear;
int uFar;
//uniform de profundidad
int uDepthTex;

//Atributos
int inPos;
int inColor;
int inNormal;
int inTexCoord;
int inTangent;

unsigned int postProccesVShader;
unsigned int postProccesFShader;
unsigned int postProccesProgram;
//Uniform
unsigned int uColorTexPP;
unsigned int uVertexTexPP;
unsigned int vertexBuffTexId;

//PostProceso para buffer de profundidad
unsigned int depthBuffTexId;
unsigned int uDepthTexPP;

//Atributos
int inPosPP;
//Define las variables que nos darán acceso al objeto cuadrado
unsigned int fbo;
unsigned int colorBuffTexId;
       

//Mask
float mask = 1.0f;
int uMask;


//////////////////////////////////////////////////////////////
// Funciones auxiliares
//////////////////////////////////////////////////////////////

//Declaración de CB
void renderFunc();
void resizeFunc(int width, int height);
void idleFunc();
void keyboardFunc(unsigned char key, int x, int y);
void mouseFunc(int button, int state, int x, int y);

void renderCube();

//Funciones de inicialización y destrucción
void initContext(int argc, char** argv);
void initOGL();
void initShaderFw(const char *vname, const char *fname);
void initShaderPP(const char *vname, const char *fname);
void initObj();
void initPlane();
void destroy();
void initFBO();
void resizeFBO(unsigned int w, unsigned int h);

//Carga el shader indicado, devuele el ID del shader
//!Por implementar
GLuint loadShader(const char *fileName, GLenum type);

//Crea una textura, la configura, la sube a OpenGL, 
//y devuelve el identificador de la textura 
//!!Por implementar
unsigned int loadTex(const char *fileName);
//////////////////////////////////////////////////////////////
// Nuevas variables auxiliares
//////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////
// Nuevas funciones auxiliares
//////////////////////////////////////////////////////////////
//!!Por implementar


int main(int argc, char** argv)
{
	std::locale::global(std::locale("spanish"));// acentos ;)
	//Pregunta si tu ordenador soporta el filtro anisotropico
	if (glewIsSupported("GL_EXT_texture_filter_anisotropic")){
	   std::cout << "Extension es soportado ";
    }
    else{
	   std::cout << "Extension no es soportado ";
    }


	initContext(argc, argv);
	initOGL();
	initShaderFw("../shaders_P4/fwRendering.v2.vert", "../shaders_P4/fwRendering.v2.frag");
	initObj();
	initShaderPP( "../shaders_P4/postProcessing.v2.vert", "../shaders_P4/postProcessing.v2.frag");
	initPlane();
	initFBO();
    resizeFBO(SCREEN_SIZE);

	glutMainLoop();

	destroy();

	return 0;
}

//////////////////////////////////////////
// Funciones auxiliares 
void initContext(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 3);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	//glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(SCREEN_SIZE);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Prácticas GLSL");

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cout << "Error: " << glewGetErrorString(err) << std::endl;
		exit(-1);
	}

	const GLubyte *oglVersion = glGetString(GL_VERSION);
	std::cout << "This system supports OpenGL Version: " << oglVersion << std::endl;

	glutReshapeFunc(resizeFunc);
	glutDisplayFunc(renderFunc);
	glutIdleFunc(idleFunc);
	glutKeyboardFunc(keyboardFunc);
	glutMouseFunc(mouseFunc);
}

void initOGL()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);

	glFrontFace(GL_CCW);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);

	proj = glm::perspective(glm::radians(60.0f), 1.0f, 1.0f, 50.0f);
	view = glm::mat4(1.0f);
	view[3].z = -25.0f;
}


void destroy()
{
	glDetachShader(program, vshader);
	glDetachShader(program, fshader);
	glDeleteShader(vshader);
	glDeleteShader(fshader);
	glDeleteProgram(program);

	glDetachShader(postProccesProgram, postProccesVShader);
    glDetachShader(postProccesProgram, postProccesFShader);
    glDeleteShader(postProccesVShader);
    glDeleteShader(postProccesFShader);
    glDeleteProgram(postProccesProgram);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &planeVertexVBO);
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &planeVAO);

	glBindFramebuffer(GL_FRAMEBUFFER,0);
    glDeleteFramebuffers(1, &fbo);
    glDeleteTextures(1, &colorBuffTexId);
    glDeleteTextures(1, &depthBuffTexId);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	if (inPos != -1) glDeleteBuffers(1, &posVBO);
	if (inColor != -1) glDeleteBuffers(1, &colorVBO);
	if (inNormal != -1) glDeleteBuffers(1, &normalVBO);
	if (inTexCoord != -1) glDeleteBuffers(1, &texCoordVBO);
	if (inTangent != -1) glDeleteBuffers(1, &tangentVBO);
	glDeleteBuffers(1, &triangleIndexVBO);

	glBindVertexArray(0);
	glDeleteVertexArrays(1, &vao);

	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &colorTexId);
	glDeleteTextures(1, &emiTexId);
	glDeleteTextures(1, &normalTexId);
	glDeleteTextures(1, &specularTexId);
	glDeleteTextures(1, &vertexBuffTexId);	
}

void initShaderFw(const char *vname, const char *fname)
{
	vshader = loadShader(vname, GL_VERTEX_SHADER);
	fshader = loadShader(fname, GL_FRAGMENT_SHADER);

	program = glCreateProgram();
	glAttachShader(program, vshader);
	glAttachShader(program, fshader);

	glBindAttribLocation(program, 0, "inPos");
	glBindAttribLocation(program, 1, "inColor");
	glBindAttribLocation(program, 2, "inNormal");
	glBindAttribLocation(program, 3, "inTexCoord");
	glBindAttribLocation(program, 4, "inTangent");


	glLinkProgram(program);

	int linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		//Calculamos una cadena de error
		GLint logLen;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);

		char *logString = new char[logLen];
		glGetProgramInfoLog(program, logLen, NULL, logString);
		std::cout << "Error: " << logString << std::endl;
		delete logString;

		glDeleteProgram(program);
		program = 0;
		exit(-1);
	}

	uNormalMat = glGetUniformLocation(program, "normal");
	uModelViewMat = glGetUniformLocation(program, "modelView");
	uModelViewProjMat = glGetUniformLocation(program, "modelViewProj");
	ufocalDistance = glGetUniformLocation(program, "focalDistance");
	umaxDistanceFactor = glGetUniformLocation(program, "maxDistanceFactor");


	uColorTex = glGetUniformLocation(program, "colorTex");
	uEmiTex = glGetUniformLocation(program, "emiTex");
	uNormalTex = glGetUniformLocation(program, "normalTex");
	uSpecularTex = glGetUniformLocation(program, "specularTex");
	uDepthTex = glGetUniformLocation(program, "depthTex");

	inPos = glGetAttribLocation(program, "inPos");
	inColor = glGetAttribLocation(program, "inColor");
	inNormal = glGetAttribLocation(program, "inNormal");
	inTexCoord = glGetAttribLocation(program, "inTexCoord");
	inTangent = glGetAttribLocation(program, "inTangent");
}

void initObj()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	if (inPos != -1)
	{
		glGenBuffers(1, &posVBO);
		glBindBuffer(GL_ARRAY_BUFFER, posVBO);
		glBufferData(GL_ARRAY_BUFFER, cubeNVertex*sizeof(float) * 3,
			cubeVertexPos, GL_STATIC_DRAW);
		glVertexAttribPointer(inPos, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inPos);
	}

	if (inColor != -1)
	{
		glGenBuffers(1, &colorVBO);
		glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
		glBufferData(GL_ARRAY_BUFFER, cubeNVertex*sizeof(float) * 3,
			cubeVertexColor, GL_STATIC_DRAW);
		glVertexAttribPointer(inColor, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inColor);
	}

	if (inNormal != -1)
	{
		glGenBuffers(1, &normalVBO);
		glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
		glBufferData(GL_ARRAY_BUFFER, cubeNVertex*sizeof(float) * 3,
			cubeVertexNormal, GL_STATIC_DRAW);
		glVertexAttribPointer(inNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inNormal);
	}

	if (inTexCoord != -1)
	{
		glGenBuffers(1, &texCoordVBO);
		glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
		glBufferData(GL_ARRAY_BUFFER, cubeNVertex*sizeof(float) * 2,
			cubeVertexTexCoord, GL_STATIC_DRAW);
		glVertexAttribPointer(inTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inTexCoord);
	}

		if (inTangent != -1)
	{
		glGenBuffers(1, &tangentVBO);
		glBindBuffer(GL_ARRAY_BUFFER, tangentVBO);
		glBufferData(GL_ARRAY_BUFFER, cubeNVertex*sizeof(float) * 2,
			cubeVertexTexCoord, GL_STATIC_DRAW);
		glVertexAttribPointer(inTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(inTangent);
	}

	glGenBuffers(1, &triangleIndexVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleIndexVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		cubeNTriangleIndex*sizeof(unsigned int) * 3, cubeTriangleIndex,
		GL_STATIC_DRAW);

	model = glm::mat4(1.0f);

	colorTexId = loadTex("../img/color2.png");
	emiTexId = loadTex("../img/emissive.png");
	normalTexId = loadTex("../img/normal.png");
	specularTexId = loadTex("../img/specMap.png");
}

GLuint loadShader(const char *fileName, GLenum type)
{
	unsigned int fileLen;
	char *source = loadStringFromFile(fileName, fileLen);

	//////////////////////////////////////////////
	//Creación y compilación del Shader
	GLuint shader;
	shader = glCreateShader(type);
	glShaderSource(shader, 1,
		(const GLchar **)&source, (const GLint *)&fileLen);
	glCompileShader(shader);
	delete source;

	//Comprobamos que se compilo bien
	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		//Calculamos una cadena de error
		GLint logLen;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);

		char *logString = new char[logLen];
		glGetShaderInfoLog(shader, logLen, NULL, logString);
		std::cout << "Error: " << logString << std::endl;
		delete logString;

		glDeleteShader(shader);
		exit(-1);
	}

	return shader;
}

unsigned int loadTex(const char *fileName)
{
    //Filtro anisotropico
	GLfloat fLargest;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest);

	unsigned char *map;
	unsigned int w, h;
	map = loadTexture(fileName, w, h);

	if (!map)
	{
		std::cout << "Error cargando el fichero: "
			<< fileName << std::endl;
		exit(-1);
	}

	unsigned int texId;
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, (GLvoid*)map);
	delete[] map;
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);

	return texId;
}

void renderFunc()
{
	//Situar antes de limpiar el FB
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/**/
	glUseProgram(program);

	//Texturas
	if (uColorTex != -1)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, colorTexId);
		glUniform1i(uColorTex, 0);
	}

	if (uEmiTex != -1)
	{
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, emiTexId);
		glUniform1i(uEmiTex, 1);
	}

	if (uDepthTex != -1)
	{
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, depthTexId);
		glUniform1i(uDepthTex, 1);
	}


	if (uNormalTex != -1)
	{
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, normalTexId);
		glUniform1i(uNormalTex, 1);
	}

	if (uSpecularTex != -1)
	{
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, specularTexId);
		glUniform1i(uSpecularTex, 1);
	}

	model = glm::mat4(2.0f);
	model[3].w = 1.0f;
	model = glm::rotate(model, angle, glm::vec3(1.0f, 1.0f, 0.0f));
	renderCube();

	std::srand(RAND_SEED);
	for (unsigned int i = 0; i < 10; i++)
	{
		float size = float(std::rand() % 3 + 1);

		glm::vec3 axis(glm::vec3(float(std::rand() % 2),
			float(std::rand() % 2), float(std::rand() % 2)));
		if (glm::all(glm::equal(axis, glm::vec3(0.0f))))
			axis = glm::vec3(1.0f);

		float trans = float(std::rand() % 7 + 3) * 1.00f + 0.005f;
		glm::vec3 transVec = axis * trans;
		transVec.x *= (std::rand() % 2) ? 1.0f : -1.0f;
		transVec.y *= (std::rand() % 2) ? 1.0f : -1.0f;
		transVec.z *= (std::rand() % 2) ? 1.0f : -1.0f;

		model = glm::rotate(glm::mat4(1.0f), angle*2.0f*size, axis);
		model = glm::translate(model, transVec);
		model = glm::rotate(model, angle*1.0f*size, axis);
		model = glm::scale(model, glm::vec3(2.0f / (size*0.7f)));
		renderCube();
	}//*/
	//Situar después de renderizar los cubos
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	glUseProgram(postProccesProgram);
	glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

	
	//Motion Blur blending
    //Utiliza las funciones de blending para fusionar el frame actual con el anterior
    glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_CONSTANT_ALPHA, GL_CONSTANT_COLOR);
    glBlendColor(glcolor.x, glcolor.y, glcolor.z, glcolor.w);
    glBlendEquation(GL_FUNC_ADD);   
	
	//Asignamos distancia focal y desenfoque max(despues de BlendColor)
	float  focalDistance = dFocal;
    float  maxDistanceFactor = maxD; 

	float variable_near = Near;
	float variable_far = Far;

	if (ufocalDistance != -1)
	{
		glUniform1f(ufocalDistance, focalDistance);
	}	
	if (umaxDistanceFactor != -1)
	{
		glUniform1f(umaxDistanceFactor,maxDistanceFactor);
	}

	if (uFar != -1)
	{
		glUniform1f(uFar, Far);
	}	
	if (uNear != -1)
	{
		glUniform1f(uNear,Near);
	}

    if (uMask != -1)
	{
		glUniform1f(uMask,mask);
	}

    if (uColorTexPP != -1)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, colorBuffTexId);
        glUniform1i(uColorTexPP, 0);
    }
	if (uVertexTexPP != -1)
    {
        glActiveTexture(GL_TEXTURE0+1);
        glBindTexture(GL_TEXTURE_2D, vertexBuffTexId);
        glUniform1i(uVertexTexPP, 1);
    }
	if (uDepthTexPP != -1)
    {
        glActiveTexture(GL_TEXTURE0+2);
        glBindTexture(GL_TEXTURE_2D, depthBuffTexId);
        glUniform1i(uDepthTexPP, 2);
    }



    glBindVertexArray(planeVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);//post proceso

    glUseProgram(NULL);

	glutSwapBuffers();
}

void renderCube()
{
	glm::mat4 modelView = view * model;
	glm::mat4 modelViewProj = proj * view * model;
	glm::mat4 normal = glm::transpose(glm::inverse(modelView));

	if (uModelViewMat != -1)
		glUniformMatrix4fv(uModelViewMat, 1, GL_FALSE,
		&(modelView[0][0]));
	if (uModelViewProjMat != -1)
		glUniformMatrix4fv(uModelViewProjMat, 1, GL_FALSE,
		&(modelViewProj[0][0]));
	if (uNormalMat != -1)
		glUniformMatrix4fv(uNormalMat, 1, GL_FALSE,
		&(normal[0][0]));
	
	glBindVertexArray(vao);//Pinta con el mismo vao
	glDrawElements(GL_TRIANGLES, cubeNTriangleIndex * 3,
		GL_UNSIGNED_INT, (void*)0);

	//Tercer cubo
	modelView = view * model3;
	modelViewProj = proj * view * model3;
	normal = glm::transpose(glm::inverse(modelView));

	if (uModelViewMat != -1)
		glUniformMatrix4fv(uModelViewMat, 1, GL_FALSE,
		&(modelView[0][0]));

	if (uModelViewProjMat != -1)
		glUniformMatrix4fv(uModelViewProjMat, 1, GL_FALSE,
		&(modelViewProj[0][0]));

	if (uNormalMat != -1)
		glUniformMatrix4fv(uNormalMat, 1, GL_FALSE,
		&(normal[0][0]));
	//Activa el VAO con la configuración del objeto y pinta la lista de triángulos
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, cubeNTriangleIndex * 3,
		GL_UNSIGNED_INT, (void*)0);
}



void resizeFunc(int width, int height)
{
	glViewport(0, 0, width, height);
	proj = glm::perspective(glm::radians(60.0f), float(width) /float(height), 1.0f, 50.0f);

	resizeFBO(width, height);

	glutPostRedisplay();
}

void idleFunc()
{
	angle = (angle > 3.141592f * 2.0f) ? 0 : angle + 0.0005f;

	//Tercer cubo con trayectoria de curva de Bezier
	static float t = 0.0f;
	t = (t < 1.0f)? t + 0.0002f : 0.0f;
	float y = ((1.0f - t) * (1.0f - t) * (1.0f - t) * (1.0f - t)) * 4.0f + ((1.0f - t) * (1.0f - t)) * 8 * (t*t) * (-12.0f) + (t*t*t*t) * 4.0f;
	float z =  ((1.0f - t) * (1.0f - t) * (1.0f - t)) * 6 * t * 15.0f + (1.0f - t) * 6 * (t*t*t) * (-15.0f);
	model3 = glm::mat4(1.0f);
	model3 = glm::scale(model3, glm::vec3(0.3f, 0.3f, 0.3f));
	model3 = glm::translate (model3, glm::vec3(0.0f, y, z));
	
	glutPostRedisplay();
}

void keyboardFunc(unsigned char key, int x, int y){
    std::cout << "Se ha pulsado la tecla " << key << std::endl << std::endl;
    //Hacemos un switch para hacer una acción determinada dependiendo del teclado pulsado
        
    switch (key){
		//MotionBlur
		case 'z':
        if (glcolor.x < 1){
		   glcolor.x += 0.1;
	    }
		break;
		case 'x':
        if (glcolor.x > 0){
		   glcolor.x -= 0.1;
	    }
		break;
		case 'c':
	    if (glcolor.y < 1){
		   glcolor.y += 0.1;
	    }
		break;
		case 'v':
	    if (glcolor.y > 0){
		   glcolor.y -= 0.1;
	    }
		break;
		case 'd':
	    if (glcolor.z < 1){
		   glcolor.z += 0.1;
	    }
		break;
		case 'f':
        if (glcolor.z > 0){
		   glcolor.z -= 0.1;
	    }
		break;
		case 'b':
	    if (glcolor.w < 1){
		   glcolor.w += 0.1;
	    }
		break;
		case 'n':
        if (glcolor.w > 0){
		   glcolor.w -= 0.1;
	    }
		break;

		//Distancia focal
	    case 'w':
	        dFocal += 1.0f;
            break;
	    case 'q':
		    dFocal -= 1.0f;
            break;
	    
		//Desenfoque máximo
	    case 'a':
	        maxD -= 1.0f;
            break;
	    case 's':
		    maxD += 1.0f;
            break;

	    //Mask
		case 'i':
			mask = 1.0f;
		        break;
		case 'o':
			mask = 2.0f;
		        break;
		case 'p':
			mask = 3.0f;
		        break;
		case 'j':
			mask = 4.0f;
		        break;
		case 'k':
			mask = 5.0f;
		        break;
		case 'l':
			mask = 6.0f;
		        break;
	}

	glutPostRedisplay();
}

//Nos creamos las variables para la funcion mouseFunc
float xAhora = 0.0f;
float yAhora = 0.0f;
float xDiferencia;
float yDiferencia;
float angulo = 0.2;

void mouseFunc(int button, int state, int x, int y){
	glm::mat4 matAux (1.0); //Matriz auxiliar para trasladar y rotar
    
	if (state==0){
		std::cout << "Se ha pulsado el botón ";
        xAhora = x;  //Posicion inicial de coordenada x
        yAhora = y;  //Posicion inicial de coordenada y
	}else{
		std::cout << "Se ha soltado el botón ";
		//coordenadas inciales - actuales
        xDiferencia = xAhora - x; 
        yDiferencia = yAhora - y;
	}
	
    if (button == 0){
        std::cout << "de la izquierda del ratón " << std::endl;
        if (xDiferencia <= 40){
            matAux = glm::rotate(matAux, angulo*(yDiferencia/100.0f),glm::vec3(2.0f, 0.0f ,0.0f));//matriz para rotar en el eje X
            view = matAux*view;
        }
    }
    
    if (button == 1){
        std::cout << "central del ratón " << std::endl;
        matAux = glm::translate(matAux,glm::vec3(0.0f, 0.0f, yDiferencia/100.0f)); //Sirve para avanzar o retroceder con la cámara
        view = matAux*view;
	}
    
    if (button == 2){
        std::cout << "de la derecha del ratón " << std::endl;
        if (yDiferencia <= 40){
        matAux = glm::rotate(matAux, angulo*(xDiferencia/100.0f),glm::vec3(0.0f, 2.0f ,0.0f));//matriz para rotar en el eje Y
        view = matAux*view;
        }
    }

	std::cout << "en la posición " << x << " " << y << std::endl << std::endl;

	glutPostRedisplay();
}

 void initShaderPP(const char *vname, const char *fname){
	 postProccesVShader = loadShader(vname, GL_VERTEX_SHADER);
     postProccesFShader = loadShader(fname, GL_FRAGMENT_SHADER);

     postProccesProgram = glCreateProgram();
     glAttachShader(postProccesProgram, postProccesVShader);

	 glAttachShader(postProccesProgram, postProccesFShader);
     glBindAttribLocation(postProccesProgram, 0, "inPos");
	 glLinkProgram(postProccesProgram);
	 int linked;
	 glGetProgramiv(postProccesProgram, GL_LINK_STATUS, &linked);
	 if (!linked)
	 {

	 //Calculamos una cadena de error
     GLint logLen;
	 glGetProgramiv(postProccesProgram, GL_INFO_LOG_LENGTH, &logLen);
	 char *logString = new char[logLen];
	 glGetProgramInfoLog(postProccesProgram, logLen, NULL, logString);
	 std::cout << "Error en el enlazado de postproceso " << logString << std::endl;
	 std::cout << "Error: " << logString << std::endl;
	 delete logString;
	 glDeleteProgram(postProccesProgram);
	 postProccesProgram = 0;
	 exit(-1);
	 }

	 uColorTexPP = glGetUniformLocation(postProccesProgram, "colorTex");//Subo la variable con el identificador
	 inPosPP = glGetAttribLocation(postProccesProgram, "inPos");
	 //Configura la carga de los shaders de post-proceso para acceder a la nueva textura
	 uVertexTexPP = glGetUniformLocation(postProccesProgram, "vertexTex");

	 //Profundidad uniforme le asignamos la textura
	 uDepthTexPP = glGetUniformLocation(postProccesProgram, "depthTex");


	 ufocalDistance = glGetUniformLocation(postProccesProgram, "focalDistance");
	 umaxDistanceFactor = glGetUniformLocation(postProccesProgram, "maxDistanceFactor");
     
	 uNear = glGetUniformLocation(postProccesProgram, "Nnear");
	 uFar = glGetUniformLocation(postProccesProgram, "Nfar");

	 uMask = glGetUniformLocation(postProccesProgram, "mask");
}

 void initPlane() {
	 //unsigned int vaolist[8];
	 //glGenVertexArrays(8, vaolist);
	 glGenVertexArrays(1, &planeVAO);
     glBindVertexArray(planeVAO);

     glGenBuffers(1, &planeVertexVBO);
     glBindBuffer(GL_ARRAY_BUFFER, planeVertexVBO);
     glBufferData(GL_ARRAY_BUFFER, planeNVertex*sizeof(float) * 3,
                  planeVertexPos, GL_STATIC_DRAW);

     glVertexAttribPointer(inPosPP, 3, GL_FLOAT, GL_FALSE, 0, 0);
     glEnableVertexAttribArray(inPosPP);
 }

  void initFBO(){
	  glGenFramebuffers(1, &fbo);
      glGenTextures(1, &colorBuffTexId);
      glGenTextures(1, &depthBuffTexId);
	  glGenTextures(1, &vertexBuffTexId);
  }

void resizeFBO(unsigned int w, unsigned int h){
	glBindTexture(GL_TEXTURE_2D, colorBuffTexId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0,
                GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, depthBuffTexId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, w, h, 0,
                GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, vertexBuffTexId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, w, h, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                GL_TEXTURE_2D, colorBuffTexId, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D,
                vertexBuffTexId, 0);

	//... Después de activar el FBO
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                           GL_TEXTURE_2D, depthBuffTexId, 0);

    const GLenum buffs[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT3};//A salida 0 corresponde color attachment3
    glDrawBuffers(2, buffs);

    //const GLenum buffs[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
	//glDrawBuffers(2, buffs);

    if (GL_FRAMEBUFFER_COMPLETE != glCheckFramebufferStatus(GL_FRAMEBUFFER))
    {
          std::cerr << "Error configurando el FBO" << std::endl;
          exit(-1);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}