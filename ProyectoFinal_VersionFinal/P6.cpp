

/*
Semestre 2022-2
Poryecto Final LCGeHC
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


int luces=0;
int contador;
int contador2;
int contadorBran = 0;
int bandera;
int direccion;
int direccionEsp;
float paraX = 0.0f;
float paraY = 0.0f;
float timeAux = 0.0f;
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;
//////////Variables de Texturas
Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;



//////////////////////////////Variables de Modelos
Model volcan_VM;
Model cuerpo_Pablo;
Model brazoDer_Pablo;
Model brazoIz_Pablo;
Model piernaDer_Pablo;
Model piernaIz_Pablo;

Model carritoPablo_VM;
Model rueda_VM;
Model camino_Voxel_M;

//Pterodactilo
Model ptero_VM;
Model alaDer_VM;
Model alaIzq_VM;

//Cuello Largo
Model Brachiosaurus_M;
Model Bcuello_M;
Model Bquij_M;

//Arboles
Model SakuraT_M;
Model TreeT_M;

//Lampara 
Model Lamp_M;


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

int main()
{

	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();


	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 5.0f, 0.5f);

	

	//////////////////////////////DEclaracion dee Texturas

	//Texturas Volcan

	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/bt.png");
	pisoTexture.LoadTextureA();


	//////////////////////////////Declaracion de Modelos
	////Volvan
	volcan_VM = Model();
	volcan_VM.LoadModel("Models/volvan_Voxel_OP.obj");

	/////Carrito
	carritoPablo_VM = Model();
	carritoPablo_VM.LoadModel("Models/carritoConConductor.obj");
	rueda_VM = Model();
	rueda_VM.LoadModel("Models/rueda Carrito.obj");

	//Camino Principal
	camino_Voxel_M = Model();
	camino_Voxel_M.LoadModel("Models/camino_Op_Voxel.obj");

	/// Pablo
	cuerpo_Pablo = Model();
	cuerpo_Pablo.LoadModel("Models/pablo_Cuerpo.obj");
	brazoDer_Pablo = Model();
	brazoDer_Pablo.LoadModel("Models/brazo_Der.obj");
	brazoIz_Pablo = Model();
	brazoIz_Pablo.LoadModel("Models/brazo_Izq.obj");
	piernaDer_Pablo = Model();
	piernaDer_Pablo.LoadModel("Models/pierna_Der.obj");
	piernaIz_Pablo = Model();
	piernaIz_Pablo.LoadModel("Models/pierna_Izq.obj");

	////Pterodactilo
	ptero_VM = Model();
	ptero_VM.LoadModel("Models/cuerpo_Ptero.obj");
	alaDer_VM = Model();
	alaDer_VM.LoadModel("Models/alaDer_VM.obj");
	alaIzq_VM = Model();
	alaIzq_VM.LoadModel("Models/alaIzq_VM.obj");

	//Brachiosaurus

	Brachiosaurus_M = Model();
	Brachiosaurus_M.LoadModel("Models/cuelloCuerpo_VM.obj");
	Bcuello_M = Model();
	Bcuello_M.LoadModel("Models/dinoCabe_VM.obj");
	Bquij_M = Model();
	Bquij_M.LoadModel("Models/quiCuello_VM.obj");

	//Arboles
	SakuraT_M=Model();
	SakuraT_M.LoadModel("Models/SakuraTree.obj");
	TreeT_M = Model();
	TreeT_M.LoadModel("Models/tree.obj");


	//Lampara 
	Lamp_M = Model();
	Lamp_M.LoadModel("Models/lamp_VM.obj");


	std::vector<std::string> skyboxFaces;
	std::vector<std::string> skyboxFacesNi;
	skyboxFaces.push_back("Textures/Skybox/pxNi.png");//
	skyboxFaces.push_back("Textures/Skybox/nxNi.png");//
	skyboxFaces.push_back("Textures/Skybox/nyNi.png");//
	skyboxFaces.push_back("Textures/Skybox/pyNi.png");//
	skyboxFaces.push_back("Textures/Skybox/pzNi.png");
	skyboxFaces.push_back("Textures/Skybox/nzNi.png");

	/*	skyboxFaces.push_back("Textures/Skybox/pxNi.png");//
			skyboxFaces.push_back("Textures/Skybox/nxNi.png");//
			skyboxFaces.push_back("Textures/Skybox/nyNi.png");//
			skyboxFaces.push_back("Textures/Skybox/pyNi.png");//
			skyboxFaces.push_back("Textures/Skybox/pzNi.png");
			skyboxFaces.push_back("Textures/Skybox/nzNi.png");*/
	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);
	skybox = Skybox(skyboxFaces);
	unsigned int pointLightCount = 0;
	unsigned int spotLightCount = 0;

	////////Luces
	int DiaNoche = 0;
	int banderaDN = 0;
	int SpotL1 = 0;
	int SpotL2 = 1;
	int SpotL3 = 2;
	int SpotL4 = 3;
	int SpotL5 = 4;
	spotLightCount = 0;


	///// Variables para PAblo
	bandera = 0;
	//Brazos y piernas 
	float derecha = 0.0f;
	float izquierda = 0.0f;
	int direccionExtr = 6;
	float brazoDer = 0.0f;
	//Recorrido
	int direccionP = 0;
	float pabloInit = 0.0f;
	float movPabloX = 0.0f;
	float movPabloY = 0.0f;
	float movPabloZ = 0.0f;
	float rotPablo = 0.0f;
	float anguAz = 0.0f;

	///Variables para Pterodactilo
	int direccionPter = 0;
	int direccionAlasP = 0;
	//Alas 
	float alaDerecha = 0.0f;
	float alaIzquierda = 0.0f;
	//Recorrido
	float anguPtero = 0.0f;
	float pteroInit = 0.0f;
	float movPterX = 0.0f;
	float movPterY = 0.0f;
	float movPterZ = 0.0f;
	float rotPteroZ = 0.0f;
	float rotPteroY = 0.0f;
	//Variales para los Brachiosaurus
	float brancInit = 0.0f;
	int posCue = 0;
	int posCuello = 0;
	int contadorBr = 0;
	int mandiB = 0;
	//float contador = 0.0f;
	//Branch 1
	float rotCuello = 0.0f;
	float rotMandi = 0.0f;
	//Branch 2
	float rotBcuello = 0.0f;
	float rotBmandi = 0.0f;
	float cuelloAsc = 0.0f;



	////// Variable para carrito
	float init = 0.00f;
	rotCoche = 0.0f;
	movCoche = 0.0f;
	movCocheI = 0.0f;
	movOffset = 0.05f;//0.005
	rotllanta = 0.0f;
	rotllantaOffset = 10.0f;
	direccion = 0;

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		//Variables Para controlar las luces
		contador = mainWindow.apagar();
		contador2 = mainWindow.apagarLuces();
		bandera = mainWindow.pablo();

		//Cambio de la lus Ambiental
		float tiempoDN = 0.0f;
		//luz direccional
		//Establece el tiempo entre Luz y Noche
		DiaNoche++;
		if (DiaNoche > 1500) {
			banderaDN = 1;
			tiempoDN = 0.3;
		}
		else { banderaDN = 0; }
		if (banderaDN == 1) {
			if (DiaNoche > 3000) {
				tiempoDN = 0.8f;
				DiaNoche = 0;
			}
		}
		if (banderaDN == 0) {
			tiempoDN = 0.9;
			contador2 = 1;
		}
		else if (banderaDN == 1) { tiempoDN = 0.3; }

		mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
			tiempoDN, 0.3f,
			0.0f, 0.0f, -1.0f);

		//////Cambio del Skybox

	

		//Proceso de Encendiido y apagado de Luces
		if (banderaDN == 1) {
			if (contador == 0){
				//Linterna Apagada
				if(contador2 == 1){
				//Lamparas encendidas
					if (luces == 0) {
						//Solo estan las lamparas encendidas
						//Si la lampara de Pablo esta apagada
						SpotL1 = 0;//Luz
						SpotL2 = 1;//Luz
						SpotL3 = 2;
						SpotL4 = 3;
						spotLightCount = 2;
						//printf("Lamparas cendidas\n");

					}
					else if (luces == 1) {
						//Estan las Lamparas y la luz de pablo
						//Si la lampara ded Pablo esta encendiad
						SpotL1 = 0;//Luz
						SpotL2 = 1;//Luz
						SpotL4 = 2;//Luz
						SpotL3 = 3;
						spotLightCount = 3;
						//printf("Lamparas cendidas Y pablo\n");
					}
				}
				else if(contador2 == 0 )
				{
					//Lamparas Apagadas
					if (luces == 0) {
						//Mo hay ninguna luz encendida 
						//Si la lampara de Pablo esta apagada
						spotLightCount = 0;
					//	printf("Ninguna Luz\n");

					}
					else if (luces == 1) {
						//solo Pablo esta encendida
						//Si la lampara ded Pablo esta encendida 
						//printf("Solo Pablo\n");

						SpotL4 = 0;//Luz
						SpotL1 = 1;
						SpotL2 = 2;
						SpotL3 = 3;
						spotLightCount = 1;
					}
				}
			}
			else if (contador == 1) {
				//Linterna Encendida 
				if (contador2 == 1) {
					//Lamparas 
					//Lamparas Encendida
					if (luces == 0) {
						//Linterna y lamparas encendidas
						//Si la lampara de Pablo esta apagada
						SpotL1 = 0;//Luz
						SpotL2 = 1;//Luz
						SpotL3 = 2;//Luz
						SpotL4 = 3;
						spotLightCount = 3;
						//printf("Lamparas y lnterna Encendidas\n");

					}
					else if (luces == 1) {
						//Estan las Lamparas y la luz de pablo
						//Si la lampara ded Pablo esta encendida 
						SpotL1 = 0;//Luz
						SpotL2 = 1;//Luz
						SpotL3 = 2;//Luz
						SpotL4 = 3;//Luz
						//printf("Lamparas, pablo y linterna\n");
						spotLightCount = 4;
					}
				}
				else if (contador2 == 0)
				{
					//Lamparas Apagadas
					if (luces == 0) {
						//Solo la linterna Encendia 
						//Si la lampara de Pablo esta apagada
						//printf("Solo Linterna encendia\n");
						SpotL3 = 0;//luz
						SpotL1 = 1;
						SpotL2 = 2;
						SpotL4 = 3;
						spotLightCount = 1;

					}
					else if (luces == 1) {
						//Linterna Y pablo Encendidas
						//Si la lampara ded Pablo esya apagada 
						//printf("Linterna y pablo\n");
						SpotL3 = 0;//Luz
						SpotL4 = 1;//Luz
						SpotL1 = 2;
						SpotL2 = 3;
						spotLightCount = 2;
					}
				}
			}


		}
		else if (banderaDN == 0) {
			spotLightCount = 0;
		}
	
		//printf("Contador:%i, Co2:%i, Luces:%i\n", contador,contador2,luces);
		if (contador == 2) { mainWindow.resetLin(); contador = 0; }
		if (contador2 == 2) { mainWindow.resetC2(); contador2 = 0; }
		//Lusces del tipo Spotlight
		////Luz lampara 1
		spotLights[SpotL1] = SpotLight(0.7f, 0.0f, 0.0,
			6.0f, 3.0f,
			-3.5f, 4.7f, 34.3f,
			0.0f, -1.0f, 0.0f,
			0.3f, 0.2f, 0.1f,
			60.0f);
		
		//Luz de la lampara 2
		spotLights[SpotL2] = SpotLight(0.7f, 0.0f, 0.0f,
			6.0f, 3.0f,
			26.1f, 4.7f, 45.5f,
			0.0f, -1.0f, 0.0f,
			0.3f, 0.2f, 0.1f,
			60.0f);

		//luz de linterna 
		spotLights[SpotL3] = SpotLight(1.0f, 1.0f, 1.0f, 
			6.0f, 3.0f,//
			0.0f, 0.0f, 0.0f,//poss
			0.0f, 0.0f, 0.0f,//dire
			1.0f, 0.0f, 0.01f,
			15.0f);
		//3

		//luz de Pablo
		spotLights[SpotL4] = SpotLight(0.0f, 1.0f, 1.0f,
			6.0f, 3.0f,
			-20.9f, 4.7f, 46.0f,
			0.0f, -1.0f, 0.0f,
			0.3f, 0.2f, 0.1f,
			60.0f);
	

		///////////////////////Inician Animaciones	
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		init += 0.001f;
		if(bandera ==1){ pabloInit += 0.001f; }
		
		pteroInit += 0.001f;

		//brancInit += 0.001f;
	//	contador += 0.1f;
		///////Animacion de Carrito 
		if (direccion == 0) {
			if (movCocheI > -6.7f) {
				//movCoche += 0.03 * deltaTime;
				movCocheI -= 0.1 * init;
				movCoche -= 0.07 * init;
				rotllanta += 0.5 * rotllantaOffset * deltaTime;
				if (rotCoche < 1.55f) {
					rotCoche += 0.22 * toRadians;
				}
			}
			else {
				if (rotCoche < 1.55f) {
					movCoche -= 0.08 * init;
					rotCoche += 0.22 * toRadians;
				}
				else { direccion = 1; }
			};
		}
		else if (direccion == 1) {
			if (movCoche > -75.2f) {
				movCoche -= 0.5 * init;
				rotllanta += 0.5 * rotllantaOffset * deltaTime;
			}
			else {
				direccion = 2;
			};
		}
		else if (direccion == 2) {
			if (movCocheI < 0.0f) {
				movCoche -= 0.07 * init;
				movCocheI += 0.07 * init;
				rotllanta += 0.5 * rotllantaOffset * deltaTime;
				if (rotCoche < 3.1f) {
					rotCoche += 0.8 * toRadians;
				}
			}
			else {
				if (rotCoche < 3.1f) {
					movCocheI += 0.08 * init;
					rotCoche += 0.22 * toRadians;
				}
				else { direccion = 3; }
			}
		}
		else if (direccion == 3) {
			if (movCocheI > -13.0f) {
				rotllanta -= rotllantaOffset * deltaTime;
				movCocheI -= 0.1 * init;
			}
			else {
				direccion = 4;
			};
		}
		else if (direccion == 4) {
			if (movCocheI < -5.5f) {
				movCoche += 0.07 * init;
				movCocheI += 0.1 * init;
				rotllanta += 0.5 * rotllantaOffset * deltaTime;
				if (rotCoche < 4.65f) {
					rotCoche += 0.8 * toRadians;
				}
			}
			else {
				if (rotCoche < 4.65f) {
					movCoche += 0.07 * init;
					rotCoche += 0.8 * toRadians;
					rotllanta += 0.5 * rotllantaOffset * deltaTime;
				}
				else {
					direccion = 5;
				};

			}
		}
		else if (direccion == 5) {
			if (movCoche < -7.0f) {
				movCoche += 0.5 * init;
				rotllanta += 0.5 * rotllantaOffset * deltaTime;
			}
			else {
				direccion = 6;
			};

		}
		else if (direccion == 6) {
			if (movCocheI > -12.0f) {
				movCocheI -= 0.07 * init;
				if (movCoche < 0) {
					movCoche += 0.1 * init;
				}
				if (rotCoche < 6.2f) {
					rotCoche += 0.8 * toRadians;
				}
				rotllanta += rotllantaOffset * deltaTime;
			}
			else {
				if (rotCoche < 6.2f) {
					movCocheI -= 0.01 * init;
					rotCoche += 0.3 * toRadians;
					rotllanta += 0.5 * rotllantaOffset * deltaTime;
				}
				else {
					direccion = 7;
				};
			}
		}
		else if (direccion == 7) {
			if (movCocheI < 0.0f) {
				rotllanta += rotllantaOffset * deltaTime;
				movCocheI += 0.1 * init;
			}
			else {
				init = 0.0f;
				direccion = 0;
				rotCoche = 0.0f;
				movCoche = 0.0f;
				movCocheI = 0.0f;
				rotllanta = 0.0f;
			};
		}
		if (rotllanta > 359.0f) {
			rotllanta = 0.0f;
		};

		//////////////Animacion de Pablo
		if (direccionP == 0) {
			if (bandera == 1) {
				direccionP = 1;
				//direccionExtr = 0;
			}
			else {
				direccionP = 0;
			}
		}
		else if (direccionP == 1) {
			if (rotPablo > -1.55f) {
				rotPablo -= 0.5 * toRadians;
			}
			else {
				direccionP = 2;
			};
		}
		else if (direccionP == 2) {
			if (anguAz < 180) {
				anguAz += 0.1;
			}
			else { anguAz = 0.0f; }
			if (movPabloX < 25.4f) {
				movPabloX += 0.07 * pabloInit;
				movPabloY += 0.01 * sin(anguAz);
				movPabloZ = -2 * movPabloY;
			}
			else { movPabloZ = 0.0f; movPabloZ = 0.0f; direccionP = 3; pabloInit = 0.0f; }
		}
		else if (direccionP == 3) {
			if (rotPablo > -4.65f) {
				rotPablo -= 0.5 * toRadians;
			}
			if (movPabloX > 23.4) {
				movPabloX -= 0.01 * pabloInit;
				if (movPabloZ < 2.0f) {
					movPabloZ += 0.07 * pabloInit;
				}
				if (movPabloY > -0.3) {
					movPabloY -= 0.008 * pabloInit;
				}
			}
			else {
				direccionP = 4;
			};
		}
		else if (direccionP == 4) {
			if (movPabloX > 0.0) {
				movPabloX -= 0.04 * pabloInit;
			}
			else {
				direccionP = 5;
			}
		}
		else if (direccionP == 5) {
			if (direccionExtr != 6) {
				direccionP = 5;
			}
			else { direccionP = 6; direccionExtr = 0; }
		}
		else if (direccionP == 6) {
			if (rotPablo > -6.2f) {
				rotPablo -= 0.5 * toRadians;
				if (movPabloZ > 0.0f) {
					movPabloZ -= 0.01 * pabloInit;
				}
				if (movPabloY < 0.0f) {
					movPabloY += 0.005 * pabloInit;
				}
			}
			else {
				direccionP = 7;
			};

		}
		else if (direccionP == 7) {
			movPabloX = 0.0f;
			movPabloY = 0.0f;
			movPabloZ = 0.0f;
			pabloInit = 0.0f;
			rotPablo = 0.0f;
			direccionP = 0;
			direccionExtr = 6;
			mainWindow.resetPablo();
			bandera = 0;
		}


		/////////////////Movimeinto de piernas y brzaso 
		if (direccionExtr == 0) {
			if (derecha < 0.775f) {
					derecha += 2 * toRadians;
					brazoDer = derecha;
				}
				else {
					direccionExtr = 1;
				}
		}
		else if (direccionExtr == 1) {
			if (derecha > -0.775) {
				derecha -= 2 * toRadians;
				brazoDer = derecha;
				if (izquierda < 0.775) {
					izquierda += 2 * toRadians;
				}
			}
			else {
				direccionExtr = 2;
			}
		}
		else if (direccionExtr == 2) {
			if (izquierda > -0.775) {
				izquierda -= 2 * toRadians;
				if (derecha < 0.775) {
					derecha += 2 * toRadians;
					brazoDer = derecha;
				}
			}
			else {
				if (direccionP == 5 || direccionP == 6) {
					direccionExtr = 3;
				}
				else { direccionExtr = 1; }
			}
		}
		else if (direccionExtr == 3) {
			if (izquierda < 0.0f) {
				izquierda += 2 * toRadians;
				if (derecha > 0.0f) {
					derecha -= 2 * toRadians;
					brazoDer = derecha;
				}
			}
			else {
				izquierda = 0.0f;
				derecha = 0.0f;
				brazoDer = derecha;
				if (direccionP == 7 || direccionP == 6) {
					direccionExtr = 6;
				}
				else {
					direccionExtr = 4;
				}
			}
		}
		else if (direccionExtr == 4) {
			if (direccionP == 5) {
				if (brazoDer < 1.55f) {
					brazoDer += 2 * toRadians;
				}
				else {
					luces += 1;
					if (luces > 1) { luces = 0; }
					direccionExtr = 5;
				}
			}
			else { direccionExtr = 0; }

		}
		else if (direccionExtr == 5) {
			if (direccionP == 5) {
				if (brazoDer > 0.0f) {
					brazoDer -= 2 * toRadians;
				}
				else {
					direccionExtr = 6;
				}
			}
			else { direccionExtr = 6; }
		}
		else if (direccionExtr == 6) {
			if (direccionP == 6 || direccionP == 7) {
				direccionExtr = 6;
			}
			else if (direccionP == 7) { direccionExtr = 6; }
			else if (direccionP == 0) { direccionExtr = 6; }
			else {
				direccionExtr = 0;
			}
		}

		//Animacion Pterodactilo 
		//Mov Alas
		if (direccionAlasP == 0) {
			if (alaDerecha < 1.55f) {
				alaDerecha += 6 * toRadians;
				alaIzquierda = -1 * alaDerecha;
			}
			else {
				direccionAlasP = 1;
			}
		}
		else if (direccionAlasP == 1) {
			if (alaDerecha > -1.55) {
				alaDerecha -= 6 * toRadians;
				alaIzquierda = -1 * alaDerecha;
			}
			else {
				direccionAlasP = 2;
			}
		}
		else if (direccionAlasP == 2) {
			if (direccionPter == 3 || direccionPter == 7) {
				if (alaDerecha < -1.55f) { direccionAlasP = 3; }
				else if (alaDerecha > 1.55f) { direccionAlasP = 4; }
			}
			else {
				direccionAlasP = 0;
			}
		}
		else if (direccionAlasP == 3) {
			if (alaDerecha < 0.0f) {
				alaDerecha += 6 * toRadians;
				alaIzquierda = -1 * alaDerecha;
			}
			else {
				direccionAlasP = 5;
			}
		}
		else if (direccionAlasP == 4) {
			if (alaDerecha > 0.0f) {
				alaDerecha -= 6 * toRadians;
				alaIzquierda = -1 * alaDerecha;
			}
			else {
				direccionAlasP = 5;
			}
		}
		else if (direccionAlasP == 5) {
			if (direccionPter == 4 || direccionPter == 8) {
				direccionAlasP = 0;
			}
		}

		//Recoerrido
		if (direccionPter == 0) {
			if (anguPtero < 180) {
				anguPtero += 0.09;
			}
			else { anguPtero = 0.0f; }
			if(movPterX > -80.0f ){
				movPterX -= 0.5 * pteroInit;
				movPterY += 0.1 * sin(anguPtero);
			}
			else { direccionPter = 1; }
		}
		else if (direccionPter == 1) {
			if (movPterX > -90.0f) {
				movPterX -= 0.3 * pteroInit;
			}
			else { direccionPter = 2; }
		}
		else if (direccionPter == 2) {
			if (movPterX > -100.0f) {
				movPterX -= 0.1 * pteroInit;
			}
			else { direccionPter = 3; }
		}
		else if (direccionPter == 3) {
			if (rotPteroY < 1.55f) {
				if (rotPteroZ > -0.775f ){ rotPteroZ -= 0.5 * toRadians; }
				movPterY += 0.05 * pteroInit;
				movPterX -= 0.05 * pteroInit;
				rotPteroY += 0.5 * toRadians;
			}
			else {
				direccionPter = 4;
			}
		}
		else if (direccionPter == 4) {
			if (rotPteroY < 3.1f) {
				if (rotPteroZ < 0.0f) { rotPteroZ += 0.5 * toRadians;}
				movPterY += 0.05 * pteroInit;
				movPterX += 0.01 * pteroInit;
				rotPteroY += 0.5 * toRadians;
			}
			else {
				direccionPter = 5;
				pteroInit = 0.0f;
			}
		}
		else if (direccionPter == 5) {
			if (anguPtero < 180) {
				anguPtero += 0.09;
			}
			else { anguPtero = 0.0f; }
			if (movPterX < -10.0f) {
				movPterX += 0.5 * pteroInit;
				movPterY += 0.1 * sin(anguPtero);
			}
			else { direccionPter = 6; }
		}
		else if (direccionPter == 6) {

			if (movPterX < 0.0f) {
				movPterX += 0.2 * pteroInit;
			}
			else { direccionPter = 7; }
		}
		else if (direccionPter == 7) {
			if (rotPteroY < 4.65f) {
				if (rotPteroZ > -0.775f) { rotPteroZ -= 0.5 * toRadians; }
				movPterY -= 0.05 * pteroInit;
				movPterX += 0.05 * pteroInit;
				rotPteroY += 0.5 * toRadians;
			}
			else {
				direccionPter = 8;
			}
		}
		else if (direccionPter == 8) {
			if (rotPteroY < 6.2f) {
				if (rotPteroZ < 0.0f) { rotPteroZ += 0.5 * toRadians; }
				movPterY -= 0.05 * pteroInit;
				movPterX -= 0.01 * pteroInit;
				rotPteroY += 0.5 * toRadians;
			}
			else {
				direccionPter = 9;
				pteroInit = 0.0f;
			}
		}
		else if (direccionPter == 9) {
			if (movPterX > 0.0f) {
				if(movPterY > 0.0f){ movPterY -= 0.1 *pteroInit; }
				movPterX -= 0.5 * pteroInit;
			}
			else {
				direccionPter = 0;
				pteroInit = 0.0f;
				movPterX = 0.0f;
				movPterY = 0.0f;
				movPterZ= 0.0f;
			}
		}


		/// Animacion de los Brachiosaurus
		if (posCue == 0) {
			if (rotCuello < 0.775) {
				rotCuello += 0.5 * toRadians;
			}
			else {
				posCue = 1;
			}
		}
		else if (posCue == 1) {
			if (rotMandi < 0.775) {
				rotMandi += 0.5 * toRadians;
			}
			else {
				posCue = 2;
			}
		}
		else if (posCue == 2) {
			if (rotMandi > 0.0) {
				rotMandi -= 0.5 * toRadians;
			}
			else {
				posCue = 3;
			}
		}
		else if (posCue == 3) {
			if (rotCuello > 0.0) {
				rotCuello -= 0.5 * toRadians;
			}
			else {
				posCue = 4;
			}
		}
		else if (posCue == 4) {
			
			if (rotMandi < 0.775) {
				rotMandi += 2 * toRadians;
			}
			else {
				posCue = 5;
			}
		}
		else if (posCue == 5) {
			if (contadorBran < 10) {
				if (rotMandi > 0.0) {
					rotMandi -= 2 * toRadians;
				}
				else {
					contadorBran += 1;
					posCue = 4;
				}
			}
			else {
				contadorBran = 0;
				posCue = 0;
			}
		}
		
		/////Animacion Segundo Brachiosaurus 
		
		if(posCuello ==0){
			if (rotBcuello < 0.775) {
				if (cuelloAsc > -0.5) { cuelloAsc -= 0.5 * toRadians; }
				rotBcuello += 0.5 * toRadians;
			}
			else {
				posCuello = 1;
			}
		}
		else if(posCuello == 1) {
			if (rotBcuello > -0.775) {
				if (cuelloAsc < 0.5) { cuelloAsc += 0.5 * toRadians; }
				rotBcuello -= 0.5 * toRadians;
			}
			else {
				posCuello = 0;
			}
		}
		if (mandiB == 0) {
			if ( rotBmandi< 0.7) {
				rotBmandi += 0.5 * toRadians;
			}
			else {
				mandiB = 1;
			}
		}
		else if (mandiB == 1) {
			if (rotBmandi > 0.0) {
				rotBmandi -= 0.5 * toRadians;
			}
			else {
				mandiB = 0;
			}
		}
		
		///////////////////////////////Termina las animaciones
		//printf("Contador Del Dia: %i\n", DiaNoche);
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
		spotLights[SpotL3].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

			

		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::mat4 maux(1.0);
		glm::mat4 modelPtero(1.0);
		glm::mat4 modelTree(1.0);
		glm::mat4 modelLamp(1.0);
		glm::mat4 modelBrach1(1.0);
		glm::mat4 modelBrach2(1.0);
		
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
		model = glm::translate(model, glm::vec3(43.0f, -1.2f, 46.0));
		
		
		model = glm::translate(model, glm::vec3(movCoche, 0.0f, movCocheI));
		model = glm::rotate(model, rotCoche, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		maux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		carritoPablo_VM.RenderModel();
		
		//printf("Valor en X: %f, Valor en Y:%f, Valor en Z: %f\n", mainWindow.getmuevex(), mainWindow.getmuevey(),mainWindow.getmuevez());
		//printf("movCoche: %f, movCocheI: %f, valor de rotacion: %f \n",movCoche, movCocheI, rotCoche);
		//printf("Rotacion de llanta:%f\n",rotllanta);

		/////////////////////////////////////////Llanta enfrente derecha 
		model =maux;
		model = glm::translate(model, glm::vec3(2.2f, -0.6f, -3.8f));//(2.0f, -0.3f, -2.0f));
		model = glm::rotate(model, rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		rueda_VM.RenderModel();

		/////////////////////////////////////////Llanta enfrente izquierda 

		model = maux;
		model = glm::translate(model, glm::vec3(-2.2f, -0.6f, -3.8f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.1f, 0.0f));
		model = glm::rotate(model, -1*rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		rueda_VM.RenderModel();

		/////////////////////////////////////////Llanta atras derecha 

		model = maux;
		model = glm::translate(model, glm::vec3(2.2f, -0.6f, 1.8f));
		model = glm::rotate(model, rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		rueda_VM.RenderModel();

		/////////////////////////////////////////Llanta atras izquierda 

		model = maux;
		model = glm::translate(model, glm::vec3(-2.2f, -0.6f, 1.8f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.1f, 0.0f));
		model = glm::rotate(model, -1 * rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		rueda_VM.RenderModel();

		///////////////// Volcan
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(23.0f, 0.0f, -160.0f));
		model = glm::scale(model, glm::vec3(2.3f, 2.3f, 2.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		volcan_VM.RenderModel();


		////////////////Camino
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-3.0f, -2.1f, 20.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		camino_Voxel_M.RenderModel();

		/////////Arboles 
		
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(17.8f, 5.4f, 0.0f));
		model = glm::scale(model, glm::vec3(2.3f, 2.3f, 2.3f));
		modelTree = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		SakuraT_M.RenderModel();
		
		model = modelTree;
		model = glm::translate(model, glm::vec3(20.2f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		SakuraT_M.RenderModel();

		model = modelTree;
		model = glm::translate(model, glm::vec3(-23.2f, -3.2f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		TreeT_M.RenderModel();

		model = modelTree;
		model = glm::translate(model, glm::vec3(-25.4f, -3.2f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		TreeT_M.RenderModel();
		
		model = modelTree;
		model = glm::translate(model, glm::vec3(-4.9f, -3.2f, 22.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		TreeT_M.RenderModel();
		
		/////////////////Lamparas

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-20.9f, -2.0f, 45.8f));
		modelLamp = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lamp_M.RenderModel();

		model = modelLamp;
		model = glm::translate(model, glm::vec3(17.4f, 0.0f, -11.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lamp_M.RenderModel();

		model = modelLamp;
		model = glm::translate(model, glm::vec3(47.0f, 0.0f, -0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lamp_M.RenderModel();


		//////////////Pablo Voxel 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-20.4f, -1.55f, 43.6f));
		model = glm::translate(model, glm::vec3(movPabloX, movPabloY , movPabloZ));
		model = glm::rotate(model, rotPablo, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cuerpo_Pablo.RenderModel();
		//printf("PabloX: %f, PabloY: %f,  PabloZ: %f, rotPAblo %f, DirP: %d, DirExtr: %d, PAblo: %d \n",movPabloX, movPabloY, movPabloZ, rotPablo, direccionP, direccionExtr, banderaPablo );
		
		///////////////Brazo derecho
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.8f, 1.5f, -0.0f));
		model = glm::rotate(model, izquierda, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		brazoDer_Pablo.RenderModel();

		////////////////BRazo Izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.75f, 1.5f, -0.0f));
		model = glm::rotate(model,brazoDer, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		brazoIz_Pablo.RenderModel();
		////////////////Pierna Derecha 
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.3f, 0.4f, 0.0f));
		model = glm::rotate(model, derecha, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		piernaDer_Pablo.RenderModel();
		////////////////Pierna Izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.3f, 0.4f, 0.0f));
		model = glm::rotate(model, izquierda, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		piernaIz_Pablo.RenderModel();

		////////////////////Pterodactyl
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(16.5f, 3.3f, 14.7f));
		model = glm::translate(model, glm::vec3(movPterX, movPterY, movPabloZ));
		model = glm::rotate(model,rotPteroZ, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, rotPteroY, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//printf("X:%f, Y:%f, Z:%f, rotZ:%f, rotY:%f, AlDer:%f, AlIzq:%f\n", movPterX, movPterY, movPterZ, rotPteroZ, rotPteroY,alaDerecha, alaIzquierda);
		modelPtero = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ptero_VM.RenderModel();
		
		//Ala Derecha
		model = modelPtero;
		model = glm::translate(model, glm::vec3(-0.5f, 0.0f, -1.2f));
		model = glm::rotate(model, alaDerecha, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		alaDer_VM.RenderModel();

		// Ala izquierda
		model = modelPtero;
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 1.2f));
		model = glm::rotate(model, alaIzquierda , glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		alaIzq_VM.RenderModel();

		////////////////Brachiosaurus
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-27.4f, 0.6f, 0.0f));

		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.1f, 0.0f));
		model = glm::scale(model, glm::vec3(2.3f, 2.3f, 2.3f));
		//printf("rotC:%f, rotM:%f, PosC:%i\n", rotCuello, rotMandi, posCue);
		modelBrach1 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Brachiosaurus_M.RenderModel();

		model = modelBrach1;
		model = glm::translate(model, glm::vec3(0.0f, 0.9f, 1.2f));
		model = glm::rotate(model, rotCuello, glm::vec3(1.0f, 0.0f, 0.0f));
		modelBrach1 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bcuello_M.RenderModel();


		model = modelBrach1;
		model = glm::translate(model, glm::vec3(0.01f, 2.0f, 1.4f));
		model = glm::rotate(model, rotMandi, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bquij_M.RenderModel();

		//Segundo Brachiosaurus 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(41.2f, 0.6f, -26.8f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.1f, 0.0f));
		model = glm::scale(model, glm::vec3(2.3f, 2.3f, 2.3f));
		//printf("CueA:%f, rotBC:%f, :%i\n", cuelloAsc, rotBcuello, posCuello);
		modelBrach2 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Brachiosaurus_M.RenderModel();
		
		model = modelBrach2;
		model = glm::translate(model, glm::vec3(0.0f, 0.9f, 1.2f));
		model = glm::rotate(model, cuelloAsc, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, rotBcuello, glm::vec3(0.0f, 1.0f, 0.0f));
		modelBrach2 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bcuello_M.RenderModel();


		model = modelBrach2;
		model = glm::translate(model, glm::vec3(0.01f, 2.0f, 1.4f));
		model = glm::rotate(model, rotBmandi, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bquij_M.RenderModel();

		//////////////////////////Termina los modelos////////////////////////////////
		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}

