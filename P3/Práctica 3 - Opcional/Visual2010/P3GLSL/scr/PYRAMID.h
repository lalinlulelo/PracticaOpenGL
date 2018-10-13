#ifndef __PYRAMIDFILE__
#define __PYRAMIDFILE__

//Descripci�n de una pir�mide regular

//N�mero de v�rtices

const int pyraNVertex = 16; // 3 v�rtices x 4 caras + 4 v�rtices x 1 cara
const int pyraNTriangleIndex = 6; // 1 tri�ngulo x 4 caras + 2 tri�ngulos x 1 cara


const unsigned int pyraTriangleIndex [] = {
	//Cara frontal 
	0,1,2, 		

	//Cara trasera 
	3,5,4,

	//Cara derecha
	6,7,8,

	//Cara izquierda
	9,10,11,

	//Base
	12,14,13, 13,14,15
};

//Posici�n de los v�rtices
const float pyraVertexPos[] = {
	//Cara frontal
	-1.0f,	 1.0f,	  1.0f, //0
	 1.0f,	 1.0f,	  1.0f, //1
	 0.0f,	 3.0f,	  0.0f, //2

	//Cara trasera
   -1.0f,	 1.0f,	 -1.0f, //3
	1.0f,	 1.0f,	 -1.0f, //4
	0.0f,	 3.0f,	  0.0f, //5

	//Cara derecha
	1.0f,	 1.0f,	  1.0f,	//6
	1.0f,	 1.0f,	 -1.0f, //7
	0.0f,	 3.0f,	  0.0f, //8

	//Cara izquierda
	-1.0f,	 1.0f,	 -1.0f, //9
	-1.0f,	 1.0f,	  1.0f, //10
	 0.0f,	 3.0f,	  0.0f, //11

	//Base
	-1.0f,	 1.0f,	 -1.0f, //12
	-1.0f,	 1.0f,	  1.0f, //13
	 1.0f,	 1.0f,	 -1.0f, //14
	 1.0f,	 1.0f,	  1.0f, //15
};

//Normal de los v�rtices
const float pyraVertexNormal[] = {
	//Cara frontal
	 0.0f, 0.0f, 1.0f,
	 0.0f, 0.0f, 1.0f,
	 0.0f, 0.0f, 1.0f,

	//Cara trasera
	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, -1.0f,

	//Cara derecha
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,

	//Cara izquierda
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,

	//Base
	0.0f,	3.0f,	0.0f, 
	0.0f,	3.0f,	0.0f, 
	0.0f,	3.0f,	0.0f, 
	0.0f,	3.0f,	0.0f  

};

//Color de los v�rtices
const float pyraVertexColor[] = {
	//Cara frontal
	 0.0f, 0.0f, 1.0f,
	 0.0f, 0.0f, 1.0f,
	 0.0f, 0.0f, 0.0f,

	//Cara trasera
	0.5f, 0.5f,  1.0f,
	0.5f, 0.5f,  1.0f,
	0.5f, 0.5f,  1.0f,

	//Cara derecha
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,

	//Cara izquierda
	1.0f, 0.5f, 0.5f,
	1.0f, 0.5f, 0.0f,
	1.0f, 0.5f, 0.5f,

	//Base
	0.5f,	1.0f,	0.5f, 
	0.5f,	1.0f,	0.5f, 
	0.5f,	1.0f,	0.5f, 
	0.5f,	1.0f,	0.5f 
};

//Coordenadas de textura de los v�rtices
const float pyraVertexTexCoord[] = {
	//Cara frontal
	 0.0f, 0.0f,
	 1.0f, 0.0f,
	 0.0f, 1.0f,

	//Cara trasera
	0.0f, 1.0f, 
	1.0f, 1.0f, 
	0.0f, 0.0f, 

	//Cara derecha	
	0.0f,	1.0f,
	1.0f,	1.0f,
	0.0f,	0.0f,

	//Cara izquierda
	0.0f,	0.0f,
	1.0f,	0.0f,
	0.0f,	1.0f,	

	//Base
	0.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
}; 

const float pyraVertexTangent[] = {
	//Cara frontal
	1.0f,	0.0f,	0.0f,
	1.0f,	0.0f,	0.0f,
	0.0f,	0.0f,	0.0f,

	//Cara trasera				
	1.0f,	0.0f,	0.0f,		
	1.0f,	0.0f,	0.0f,		
	0.0f,	0.0f,	0.0f,			

	//Cara derecha				
	0.0f,	0.0f,	-1.0f,		
	0.0f,	0.0f,	-1.0f,		
	0.0f,	0.0f,	0.0f,			

	//Cara izquierda				
	0.0f,	0.0f,	1.0f,		
	0.0f,	0.0f,	1.0f,		
	0.0f,	0.0f,	0.0f,			
					
	//Base					   
	1.0f,	0.0f,	0.0f,
	1.0f,	0.0f,	0.0f,
	1.0f,	0.0f,	0.0f,
	1.0f,	0.0f,	0.0f,
};	

#endif

