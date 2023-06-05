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
		movPinZ = -638.8f;

// Elefante
float	movX = 0.0f,
		movY = 0.0f,
		movZ = 0.0f,
		rotPDelanDer = 0.0f,
		rotPDelanIzq = 0.0f,
		rotPTrasDer = 0.0f,
		rotPTrasIzq = 0.0f,
		giroElefante = 0.0f,
		movTAlta = 0.0f,
		movTMedia = 0.0f,
		movTBaja = 0.0f;

// Variables para calculo del incremento
float	inX = 0.0f,
		inY = 0.0f,
		inZ = 0.0f,
		incPDelanDer = 0.0f,
		incPDelanIzq = 0.0f,
		incPTrasDer = 0.0f,
		incPTrasIzq = 0.0f,
		incGiroElefante = 0.0f,
		incTAlta = 0.0f,
		incTMedia = 0.0f,
		incTBaja = 0.0f;


//Keyframes (Manipulación y dibujo)
float	posX = 0.0f,
		posY = 0.0f,
		posZ = 0.0f,
		rotRodIzq = 0.0f,
		giroMonito = 0.0f;
float	incX = 0.0f,
		incY = 0.0f,
		incZ = 0.0f,
		rotInc = 0.0f,
		giroMonitoInc = 0.0f;

#define MAX_FRAMES 30
int i_max_steps = 60;
int i_curr_steps = 0;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float rotRodIzq;
	float giroMonito;

	// Elefante
	float	movX;
	float	movY;
	float	movZ;
	float	rotPDelanDer;
	float	rotPDelanIzq;
	float	rotPTrasDer;
	float	rotPTrasIzq;
	float	giroElefante;
	float	movTAlta;
	float	movTMedia;
	float	movTBaja;

}FRAME_ELEFANT;

FRAME_ELEFANT KeyFrame[MAX_FRAMES];
int FrameIndexElefant = 20;			//introducir número en caso de tener Key guardados
bool play = true;
int playIndexElefant = 0;

void saveFrame(void)
{
	//printf("frameindex %d\n", FrameIndex);
	std::cout << "Frame Index = " << FrameIndexElefant << std::endl;

	// Elefante
	KeyFrame[FrameIndexElefant].movX = movX;
	KeyFrame[FrameIndexElefant].movY = movY;
	KeyFrame[FrameIndexElefant].movZ = movZ;
	KeyFrame[FrameIndexElefant].rotPDelanDer = rotPDelanDer;
	KeyFrame[FrameIndexElefant].rotPDelanIzq = rotPDelanIzq;
	KeyFrame[FrameIndexElefant].rotPTrasDer = rotPTrasDer;
	KeyFrame[FrameIndexElefant].rotPTrasIzq = rotPTrasIzq;
	KeyFrame[FrameIndexElefant].giroElefante = giroElefante;
	KeyFrame[FrameIndexElefant].movTAlta = movTAlta;
	KeyFrame[FrameIndexElefant].movTMedia = movTMedia;
	KeyFrame[FrameIndexElefant].movTBaja = movTBaja;

	FrameIndexElefant++;
}

void resetElements(void)
{
	posX = KeyFrame[0].posX;
	posY = KeyFrame[0].posY;
	posZ = KeyFrame[0].posZ;

	rotRodIzq = KeyFrame[0].rotRodIzq;
	giroMonito = KeyFrame[0].giroMonito;

	// Elefante
	movX = KeyFrame[0].movX;
	movY = KeyFrame[0].movY;
	movZ = KeyFrame[0].movZ;
	rotPDelanDer = KeyFrame[0].rotPDelanDer;
	rotPDelanIzq = KeyFrame[0].rotPDelanIzq;
	rotPTrasDer = KeyFrame[0].rotPTrasDer;
	rotPTrasIzq = KeyFrame[0].rotPTrasIzq;
	giroElefante = KeyFrame[0].giroElefante;
	movTAlta = KeyFrame[0].movTAlta;
	movTMedia = KeyFrame[0].movTMedia;
	movTBaja = KeyFrame[0].movTBaja;
}

