#include "BOX.h"
#include "auxiliar.h"
#include "PYRAMID.h"

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


//////////////////////////////////////////////////////////////
// Datos que se almacenan en la memoria de la CPU
//////////////////////////////////////////////////////////////

//Matrices
glm::mat4	proj = glm::mat4(1.0f); 
glm::mat4	view = glm::mat4(1.0f);
glm::mat4	model = glm::mat4(1.0f);
glm::vec3   posi = glm::vec3(1.0f);
glm::vec3   inten = glm::vec3(1.0f);
glm::mat4	model2 = glm::mat4(1.0f);
glm::mat4   model3 = glm::mat4(1.0f);
glm::mat4   model4 = glm::mat4(1.0f);

//////////////////////////////////////////////////////////////
// Variables que nos dan acceso a Objetos OpenGL
//////////////////////////////////////////////////////////////
//Por definir

unsigned int vshader;
unsigned int fshader;
unsigned int program;

//Variables Uniform
int uModelViewMat;
int uModelViewProjMat;
int uNormalMat;
int uPosicion;
int uIntensidad;

//Atributos
int inPos;
int inColor;
int inNormal;
int inTexCoord;
int inTangent;

//VAO
unsigned int vao;
//VBOs que forman parte del objeto
unsigned int posVBO;
unsigned int colorVBO;
unsigned int normalVBO;
unsigned int texCoordVBO;
unsigned int triangleIndexVBO;
unsigned int tangentVBO;

//VAO de la piramide
unsigned int vao2;
//VBOs que forman parte de la piramide
unsigned int posVBO2;
unsigned int colorVBO2;
unsigned int normalVBO2;
unsigned int texCoordVBO2;
unsigned int triangleIndexVBO2;
unsigned int tangentVBO2;

//Texturas
unsigned int colorTexId;
unsigned int emiTexId;
unsigned int specularTexId;
unsigned int normalTexId;

//Texturas Uniform
int uColorTex;
int uEmiTex;
int uSpecularTex;
int uNormalTex;

//////////////////////////////////////////////////////////////
// Funciones auxiliares
//////////////////////////////////////////////////////////////
//!!Por implementar

//Declaración de CB
void renderFunc();
void resizeFunc(int width, int height);
void idleFunc();
void keyboardFunc(unsigned char key, int x, int y);
void mouseFunc(int button, int state, int x, int y);

//Funciones de inicialización y destrucción
void initContext(int argc, char** argv);
void initOGL();
void initShader(const char *vname, const char *fname);
void initObj(); //Creamos un objeto
void destroy();

//Carga el shader indicado, devuele el ID del shader
//!Por implementar
GLuint loadShader(const char *fileName, GLenum type);

//Crea una textura, la configura, la sube a OpenGL, 
//y devuelve el identificador de la textura 
//!!Por implementar
unsigned int loadTex(const char *fileName){
	unsigned char *map;
    unsigned int w, h;
    map = loadTexture(fileName, w, h);
    if (!map)
    {
      std::cout << "Error cargando el fichero: "
      << fileName << std::endl;
      exit(-1);
    }

	//Crea una textura, la activa y sube a la tarjeta gráfica
	unsigned int texId;
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, //indicamos tamaño en pixeles de ancho y alto, canal de 4 colores y para cada uno reservamos 32 bits de memoria
       GL_UNSIGNED_BYTE, (GLvoid*)map);

	//Libera la memoria de la CPU:
	delete[] map;

	//Crea los mipmaps asociados a la textura: 
	glGenerateMipmap(GL_TEXTURE_2D);

	//Configura el modo de acceso:
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, //min_filter cuando fragmentos tiene cooordenadas de texturas muy similares interpolamos linelamente
	GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT); 
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

	//Filtro anisotropico
	GLfloat fLargest;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest);

	//Devuelve el identificador de la textura
	return texId;
}

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

	initContext(argc, argv); //Contexto para inicializar openGL
	initOGL();
	initShader("../shaders_P3/shader.v2.vert", "../shaders_P3/shader.v2.frag");
	initObj();

	//Bucle de eventos
	glutMainLoop();

	destroy();

	return 0;
}
	
