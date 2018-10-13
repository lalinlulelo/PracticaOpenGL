#version 330 core
//Color de salida
out vec4 outColor;


//Variables Variantes
in vec2 texCoord;

//Textura
uniform sampler2D colorTex;
uniform sampler2D vertexTex;//textura con la posición del fragmento en coordenadas de la cámara
uniform sampler2D depthTex;

uniform float focalDistance;
uniform float maxDistanceFactor;
uniform float Nnear;
uniform float Nfar;

uniform float mask;

//Añade la distancia de enfoque y el desenfoque máximo, focald negativo porque miramos hacia eje z negatvio
//float focalDistance = -25.0;
//float maxDistanceFactor = 1.0/5.0;

	//outColor = vec4(textureLod(colorTex, texCoord,0).xyz, 0.6);	//0.6 para cambiar peso que quiero dar a cada imagen y cambia motion blur
	//outColor = vec4(texCoord,vec2(1.0));

	/*#define MASK_SIZE 9u
    const float maskFactor = float (1.0/14.0);
    const vec2 texIdx[MASK_SIZE] = vec2[](
          vec2(-1.0,1.0), vec2(0.0,1.0), vec2(1.0,1.0),
          vec2(-1.0,0.0), vec2(0.0,0.0), vec2(1.0,1.0),
          vec2(-1.0,-1.0), vec2(0.0,-1.0), vec2(1.0,-1.0));
    const float mask[MASK_SIZE] = float[](
          float (1.0*maskFactor), float (2.0*maskFactor), float (1.0*maskFactor),
          float (2.0*maskFactor), float (2.0*maskFactor), float (2.0*maskFactor),
          float (1.0*maskFactor), float (2.0*maskFactor), float (1.0*maskFactor));*/

    //Añade una máscara de convolución 5x5
    const vec2 texIdx[25] = vec2[](
          vec2(-2.0,2.0f), vec2(-1.0,2.0f), vec2(0.0,2.0f), vec2(1.0,2.0f), vec2(2.0,2.0),
          vec2(-2.0,1.0f), vec2(-1.0,1.0f), vec2(0.0,1.0f), vec2(1.0,1.0f), vec2(2.0,1.0),
          vec2(-2.0,0.0f), vec2(-1.0,0.0f), vec2(0.0,0.0f), vec2(1.0,0.0f), vec2(2.0,0.0),
          vec2(-2.0,-1.0f), vec2(-1.0,-1.0f), vec2(0.0,-1.0f), vec2(1.0,-1.0f), vec2(2.0,-1.0),
          vec2(-2.0,-2.0f), vec2(-1.0,-2.0f), vec2(0.0,-2.0f), vec2(1.0,-2.0f), vec2(2.0,-2.0));
    const float maskFactor = float (1.0/65.0);

	//i
    const float mask0[25] = float[](
    1.0*maskFactor, 2.0*maskFactor, 3.0*maskFactor,2.0*maskFactor, 1.0*maskFactor,
    2.0*maskFactor, 3.0*maskFactor, 4.0*maskFactor,3.0*maskFactor, 2.0*maskFactor,
    3.0*maskFactor, 4.0*maskFactor, 5.0*maskFactor,4.0*maskFactor, 3.0*maskFactor,
    2.0*maskFactor, 3.0*maskFactor, 4.0*maskFactor,3.0*maskFactor, 2.0*maskFactor,
    1.0*maskFactor, 2.0*maskFactor, 3.0*maskFactor,2.0*maskFactor, 1.0*maskFactor);
	
     //o
     const float maskSharpen[25] = float[](
     0.0, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, -1.0, 0.0, 0.0,
     0.0, -1.0, 5.0, -1.0, 0.0,
     0.0, 0.0, -1.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.0, 0.0 );

     //p
     const float maskBlur[25] = float[](
     0.0, 0.0, 0.0, 0.0, 0.0,
     0.0, 1.0, 1.0, 1.0, 0.0,
     0.0, 1.0, 1.0, 1.0, 0.0,
     0.0, 1.0, 1.0, 1.0, 0.0,
     0.0, 0.0, 0.0, 0.0, 0.0 );

     //j
     const float maskEnhance[25] = float[](
     0.0, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.0, 0.0,
     0.0, -1.0, 1.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.0, 0.0 );

     //k
     const float maskDetect[25] = float[](
     0.0, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 1.0, 0.0, 0.0,
     0.0, 1.0, -4.0, 1.0, 0.0,
     0.0, 0.0, 1.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.0, 0.0 );

     //l
     const float maskEmboss[25] = float[](
     0.0, 0.0, 0.0, 0.0, 0.0,
     0.0, -2.0, 1.0, 0.0, 0.0,
     0.0, -1.0, 1.0, 1.0, 0.0,
     0.0, 0.0, 1.0, 2.0, 0.0,
     0.0, 0.0, 0.0, 0.0, 0.0 );

    void main()
    {
    //Sería más rápido utilizar una variable uniform el tamaño de la textura.
          vec2 ts = vec2(1.0) / vec2 (textureSize (colorTex,0));
          vec4 color = vec4 (0.0);
		  /*float dof = abs(texture(vertexTex,texCoord).z - focalDistance)
                         * maxDistanceFactor; //factor para rango*/
          
		  float dof = abs((-Nnear*Nfar/(texture(depthTex,texCoord).r*(Nnear-Nfar))) - focalDistance)* maxDistanceFactor;
		         
		  dof = clamp (dof, 0.0, 1.0);
		  dof *= dof;

		  
          for (int i = 0; i < 25; i++)
          {
		    if (mask == 1.0){
              vec2 iidx = texCoord + ts * texIdx[i]*dof;
              color += texture(colorTex, iidx,0.0) * mask0[i];
            }
		    if (mask == 2.0){
              vec2 iidx = texCoord + ts * texIdx[i]*dof;
              color += texture(colorTex, iidx,0.0) * maskSharpen[i];
            }
			if (mask == 3.0){
              vec2 iidx = texCoord + ts * texIdx[i]*dof;
              color += texture(colorTex, iidx,0.0) * maskBlur[i];
            }
			if (mask == 4.0){
              vec2 iidx = texCoord + ts * texIdx[i]*dof;
              color += texture(colorTex, iidx,0.0) * maskEnhance[i];
            }
			if (mask == 5.0){
              vec2 iidx = texCoord + ts * texIdx[i]*dof;
              color += texture(colorTex, iidx,0.0) * maskDetect[i];
            }
		    if (mask == 6.0){
              vec2 iidx = texCoord + ts * texIdx[i]*dof;
              color += texture(colorTex, iidx,0.0) * maskEmboss[i];
            }
          }

		  outColor = color;
    }