void interpolation(void)
{
	// Elefante
	inX = (KeyFrame[playIndexElefant + 1].movX - KeyFrame[playIndexElefant].movX) / i_max_steps;
	inY = (KeyFrame[playIndexElefant + 1].movY - KeyFrame[playIndexElefant].movY) / i_max_steps;
	inZ = (KeyFrame[playIndexElefant + 1].movZ - KeyFrame[playIndexElefant].movZ) / i_max_steps;
	incPDelanDer = (KeyFrame[playIndexElefant + 1].rotPDelanDer - KeyFrame[playIndexElefant].rotPDelanDer) / i_max_steps;
	incPDelanIzq = (KeyFrame[playIndexElefant + 1].rotPDelanIzq - KeyFrame[playIndexElefant].rotPDelanIzq) / i_max_steps;
	incPTrasDer = (KeyFrame[playIndexElefant + 1].rotPTrasDer - KeyFrame[playIndexElefant].rotPTrasDer) / i_max_steps;
	incPTrasIzq = (KeyFrame[playIndexElefant + 1].rotPTrasIzq - KeyFrame[playIndexElefant].rotPTrasIzq) / i_max_steps;
	incGiroElefante = (KeyFrame[playIndexElefant + 1].giroElefante - KeyFrame[playIndexElefant].giroElefante) / i_max_steps;
	incTAlta = (KeyFrame[playIndexElefant + 1].movTAlta - KeyFrame[playIndexElefant].movTAlta) / i_max_steps;
	incTMedia = (KeyFrame[playIndexElefant + 1].movTMedia - KeyFrame[playIndexElefant].movTMedia) / i_max_steps;
	incTBaja = (KeyFrame[playIndexElefant + 1].movTBaja - KeyFrame[playIndexElefant].movTBaja) / i_max_steps;
}