//////////////////////////////////////////
// Funciones auxiliares 
void initContext(int argc, char** argv){
	glutInit(&argc, argv);
    glutInitContextVersion(3, 3);
	//glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    //glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); 
    glutInitWindowSize(640, 480); //Tamaño ventana
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Prácticas GLSL");

	//Depende del sistema operativo
	glewExperimental = GL_TRUE; //Inicializamos glew
    GLenum err = glewInit();  //Si no se ha inicializado sale error
    if (GLEW_OK != err)
    {
    std::cout << "Error: " << glewGetErrorString(err) << std::endl; //Si da error nos dice la version de OpenGl que utilizamos
    exit (-1);
    }
    const GLubyte *oglVersion = glGetString(GL_VERSION);
    std::cout << "This system supports OpenGL Version: " << oglVersion << std::endl;

	//Reciben como parametros una funcion
	glutReshapeFunc(resizeFunc);
    glutDisplayFunc(renderFunc);
    glutIdleFunc(idleFunc);
    glutKeyboardFunc(keyboardFunc);
    glutMouseFunc(mouseFunc);
}

void initOGL(){
	glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.2f, 0.2f, 0.0f);

	glFrontFace(GL_CCW);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_CULL_FACE);

	proj = glm::perspective(glm::radians(60.0f), 1.0f, 0.1f, 50.0f);
    view = glm::mat4(1.0f);
    view[3].z = -6; 
}

void destroy(){
	glDetachShader(program, vshader);
    glDetachShader(program, fshader);
    glDeleteShader(vshader);
    glDeleteShader(fshader);
    glDeleteProgram(program);

	//libera los recursos utilizados
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
	glDeleteVertexArrays(1, &vao2);
	//Liberar recursos en la función void destroy()
	glDeleteTextures(1, &colorTexId);
    glDeleteTextures(1, &emiTexId);
	glDeleteTextures(1, &specularTexId);
	glDeleteTextures(1, &normalTexId);
}

void initShader(const char *vname, const char *fname){
	vshader = loadShader(vname, GL_VERTEX_SHADER);
    fshader = loadShader(fname, GL_FRAGMENT_SHADER);

	program = glCreateProgram(); //Devuelva identificaodr programa
    glAttachShader(program, vshader); //
    glAttachShader(program, fshader);
    glLinkProgram(program);

	glBindAttribLocation(program, 0, "inPos");
    glBindAttribLocation(program, 1, "inColor");
    glBindAttribLocation(program, 2, "inNormal");
    glBindAttribLocation(program, 3, "inTexCoord");
	glBindAttribLocation(program, 4, "inTangent");

	//Comprobacion de error de linkado
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
    delete logString; //Borramos cadena de error
    glDeleteProgram(program); //Borramos programa
	glDeleteShader(vshader);
	glDeleteShader(fshader);

    exit (-1);
    }
	//Despues de linkado
	//identificadores de las variables uniformes:
	uNormalMat = glGetUniformLocation(program, "normal");
    uModelViewMat = glGetUniformLocation(program, "modelView");
    uModelViewProjMat = glGetUniformLocation(program, "modelViewProj");
	uPosicion = glGetUniformLocation(program, "posicion");
	uIntensidad = glGetUniformLocation(program, "intensidad");

	//En la función void initShader(const char *vname, const char *fname), crea los identificadores de las variables uniformes
    uColorTex = glGetUniformLocation(program, "colorTex");
    uEmiTex = glGetUniformLocation(program, "emiTex");
	uSpecularTex = glGetUniformLocation(program, "specularTex");
    uNormalTex = glGetUniformLocation(program, "normalTex");


	//identificadores de los atributos:
	inPos = glGetAttribLocation(program, "inPos");
    inColor = glGetAttribLocation(program, "inColor");
    inNormal = glGetAttribLocation(program, "inNormal");
    inTexCoord = glGetAttribLocation(program, "inTexCoord");
	inTangent = glGetAttribLocation(program, "inTangent");

}

