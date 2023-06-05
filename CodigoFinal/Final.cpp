/*------------------------------------------------------------------------*/
/* ----------------             PROYECTO FINAL TEORÍA          -----------*/
/* ----------------   COMPUTACIÓN GRÁFICA E INTERACCIÓN H-C    -----------*/
/*-----------------                   2023-2      ------------------------*/
/*-------------        Alumnos             |    No. Cuenta ---------------*/
/*------------- Fajardo Téllez Eduardo     |    316053206  ---------------*/
/*------------- Rodríguez Jiménez Karla    |    313322752  ---------------*/
/*------------- Romero Rivera Geovanni	   | 	316215817  ---------------*/

#include <Windows.h>

#include <glad/glad.h>
#include <glfw3.h>	//main
#include <stdlib.h>		
#include <glm/glm.hpp>	//camera y model
#include <glm/gtc/matrix_transform.hpp>	//camera y model
#include <glm/gtc/type_ptr.hpp>
#include <time.h>


#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>	//Texture

#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>

#include <shader_m.h>
#include <camera.h>
#include <modelAnim.h>
#include <model.h>
#include <Skybox.h>
#include <iostream>
//Para leer archivos
#include <fstream>
#include <string.h>

//#pragma comment(lib, "winmm.lib")

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void my_input(GLFWwindow *window);
void my_input(GLFWwindow* window, int key, int scancode, int action, int mods);
void animate(void);

// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
GLFWmonitor *monitors;

void getResolution(void);

// camera
Camera camera(glm::vec3(0.0f, 10.0f, 90.0f));
float MovementSpeed = 0.1f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
const int FPS = 60;
const int LOOP_TIME = 1000 / FPS; // = 16 milisec // 1000 millisec == 1 sec
double	deltaTime = 0.0f,
		lastFrame = 0.0f;

//Lighting
glm::vec3 lightPosition(0.0f, 4.0f, -10.0f);
glm::vec3 lightDirection(0.0f, -1.0f, -1.0f);
glm::vec3 lightDiff(0.0f, 0.0f, 0.0f);



float myVariable = 0.0f;

float myvariableDiff = 0.0f;

// posiciones
float	movAuto_x = 0.0f,
		movAuto_z = 0.0f,
		orienta = 0.0f;
bool	animacion = true,
		recorrido1 = true,
		recorrido2 = false,
		recorrido3 = false,
		recorrido4 = false;

//Variables animación pingüino
bool recorridoPingu = false;

float	movPinX = -477.0f,
		movPinY = 3.3f,
		movPinZ = -638.8f,
		movPinX1 = -573.0f,
		movPinY1 = -2.0f,
		movPinZ1 = -565.0f;

// Variables para la animación de la cebra
ifstream AniZebra;
string strZ;
int indZ = 0;
float	Esc_Zebra = 10.0f,
		ZposX = -540.0f,
		ZposY = 10.0f,
		ZposZ = -140.0f,
		Zgiro = 180.0f,
		Zgcabeza = 0.0f,
		Zgpatafd = 0.0f,
		Zgpatafi = 0.0f,
		Zgpatatd = 0.0f,
		Zgpatati = 0.0f,
		Zgpzfd = 0.0f,
		Zgpzfi = 0.0f,
		Zgpztd = 0.0f,
		Zgpzti = 0.0f,
		Zgcola = 0.0f;

float	incZposX = 0.0f,
		incZposY = 0.0f,
		incZposZ = 0.0f,
		incZgiro = 0.0f,
		incZgcabeza = 0.0f,
		incZgpatafd = 0.0f,
		incZgpatafi = 0.0f,
		incZgpatatd = 0.0f,
		incZgpatati = 0.0f,
		incZgpzfd = 0.0f,
		incZgpzfi = 0.0f,
		incZgpztd = 0.0f,
		incZgpzti = 0.0f,
		incZgcola = 0.0f;

// Variables para la animación del jeep
ifstream AniJeep;
string strJ;
int indJ = 0;
float	Esc_Jeep = 25.0f,
		JposX = 85.0f,
		JposZ = -192.0f,
		Jgiro = -50.0f;
float   incJposX = 0.0f,
		incJposZ = 0.0f,
		incJgiro = 0.0f;

#define MAX_FRAMESZ 7
#define MAX_FRAMESJ 36
int i_max_steps = 60;
int i_curr_steps = 0;