void animate(void)
{
	lightPosition.x = 150.0f * cos(myVariable);
	lightPosition.z = 150.0f * sin(myVariable);
	myVariable += 0.05f;


	lightDiff.x = 1.0f * cos(myVariable);
	lightDiff.z = 1.0f * sin(myVariable);
	myvariableDiff += 0.09f;
	 

	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndexElefant++;
			if (playIndexElefant > FrameIndexElefant - 2)	//end of total animation?
			{
				std::cout << "Animation ended" << std::endl;
				//printf("termina anim\n");
				playIndexElefant = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
								  //Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation
			posX += incX;
			posY += incY;
			posZ += incZ;

			rotRodIzq += rotInc;
			giroMonito += giroMonitoInc;

			//Elefante
			movX += inX;
			movY += inY;
			movZ += inZ;
			rotPDelanDer += incPDelanDer;
			rotPDelanIzq += incPDelanIzq;
			rotPTrasDer += incPTrasDer;
			rotPTrasIzq += incPTrasIzq;
			giroElefante += incGiroElefante;
			movTAlta += incTAlta;
			movTMedia += incTMedia;
			movTBaja += incTBaja;

			i_curr_steps++;
		}
	}

	//Vehículo
	if (animacion)
	{
		//movAuto_z += 3.0f;

		if (recorridoPingu == true) {
			movPinZ += 0.1;
			if (movPinZ <= -635.0f) {
				movPinZ += 0.01f;
			}
			else {
				recorridoPingu = false;
			}
			movPinY = (- 3) * sin(movPinZ);
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
	Model Base("resources/objects/ZooBase/Base.obj");
	Model Cebra("resources/objects/Cebra/cebra.obj");
	Model Cocodrilo("resources/objects/Cocodrilo/Cocodrilo.obj");
	Model Lion("resources/objects/Leon/Lion.obj");
	Model Oso("resources/objects/Oso/Bear.obj");
	Model Panda("resources/objects/Panda/OsoPanda.obj");
	Model Carro("resources/objects/jeep/jeep.obj");
	Model Pingu("resources/objects/Pingu/Pingu.obj");

	// Elefante
	Model cuerpo_Elefante("resources/objects/Elefante/cuerpo_Elefante.obj");
	Model trompa_Alta("resources/objects/Elefante/trompa_Alta.obj");
	Model trompa_Media("resources/objects/Elefante/trompa_Media.obj");
	Model trompa_Baja("resources/objects/Elefante/trompa_Baja.obj");
	Model pata_DelanDer("resources/objects/Elefante/pata_DelanDer.obj");
	Model pata_DelanIzq("resources/objects/Elefante/pata_DelanIzq.obj");
	Model pata_TrasDer("resources/objects/Elefante/pata_TrasDer.obj");
	Model pata_TrasIzq("resources/objects/Elefante/pata_TrasIzq.obj");

	/*ModelAnim animacionPersonaje("resources/objects/Personaje1/PersonajeBrazo.dae");
	animacionPersonaje.initShaders(animShader.ID);

	ModelAnim ninja("resources/objects/ZombieWalk/ZombieWalk.dae");
	ninja.initShaders(animShader.ID);*/

	//Inicialización de KeyFrames
	
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].posX = 0;
		KeyFrame[i].posY = 0;
		KeyFrame[i].posZ = 0;
		KeyFrame[i].rotRodIzq = 0;
		KeyFrame[i].giroMonito = 0;

		// Elefante
		KeyFrame[i].movX = 0;
		KeyFrame[i].movY = 0;
		KeyFrame[i].movZ = 0;
		KeyFrame[i].rotPDelanDer = 0;
		KeyFrame[i].rotPDelanIzq = 0;
		KeyFrame[i].rotPTrasDer = 0;
		KeyFrame[i].rotPTrasIzq = 0;
		KeyFrame[i].giroElefante = 0;
		KeyFrame[i].movTAlta = 0;
		KeyFrame[i].movTMedia = 0;
		KeyFrame[i].movTBaja = 0;
	}
	

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
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.2f, 0.2f, 0.0f));
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[0].constant", 0.08f);
		staticShader.setFloat("pointLight[0].linear", 0.009f);
		staticShader.setFloat("pointLight[0].quadratic", 0.00032f);

		staticShader.setVec3("pointLight[1].position", glm::vec3(-80.0, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].ambient", glm::vec3(0.3f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].diffuse", glm::vec3(lightDiff));
		staticShader.setVec3("pointLight[1].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[1].constant", 1.0f);
		staticShader.setFloat("pointLight[1].linear", 0.009f);
		staticShader.setFloat("pointLight[1].quadratic", 0.00032f);

		staticShader.setVec3("spotLight[0].position", glm::vec3(camera.Position.x, camera.Position.y, camera.Position.z)); //Tipo reflector
		staticShader.setVec3("spotLight[0].direction", glm::vec3(camera.Front.x, camera.Front.y, camera.Front.z));
		staticShader.setVec3("spotLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("spotLight[0].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("spotLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("spotLight[0].cutOff", glm::cos(glm::radians(10.0f)));
		staticShader.setFloat("spotLight[0].outerCutOff", glm::cos(glm::radians(15.0f)));
		staticShader.setFloat("spotLight[0].constant", 0.7f);
		staticShader.setFloat("spotLight[0].linear", 0.0009f);
		staticShader.setFloat("spotLight[0].quadratic", 0.00005f);

		staticShader.setFloat("material_shininess", 32.0f);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 tmp = glm::mat4(1.0f);
		glm::mat4 tmp2 = glm::mat4(1.0f);
		glm::mat4 tmp3 = glm::mat4(1.0f);
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

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-40.3f, 1.75f, 0.3f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.2f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		/*animacionPersonaje.Draw(animShader);*/

		// -------------------------------------------------------------------------------------------------------------------------
		// Segundo Personaje Animacion
		// -------------------------------------------------------------------------------------------------------------------------
		/*
		model = glm::translate(glm::mat4(1.0f), glm::vec3(40.3f, 1.75f, 0.3f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.5f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		ninja.Draw(animShader);*/

		// -------------------------------------------------------------------------------------------------------------------------
		// Animacion del Elefante
		// -------------------------------------------------------------------------------------------------------------------------
		
		KeyFrame[0].movX = 0.000000;
		KeyFrame[0].movY = 0.000000;
		KeyFrame[0].movZ = 0.000000;
		KeyFrame[0].rotPDelanDer = 0.000000;
		KeyFrame[0].rotPDelanIzq = 0.000000;
		KeyFrame[0].rotPTrasDer = 0.000000;
		KeyFrame[0].rotPTrasIzq = 0.000000;
		KeyFrame[0].giroElefante = 0.000000;
		KeyFrame[0].movTAlta = 0.000000;
		KeyFrame[0].movTMedia = 0.000000;
		KeyFrame[0].movTBaja = 0.000000;
		
		KeyFrame[1].movX = 0.000000;
		KeyFrame[1].movY = 0.000000;
		KeyFrame[1].movZ = 0.000000;
		KeyFrame[1].rotPDelanDer = 12.000000;
		KeyFrame[1].rotPDelanIzq = -12.000000;
		KeyFrame[1].rotPTrasDer = 12.000000;
		KeyFrame[1].rotPTrasIzq = -12.000000;
		KeyFrame[1].giroElefante = -17.000000;
		KeyFrame[1].movTAlta = -15.000000;
		KeyFrame[1].movTMedia = 0.000000;
		KeyFrame[1].movTBaja = 0.000000;

		KeyFrame[2].movX = 0.000000;
		KeyFrame[2].movY = 0.000000;
		KeyFrame[2].movZ = 0.000000;
		KeyFrame[2].rotPDelanDer = -21.000000;
		KeyFrame[2].rotPDelanIzq = 21.000000;
		KeyFrame[2].rotPTrasDer = -21.000000;
		KeyFrame[2].rotPTrasIzq = 21.000000;
		KeyFrame[2].giroElefante = -76.000000;
		KeyFrame[2].movTAlta = -15.000000;
		KeyFrame[2].movTMedia = 0.000000;
		KeyFrame[2].movTBaja = 0.000000;

		KeyFrame[3].movX = -10.000000;
		KeyFrame[3].movY = 0.000000;
		KeyFrame[3].movZ = 0.000000;
		KeyFrame[3].rotPDelanDer = -51.000000;
		KeyFrame[3].rotPDelanIzq = 51.000000;
		KeyFrame[3].rotPTrasDer = -51.000000;
		KeyFrame[3].rotPTrasIzq = 51.000000;
		KeyFrame[3].giroElefante = -76.000000;
		KeyFrame[3].movTAlta = -66.000000;
		KeyFrame[3].movTMedia = 0.000000;
		KeyFrame[3].movTBaja = 0.000000;

		KeyFrame[4].movX = -21.000000;
		KeyFrame[4].movY = 0.000000;
		KeyFrame[4].movZ = 0.000000;
		KeyFrame[4].rotPDelanDer = 9.000000;
		KeyFrame[4].rotPDelanIzq = -9.000000;
		KeyFrame[4].rotPTrasDer = 9.000000;
		KeyFrame[4].rotPTrasIzq = -9.000000;
		KeyFrame[4].giroElefante = -76.000000;
		KeyFrame[4].movTAlta = -114.000000;
		KeyFrame[4].movTMedia = 0.000000;
		KeyFrame[4].movTBaja = 0.000000;

		KeyFrame[5].movX = -38.000000;
		KeyFrame[5].movY = 0.000000;
		KeyFrame[5].movZ = 0.000000;
		KeyFrame[5].rotPDelanDer = -42.000000;
		KeyFrame[5].rotPDelanIzq = 42.000000;
		KeyFrame[5].rotPTrasDer = -42.000000;
		KeyFrame[5].rotPTrasIzq = 42.000000;
		KeyFrame[5].giroElefante = -76.000000;
		KeyFrame[5].movTAlta = -114.000000;
		KeyFrame[5].movTMedia = 0.000000;
		KeyFrame[5].movTBaja = 0.000000;

		KeyFrame[6].movX = -58.000000;
		KeyFrame[6].movY = 0.000000;
		KeyFrame[6].movZ = 0.000000;
		KeyFrame[6].rotPDelanDer = 15.000000;
		KeyFrame[6].rotPDelanIzq = -15.000000;
		KeyFrame[6].rotPTrasDer = 15.000000;
		KeyFrame[6].rotPTrasIzq = -15.000000;
		KeyFrame[6].giroElefante = -48.000000;
		KeyFrame[6].movTAlta = -114.000000;
		KeyFrame[6].movTMedia = 0.000000;
		KeyFrame[6].movTBaja = 36.000000;

		KeyFrame[7].movX = -68.000000;
		KeyFrame[7].movY = 0.000000;
		KeyFrame[7].movZ = 9.000000;
		KeyFrame[7].rotPDelanDer = -33.000000;
		KeyFrame[7].rotPDelanIzq = 33.000000;
		KeyFrame[7].rotPTrasDer = -33.000000;
		KeyFrame[7].rotPTrasIzq = 33.000000;
		KeyFrame[7].giroElefante = -27.000000;
		KeyFrame[7].movTAlta = -114.000000;
		KeyFrame[7].movTMedia = 0.000000;
		KeyFrame[7].movTBaja = 36.000000;

		KeyFrame[8].movX = -68.000000;
		KeyFrame[8].movY = 0.000000;
		KeyFrame[8].movZ = 18.000000;
		KeyFrame[8].rotPDelanDer = 12.000000;
		KeyFrame[8].rotPDelanIzq = -12.000000;
		KeyFrame[8].rotPTrasDer = 12.000000;
		KeyFrame[8].rotPTrasIzq = -12.000000;
		KeyFrame[8].giroElefante = 3.000000;
		KeyFrame[8].movTAlta = -114.000000;
		KeyFrame[8].movTMedia = 0.000000;
		KeyFrame[8].movTBaja = 36.000000;

		KeyFrame[9].movX = -68.000000;
		KeyFrame[9].movY = 0.000000;
		KeyFrame[9].movZ = 18.000000;
		KeyFrame[9].rotPDelanDer = 12.000000;
		KeyFrame[9].rotPDelanIzq = -12.000000;
		KeyFrame[9].rotPTrasDer = 12.000000;
		KeyFrame[9].rotPTrasIzq = -12.000000;
		KeyFrame[9].giroElefante = 23.000000;
		KeyFrame[9].movTAlta = -114.000000;
		KeyFrame[9].movTMedia = 0.000000;
		KeyFrame[9].movTBaja = -39.000000;

		KeyFrame[10].movX = -55.000000;
		KeyFrame[10].movY = -3.000000;
		KeyFrame[10].movZ = 22.000000;
		KeyFrame[10].rotPDelanDer = -27.000000;
		KeyFrame[10].rotPDelanIzq = 27.000000;
		KeyFrame[10].rotPTrasDer = -27.000000;
		KeyFrame[10].rotPTrasIzq = 27.000000;
		KeyFrame[10].giroElefante = 52.000000;
		KeyFrame[10].movTAlta = -114.000000;
		KeyFrame[10].movTMedia = 0.000000;
		KeyFrame[10].movTBaja = -39.000000;

		KeyFrame[11].movX = -55.000000;
		KeyFrame[11].movY = -3.000000;
		KeyFrame[11].movZ = 22.000000;
		KeyFrame[11].rotPDelanDer = -51.000000;
		KeyFrame[11].rotPDelanIzq = 51.000000;
		KeyFrame[11].rotPTrasDer = -51.000000;
		KeyFrame[11].rotPTrasIzq = 51.000000;
		KeyFrame[11].giroElefante = 71.000000;
		KeyFrame[11].movTAlta = -57.000000;
		KeyFrame[11].movTMedia = 0.000000;
		KeyFrame[11].movTBaja = -39.000000;

		KeyFrame[12].movX = -33.000000;
		KeyFrame[12].movY = -3.000000;
		KeyFrame[12].movZ = 22.000000;
		KeyFrame[12].rotPDelanDer = 9.000000;
		KeyFrame[12].rotPDelanIzq = -9.000000;
		KeyFrame[12].rotPTrasDer = 9.000000;
		KeyFrame[12].rotPTrasIzq = -9.000000;
		KeyFrame[12].giroElefante = 71.000000;
		KeyFrame[12].movTAlta = 3.000000;
		KeyFrame[12].movTMedia = 0.000000;
		KeyFrame[12].movTBaja = -39.000000;

		KeyFrame[13].movX = -10.000000;
		KeyFrame[13].movY = -5.000000;
		KeyFrame[13].movZ = 22.000000;
		KeyFrame[13].rotPDelanDer = -42.000000;
		KeyFrame[13].rotPDelanIzq = 42.000000;
		KeyFrame[13].rotPTrasDer = -42.000000;
		KeyFrame[13].rotPTrasIzq = 42.000000;
		KeyFrame[13].giroElefante = 82.000000;
		KeyFrame[13].movTAlta = 3.000000;
		KeyFrame[13].movTMedia = 0.000000;
		KeyFrame[13].movTBaja = -39.000000;

		KeyFrame[14].movX = -4.000000;
		KeyFrame[14].movY = -5.000000;
		KeyFrame[14].movZ = 22.000000;
		KeyFrame[14].rotPDelanDer = 6.000000;
		KeyFrame[14].rotPDelanIzq = -6.000000;
		KeyFrame[14].rotPTrasDer = 6.000000;
		KeyFrame[14].rotPTrasIzq = -6.000000;
		KeyFrame[14].giroElefante = 82.000000;
		KeyFrame[14].movTAlta = 3.000000;
		KeyFrame[14].movTMedia = 0.000000;
		KeyFrame[14].movTBaja = 72.000000;

		KeyFrame[15].movX = 23.000000;
		KeyFrame[15].movY = -8.000000;
		KeyFrame[15].movZ = 22.000000;
		KeyFrame[15].rotPDelanDer = -45.000000;
		KeyFrame[15].rotPDelanIzq = 45.000000;
		KeyFrame[15].rotPTrasDer = -45.000000;
		KeyFrame[15].rotPTrasIzq = 45.000000;
		KeyFrame[15].giroElefante = 82.000000;
		KeyFrame[15].movTAlta = -51.000000;
		KeyFrame[15].movTMedia = 0.000000;
		KeyFrame[15].movTBaja = 72.000000;

		KeyFrame[16].movX = 48.000000;
		KeyFrame[16].movY = -10.000000;
		KeyFrame[16].movZ = 22.000000;
		KeyFrame[16].rotPDelanDer = 12.000000;
		KeyFrame[16].rotPDelanIzq = -12.000000;
		KeyFrame[16].rotPTrasDer = 12.000000;
		KeyFrame[16].rotPTrasIzq = -12.000000;
		KeyFrame[16].giroElefante = 82.000000;
		KeyFrame[16].movTAlta = -51.000000;
		KeyFrame[16].movTMedia = 0.000000;
		KeyFrame[16].movTBaja = 72.000000;

		KeyFrame[17].movX = 64.000000;
		KeyFrame[17].movY = -13.000000;
		KeyFrame[17].movZ = 11.000000;
		KeyFrame[17].rotPDelanDer = -27.000000;
		KeyFrame[17].rotPDelanIzq = 27.000000;
		KeyFrame[17].rotPTrasDer = -27.000000;
		KeyFrame[17].rotPTrasIzq = 27.000000;
		KeyFrame[17].giroElefante = 114.000000;
		KeyFrame[17].movTAlta = -93.000000;
		KeyFrame[17].movTMedia = 0.000000;
		KeyFrame[17].movTBaja = 72.000000;

		KeyFrame[18].movX = 79.000000;
		KeyFrame[18].movY = -13.000000;
		KeyFrame[18].movZ = -7.000000;
		KeyFrame[18].rotPDelanDer = 9.000000;
		KeyFrame[18].rotPDelanIzq = -9.000000;
		KeyFrame[18].rotPTrasDer = 9.000000;
		KeyFrame[18].rotPTrasIzq = -9.000000;
		KeyFrame[18].giroElefante = 127.000000;
		KeyFrame[18].movTAlta = -93.000000;
		KeyFrame[18].movTMedia = 0.000000;
		KeyFrame[18].movTBaja = -48.000000;

		KeyFrame[19].movX = 0.000000;
		KeyFrame[19].movY = 0.000000;
		KeyFrame[19].movZ = 0.000000;
		KeyFrame[19].rotPDelanDer = 0.000000;
		KeyFrame[19].rotPDelanIzq = 0.000000;
		KeyFrame[19].rotPTrasDer = 0.000000;
		KeyFrame[19].rotPTrasIzq = 0.000000;
		KeyFrame[19].giroElefante = 0.000000;
		KeyFrame[19].movTAlta = 0.000000;
		KeyFrame[19].movTMedia = 0.000000;
		KeyFrame[19].movTBaja = 0.000000;

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

		// Cuerpo
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-150.0f, 1.0f, -500.0f));
		model = glm::translate(model, glm::vec3(movX, movY, movZ));
		tmp = model = glm::rotate(model, glm::radians(giroElefante), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(8.0f));
		staticShader.setMat4("model", model);
		cuerpo_Elefante.Draw(staticShader);
		
		// Trompa Alta
		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		model = glm::translate(model, glm::vec3(-21.531f, 13.464f, 155.143f));
		tmp2 = model = glm::rotate(model, glm::radians(movTAlta), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		trompa_Alta.Draw(staticShader);

		// Trompa Media
		model = glm::translate(tmp2, glm::vec3(2.5f, -20.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		tmp3 = model = glm::rotate(model, glm::radians(movTMedia), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		trompa_Media.Draw(staticShader);

		// Trompa Baja
		model = glm::translate(tmp3, glm::vec3(0.0f, -52.0f, 12.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(movTBaja), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		trompa_Baja.Draw(staticShader);

		// Pata Delantera Derecha
		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		model = glm::translate(model, glm::vec3(-55.719f, -17.632f, 28.851f));
		model = glm::rotate(model, glm::radians(rotPDelanDer), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		pata_DelanDer.Draw(staticShader);

		// Pata Delantera Izquierda
		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		model = glm::translate(model, glm::vec3(-1.414f, -15.759f, 64.175f));
		model = glm::rotate(model, glm::radians(rotPDelanIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		pata_DelanIzq.Draw(staticShader);

		// Pata Trasera Derecha
		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		model = glm::translate(model, glm::vec3(-43.287f, -45.054f, -85.047f));
		model = glm::rotate(model, glm::radians(rotPTrasDer), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		pata_TrasDer.Draw(staticShader);

		// Pata Trasera Izquierda
		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		model = glm::translate(model, glm::vec3(-5.863f, -52.915f, -64.431f));
		model = glm::rotate(model, glm::radians(rotPTrasIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		pata_TrasIzq.Draw(staticShader);

		
		// -------------------------------------------------------------------------------------------------------------------------
		// Cebra
		// -------------------------------------------------------------------------------------------------------------------------
		//model = glm::translate(glm::mat4(1.0f), glm::vec3(-500.0f, 0.0f, -150.0f));
		/*model = glm::rotate();*/
		/*model = glm::scale(model, glm::vec3(3.0f));
		staticShader.setMat4("model", model);
		Cebra.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Cocodilo
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::translate(glm::mat4(1.0f), glm::vec3(80.0f, -2.0f, -510.0f));
		model = glm::scale(model, glm::vec3(1.5f));
		staticShader.setMat4("model", model);
		Cocodrilo.Draw(staticShader);*/

		// -------------------------------------------------------------------------------------------------------------------------
		// León
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-40.0f, -10.0f, -275.0f));
		model = glm::scale(model, glm::vec3(2.0f));
		staticShader.setMat4("model", model);
		Lion.Draw(staticShader);
		
		// -------------------------------------------------------------------------------------------------------------------------
		// Panda
		// -------------------------------------------------------------------------------------------------------------------------
		//model = glm::translate(glm::mat4(1.0f), glm::vec3(-800.0f, 10.0f, -300.0f));
		/*model = glm::rotate();*/
		//model = glm::scale(model, glm::vec3(7.0f));
		//staticShader.setMat4("model", model);
		//Panda.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Pingüino 1
		// -------------------------------------------------------------------------------------------------------------------------
		//model = glm::translate(glm::mat4(1.0f), glm::vec3(movPinX, movPinY, movPinZ));
		/*model = glm::rotate();*/
		/*model = glm::scale(model, glm::vec3(1.5f));
		staticShader.setMat4("model", model);
		Pingu.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		//                                                 Carro
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::translate(glm::mat4(1.0f), glm::vec3(30.0f, 6.0f, -150.0f));
		model = glm::scale(model, glm::vec3(0.7f));
		staticShader.setMat4("model", model);
		Carro.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Carro
		// -------------------------------------------------------------------------------------------------------------------------
		//model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::translate(model, glm::vec3(15.0f + movAuto_x, -1.0f, movAuto_z));
		//tmp = model = glm::rotate(model, glm::radians(orienta), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		//staticShader.setMat4("model", model);
		//carro.Draw(staticShader);

		//model = glm::translate(tmp, glm::vec3(8.5f, 2.5f, 12.9f));
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		//staticShader.setMat4("model", model);
		//llanta.Draw(staticShader);	//Izq delantera

		//model = glm::translate(tmp, glm::vec3(-8.5f, 2.5f, 12.9f));
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//staticShader.setMat4("model", model);
		//llanta.Draw(staticShader);	//Der delantera

		//model = glm::translate(tmp, glm::vec3(-8.5f, 2.5f, -14.5f));
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//staticShader.setMat4("model", model);
		//llanta.Draw(staticShader);	//Der trasera

		//model = glm::translate(tmp, glm::vec3(8.5f, 2.5f, -14.5f));
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		//staticShader.setMat4("model", model);
		//llanta.Draw(staticShader);	//Izq trase
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
		//cabeza.Draw(staticShader);*/
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
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		posZ++;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		posZ--;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		posX--;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		posX++;
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		rotRodIzq--;
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		rotRodIzq++;
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
		giroMonito--;
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		giroMonito++;
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		lightPosition.z++;
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		lightPosition.z--;

	// Elefante
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		movX--;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		movX++;
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		movY++;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		movY--;
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
		movZ++;
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		movZ--;

	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
		giroElefante--;
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		giroElefante++;
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		rotPDelanDer -= 3.0f;
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		rotPDelanDer += 3.0f;
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		rotPDelanIzq += 3.0f;
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		rotPDelanIzq -= 3.0f;
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		rotPTrasDer += 3.0f;
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		rotPTrasDer -= 3.0f;
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		rotPTrasIzq -= 3.0f;
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		rotPTrasIzq += 3.0f;
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		movTAlta += 3.0f;
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		movTAlta -= 3.0f;
	if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
		movTMedia += 3.0f;
	if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
		movTMedia -= 3.0f;
	if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
		movTBaja += 3.0f;
	if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
		movTBaja -= 3.0f;

	/*if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		lightPosition.y++;*/

	//Car animation
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		animacion ^= true;
	}

	//To play KeyFrame animation 
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		if (play == false && (FrameIndexElefant > 1))
		{
			std::cout << "Play animation" << std::endl;
			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndexElefant = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
			std::cout << "Not enough Key Frames" << std::endl;
		}
		/*
		for (int i = 0; i < FrameIndexElefant; i++)
		{
			printf("KeyFrame[i].movX = %f;\n", KeyFrame[i].movX);
			printf("KeyFrame[i].movY = %f;\n", KeyFrame[i].movY);
			printf("KeyFrame[i].movZ = %f;\n", KeyFrame[i].movZ);
			printf("KeyFrame[i].rotPDelanDer = %f;\n", KeyFrame[i].rotPDelanDer);
			printf("KeyFrame[i].rotPDelanIzq = %f;\n", KeyFrame[i].rotPDelanIzq);
			printf("KeyFrame[i].rotPTrasDer = %f;\n", KeyFrame[i].rotPTrasDer);
			printf("KeyFrame[i].rotPTrasIzq = %f;\n", KeyFrame[i].rotPTrasIzq);
			printf("KeyFrame[i].giroElefante = %f;\n", KeyFrame[i].giroElefante);
			printf("KeyFrame[i].movTAlta = %f;\n", KeyFrame[i].movTAlta);
			printf("KeyFrame[i].movTMedia = %f;\n", KeyFrame[i].movTMedia);
			printf("KeyFrame[i].movTBaja = %f;\n", KeyFrame[i].movTBaja);
		}
		*/
	}

	//To Save a KeyFrame
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		if (FrameIndexElefant < MAX_FRAMES)
		{
			saveFrame();
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