void initObj(){
	//crea y activa el VAO en el que se almacenará la configuración del objeto
	glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

	// los atributos de la malla
	if (inPos != -1)
    {
       glGenBuffers(1, &posVBO); //Activar como buffer de propiedades
       glBindBuffer(GL_ARRAY_BUFFER, posVBO); //Sube las posiciones del cubo
       glBufferData(GL_ARRAY_BUFFER, cubeNVertex*sizeof(float) * 3, //Reserva este espacio para la informacion que pasa
                    cubeVertexPos, GL_STATIC_DRAW); 
       glVertexAttribPointer(inPos, 3, GL_FLOAT, GL_FALSE, 0, 0); //"inPos" identificador de atributo, "3" vector de 3 componentes,...,str,offset
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
    if (inNormal != -1) //igual que inPos
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
       glBufferData(GL_ARRAY_BUFFER, cubeNVertex*sizeof(float) * 3,
                    cubeVertexTangent, GL_STATIC_DRAW);
       glVertexAttribPointer(inTangent, 3, GL_FLOAT, GL_FALSE, 0, 0);
       glEnableVertexAttribArray(inTangent);
    }

	//Crea la lista de índices
	glGenBuffers(1, &triangleIndexVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleIndexVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 cubeNTriangleIndex*sizeof(unsigned int) * 3, cubeTriangleIndex,
                 GL_STATIC_DRAW);

	model = glm::mat4(1.0f);
	model2 = glm::mat4(1.0f);
	model3 = glm::mat4(1.0f);

	//crea y activa el VAO en el que se almacenará la configuración de la piramide
	glGenVertexArrays(1, &vao2);
    glBindVertexArray(vao2);

	if (inPos != -1)
    {
       glGenBuffers(1, &posVBO2);
	   glBindBuffer(GL_ARRAY_BUFFER, posVBO2);
       glBufferData(GL_ARRAY_BUFFER, pyraNVertex*sizeof(float)* 3,
		            pyraVertexPos, GL_STATIC_DRAW);
	   glVertexAttribPointer(inPos, 3, GL_FLOAT, GL_FALSE, 0, 0);
	   glEnableVertexAttribArray(inPos);
    }
    if (inColor != -1)
    {
	   glGenBuffers(1, &colorVBO);
	   glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
	   glBufferData(GL_ARRAY_BUFFER, pyraNVertex*sizeof(float)* 3,
		            pyraVertexColor, GL_STATIC_DRAW);
       glVertexAttribPointer(inColor, 3, GL_FLOAT, GL_FALSE, 0, 0);
       glEnableVertexAttribArray(inColor);
    }
    if (inNormal != -1)
    {
	   glGenBuffers(1, &normalVBO);
	   glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
	   glBufferData(GL_ARRAY_BUFFER, pyraNVertex*sizeof(float)* 3,
		            pyraVertexNormal, GL_STATIC_DRAW);
       glVertexAttribPointer(inNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);
       glEnableVertexAttribArray(inNormal);
    }
    if (inTexCoord != -1)
    {
	   glGenBuffers(1, &texCoordVBO);
	   glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
	   glBufferData(GL_ARRAY_BUFFER, pyraNVertex*sizeof(float)* 2,
		            pyraVertexTexCoord, GL_STATIC_DRAW);
	   glVertexAttribPointer(inTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);
	   glEnableVertexAttribArray(inTexCoord);
    }
    if(inTangent != -1)
    {
       glGenBuffers(1, &tangentVBO2);
	   glBindBuffer(GL_ARRAY_BUFFER, tangentVBO2);
	   glBufferData(GL_ARRAY_BUFFER, cubeNVertex*sizeof(float)* 3,
		            cubeVertexTangent, GL_STATIC_DRAW);
	   glVertexAttribPointer(inTangent, 3, GL_FLOAT, GL_FALSE, 0, 0);
	   glEnableVertexAttribArray(inTangent);
    }

	glGenBuffers(1, &triangleIndexVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleIndexVBO);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
	             pyraNTriangleIndex*sizeof(unsigned int)* 3, pyraTriangleIndex,
	             GL_STATIC_DRAW);

	model4 = glm::mat4(1.0f);

	colorTexId = loadTex("../img/color2.png");
    emiTexId = loadTex("../img/emissive.png");
	specularTexId = loadTex("../img/specMap.png");
	normalTexId = loadTex("../img/normal.png");
}