typedef struct _frameZebra
{
	//Variables para GUARDAR Key Frames
	float ZposX = 0.0f;
	float ZposY = 0.0f;
	float ZposZ = 0.0f;
	float Zgiro = 0.0f;
	float Zgcabeza = 0.0f;
	float Zgpatafd = 0.0f;
	float Zgpatafi = 0.0f;
	float Zgpatatd = 0.0f;
	float Zgpatati = 0.0f;
	float Zgpzfd = 0.0f;
	float Zgpzfi = 0.0f;
	float Zgpztd = 0.0f;
	float Zgpzti = 0.0f;
	float Zgcola = 0.0f;
}FRAMEZ;

typedef struct _frameJeep
{
	//Variables para GUARDAR Key Frames
	float JposX = 0.0f;
	float JposZ = 0.0f;
	float Jgiro = 0.0f;
}FRAMEJ;

void printPos(void) {
	printf("PosX = %f\n", JposX);
	printf("PosZ = %f\n", JposZ);
	printf("Giro = %f\n", Jgiro);
}

FRAMEZ KFZ[MAX_FRAMESZ];
int FIZ = 0;			//introducir número en caso de tener Key guardados
bool playZ = false;
int playIndexZ = 0;

FRAMEJ KFJ[MAX_FRAMESJ];
int FIJ = 0;			//introducir número en caso de tener Key guardados
bool playJ = false;
int playIndexJ = 0;


void saveFrameZ(void)
{
	//printf("frameindex %d\n", FrameIndex);
	std::cout << "Frame Index = " << FIZ << std::endl;

	KFZ[FIZ].ZposX = ZposX;
	KFZ[FIZ].ZposY = ZposY;
	KFZ[FIZ].ZposZ = ZposZ;
	KFZ[FIZ].Zgiro = Zgiro;
	KFZ[FIZ].Zgcabeza = Zgcabeza;
	KFZ[FIZ].Zgpatafd = Zgpatafd;
	KFZ[FIZ].Zgpatafi = Zgpatafi;
	KFZ[FIZ].Zgpatatd = Zgpatatd;
	KFZ[FIZ].Zgpatati = Zgpatati;
	KFZ[FIZ].Zgpzfd = Zgpzfd;
	KFZ[FIZ].Zgpzfi = Zgpzfi;
	KFZ[FIZ].Zgpztd = Zgpztd;
	KFZ[FIZ].Zgpzti = Zgpzti;
	KFZ[FIZ].Zgcola = Zgcola;

	printf("frame guardado, KFZ[%d].posZ = %f\n", FIZ, ZposZ);

	FIZ++;
}

void saveFrameJ(void)
{
	//printf("frameindex %d\n", FrameIndex);
	std::cout << "Frame Index = " << FIJ << std::endl;

	KFJ[FIJ].JposX = JposX;
	KFJ[FIJ].JposZ = JposZ;
	KFJ[FIJ].Jgiro = Jgiro;

	printf("frame guardado, KFJ[%d].posZ = %f\n", FIJ, Jgiro);

	FIJ++;
}

void resetElementsZ(void)
{
	ZposX = KFZ[0].ZposX;
	ZposY = KFZ[0].ZposY;
	ZposZ = KFZ[0].ZposZ;
	Zgiro = KFZ[0].Zgiro;
	Zgcabeza = KFZ[0].Zgcabeza;
	Zgpatafd = KFZ[0].Zgpatafd;
	Zgpatafi = KFZ[0].Zgpatafi;
	Zgpatatd = KFZ[0].Zgpatatd;
	Zgpatati = KFZ[0].Zgpatati;
	Zgpzfd = KFZ[0].Zgpzfd;
	Zgpzfi = KFZ[0].Zgpzfi;
	Zgpztd = KFZ[0].Zgpztd;
	Zgpzti = KFZ[0].Zgpzti;
	Zgcola = KFZ[0].Zgcola;
}

void resetElementsJ(void)
{
	JposX = KFJ[0].JposX;
	JposZ = KFJ[0].JposZ;
	Jgiro = KFJ[0].Jgiro;
}

