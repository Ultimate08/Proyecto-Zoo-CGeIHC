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

// posiciones
bool	animacion = true;

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

//Variables Cocodrilo
ifstream AnimCoco;
string strC;
int indC = 0;
//Keyframes (Cocodrilo)
float	posCocoX = 110.0f;
float	incXcoco = 0.0f;
float	posCocoY = -13.0f;
float	incYcoco = 0.0f;
float	posCocoZ = -515.0f;
float	incZcoco = 0.0f;
float	giroCoco = 0.0f;
float	giroCocoInc = 0.0f;

//Variables recorrido de la mujer en la fuente
float	WposX = -455.0f,
		WposZ = -270.0f,
		Wgiro = 180.0f,
		WDposX = -100.0f,
		WDgiro = 270.0f;
int		Wreco = 0,
		camina = 0;
bool	Wanimation = false;

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

#define MAX_FRAMES 30
#define MAX_FRAMESZ 7
#define MAX_FRAMESJ 36
#define MAX_FRAMES_C 20

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

typedef struct _frameC
{	//Variables para guardar KeyFrames Cocodrilo
	float posCocoX;
	float posCocoY;
	float posCocoZ;
	float giroCoco;

}FRAME_COCODRILO;

FRAME_COCODRILO KFC[MAX_FRAMES_C];
int FrameIndexCocodrilo = 0;			//introducir número en caso de tener Key guardados
bool playC = false;
int playIndex = 0;

void saveFrameC(void)
{
	//printf("frameindex %d\n", FrameIndex);
	std::cout << "Frame Index = " << FrameIndexCocodrilo << std::endl;

	KFC[FrameIndexCocodrilo].posCocoX = posCocoX;
	KFC[FrameIndexCocodrilo].posCocoY = posCocoY;
	KFC[FrameIndexCocodrilo].posCocoZ = posCocoZ;
	KFC[FrameIndexCocodrilo].giroCoco = giroCoco;

	FrameIndexCocodrilo++;
}

void resetElementsC(void)
{
	posCocoX = KFC[0].posCocoX;
	posCocoY = KFC[0].posCocoY;
	posCocoZ = KFC[0].posCocoZ;
	giroCoco = KFC[0].giroCoco;
}

void interpolationC(void)
{
	incXcoco = (KFC[playIndex + 1].posCocoX - KFC[playIndex].posCocoX) / i_max_steps;
	incYcoco = (KFC[playIndex + 1].posCocoY - KFC[playIndex].posCocoY) / i_max_steps;
	incZcoco = (KFC[playIndex + 1].posCocoZ - KFC[playIndex].posCocoZ) / i_max_steps;
	giroCocoInc = (KFC[playIndex + 1].giroCoco - KFC[playIndex].giroCoco) / i_max_steps;

}