GLuint loadShader(const char *fileName, GLenum type){
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

	//Comprobamos que se compiló bien
    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled)
    {
    //Calculamos una cadena de error
    GLint logLen;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
    char *logString = new char[logLen]; //Reservar cadena de error
    glGetShaderInfoLog(shader, logLen, NULL, logString);

	std::cout << fileName << std::endl;
    std::cout << "Error: " << logString << std::endl;
    delete logString;
    glDeleteShader(shader);
    exit (-1);
    }

	return shader; 
}

void renderFunc(){
	//glClearColor(0.5f, 0.2f, 0.2f, 1.0f); // cambia el color del fondo

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Buffer de color y profundidad

	//Todo lo que pintemos despues se va a pintar con programa activo
	glUseProgram(program);

	// calcula y sube las matrices requeridas por el shader de vértices
	glm::mat4 modelView = view * model;
    glm::mat4 modelViewProj = proj * view * model;
    glm::mat4 normal = glm::transpose(glm::inverse(modelView));
	glm::vec3 posicion = posi;
	glm::vec3 intensidad = inten;

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

	//Segundo cubo
	modelView = view * model2;
	modelViewProj = proj * view * model2;
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

	//Piramide
	modelView = view * model4;
	modelViewProj = proj * view * model4;
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
	//Activa el VAO2 con la configuración de la piramide y pinta la lista de triángulos
	glBindVertexArray(vao2);
	glDrawElements(GL_TRIANGLES, pyraNTriangleIndex * 3,
		GL_UNSIGNED_INT, (void*)0);

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
	if (uSpecularTex != -1)
    {
    glActiveTexture(GL_TEXTURE0 + 2);
    glBindTexture(GL_TEXTURE_2D, specularTexId);
    glUniform1i(uSpecularTex, 2);
    }
    if (uNormalTex != -1)
    {
    glActiveTexture(GL_TEXTURE0 + 3);
    glBindTexture(GL_TEXTURE_2D, normalTexId);
    glUniform1i(uNormalTex, 3);
    }

	if (uPosicion != -1)
	{
		glUniform3f(uPosicion, posicion.x, posicion.y, posicion.z);
	}	
	if (uIntensidad != -1)
	{
		glUniform3f(uIntensidad, intensidad.x, intensidad.y, intensidad.z);
	}

    // pintado del objeto!!!!
    glUseProgram(NULL);

	glutSwapBuffers();
}


void resizeFunc(int width, int height){
	glViewport(0, 0, width, height);
	float w = width;
	float h = height;
	float aspectRatio = w / h;

	glm::mat4 newproj(0.0f);

	newproj[0].x = (1.0f / tan(3.14159f / 6.0f));
	newproj[1].y = newproj[0].x*aspectRatio;
	newproj[2].z = -(100 + 0.1) / (100 - 0.1);
	newproj[2].w = -1.0f;
	newproj[3].z = -2.0f * 100 * 0.1 / (100 - 0.1);
	proj = newproj;

	glutPostRedisplay();
}

