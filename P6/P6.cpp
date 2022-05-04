

/*
Semestre 2022-2
Práctica 8: Iluminación 2
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;
////variables para animación
float movCoche;
float movOffset;
float rotllanta;
float rotllantaOffset;
float anguloa;
float anguloB;
bool avanza;
float rotCoche;
float movCocheI;

float Helisinoidal;
float inclinacion; 
float movHeli;
float movHeliI;
float rotHeli;
int direccionHeli;
int direccion;
int direccionEsp;
int radio;
float paraX = 0.0f;
float paraY = 0.0f;
float timeAux = 0.0f;
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture DadoTexture;
Texture DadoOchoTexture;

Model Kitt_M;
Model Llanta_M;
Model Camino_M;
Model Blackhawk_M;
Model Road_M;
Model HeliceCe_M;
Model HeliceTr_M;
Model Ovni_M; 

Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];
SpotLight spotLights2[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";



void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}



void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};
	unsigned int vegetacionIndices[] = {
	0, 1, 2,
	0, 2, 3,
	4,5,6,
	4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,
	};
	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);
	// fin create objects


}


void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}


void CrearPiramideCuadrangular()
{
	unsigned int dadoOcho_indices[] = {
		//cara 1
			0,1,2,
		//cara 2//345
			3,4,5,
		//cara 3
			6,7,8,
		//cara 4
			9,10,11,
		//cara 5
			12,13,14,
		//cara 6
			15,16,17,
		//cara 7
			18,19,20,
		//cara 8
			21,22,23
	};
	GLfloat dadoOcho_vertices[] = {
		//cara 1, 5
		//x		y		z		S		T			NX		NY		NZ
		 0.5f, 0.5f, 0.0f,		0.57f,  0.99,		0.0f,	-1.0f,	-1.0f,	//0
		-0.5f, 0.5f, 0.0f,		0.44f,  0.675f,		0.0f,	-1.0f,	-1.0f,	//1
		 0.0f, 0.0f, 0.625f,	0.716f,  0.67f,		0.0f,	-1.0f,	-1.0f,	//2
		//cara 2
		-0.5f,  0.5f, 0.0f,		0.439f,  0.66f,		1.0f,	0.0f,	-1.0f,	//3
		-0.5f, -0.5f, 0.0f,		0.573f,  0.354f,	1.0f,	0.0f,	-1.0f, 	//4
		 0.0f,  0.0f, 0.625f,	0.712f,  0.668f,	1.0f,	0.0f,	-1.0f,  //5
		/// 3
		-0.5f,  -0.5f, 0.0f,	0.58f,  0.347f,		0.0f,	1.0f,	-1.0f,  //6
		 0.5f, -0.5f, 0.0f,		0.853f,  0.347f,	0.0f,	1.0f,	-1.0f,	//7
		 0.0f,  0.0f, 0.625f,	0.715f,  0.66f,		0.0f,	1.0f,	-1.0f,  //8
		//4
		0.5f, -0.5f, 0.0f,		0.859f,  0.355f,	-1.0f,	0.0f,	-1.0f,	//9
		0.5f,  0.5f, 0.0f,		0.994f,  0.667f,	-1.0f,	0.0f,	-1.0f,	//10
		0.0f,  0.0f, 0.625f,	0.722f,  0.667f,	-1.0f,	0.0f,	-1.0f,	//11 
		//cara 5 
		//x		y		z		S		T			NX		NY		NZ 
		 0.5f, 0.5f, 0.0f,		0.154f,  0.66f,		0.0f,	-1.0f,	1.0f,	//12
		-0.5f, 0.5f, 0.0f,		0.425f,  0.66f,		0.0f,	-1.0f,	1.0f,	//13
		 0.0f, 0.0f, -0.625f,	0.288f,  0.354f,	0.0f,	-1.0f,	1.0f,	//14
		 //cara 6
		-0.5f,  0.5f, 0.0f,		0.431f,  0.659f,		1.0f,	0.0f,	1.0f,	//15
		-0.5f, -0.5f, 0.0f,		0.565f,  0.348f,		1.0f,	0.0f,	1.0f,	//16
		 0.0f,  0.0f, -0.625f,	0.295f,  0.347f,		1.0f,	0.0f,	1.0f, //17
		// 7
		-0.5f,  -0.5f, 0.0f,	0.565f,  0.338f,		0.0f,	1.0f,	1.0f,	//18
		 0.5f, -0.5f, 0.0f,		0.431f,  0.025f,		0.0f,	1.0f,	1.0f,	//19
		 0.0f,  0.0f, -0.625f,	0.294f,  0.338f,		0.0f,	1.0f,	1.0f,  //20
		//8
		0.5f, -0.5f, 0.0f,		0.011f,  0.347f,		-1.0f,	0.0f,	1.0f,	//21
		0.5f,  0.5f, 0.0f,		0.145f,  0.661f,		-1.0f,	0.0f,	1.0f,	//22
		0.0f,  0.0f, -0.625f,	0.282f,  0.347f,		-1.0f,	0.0f,	1.0f,	//23
	};
	Mesh* dadoOcho = new Mesh();
	dadoOcho->CreateMesh(dadoOcho_vertices, dadoOcho_indices, 192, 24);
	meshList.push_back(dadoOcho);
}
int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CrearPiramideCuadrangular();
	CreateShaders();


	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 5.0f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	DadoTexture = Texture("Textures/dado_V2.png");
	DadoTexture.LoadTextureA();
	DadoOchoTexture = Texture("Textures/dado8carasV1.png");
	DadoOchoTexture.LoadTextureA();


	Kitt_M = Model();
	Kitt_M.LoadModel("Models/bourak_M.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/bourak_Rueda_M.obj");
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60_sE.obj");
	HeliceCe_M = Model();
	HeliceCe_M.LoadModel("Models/helicentral.obj");
	HeliceTr_M = Model();
	HeliceTr_M.LoadModel("Models/helitrase.obj");
	Ovni_M = Model();
	Ovni_M.LoadModel("Models/ovni_M.obj");
	Camino_M = Model();
	Camino_M.LoadModel("Models/railroad track.obj");
	Road_M = Model();
	Road_M.LoadModel("Models/road.obj");


	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.7f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	/*pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.3f, 1.0f,
		2.0f, 1.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;*/

	unsigned int spotLightCount = 0;

	
	////Luz Fija Cian helicptero
	spotLights[0] = SpotLight(0.0f, 1.0f, 1.0f,
		6.0f, 3.0f,
		0.0f, 0.0f, 10.0f,
		0.0f, -1.0f, 0.0f,
		0.3f, 0.2f, 0.1f,
		20.0f);
	spotLightCount++;//1
	//linterna
	spotLights[1] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		30.0f);
	spotLightCount++;//2
	//luz fija Cian delantera de vehiculo 
	spotLights[2] = SpotLight(0.0f, 1.0f, 1.0f,//colores
		6.0f, 3.0f,//
		8.0f, 0.0f, 0.0f,//poss
		-1.0f, -0.4f, 0.0f,//dire
		1.0f, 0.0f, 0.01f,
		15.0f);
	spotLightCount++;//3
	//luz fija roja trasera del vehiculo
	spotLights[3] = SpotLight(0.7f, 0.0f, 0.0f,
		6.0f, 3.0f,
		-2.0f, 3.0f, -1.0f,
		1.0f, -0.5f, 0.0f,
		0.0f, 1.0f, 0.01f,
		15.0f);
	spotLightCount++;//4



	int contador = 0;
	int contador2 = 0;
	int reset = spotLightCount;
	int aux= 0;
	int aux2 = 0;
	int bandera=0;
	int aux3 = 0;
	int aux4 = 0;
	int aux5 = 0;
	rotCoche = 0.0f;
	movCoche = 0.0f;
	movCocheI = 0.0f;
	movOffset = 0.05f;//0.005
	rotllanta = 0.0f;
	rotllantaOffset = 10.0f;
	direccion = 0;
	movHeli=0.0f;
	radio = 10.0f;
	direccionHeli=0;
	anguloB = 0.0; 


	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		
		//rotllanta += 0.05*rotllantaOffset * deltaTime;
		//////////
		// pista recta esta entre 32 y 33 en Z
		// giro al lado izquierdo es de 8 
		// 
		///////Giro del Coche a la izquerda 
		if(direccion ==0){
			if (movCoche > -43.0f) {
				movCoche -= 4* movOffset * deltaTime;
				rotllanta +=  rotllantaOffset * deltaTime;
				bandera = 1;
			}
			else {
				direccion = 1;
			};
			
		} else if (direccion == 1) {
			if (movCocheI < 8.5f && movCoche > -51.0f) {
				if (rotCoche < 1.55f) {
					rotCoche += 0.45 * toRadians;
				}
				movCoche -= 0.8*movOffset * deltaTime;
				rotllanta +=  rotllantaOffset * deltaTime;
				movCocheI += 0.03 * deltaTime;
				bandera = 2;
			}
			else {
				direccion = 2;
			};
		
		}
		else if(direccion ==2){
			if(movCocheI < 43.0f){
				movCocheI += 4 * movOffset * deltaTime;
				rotllanta +=  rotllantaOffset * deltaTime;
			}else {
				direccion = 3;
				
			}
		}else if (direccion == 3) {
			
			if (movCoche < -43.0f && movCocheI < 52.5f) {
				rotllanta += rotllantaOffset * deltaTime;
				if (rotCoche < 3.1f) {
					rotCoche += 0.45 * toRadians;
				}
				movCoche += 0.03 * deltaTime;
				movCocheI += 0.8 * movOffset * deltaTime;
				
				bandera = 2;
			}
			else {
				direccion = 4;
			};
		}
		else if (direccion == 4) {
			if (movCoche < -4.0f) {
				movCoche += 4 * movOffset * deltaTime;
				rotllanta +=  rotllantaOffset * deltaTime;
			}
			else {
				direccion = 5;

			}
		}
		else if (direccion == 5) {
			if (movCoche < 6.0f && movCocheI > 43.5f) {
				rotllanta += rotllantaOffset * deltaTime;
				if (rotCoche < 4.65f) {
					rotCoche += 0.45 * toRadians;
				}
				movCoche += 0.8 * movOffset * deltaTime;
				movCocheI -= 0.03 * deltaTime; 
			
				bandera = 2;
			}
			else {
				direccion = 6;
			};

		}
		else if (direccion == 6) {
			if (movCocheI > 9.0f) {
				movCocheI -= 4 * movOffset * deltaTime;
				rotllanta +=  rotllantaOffset * deltaTime;
			}
			else {
				direccion = 7;

			}
		}
		else if (direccion == 7) {
			if (movCoche > 0.0f && movCocheI > 0.0f) {
				rotllanta += rotllantaOffset * deltaTime;
				if (rotCoche < 6.2f) {
					rotCoche += 0.45 * toRadians;
				}
				movCoche -= 0.03 * deltaTime; 
				movCocheI -= 0.8 * movOffset * deltaTime;
				bandera = 2;
			}
			else {
				direccion = 0;
				rotCoche = 0.0f;
				rotllanta = 0.0f;
			};
		}



		//////////////////////////////////////////////////////////////////////////////
		///////Ruta del helicoptero
		
		if (direccionHeli == 0) {
			if (movHeli < 30.0f) {
				movHeli += movOffset * deltaTime;
				anguloa += 0.05 * deltaTime;
				Helisinoidal = 0.1 * movHeli * sin(anguloa);
			}
			else {
				direccionHeli = 1;
			};

		}
		else if (direccionHeli == 1) {
			if (movHeliI > -8.5f && movHeli < 38.0f) {
				if (rotHeli < 1.55f) {
					rotHeli += 0.5 * toRadians;
				}
				if (inclinacion > -0.55f) {
					inclinacion -= 0.5 * toRadians;
				}
				movHeli += movOffset * deltaTime;
				//rotllanta += 0.05 * rotllantaOffset * deltaTime;
				movHeliI -= 0.05 * deltaTime;
			}
			else {
				direccionHeli = 2;
			}
		}
		else if (direccionHeli == 2) {
			if (movHeliI > -17.0f && movHeli > 30.0f) {
				if (rotHeli < 3.1f) {
					rotHeli += 0.5 * toRadians;
				}
				if (inclinacion < 0.0f) {
					inclinacion += 0.5 * toRadians;
				}
				movHeli -= movOffset * deltaTime; 
				//rotllanta += 0.05 * rotllantaOffset * deltaTime;
				movHeliI -= 0.05 * deltaTime;
			}
			else {
				direccionHeli = 3;
			}
		}
		else if (direccionHeli == 3) {
	
			if (movHeli > 0.0f) {
				movHeli -= movOffset * deltaTime;
				Helisinoidal = 0.1 * movHeli * sin(anguloa);
				anguloa += 0.05 * deltaTime;
			}
			else {
				direccionHeli = 4;
				//rotHeli = 0.0f;
			};
		}
		else if (direccionHeli ==4) {
			if (movHeliI < -6.5f && movHeli > -8.0f) {
				if (rotHeli < 4.6f) {
					rotHeli += 0.5 * toRadians;
				}
				if (inclinacion > -0.55f) {
					inclinacion -= 0.5 * toRadians;
				}
				movHeli -=  movOffset * deltaTime;
				//rotllanta += 0.05 * rotllantaOffset * deltaTime;
				movHeliI += 0.05 * deltaTime; 
			}
			else {
				direccionHeli = 5;
			}
		}else if (direccionHeli == 5) {
			if (movHeliI < 0.0f && movHeli < 0.0f) {
				if (rotHeli < 6.2f) {
					rotHeli += 0.5 * toRadians;
				}
				if (inclinacion < 0.0f) {
					inclinacion += 0.5 * toRadians;
				}
				movHeli += movOffset * deltaTime;
				//rotllanta += 0.05 * rotllantaOffset * deltaTime;
				movHeliI += 0.05 * deltaTime;
			}
			else {
				rotHeli = 0.0f;
				movHeliI = 0.0f;
				direccionHeli = 0;
			}
		}


		if (anguloa > 359.0f) {
			anguloa = 0.0f;
		};
		if (anguloB > 359.0f) {
			anguloa = 0.0f;
		};
		/////////////////////////////////////////ruta esiral
		anguloB += 0.05 * deltaTime;
		radio = 1 * anguloB + 0.0f;
		if (direccionEsp == 0) {
			if (paraX < radio && paraY < radio) {
				paraX += 0.0005 * radio*( 3.14159265f) * cos(anguloB);
				paraY += 0.0005 *  radio * (3.14159265f) * sin(anguloB);
			}
			else {
				direccionEsp = 9;
			}
		}
		anguloB += 0.05 * deltaTime;
		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);
		
		// luz ligada a la cámara de tipo flash
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[1].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		contador = mainWindow.apagar();
		contador2 = mainWindow.apagarLuces();
		//bandera = mainWindow.vehiculo();
		///////Luces del vehiculo
		if (spotLightCount <= 4) {
			////cuando el vehiculo esta detenido
			if (bandera == 0) {
				if (spotLightCount == 2 || spotLightCount==1 || spotLightCount == 0){
					aux3 = 0;
				} else {
					if (spotLightCount > 3 ) {
						aux3 += 1;
						if (aux3 == 1) {
							spotLightCount = spotLightCount - 2;
						}
					}
					else if (spotLightCount == 3) {
						spotLightCount = spotLightCount - 1;
					}
				}
			// Cuando el vehiculo avanza 
			} else if(bandera == 1){
				if (spotLightCount == 2 ) {
					aux4 += 1;
					if (aux4 == 1) {
						spotLightCount = spotLightCount + 1;
					}
					else if(aux4 !=0){
						aux4 = 0;
					}
				}
			///// Cuando el vehiculo retrocede. 
			} else if (bandera == 2) {
				if (spotLightCount == 2) {
					aux5 += 1;
					if (aux5 == 1) {
						spotLightCount = spotLightCount + 2;
					}
					else if (aux5 != 0) {
						aux5 = 0;
					}
				}
			}
		};
	//Apagar o encender la linterna
		if (spotLightCount <= 2 ) {
			if (contador == 1) {
				aux += 1;
				if (aux == 1) {
					if (spotLightCount == 2) {
						spotLightCount = spotLightCount - 1;
					}
					else
					{
						if (spotLightCount > 2) {
							spotLightCount = 1;
						};
					}; 

				}
			}
			else if (contador == 2) {
				aux += 1;
				if (aux != 1) {
					if (spotLightCount == 1) {
						spotLightCount = spotLightCount + 1;
					}
					else {
						spotLightCount == 1;
					}
					mainWindow.resetLin();
					aux = 0;
				};
			};
		};

		//Apagar o encender la luz del helicoptero
		if (spotLightCount <= 1) {
			if (contador2 == 1) {
				aux2 += 1;
				if (aux2 == 1) {
					if (spotLightCount == 1) {
						spotLightCount = spotLightCount - 1 ;
					}
					else
					{
						if (spotLightCount > 2) {
							spotLightCount = 1;
						};
					};
				}
			}
			else if (contador2 == 2) {
				aux2 += 1;
				if (aux2 != 1) {
					if (spotLightCount == 0) {
						spotLightCount = spotLightCount + 1;
					}
					else {
						spotLightCount == 0;
					}
					mainWindow.resetLin();
					aux2 = 0;
				};
			}
		};
		//printf("SpotLight: %d\n", spotLightCount);
		shaderList[0].SetSpotLights(spotLights, 0/*spotLightCount*/);

			

		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::mat4 maux(1.0);
		glm::mat4 heliaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();
		//////////////////////////Inician los modelos////////////////////////////////
		///////////////////////////////////////Creacion de vehiculo

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(14.0f, -1.4f, 0.0));
		
		model = glm::translate(model, glm::vec3(movCoche, 0.0f, movCocheI));
		//model = glm::translate(model, glm::vec3(mainWindow.getmuevex(),mainWindow.getmuevey(), mainWindow.getmuevez()));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotCoche, glm::vec3(0.0f, 1.0f, 0.0f));
		maux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		/*float posXLight = movCoche;
		posXLight -= -7.3f;
		spotLights[2].SetPos(glm::vec3(posXLight, -1.4f, 0.35));
		float posYLight = movCoche;
		posYLight -= -10.1f;
		spotLights[3].SetPos(glm::vec3(posYLight, -1.35f, -0.46));*/
		Kitt_M.RenderModel();
		
		//printf("Valor en X: %f, Valor en Y:%f, Valor en Z: %f\n", mainWindow.getmuevex(), mainWindow.getmuevey(),mainWindow.getmuevez());
		//printf("movCoche: %f, movCocheI: %f, valor de rotacion: %f\n",movCoche, movCocheI, rotCoche);
		//printf("Rotacion de llanta:%f\n",rotllanta);

		/////////////////////////////////////////Llanta enfrente derecha 
		model =maux;
		model = glm::translate(model, glm::vec3(-0.5f, -0.3f, 0.65));
		model = glm::rotate(model, rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		/////////////////////////////////////////Llanta enfrente izquierda 

		model = maux;
		model = glm::translate(model, glm::vec3(0.5f, -0.3f, 0.65f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.1f, 0.0f));
		model = glm::rotate(model, -1*rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		/////////////////////////////////////////Llanta atras derecha 

		model = maux;
		model = glm::translate(model, glm::vec3(-0.55f, -0.28f, -1.27));
		model = glm::rotate(model, rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		/////////////////////////////////////////Llanta atras izquierda 

		model = maux;
		model = glm::translate(model, glm::vec3(0.55f, -0.28f, -1.27));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.1f, 0.0f));
		model = glm::rotate(model, -1 * rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		/////////////////		Helicoptero 

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.0f, 3.0f, -7.0));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(movHeliI, movHeli, Helisinoidal));
		model = glm::rotate(model, rotHeli, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, inclinacion, glm::vec3(0.0f, 1.0f, 0.0f));
		heliaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		float posYHeli = mainWindow.getmuevey();
		posYHeli -= -2.8f;
		spotLights[0].SetPos(glm::vec3(mainWindow.getmuevex(), posYHeli, -7.0));
		Blackhawk_M.RenderModel();
		//printf("movHeli: %f, moHeliI: %f, valor de rotacion: %f, Valor de inclinacion: %f\n",movHeli, movHeliI, rotHeli,inclinacion);

		/////////////////////////////////////////helice central

		model = heliaux;
		model = glm::translate(model, glm::vec3(0.05f, -0.05f, 2.1f));
		model = glm::rotate(model, rotllanta * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		HeliceCe_M.RenderModel();


		/////////////////////////////////////////helice central

		model = heliaux;
		model = glm::translate(model, glm::vec3(0.3f, -10.4f, 2.3f));
		model = glm::rotate(model, rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		HeliceTr_M.RenderModel();




		/////////////////		Pista

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-3.0f, -3.0f, 23.0f));
		model = glm::scale(model, glm::vec3(1.1f, 1.1f, 1.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Road_M.RenderModel();



		/////////////////		Ovni

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 3.0f, 30.0f));
		model = glm::translate(model, glm::vec3(paraY, 0.0f, paraX));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Ovni_M.RenderModel();
		//printf("valor de x: %f, valor de y: %f,\n", paraX, paraY);

		//////////////////////////Termina los modelos////////////////////////////////
		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}

