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
bool recorridoPingu1 = false;
bool recorridoPingu2 = false;
bool recorridoPingu3 = false;

float	movPinX1 = -592.5f,
		movPinY1 = 0.4f,
		movPinZ1 = -554.5f, 
		movAlaDerX = -563.5f,
		movAlaDerY = 1.6f,
		movAlaDerZ = -575.5f,
		rotAlaDer = 0.0f,
		movAlaIzqX = -563.8f,
		movAlaIzqY = 1.6f,
		movAlaIzqZ = -575.5f,
		rotAlaIzq = 0.0f;

//Keyframes (Cocodrilo)
float	posCocoX = 80.0f;
float	incXcoco = 0.0f;
float	posCocoY = 0.0f;
float	incYcoco = 0.0f;
float	posCocoZ = -515.0f;
float	incZcoco = 0.0f;
float	giroCoco = 0.0f;
float	giroCocoInc = 0.0f;

////Keyframes (Manipulación y dibujo)
//float	posX = 0.0f,
//		posY = 0.0f,
//		posZ = 0.0f,
//		rotRodIzq = 0.0f,
//		giroMonito = 0.0f;
//float	incX = 0.0f,
//		incY = 0.0f,
//		incZ = 0.0f,
//		rotInc = 0.0f,
//		giroMonitoInc = 0.0f;