void interpolationZ(void)
{	
	incZposX = (KFZ[playIndexZ + 1].ZposX - KFZ[playIndexZ].ZposX) / i_max_steps;
	incZposY = (KFZ[playIndexZ + 1].ZposY - KFZ[playIndexZ].ZposY) / i_max_steps;
	incZposZ = (KFZ[playIndexZ + 1].ZposZ - KFZ[playIndexZ].ZposZ) / i_max_steps;
	incZgiro = (KFZ[playIndexZ + 1].Zgiro - KFZ[playIndexZ].Zgiro) / i_max_steps;
	incZgcabeza = (KFZ[playIndexZ + 1].Zgcabeza - KFZ[playIndexZ].Zgcabeza) / i_max_steps;
	incZgpatafd = (KFZ[playIndexZ + 1].Zgpatafd - KFZ[playIndexZ].Zgpatafd) / i_max_steps;
	incZgpatafi = (KFZ[playIndexZ + 1].Zgpatafi - KFZ[playIndexZ].Zgpatafi) / i_max_steps;
	incZgpatatd = (KFZ[playIndexZ + 1].Zgpatatd - KFZ[playIndexZ].Zgpatatd) / i_max_steps;
	incZgpatati = (KFZ[playIndexZ + 1].Zgpatati - KFZ[playIndexZ].Zgpatati) / i_max_steps;
	incZgpzfd = (KFZ[playIndexZ + 1].Zgpzfd - KFZ[playIndexZ].Zgpzfd) / i_max_steps;
	incZgpzfi = (KFZ[playIndexZ + 1].Zgpzfi - KFZ[playIndexZ].Zgpzfi) / i_max_steps;
	incZgpztd = (KFZ[playIndexZ + 1].Zgpztd - KFZ[playIndexZ].Zgpztd) / i_max_steps;
	incZgpzti = (KFZ[playIndexZ + 1].Zgpzti - KFZ[playIndexZ].Zgpzti) / i_max_steps;
	incZgcola = (KFZ[playIndexZ + 1].Zgcola - KFZ[playIndexZ].Zgcola) / i_max_steps;
}

void interpolationJ(void)
{
	incJposX = (KFJ[playIndexJ + 1].JposX - KFJ[playIndexJ].JposX) / i_max_steps;
	incJposZ = (KFJ[playIndexJ + 1].JposZ - KFJ[playIndexJ].JposZ) / i_max_steps;
	incJgiro = (KFJ[playIndexJ + 1].Jgiro - KFJ[playIndexJ].Jgiro) / i_max_steps;
	
}

void animate(void)
{
	lightPosition.x = 150.0f * cos(myVariable);
	lightPosition.z = 150.0f * sin(myVariable);
	myVariable += 0.05f;


	lightDiff.x = 1.0f * cos(myVariable);
	lightDiff.z = 1.0f * sin(myVariable);
	myvariableDiff += 0.09f;
	 

	if (playZ)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndexZ++;
			if (playIndexZ > FIZ - 2)	//end of total animation?
			{
				std::cout << "Animation ended" << std::endl;
				//printf("termina anim\n");
				playIndexZ = 0;
				playZ = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
								  //Interpolation
				interpolationZ();
			}
		}
		else
		{
			//Draw animation
			ZposX += incZposX;
			ZposY += incZposY;
			ZposZ += incZposZ;
			Zgiro += incZgiro;
			Zgcabeza += incZgcabeza;
			Zgpatafd += incZgpatafd;
			Zgpatafi += incZgpatafi;
			Zgpatatd += incZgpatatd;
			Zgpatati += incZgpatati;
			Zgpzfd += incZgpzfd;
			Zgpzfi += incZgpzfi;
			Zgpztd += incZgpztd;
			Zgpzti += incZgpzti;
			Zgcola += incZgcola;

			i_curr_steps++;
		}
	}

	if (playJ)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndexJ++;
			if (playIndexJ > FIJ - 2)	//end of total animation?
			{
				std::cout << "Animation ended" << std::endl;
				//printf("termina anim\n");
				playIndexJ = 0;
				playJ = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
				//Interpolation
				interpolationJ();
			}
		}
		else
		{
			//Draw animation
			JposX += incJposX;
			JposZ += incJposZ;
			Jgiro += incJgiro;

			i_curr_steps++;
		}
	}

	//Pinguino
	if (animacion)
	{

		if (recorridoPingu == true) {
			movPinZ1 += 0.1f;
			if (movPinZ1 <= -562.0f) {
				movPinZ1 += 0.01f;
			}
			else {
				recorridoPingu = false;
			}
			movPinY1 = 4 * cos(movPinZ1);
		}
	}
}

void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}