void idleFunc(){
	model = glm::mat4(1.0f);
    static float angle = 0.0f;
    angle = (angle > 3.141592f * 2.0f) ? 0 : angle + 0.0005f;
    model = glm::rotate(model, angle, glm::vec3(1.0f, 1.0f, 0.0f));

	//Segundo cubo que rota sobre su eje y
	static float angle2 = 0.0f;
	angle2 = (angle > 3.141592f * 2.0f) ? 0 : angle + 0.0004f;
	model2 = glm::mat4(1.0f);
	model2 = glm::rotate(model2, angle, glm::vec3(0.0f, 1.0f, 0.0f));
	model2 = glm::scale(model2, glm::vec3(0.5f, 0.5f, 0.5f));
	model2 = glm::translate(model2, glm::vec3(4.0f, 0.0f, 0.0f));
	model2 = glm::rotate(model2, angle2,glm::vec3(0.0f, 1.0f,0.0f));//Sobre si mismo en ele eje Y

	//Tercer cubo con trayectoria de curva de Bezier
	static float t = 0.0f;
	t = (t < 1.0f)? t + 0.0002f : 0.0f;
	float y = ((1.0f - t) * (1.0f - t) * (1.0f - t) * (1.0f - t)) * 4.0f + ((1.0f - t) * (1.0f - t)) * 8 * (t*t) * (-12.0f) + (t*t*t*t) * 4.0f;
	float z =  ((1.0f - t) * (1.0f - t) * (1.0f - t)) * 6 * t * 15.0f + (1.0f - t) * 6 * (t*t*t) * (-15.0f);
	model3 = glm::mat4(1.0f);
	model3 = glm::scale(model3, glm::vec3(0.3f, 0.3f, 0.3f));
	model3 = glm::translate (model3, glm::vec3(0.0f, y, z));

	//Piramide
	model4 = glm::mat4(1.0f);	
    model4 = glm::translate(model4, glm::vec3(2.7f, 0.0f, 0.0f));
	model4 = glm::rotate (model4,angle, glm::vec3(0.0f, 1.0f, 0.0f));
    model4 = glm::scale(model4, glm::vec3(0.6f, 0.6f, 0.6f));
	 
    glutPostRedisplay();
}

void keyboardFunc(unsigned char key, int x, int y){
   std::cout << "Se ha pulsado la tecla " << key << std::endl << std::endl;
   //Hacemos un switch para hacer una acción determinada dependiendo del teclado pulsado
   glm::mat4 rotacion(1.0f);
    
    switch (key){
        case 'j':
			rotacion = glm::rotate(rotacion, -0.1f, glm::vec3(0.0f, 1.0f, 0.0f));
            view = rotacion * view; //Movimiento de rotación de la cámara hacia izquierda
            break;
        case 'k':
            rotacion = glm::rotate(rotacion, 0.1f, glm::vec3(1.0f, 0.0f, 0.0f));
            view = rotacion * view; //Movimiento de rotación de la cámara hacia abajo
            break;
        case 'l':
			rotacion = glm::rotate(rotacion, 0.1f, glm::vec3(0.0f, 1.0f, 0.0f));
            view = rotacion * view; //Movimiento de rotación de la cámara hacia derecha
            break;
        case 'i':
            rotacion = glm::rotate(rotacion, -0.1f, glm::vec3(1.0f, 0.0f, 0.0f));
            view = rotacion * view;  //Movimiento de rotación de la cámara hacia arriba
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

        //Posicion de la luz
	    case 'h':
	        posi.y += 1.0f;
            break;
	    case 'b':
		    posi.x += 1.0f;
            break;
	    case 'n':
			posi.y -= 1.0f;
            break;
	    case 'm':
		    posi.x -= 1.0f;
            break;
	    case 'y':
			posi.z += 1.0f;
            break;
	    case 'u':
		    posi.z -= 1.0f;
            break;

		//Intensidad de la luz
		case 'z':
        if (inten.r < 1){
		   inten.r += 0.1;
	    }
		break;
		case 'x':
        if (inten.r > 0){
		   inten.r -= 0.1;
	    }
		break;
		case 'c':
	    if (inten.g < 1){
		   inten.g += 0.1;
	    }
		break;
		case 'v':
	    if (inten.g > 0){
		   inten.g -= 0.1;
	    }
		break;
		case 'f':
	    if (inten.b < 1){
		   inten.b += 0.1;
	    }
		break;
		case 'g':
        if (inten.b > 0){
		   inten.b -= 0.1;
	    }
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