#define MAX_FRAMES 9
int i_max_steps = 60;
int i_curr_steps = 0;
typedef struct _frame
{
	////Variables para GUARDAR Key Frames
	//float posX;		//Variable para PosicionX
	//float posY;		//Variable para PosicionY
	//float posZ;		//Variable para PosicionZ
	//float rotRodIzq;
	//float giroMonito;

	//Variables para guardar KeyFrames Cocodrilo
	float posCocoX;
	float posCocoY;
	float posCocoZ;
	float giroCoco;

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			//introducir número en caso de tener Key guardados
bool play = false;
int playIndex = 0;

void saveFrame(void)
{
	//printf("frameindex %d\n", FrameIndex);
	std::cout << "Frame Index = " << FrameIndex << std::endl;

	/*KeyFrame[FrameIndex].posX = posX;
	KeyFrame[FrameIndex].posY = posY;
	KeyFrame[FrameIndex].posZ = posZ;

	KeyFrame[FrameIndex].rotRodIzq = rotRodIzq;
	KeyFrame[FrameIndex].giroMonito = giroMonito;*/


	KeyFrame[FrameIndex].posCocoX = posCocoX;
	KeyFrame[FrameIndex].posCocoY = posCocoY;
	KeyFrame[FrameIndex].posCocoZ = posCocoZ;
	KeyFrame[FrameIndex].giroCoco = giroCoco;



	FrameIndex++;
}

void resetElements(void)
{
	/*posX = KeyFrame[0].posX;
	posY = KeyFrame[0].posY;
	posZ = KeyFrame[0].posZ;

	rotRodIzq = KeyFrame[0].rotRodIzq;
	giroMonito = KeyFrame[0].giroMonito;*/

	posCocoX = KeyFrame[0].posCocoX;
	posCocoY = KeyFrame[0].posCocoY;
	posCocoZ = KeyFrame[0].posCocoZ;
	giroCoco = KeyFrame[0].giroCoco;
}

void interpolation(void)
{
	/*incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;

	rotInc = (KeyFrame[playIndex + 1].rotRodIzq - KeyFrame[playIndex].rotRodIzq) / i_max_steps;
	giroMonitoInc = (KeyFrame[playIndex + 1].giroMonito - KeyFrame[playIndex].giroMonito) / i_max_steps;*/

	incXcoco = (KeyFrame[playIndex + 1].posCocoX - KeyFrame[playIndex].posCocoX) / i_max_steps;
	incYcoco = (KeyFrame[playIndex + 1].posCocoY - KeyFrame[playIndex].posCocoY) / i_max_steps;
	incZcoco = (KeyFrame[playIndex + 1].posCocoZ - KeyFrame[playIndex].posCocoZ) / i_max_steps;
	giroCocoInc = (KeyFrame[playIndex + 1].giroCoco - KeyFrame[playIndex].giroCoco) / i_max_steps;

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
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				std::cout << "Animation ended" << std::endl;
				//printf("termina anim\n");
				playIndex = 0;
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
			/*posX += incX;
			posY += incY;
			posZ += incZ;

			rotRodIzq += rotInc;
			giroMonito += giroMonitoInc;*/

			posCocoX += incXcoco;
			posCocoZ += incZcoco;
			giroCoco += giroCocoInc;


			i_curr_steps++;
		}
	}

	//Vehículo
	if (animacion)
	{
		if (recorridoPingu1 == true) {
			if (movAlaDerY <= 1.6f) {
				movAlaDerY += 0.1f;
				movAlaIzqY += 0.2f;
				/*recorridoPingu1 = false;
				recorridoPingu2 = true;*/
			}
			recorridoPingu1 = false;
			recorridoPingu3 = true;
		}

		//if (recorridoPingu2 == true) {
		//	if (movAlaDerY <= 1.9f) {
		//		movAlaDerY -= 0.2f;
		//		/*recorridoPingu2 = false;
		//		recorridoPingu3 = true;*/
		//	}
		//	recorridoPingu2 = false;
		//	recorridoPingu3 = true;
		//}

		if (recorridoPingu3 == true) {
			movPinZ1 += 0.1f;
			movAlaDerZ += 0.1f;
			movAlaIzqZ += 0.1f;

			movAlaDerY += 0.2f;
			if (movPinZ1 <= -551.5f) { 
				movPinZ1 += 0.01f;
				movAlaDerZ += 0.01f;
				movAlaIzqZ += 0.01f;
			} 
			else {
				recorridoPingu3 = false;
			}
			movPinY1 = 2 * -sin(movPinZ1);
			movAlaDerY = 0.9 * -sin(-0.7+movPinZ1);
			movAlaIzqY = 0.9 * -sin(-0.7+movPinZ1);
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
	Model Agua("resources/objects/ZooBase/Sea.obj");
	Model Iglu("resources/objects/ZooBase/iglu.obj");
	Model Cebra("resources/objects/Cebra/cebra.obj");
	Model Cocodrilo("resources/objects/Cocodrilo/Cocodrilo.obj");
	Model Leon("resources/objects/Leon/Leon.obj");
	Model Oso("resources/objects/Oso/Bear.obj");
	Model Panda("resources/objects/Panda/OsoPanda.obj");
	Model Carro("resources/objects/jeep/jeep.obj");
	Model Pingu1("resources/objects/Pingu/Pingu01.obj");
	Model AlaDer("resources/objects/Pingu/Ala_der.obj");
	Model AlaIzq("resources/objects/Pingu/Ala_izq.obj");
	Model BodyPingu1("resources/objects/Pingu/BodyPinguino.obj");
	ModelAnim nina("resources/objects/Mixamo/RumbaDancing.dae");
	nina.initShaders(animShader.ID);

	

	//Inicialización de KeyFrames
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		/*KeyFrame[i].posX = 0;
		KeyFrame[i].posY = 0;
		KeyFrame[i].posZ = 0;
		KeyFrame[i].rotRodIzq = 0;
		KeyFrame[i].giroMonito = 0;*/

		KeyFrame[i].posCocoX = 0;
		KeyFrame[i].posCocoY = 0;
		KeyFrame[i].posCocoZ = 0;
		KeyFrame[i].giroCoco = 0;

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
		staticShader.setVec3("dirLight.ambient", glm::vec3(1.0f, 1.0f, 1.0f));
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
		//                                               Escenario
		// -------------------------------------------------------------------------------------------------------------------------
		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		//model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(-20.0f, -1.75f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.2f));
		//staticShader.setMat4("model", model);
		///*piso.Draw(staticShader);*/


		model = glm::translate(glm::mat4(1.0f), glm::vec3(30.0f, -1.0f, -100.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		Base.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-465.7f, 2.2f, -634.0f));
		model = glm::scale(model, glm::vec3(0.50f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		Agua.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-490.0f, 2.0f, -600.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		Iglu.Draw(staticShader);



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
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-510.0f, 0.0f, -150.0f));
		model = glm::scale(model, glm::vec3(3.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		Cebra.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Cocodilo
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::translate(glm::mat4(1.0f), glm::vec3(posCocoX, posCocoY, posCocoZ));
		model = glm::scale(model, glm::vec3(1.5f));
		model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		tmp = model = glm::rotate(model, glm::radians(giroCoco), glm::vec3(0.0f, 1.0f, 0.0));
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
		// Pingüino 1
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::translate(glm::mat4(1.0f), glm::vec3(movPinX1, movPinY1, movPinZ1));
		model = glm::scale(model, glm::vec3(2.0f));
		staticShader.setMat4("model", model);
		BodyPingu1.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(movAlaDerX, movAlaDerY, movAlaDerZ));
		model = glm::scale(model, glm::vec3(1.5f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(rotAlaDer), glm::vec3(1.0f, 0.0f, .0f));
		staticShader.setMat4("model", model);
		AlaDer.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(movAlaIzqX, movAlaIzqY, movAlaIzqZ));
		model = glm::scale(model, glm::vec3(1.5f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(rotAlaDer), glm::vec3(1.0f, 0.0f, .0f));
		staticShader.setMat4("model", model);
		AlaIzq.Draw(staticShader);

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
		// Personajes
		// -------------------------------------------------------------------------------------------------------------------------
		//model = glm::translate(glm::mat4(1.0f), glm::vec3(-40.0f, 2.0f, -50.0f)); // translate it down so it's at the center of the scene
		//model = glm::scale(model, glm::vec3(0.2f));	// it's a bit too big for our scene, so scale it down
		//animShader.setMat4("model", model);
		//nina.Draw(animShader);
		//
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
	
	//To Configure Model: Cocodrilo
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		posCocoZ++;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		posCocoZ--;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		posCocoX--;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		posCocoX++;
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		posCocoY--;
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		posCocoY++;
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
		giroCoco--;
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		giroCoco++;

	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		lightPosition.z++;
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		lightPosition.z--;


	/*if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		lightPosition.y++;*/

	//Car animation
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		animacion ^= true;
	}

	//To play KeyFrame animation 
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		if (play == false && (FrameIndex > 1))
		{
			std::cout << "Play animation" << std::endl;
			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
			std::cout << "Not enough Key Frames" << std::endl;
		}
	}

	//To Save a KeyFrame
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		if (FrameIndex < MAX_FRAMES)
		{
			saveFrame();
		}
	}

	//Salto Pingüino
	if (key == GLFW_KEY_Q && action == GLFW_PRESS)
	{
		recorridoPingu1 ^= true; 
	}

	//Restart
	if (key == GLFW_KEY_R && action == GLFW_PRESS)
	{
		movPinX1 = -592.5f;
		movPinY1 = 0.5f;
		movPinZ1 = -554.5f;
		movAlaDerX = -563.5f;
		movAlaDerY = 1.6f;
		movAlaDerZ = -575.5f;
		movAlaIzqX = -563.8f;
		movAlaIzqY = 1.6f;
		movAlaIzqZ = -575.5f;
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