int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	// --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CGeIHC", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window, 0, 30);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, my_input);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	Shader staticShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights_mod.fs");
	Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.fs");
	Shader animShader("Shaders/anim.vs", "Shaders/anim.fs");

	vector<std::string> faces
	{
		"resources/skybox/right.jpg",
		"resources/skybox/left.jpg",
		"resources/skybox/top.jpg",
		"resources/skybox/bottom.jpg",
		"resources/skybox/front.jpg",
		"resources/skybox/back.jpg"
	};

	Skybox skybox = Skybox(faces);

	// Shader configuration
	// --------------------
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	// load models
	// -----------
	Model piso("resources/objects/piso/piso.obj");
	Model Elefante("resources/objects/Elefante/elefante.obj");
	Model Base("resources/objects/ZooBase/Base.obj");
	Model Cocodrilo("resources/objects/Cocodrilo/Cocodrilo.obj");
	Model Leon("resources/objects/Leon/Leon.obj");
	Model Oso("resources/objects/Oso/Bear.obj");
	Model Panda("resources/objects/Panda/OsoPanda.obj");
	Model Carro("resources/objects/jeep/jeep.obj");
	Model Pingu("resources/objects/Pingu/Pingu.obj");
	Model AlaDer("resources/objects/Pingu/Ala_der.obj");
	Model Pingu1("resources/objects/Pingu/Pingu01.obj");
	Model boy6("resources/objects/Eduardo/Kids/Boy6.obj");
	Model man("resources//objects/Eduardo/Man/ManCasual3.obj");

	// Carga de modelos para la cebra
	Model Zcuerpo("resources/objects/Zebra/Zcuerpo.obj");
	Model Zcabeza("resources/objects/Zebra/Zcabeza.obj");
	Model ZpataFD("resources/objects/Zebra/Zpatafd.obj");
	Model ZpataFI("resources/objects/Zebra/Zpatafi.obj");
	Model ZpataTD("resources/objects/Zebra/Zpatatd.obj");
	Model ZpataTI("resources/objects/Zebra/Zpatati.obj");
	Model ZpzFD("resources/objects/Zebra/Zpzfd.obj");
	Model ZpzFI("resources/objects/Zebra/Zpzfi.obj");
	Model ZpzTD("resources/objects/Zebra/Zpztd.obj");
	Model ZpzTI("resources/objects/Zebra/Zpzti.obj");
	Model Zcola("resources/objects/Zebra/Zcola.obj");

	// Carga de modelos para el jeep
	Model jeep("resources/objects/Jeep/Chasis.obj");
	Model llanta("resources/objects/Jeep/Llanta.obj");
	
	ModelAnim womanC("resources/objects/Eduardo/Woman/WomanCasual/Walking.dae");
	womanC.initShaders(animShader.ID);

	ModelAnim womanD("resources/objects/Eduardo/Woman/WomanDress/Walking.dae");
	womanD.initShaders(animShader.ID);
	/*ModelAnim animacionPersonaje("resources/objects/Personaje1/PersonajeBrazo.dae");
	animacionPersonaje.initShaders(animShader.ID);

	ModelAnim ninja("resources/objects/ZombieWalk/ZombieWalk.dae");
	ninja.initShaders(animShader.ID);*/

	//Inicialización de KeyFrames para la animación de la cebra
	AniZebra.open("Animaciones/AniZebra.txt", ios::in);
	//saveFrameZ();
	while (indZ < MAX_FRAMESZ && !AniZebra.eof()) {
		getline(AniZebra, strZ);
		ZposX = stof(strZ);
		getline(AniZebra, strZ);
		ZposY = stof(strZ);
		getline(AniZebra, strZ);
		ZposZ = stof(strZ);
		getline(AniZebra, strZ);
		Zgiro = stof(strZ);
		getline(AniZebra, strZ);
		Zgcabeza = stof(strZ);
		getline(AniZebra, strZ);
		Zgpatafd = stof(strZ);
		getline(AniZebra, strZ);
		Zgpatafi = stof(strZ);
		getline(AniZebra, strZ);
		Zgpatatd = stof(strZ);
		getline(AniZebra, strZ);
		Zgpatati = stof(strZ);
		getline(AniZebra, strZ);
		Zgpzfd = stof(strZ);
		getline(AniZebra, strZ);
		Zgpzfi = stof(strZ);
		getline(AniZebra, strZ);
		Zgpztd = stof(strZ);
		getline(AniZebra, strZ);
		Zgpzti = stof(strZ);
		getline(AniZebra, strZ);
		Zgcola = stof(strZ);
		saveFrameZ();
		indZ++;
	}
	AniZebra.close();

	//Inicialización de KeyFrames para la animación de la cebra
	AniJeep.open("Animaciones/AniJeep.txt", ios::in);
	//saveFrameJ();
	while (indJ < MAX_FRAMESJ && !AniJeep.eof()) {
		getline(AniJeep, strJ);
		JposX = stof(strJ);
		getline(AniJeep, strJ);
		JposZ = stof(strJ);
		getline(AniJeep, strJ);
		Jgiro = stof(strJ);
		saveFrameJ();
		indJ++;
	}
	AniJeep.close();

	//Inicialización de KeyFrames
	/*for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].posX = 0;
		KeyFrame[i].posY = 0;
		KeyFrame[i].posZ = 0;
		KeyFrame[i].rotRodIzq = 0;
		KeyFrame[i].giroMonito = 0;
	}*/

	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		skyboxShader.setInt("skybox", 0);
		
		// per-frame time logic
		// --------------------
		lastFrame = SDL_GetTicks();

		// input
		// -----
		//my_input(window);
		animate();

		// render
		// ------
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		staticShader.use();
		//Setup Advanced Lights
		staticShader.setVec3("viewPos", camera.Position);
		staticShader.setVec3("dirLight.direction", lightDirection);
		staticShader.setVec3("dirLight.ambient", glm::vec3(0.8f, 0.8f, 0.8f));
		staticShader.setVec3("dirLight.diffuse", glm::vec3(0.2f, 0.2f, 0.2f));
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));

		staticShader.setVec3("pointLight[0].position", lightPosition);
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[0].constant", 0.08f);
		staticShader.setFloat("pointLight[0].linear", 0.009f);
		staticShader.setFloat("pointLight[0].quadratic", 0.00032f);

		staticShader.setVec3("pointLight[1].position", glm::vec3(0.0, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].diffuse", glm::vec3(lightDiff));
		staticShader.setVec3("pointLight[1].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[1].constant", 1.0f);
		staticShader.setFloat("pointLight[1].linear", 0.009f);
		staticShader.setFloat("pointLight[1].quadratic", 0.00032f);

		staticShader.setVec3("spotLight[0].position", glm::vec3(camera.Position.x, camera.Position.y, camera.Position.z)); //Tipo reflector
		staticShader.setVec3("spotLight[0].direction", glm::vec3(camera.Front.x, camera.Front.y, camera.Front.z));
		staticShader.setVec3("spotLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("spotLight[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("spotLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("spotLight[0].cutOff", glm::cos(glm::radians(10.0f)));
		staticShader.setFloat("spotLight[0].outerCutOff", glm::cos(glm::radians(15.0f)));
		staticShader.setFloat("spotLight[0].constant", 1.0f);
		staticShader.setFloat("spotLight[0].linear", 0.0009f);
		staticShader.setFloat("spotLight[0].quadratic", 0.00005f);

		staticShader.setFloat("material_shininess", 32.0f);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 tmp = glm::mat4(1.0f);

		// Matrices temporales para la animacion de la cebra
		glm::mat4 tmpZcuerpo = glm::mat4(1.0f);
		glm::mat4 tmpZcabeza = glm::mat4(1.0f);
		glm::mat4 tmpZpataFD = glm::mat4(1.0f);
		glm::mat4 tmpZpataFI = glm::mat4(1.0f);
		glm::mat4 tmpZpataTD = glm::mat4(1.0f);
		glm::mat4 tmpZpataTI = glm::mat4(1.0f);
		glm::mat4 tmpZpzFD = glm::mat4(1.0f);
		glm::mat4 tmpZpzFI = glm::mat4(1.0f);
		glm::mat4 tmpZpzTD = glm::mat4(1.0f);
		glm::mat4 tmpZpzTI = glm::mat4(1.0f);
		glm::mat4 tmpZcola = glm::mat4(1.0f);

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		//// Light
		glm::vec3 lightColor = glm::vec3(0.6f);
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.75f);
		

		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje Animacion
		// -------------------------------------------------------------------------------------------------------------------------
		//Remember to activate the shader with the animation
		animShader.use();
		animShader.setMat4("projection", projection);
		animShader.setMat4("view", view);
	
		animShader.setVec3("material.specular", glm::vec3(0.5f));
		animShader.setFloat("material.shininess", 32.0f);
		animShader.setVec3("light.ambient", ambientColor);
		animShader.setVec3("light.diffuse", diffuseColor);
		animShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		animShader.setVec3("light.direction", lightDirection);
		animShader.setVec3("viewPos", camera.Position);

		//model = glm::translate(glm::mat4(1.0f), glm::vec3(-40.3f, 1.75f, 0.3f)); // translate it down so it's at the center of the scene
		//model = glm::scale(model, glm::vec3(1.2f));	// it's a bit too big for our scene, so scale it down
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//animShader.setMat4("model", model);
		/*animacionPersonaje.Draw(animShader);*/

		// -------------------------------------------------------------------------------------------------------------------------
		// Mujer de Vestido
		// -------------------------------------------------------------------------------------------------------------------------

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-700.0f, 1.75f, -350.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.05f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		womanD.Draw(animShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Mujer Casual
		// -------------------------------------------------------------------------------------------------------------------------

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-450.0f, 1.75f, -350.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.05f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		womanC.Draw(animShader);

		// -------------------------------------------------------------------------------------------------------------------------
		//                                               Escenario
		// -------------------------------------------------------------------------------------------------------------------------
		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-20.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		/*piso.Draw(staticShader);*/


		model = glm::translate(glm::mat4(1.0f), glm::vec3(30.0f, -1.0f, -100.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		Base.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		//                                                 Animales
		// -------------------------------------------------------------------------------------------------------------------------
		// Elefante
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-120.0f, -2.0f, -510.0f));
		model = glm::scale(model, glm::vec3(8.0f));
		staticShader.setMat4("model", model);
		Elefante.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Cebra
		// -------------------------------------------------------------------------------------------------------------------------

		//Cuerpo
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(ZposX, ZposY, ZposZ));
		tmpZcuerpo = model = glm::rotate(model, glm::radians(Zgiro), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(tmpZcuerpo, glm::vec3(Esc_Zebra));
		staticShader.setMat4("model", model);
		Zcuerpo.Draw(staticShader);

		//Cabeza
		//model = glm::mat4(1.0f);
		model = glm::translate(tmpZcuerpo, glm::vec3(0.0f, 0.0f, 0.0f));
		tmpZcabeza = model = glm::rotate(model, glm::radians(Zgcabeza), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(Esc_Zebra));
		staticShader.setMat4("model", model);
		Zcabeza.Draw(staticShader);

		//Pata Frontal Derecha
		//model = glm::mat4(1.0f);
		model = glm::translate(tmpZcuerpo, glm::vec3(0.0f, 0.0f, 0.0f));
		tmpZpataFD = model = glm::rotate(model, glm::radians(Zgpatafd), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(tmpZpataFD, glm::vec3(Esc_Zebra));
		staticShader.setMat4("model", model);
		ZpataFD.Draw(staticShader);

		//Pata Frontal Izquierda
		//model = glm::mat4(1.0f);
		model = glm::translate(tmpZcuerpo, glm::vec3(0.0f, 0.0f, 0.0f));
		tmpZpataFI = glm::rotate(model, glm::radians(Zgpatafi), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(tmpZpataFI, glm::vec3(Esc_Zebra));
		staticShader.setMat4("model", model);
		ZpataFI.Draw(staticShader);

		//Pata Trasera Derecha
		//model = glm::mat4(1.0f);
		model = glm::translate(tmpZcuerpo, glm::vec3(0.0f, 0.0f, 0.0f));
		tmpZpataTD = glm::rotate(model, glm::radians(Zgpatatd), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(tmpZpataTD, glm::vec3(Esc_Zebra));
		staticShader.setMat4("model", model);
		ZpataTD.Draw(staticShader);

		//Pata Trasera Izquierda
		//model = glm::mat4(1.0f);
		model = glm::translate(tmpZcuerpo, glm::vec3(0.0f, 0.0f, 0.0f));
		tmpZpataTI = glm::rotate(model, glm::radians(Zgpatati), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(tmpZpataTI, glm::vec3(Esc_Zebra));
		staticShader.setMat4("model", model);
		ZpataTI.Draw(staticShader);

		//Pezuña Frontal Derecha
		//model = glm::mat4(1.0f);
		model = glm::translate(tmpZpataFD, glm::vec3(0.0f, 0.0f, 0.0f));
		tmpZpzFD = model = glm::rotate(model, glm::radians(Zgpzfd), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(Esc_Zebra));
		staticShader.setMat4("model", model);
		ZpzFD.Draw(staticShader);

		//Pezuña Frontal Izquierda
		//model = glm::mat4(1.0f);
		model = glm::translate(tmpZpataFI, glm::vec3(0.0f, 0.0f, 0.0f));
		tmpZpzFI = model = glm::rotate(model, glm::radians(Zgpzfi), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(Esc_Zebra));
		staticShader.setMat4("model", model);
		ZpzFI.Draw(staticShader);

		//Pezuña Trasera Derecha
		//model = glm::mat4(1.0f);
		model = glm::translate(tmpZpataTD, glm::vec3(0.0f, 0.0f, 0.0f));
		tmpZpzTD = model = glm::rotate(model, glm::radians(Zgpztd), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(Esc_Zebra));
		staticShader.setMat4("model", model);
		ZpzTD.Draw(staticShader);

		//Pezuña Trasera Izquierda
		//model = glm::mat4(1.0f);
		model = glm::translate(tmpZpataTI, glm::vec3(0.0f, 0.0f, 0.0f));
		tmpZpzTI = model = glm::rotate(model, glm::radians(Zgpzti), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(Esc_Zebra));
		staticShader.setMat4("model", model);
		ZpzTI.Draw(staticShader);

		//Cola
		//model = glm::mat4(1.0f);
		model = glm::translate(tmpZcuerpo, glm::vec3(0.0f, 0.0f, 0.0f));
		tmpZcola = model = glm::rotate(model, glm::radians(Zgcola), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(Esc_Zebra));
		staticShader.setMat4("model", model);
		Zcola.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Cocodilo
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::translate(glm::mat4(1.0f), glm::vec3(80.0f, -2.0f, -510.0f));
		model = glm::scale(model, glm::vec3(1.5f));
		staticShader.setMat4("model", model);
		Cocodrilo.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// León
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-50.0f, 3.0f, -275.0f));
		model = glm::scale(model, glm::vec3(2.0f));
		staticShader.setMat4("model", model);
		Leon.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Panda
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-800.0f, 10.0f, -300.0f));
		/*model = glm::rotate();*/
		model = glm::scale(model, glm::vec3(7.0f));
		staticShader.setMat4("model", model);
		Panda.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Hombre
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::translate(glm::mat4(1.0f), glm::vec3(30.0f, 1.75f, -595.0f));
		model = glm::rotate(model, glm::radians(315.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.15f));
		staticShader.setMat4("model", model);
		man.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Niño
		// -------------------------------------------------------------------------------------------------------------------------
		
		model = glm::translate(glm::mat4(1.0f), glm::vec3(25.0f, 1.75f, -600.0f));
		model = glm::rotate(model, glm::radians(315.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.15f));
		staticShader.setMat4("model", model);
		boy6.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Pingüino 1
		// -------------------------------------------------------------------------------------------------------------------------
		//model = glm::translate(glm::mat4(1.0f), glm::vec3(movPinX, movPinY, movPinZ));
		///*model = glm::rotate();*/
		//model = glm::scale(model, glm::vec3(1.5f));
		//staticShader.setMat4("model", model);
		//Pingu.Draw(staticShader);

		//model = glm::translate(glm::mat4(1.0f), glm::vec3(movPinX, movPinY, movPinZ));
		//model = glm::scale(model, glm::vec3(1.5f));
		//staticShader.setMat4("model", model);
		//AlaDer.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(movPinX1, movPinY1, movPinZ1));
		model = glm::scale(model, glm::vec3(2.0f));
		staticShader.setMat4("model", model);
		Pingu1.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Jeep
		// -------------------------------------------------------------------------------------------------------------------------
		
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(JposX, 5.0f,JposZ));
		tmp = model = glm::rotate(model, glm::radians(Jgiro), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(Esc_Jeep));
		staticShader.setMat4("model", model);
		jeep.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(Esc_Jeep));
		staticShader.setMat4("model", model);
		llanta.Draw(staticShader);	//Der delantera

		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, -20.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(Esc_Jeep));
		staticShader.setMat4("model", model);  
		llanta.Draw(staticShader);  //Izq delantera

		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 17.0f));
		model = glm::scale(model, glm::vec3(Esc_Jeep));
		staticShader.setMat4("model", model);
		llanta.Draw(staticShader);	//Der trasera
		
		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, -3.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(Esc_Jeep));
		staticShader.setMat4("model", model);
		llanta.Draw(staticShader);	//Izq trasera

		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje
		// -------------------------------------------------------------------------------------------------------------------------
		//model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		//model = glm::translate(model, glm::vec3(posX, posY, posZ));
		//tmp = model = glm::rotate(model, glm::radians(giroMonito), glm::vec3(0.0f, 1.0f, 0.0));
		//staticShader.setMat4("model", model);
		//torso.Draw(staticShader);

		////Pierna Der
		//model = glm::translate(tmp, glm::vec3(-0.5f, 0.0f, -0.1f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		//model = glm::rotate(model, glm::radians(-rotRodIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		//staticShader.setMat4("model", model);
		//piernaDer.Draw(staticShader);

		////Pie Der
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		//staticShader.setMat4("model", model);
		//botaDer.Draw(staticShader);

		////Pierna Izq
		//model = glm::translate(tmp, glm::vec3(0.5f, 0.0f, -0.1f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//staticShader.setMat4("model", model);
		//piernaIzq.Draw(staticShader);

		////Pie Iz
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		//staticShader.setMat4("model", model);
		//botaDer.Draw(staticShader);	//Izq trase

		////Brazo derecho
		//model = glm::translate(tmp, glm::vec3(0.0f, -1.0f, 0.0f));
		//model = glm::translate(model, glm::vec3(-0.75f, 2.5f, 0));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//staticShader.setMat4("model", model);
		//brazoDer.Draw(staticShader);

		////Brazo izquierdo
		//model = glm::translate(tmp, glm::vec3(0.0f, -1.0f, 0.0f));
		//model = glm::translate(model, glm::vec3(0.75f, 2.5f, 0));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//staticShader.setMat4("model", model);
		//brazoIzq.Draw(staticShader);

		////Cabeza
		//model = glm::translate(tmp, glm::vec3(0.0f, -1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		//model = glm::translate(model, glm::vec3(0.0f, 2.5f, 0));
		//staticShader.setMat4("model", model);
		//cabeza.Draw(staticShader);
		// -------------------------------------------------------------------------------------------------------------------------
		// Caja Transparente --- Siguiente Práctica
		// -------------------------------------------------------------------------------------------------------------------------
		/*glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -70.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		cubo.Draw(staticShader);
		glEnable(GL_BLEND);*/
		// -------------------------------------------------------------------------------------------------------------------------
		// Termina Escenario
		// -------------------------------------------------------------------------------------------------------------------------

		//-------------------------------------------------------------------------------------
		// draw skybox as last
		// -------------------
		skyboxShader.use();
		skybox.Draw(skyboxShader, view, projection, camera);

		// Limitar el framerate a 60
		deltaTime = SDL_GetTicks() - lastFrame; // time for full 1 loop
		//std::cout <<"frame time = " << frameTime << " milli sec"<< std::endl;
		if (deltaTime < LOOP_TIME)
		{
			SDL_Delay((int)(LOOP_TIME - deltaTime));
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	skybox.Terminate();

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);
	//To Configure Model
	//if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
	//	posZ++;
	//if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
	//	posZ--;
	//if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
	//	posX--;
	//if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
	//	posX++;
	//if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
	//	rotRodIzq--;
	//if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
	//	rotRodIzq++;
	//if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
	//	giroMonito--;
	//if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
	//	giroMonito++;
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		lightPosition.z++;
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		lightPosition.z--;


	/*if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		lightPosition.y++;*/

	//Car animation
	/*if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		JposZ--;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		JposZ++;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		JposX++;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		JposX--;
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
		Jgiro++;
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
		Jgiro--;
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
		printPos();*/

	//Play para la animación del jeep
	if (key == GLFW_KEY_J && action == GLFW_PRESS)
	{
		if (playJ == false && (FIJ > 1))
		{
			std::cout << "Play animation" << std::endl;
			resetElementsJ();
			//First Interpolation				
			interpolationJ();

			playJ = true;
			playIndexJ = 0;
			i_curr_steps = 0;
		}
		else
		{
			playJ = false;
			std::cout << "Not enough Key Frames" << std::endl;
		}
	}

	//Play para la animación de la cebra
	if (key == GLFW_KEY_Z && action == GLFW_PRESS)
	{
		if (playZ == false && (FIZ > 1))
		{
			std::cout << "Play animation" << std::endl;
			resetElementsZ();
			//First Interpolation				
			interpolationZ();

			playZ = true;
			playIndexZ = 0;
			i_curr_steps = 0;
		}
		else
		{
			playZ = false;
			std::cout << "Not enough Key Frames" << std::endl;
		}
	}

	//Salto Pingüino
	if (key == GLFW_KEY_Q && action == GLFW_PRESS)
	{
		recorridoPingu ^= true; 
	}

	//Restart
	if (key == GLFW_KEY_R && action == GLFW_PRESS)
	{
		movPinX = -477.0f;
		movPinY = 3.3f;
		movPinZ = -638.7f;
		movPinX1 = -573.0f,
		movPinY1 = -2.0f,
		movPinZ1 = -565.0f;
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}