typedef struct _frameE
{
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
bool playE = true;
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
	if (camina == 1 && WDposX == -100) {
		camina = 0;
		WDgiro = 270.0f;
	}else if (camina == 0 && WDposX > -300) {
		WDposX -= 0.25f;
	} else if (camina == 0 && WDposX == -300) {
		camina = 1;
		WDgiro = 90.0f;
	} else if (camina == 1 && WDposX < -100) {
		WDposX += 0.25f;
	}



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

	//Cocodrilo
	if (playC)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndexCocodrilo - 2)	//end of total animation?
			{
				std::cout << "Animation ended" << std::endl;
				//printf("termina anim\n");
				playIndex = 0;
				playC = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
				//Interpolation
				interpolationC();
			}
		}
		else
		{	//Draw animation
			posCocoX += incXcoco;
			posCocoY += incYcoco;
			posCocoZ += incZcoco;
			giroCoco += giroCocoInc;


			i_curr_steps++;
		}
	}

	//Pinguino
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
			movAlaDerY = 0.9 * -sin(-0.7 + movPinZ1);
			movAlaIzqY = 0.9 * -sin(-0.7 + movPinZ1);
		}
	}

	//Elefante
	if (playE)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndexElefant++;
			if (playIndexElefant > FrameIndexElefant - 2)	//end of total animation?
			{
				std::cout << "Animation ended" << std::endl;
				//printf("termina anim\n");
				playIndexElefant = 0;
				playE = false;
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


	//Mujer
	if (Wanimation) {
		if (Wreco == 0 && WposZ > -390.0f) {
			WposZ -= 0.5f;
		}else if (WposZ == -390.0f && Wreco == 0) {
			Wreco = 1;
			Wgiro = 90.0f;
		}else if (Wreco == 1 && WposX < -335.0f) {
			WposX += 0.5f;
		}else if (WposX == -335.0f && Wreco == 1) {
			Wreco = 2;
			Wgiro = 0.0f;
		}else if (Wreco == 2 && WposZ < -270.0f) {
			WposZ += 0.5f;
		}else if (WposZ == -270.0f && Wreco == 2) {
			Wreco = 3;
			Wgiro = 270.0f;
		}else if (Wreco == 3 && WposX > -455.0f) {
			WposX -= 0.5f;
		}else if (WposX == -455.0f && Wreco == 3) {
			Wreco = 0;
			Wgiro = 180.0f;
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
	Model Agua("resources/objects/ZooBase/Sea.obj");
	Model Iglu("resources/objects/ZooBase/iglu.obj");
	Model Cocodrilo("resources/objects/Cocodrilo/Cocodrilo.obj");
	Model Panda("resources/objects/Panda/OsoPanda.obj");
	Model boy6("resources/objects/Eduardo/Kids/Boy6.obj");
	Model girl6("resources/objects/Eduardo/Kids/girl6.obj");
	Model chico("resources/objects/Eduardo/Kids/chico.obj");
	Model chica("resources/objects/Eduardo/Kids/chica.obj");
	Model man("resources//objects/Eduardo/Man/ManCasual3.obj");
	Model man2("resources//objects/Eduardo/Man/hombre.obj");
	Model woman("resources/objects/Eduardo/Woman/mujer.obj");

	// Carga de modelos para el pinguino
	Model Pingu1("resources/objects/Pingu/Pingu01.obj");
	Model AlaDer("resources/objects/Pingu/Ala_der.obj");
	Model AlaIzq("resources/objects/Pingu/Ala_izq.obj");
	Model BodyPingu1("resources/objects/Pingu/BodyPinguino.obj");

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

	// Elefante
	Model cuerpo_Elefante("resources/objects/Elefante/cuerpo_Elefante.obj");
	Model trompa_Alta("resources/objects/Elefante/trompa_Alta.obj");
	Model trompa_Media("resources/objects/Elefante/trompa_Media.obj");
	Model trompa_Baja("resources/objects/Elefante/trompa_Baja.obj");
	Model pata_DelanDer("resources/objects/Elefante/pata_DelanDer.obj");
	Model pata_DelanIzq("resources/objects/Elefante/pata_DelanIzq.obj");
	Model pata_TrasDer("resources/objects/Elefante/pata_TrasDer.obj");
	Model pata_TrasIzq("resources/objects/Elefante/pata_TrasIzq.obj");

	// Carga de modelos para el jeep
	Model jeep("resources/objects/Jeep/Chasis.obj");
	Model llanta("resources/objects/Jeep/Llanta.obj");
	
	ModelAnim Lion("resources/objects/Leon/Lion.fbx");
	Lion.initShaders(animShader.ID);

	ModelAnim womanC("resources/objects/Eduardo/Woman/WomanCasual/Walking.dae");
	womanC.initShaders(animShader.ID);

	ModelAnim womanD("resources/objects/Eduardo/Woman/WomanDress/Walking.dae");
	womanD.initShaders(animShader.ID);

	ModelAnim nina("resources/objects/Mixamo/RumbaDancing.dae");
	nina.initShaders(animShader.ID);

	//Inicialización de KeyFrames del leon

	for (int i = 0; i < MAX_FRAMES; i++)
	{
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

	//Inicialización de KeyFrames para la animación del cocodrilo
	AnimCoco.open("Animaciones/AnimCocodrilo.txt", ios::in);
	//SaveFrame Cocodrilo
	while (indC < MAX_FRAMES_C && !AnimCoco.eof()) {
		getline(AnimCoco, strC);
		posCocoX = stof(strC);
		getline(AnimCoco, strC);
		posCocoY = stof(strC);
		getline(AnimCoco, strC);
		posCocoZ = stof(strC);
		getline(AnimCoco, strC);
		giroCoco = stof(strC);

		saveFrameC();

		indC++;
	}
	AnimCoco.close();

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
		glm::mat4 tmp2 = glm::mat4(1.0f);
		glm::mat4 tmp3 = glm::mat4(1.0f);
		glm::mat4 tmpPingu = glm::mat4(1.0f);

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
		// Animacion Leon
		// -------------------------------------------------------------------------------------------------------------------------

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-70.0f, -40.0f, -320.0f));
		model = glm::scale(model, glm::vec3(0.02f));
		animShader.setMat4("model", model);
		Lion.Draw(animShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Mujer de Vestido
		// -------------------------------------------------------------------------------------------------------------------------

		model = glm::translate(glm::mat4(1.0f), glm::vec3(WDposX, 0.05f, -200.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.075f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(WDgiro), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		womanD.Draw(animShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Mujer Casual
		// -------------------------------------------------------------------------------------------------------------------------

		model = glm::translate(glm::mat4(1.0f), glm::vec3(WposX, 0.05f, WposZ)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.075f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(Wgiro), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		womanC.Draw(animShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Niña Mixamo
		// -------------------------------------------------------------------------------------------------------------------------

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-400.0f, 0.05f, -310.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.05f));	// it's a bit too big for our scene, so scale it down
		animShader.setMat4("model", model);
		nina.Draw(animShader);

		// -------------------------------------------------------------------------------------------------------------------------
		//                                               Escenario
		// -------------------------------------------------------------------------------------------------------------------------
		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

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
		// Cocodilo
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::translate(glm::mat4(1.0f), glm::vec3(posCocoX, posCocoY, posCocoZ));
		model = glm::scale(model, glm::vec3(1.5f));
		model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		tmp = model = glm::rotate(model, glm::radians(giroCoco), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		Cocodrilo.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Panda
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-800.0f, 10.0f, -300.0f));
		/*model = glm::rotate();*/
		model = glm::scale(model, glm::vec3(7.0f));
		staticShader.setMat4("model", model);
		Panda.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Personas
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, -1.0f, -575.0f));
		model = glm::rotate(model, glm::radians(315.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.15f));
		staticShader.setMat4("model", model);
		man.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, -1.0f, -190.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.15f));
		staticShader.setMat4("model", model);
		man2.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-150.0f, -1.0f, -580.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.15f));
		staticShader.setMat4("model", model);
		woman.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Niños
		// -------------------------------------------------------------------------------------------------------------------------
		
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, -580.0f));
		model = glm::rotate(model, glm::radians(315.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.15f));
		staticShader.setMat4("model", model);
		boy6.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(50.0f, -1.0f, -550.0f));
		model = glm::rotate(model, glm::radians(60.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.15f));
		staticShader.setMat4("model", model);
		girl6.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-530.0f, -1.0f, -250.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.15f));
		staticShader.setMat4("model", model);
		chico.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-730.0f, -1.0f, -350.0f));
		model = glm::rotate(model, glm::radians(300.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.15f));
		staticShader.setMat4("model", model);
		chica.Draw(staticShader);


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
	
	//To play KeyFrame animation cocodrilo
	if (key == GLFW_KEY_C && action == GLFW_PRESS)
	{
		if (playC == false && (FrameIndexCocodrilo > 1))
		{
			std::cout << "Play animation" << std::endl;
			resetElementsC();
			//First Interpolation				
			interpolationC();

			playC = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			playC = false;
			std::cout << "Not enough Key Frames" << std::endl;
		}
	}

	//To play KeyFrame animation elefante
	if (key == GLFW_KEY_E && action == GLFW_PRESS)
	{
		if (playE == false && (FrameIndexElefant > 1))
		{
			std::cout << "Play animation" << std::endl;
			resetElements();
			//First Interpolation				
			interpolation();

			playE = true;
			playIndexElefant = 0;
			i_curr_steps = 0;
		}
		else
		{
			playE = false;
			std::cout << "Not enough Key Frames" << std::endl;
		}
		/* Esta parte del codigo es la usada para ver los KeyFrames en consola
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

	//Play para la animación de la mujer caminando
	if (key == GLFW_KEY_M && action == GLFW_PRESS)
		Wanimation ^= true;


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