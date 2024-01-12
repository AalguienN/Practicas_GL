/***************************************************
*	Interaccion.cpp
* 
*	P6 y P7
*
*	Captura de eventos de usuario
*
*	@author	Adrián Camacho García' <acamgar1@etsinf.upv.es>
*	@date Oct,2023
*
***************************************************/
#define PROYECTO "Interaccion"

#include <iostream>	
#include <string>	

#include "codebase2.h"

#include "Asteroide.h"
#include "Blaster.h"
#include "globales.h"
#include "Explosion.h"

#include "audiere.h"


using namespace std;
using namespace cb2;
//using namespace blasterNS;

//Globales ---------------------------------------------------------------------------
static int windowWidth;
static int windowHeight;

//Listas de dibujo -------------------------------------------------------------------
static GLuint suelo, asteroide, plano, nave;

//Plataforma -------------------------------------------------------------------------
const int DIM_PLATAFORMA = 100;
const int VERTICES_POR_UNIDAD = 4;

static Sistema3d primeraPersona;
static Sistema3d cabinaPrimeraPersona;

static Sistema3d terceraPersona3d;
static Sistema3d terceraPersonaNave3d;

//Nave / Player ----------------------------------------------------------------------
Sistema3d player;
static float z0 = 1;					//Altura inicial
float speed_Player;				//Velocidad de la nave
static const float ACCELERATION = 0.5;	//Aceleración de la nave
static const float MAX_SPEED = 100;		//Velocidad máxima nave (delante y marcha atrás)
float offset_girox = 0; float offset_giroy = 0;		//  

static bool mostrarCabina = true;
static bool luces = true;

static float girox, giroy;

static int xanterior;
static int yanterior;

static bool disparando = false;
static const float CADENCIA_DISPARO = 500;

enum tipoCamara { basica = 0, primera_persona = 1, tercera_persona = 2, cinematica = 3};

static tipoCamara camaraActual = basica;

//Para la cámara cinematica
float distAsteroideCamara = INFINITY;
int camAstActual = 0;

//Texturas ----------------------------------------------------------------------------
static GLuint metal, roca, fondo, estrella, interiorNave, exteriorNave, sofa,
	suelo_techo_nave, Ventana_Nave_Exterior, lado_nave_1, lado_nave_2, reflejo_fondo;

enum tipoAsteroide
{
	senoidal, aleatorio
};

static const tipoAsteroide tipoA = senoidal;

static Vec3 verticesAsteroide[(RES_ASTEROIDE+2) * (RES_ASTEROIDE+2)];

static int blasterActual = 0;

Asteroide asteroides[NUM_ASTEROIDES];
Blaster blasters[NUM_BLASTERS];
Explosion explosiones[NUM_EXPLOSIONES];

//void cubemap(GLfloat posicionEstrella[3], GLuint textura, bool dibujarEstrella);

//AUDIERE:
audiere::AudioDevicePtr device;

audiere::OutputStreamPtr musica;
audiere::OutputStreamPtr disparo;

// Funcion de inicializacion propia
void init()
{
	cout << glGetString(GL_VERSION) << endl;

	srand(time(0)); //Inicialización de la semilla aleatoria
	
	#pragma region Luces

	static const GLfloat Ambient[4] = { 0.2,0.2,0.2,1 };

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, Ambient);

	glLightfv(GL_LIGHT0, GL_AMBIENT, GRISOSCURO);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, GRISCLARO);
	glLightfv(GL_LIGHT0, GL_SPECULAR, GRISCLARO);	
	
	glLightfv(GL_LIGHT1, GL_AMBIENT, NEGRO);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, BLANCO);
	glLightfv(GL_LIGHT1, GL_SPECULAR, BLANCO);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 20.0f);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 20.0f);


	glLightfv(GL_LIGHT2, GL_AMBIENT, NEGRO);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, BLANCO);
	glLightfv(GL_LIGHT2, GL_SPECULAR, BLANCO);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 20.0f);
	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 20.0f);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);

	// BLASTERS
	glLightfv(GL_LIGHT3, GL_AMBIENT, NEGRO);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, GRISCLARO);
	glLightfv(GL_LIGHT3, GL_SPECULAR, GRISCLARO);

	glLightfv(GL_LIGHT4, GL_AMBIENT, NEGRO);
	glLightfv(GL_LIGHT4, GL_DIFFUSE, GRISCLARO);
	glLightfv(GL_LIGHT4, GL_SPECULAR, GRISCLARO);

	glLightfv(GL_LIGHT5, GL_AMBIENT, NEGRO);
	glLightfv(GL_LIGHT5, GL_DIFFUSE, GRISCLARO);
	glLightfv(GL_LIGHT5, GL_SPECULAR, GRISCLARO);

	glLightfv(GL_LIGHT6, GL_AMBIENT, NEGRO);
	glLightfv(GL_LIGHT6, GL_DIFFUSE, GRISCLARO);
	glLightfv(GL_LIGHT6, GL_SPECULAR, GRISCLARO);

	//glEnable(GL_LIGHT3);
	//glEnable(GL_LIGHT4);
	//glEnable(GL_LIGHT5);
	//glEnable(GL_LIGHT6);
	#pragma endregion

	#pragma region Texturas
	//Texturas
	glGenTextures(1, &metal);
	glBindTexture(GL_TEXTURE_2D, metal);
	loadImageFile((char*) "metal_2.jpg");
	glGenTextures(1, &roca);
	glBindTexture(GL_TEXTURE_2D, roca);
	loadImageFile((char*)"roca.jpg");

	glGenTextures(1, &fondo);
	glBindTexture(GL_TEXTURE_2D, fondo);
	loadImageFile((char*)"fondo.jpg");

	glGenTextures(1, &estrella);
	glBindTexture(GL_TEXTURE_2D, estrella);
	loadImageFile((char*)"estrella.jpg");

	glGenTextures(1, &interiorNave);
	glBindTexture(GL_TEXTURE_2D, interiorNave);
	loadImageFile((char*)"interior_panoramica.png");

	glGenTextures(1, &exteriorNave);
	glBindTexture(GL_TEXTURE_2D, exteriorNave);
	loadImageFile((char*)"exterior_nave.png");

	glGenTextures(1, &sofa);
	glBindTexture(GL_TEXTURE_2D, sofa);
	loadImageFile((char*)"sofa.jpg");

	glGenTextures(1, &suelo_techo_nave);
	glBindTexture(GL_TEXTURE_2D, suelo_techo_nave);
	loadImageFile((char*)"suelo_techo_nave.png");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glGenTextures(1, &Ventana_Nave_Exterior);
	glBindTexture(GL_TEXTURE_2D, Ventana_Nave_Exterior);
	loadImageFile((char*)"Ventana_Nave_Exterior.png");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glGenTextures(1, &lado_nave_1);
	glBindTexture(GL_TEXTURE_2D, lado_nave_1);
	loadImageFile((char*)"lado_nave_1.png");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glGenTextures(1, &lado_nave_2);
	glBindTexture(GL_TEXTURE_2D, lado_nave_2);
	loadImageFile((char*)"lado_nave_2.png");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glGenTextures(1, &reflejo_fondo);
	glBindTexture(GL_TEXTURE_2D, reflejo_fondo);
	loadImageFile((char*)"reflejo_fondo.jpg");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	

	#pragma endregion

	#pragma region Enable
	glEnable(GL_DEPTH_TEST);	//Profundidad
	glEnable(GL_LIGHTING);		//Iluminación
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_TEXTURE_2D);
	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);

	//Blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	#pragma endregion

	#pragma region suelo lista

	suelo = glGenLists(1);
	glNewList(suelo, GL_COMPILE);
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	//glColor3f(1, 1, 1);

	glBindTexture(GL_TEXTURE_2D, metal);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	//glTexEnvi(GL_TEX_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	for (int i = 0; i < DIM_PLATAFORMA * VERTICES_POR_UNIDAD - 1; i++) {
		glBegin(GL_TRIANGLE_STRIP);
		glNormal3f(0, 0, 1);
		for (int j = 0; j < DIM_PLATAFORMA * VERTICES_POR_UNIDAD; j++) {
			glTexCoord2f(float(i) / 8 / float(VERTICES_POR_UNIDAD), float(j) / 8 / float(VERTICES_POR_UNIDAD));
			glVertex3f((i- DIM_PLATAFORMA * VERTICES_POR_UNIDAD /2)/ float(VERTICES_POR_UNIDAD), (j - DIM_PLATAFORMA * VERTICES_POR_UNIDAD / 2) / float(VERTICES_POR_UNIDAD), 0);
			glTexCoord2f(float(i+1) / 8 / float(VERTICES_POR_UNIDAD), float(j) / 8 / float(VERTICES_POR_UNIDAD));
			glVertex3f((i + 1 - DIM_PLATAFORMA * VERTICES_POR_UNIDAD / 2) / float(VERTICES_POR_UNIDAD), (j - DIM_PLATAFORMA * VERTICES_POR_UNIDAD / 2) / float(VERTICES_POR_UNIDAD), 0);
		}
		glEnd();
	}


	glPopAttrib();
	glEndList();

	#pragma endregion

	// Inicializar Asteroide ---------------------------------------------------------------------

	#pragma region iniAsteroide

	float res = RES_ASTEROIDE;
	float offset;
	offset = 1;

	if (tipoA == senoidal)
		for (int i = 1; i < RES_ASTEROIDE + 1; i++) {
			for (int j = 1; j <= RES_ASTEROIDE + 1; j++) {
				//cout <<'('<< i << ',' << j <<')' << (i+1) * (RES_ASTEROIDE) + j;
				float a = (cos(j * 8 * 2 * PI / res) * SUPERFICIE_VARIACION + SUPERFICIE_BASE);

				verticesAsteroide[i * (RES_ASTEROIDE+2) + j] = Vec3(
					cos(j * 2 * PI / res) * a * sin(i * PI / res),
					sin(j * 2 * PI / res) * a * sin(i * PI / res),
					sin(i * PI / res - PI / 2)
				) * offset;
			}
			//cout << endl;
			offset = (cos(i * 8 * 2 * PI / res) * SUPERFICIE_VARIACION + SUPERFICIE_BASE);
		}

	if (tipoA == aleatorio)
		for (int i = 1; i < RES_ASTEROIDE + 1; i++) {
			for (int j = 1; j <= RES_ASTEROIDE + 1; j++) {
				//cout <<'('<< i << ',' << j <<')' << (i+1) * (RES_ASTEROIDE) + j;
				float a = random(-1,1) * SUPERFICIE_VARIACION + SUPERFICIE_BASE;

				verticesAsteroide[i * (RES_ASTEROIDE + 2) + j] = Vec3(
					cos(j * 2 * PI / res) * a * sin(i * PI / res),
					sin(j * 2 * PI / res) * a * sin(i * PI / res),
					sin(i * PI / res - PI / 2)
				) * (random(-1, 1) * SUPERFICIE_VARIACION + SUPERFICIE_BASE);
			}
			//cout << endl;
			offset = random(-1,1) * SUPERFICIE_VARIACION + SUPERFICIE_BASE;
		}

	//Derecha izquierda
	for (int i = 1; i < RES_ASTEROIDE + 1; i++) {
		verticesAsteroide[i * (RES_ASTEROIDE + 2)] = verticesAsteroide[i * (RES_ASTEROIDE + 2) + RES_ASTEROIDE];
		verticesAsteroide[i * (RES_ASTEROIDE + 2) + RES_ASTEROIDE + 1] = verticesAsteroide[i * (RES_ASTEROIDE + 2) + 1];
	}
	//Arriba abajo
	for (int j = 1; j < RES_ASTEROIDE + 1; j++) {
		verticesAsteroide[j] = verticesAsteroide[(RES_ASTEROIDE) * (RES_ASTEROIDE + 2) + j];
		verticesAsteroide[(RES_ASTEROIDE + 1) * (RES_ASTEROIDE + 2) + j] = verticesAsteroide[RES_ASTEROIDE + 2 + j];
	}
	//Esquinas
	verticesAsteroide[0] = verticesAsteroide[(RES_ASTEROIDE) * (RES_ASTEROIDE + 2)];
	verticesAsteroide[(RES_ASTEROIDE + 1) * (RES_ASTEROIDE + 2) + RES_ASTEROIDE + 1] = verticesAsteroide[(RES_ASTEROIDE + 2) + 1];
	verticesAsteroide[(RES_ASTEROIDE + 1) * (RES_ASTEROIDE + 2)] = verticesAsteroide[(RES_ASTEROIDE + 2) + RES_ASTEROIDE+1];
	#pragma endregion

	#pragma region asteroide

	asteroide = glGenLists(1);
	glNewList(asteroide, GL_COMPILE);
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	//glColor3f(1, 1, 1);

	// Dibujar ---------------------------------------------------------------------
	res = RES_ASTEROIDE;

	//glPolygonMode(GL_BACK, GL_LINE);


	GLfloat df[4] = {0.0f, 0.0f, 0.0f, 1.0f};
	GLfloat sp[4] = {0.0f, 0.0f, 0.0f, 1.0f};

	glMaterialfv(GL_FRONT, GL_DIFFUSE, GRISCLARO);
	glMaterialfv(GL_FRONT, GL_SPECULAR, NEGRO);
	glMaterialf(GL_FRONT, GL_SHININESS, 0);
	glBindTexture(GL_TEXTURE_2D, roca);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	for (int i = 1; i < res; i++) {
		glBegin(GL_TRIANGLE_STRIP);
		for (int j = 1; j <= res + 1; j++) {
			//float a = (cos(j * 8 * 2 * PI / res) * 0.1 + 0.9);
			Vec3 v1 = verticesAsteroide[i * (RES_ASTEROIDE+2) + j];
			Vec3 v2 = verticesAsteroide[(i + 1) * (RES_ASTEROIDE + 2) + j];
			Vec3 v3 = verticesAsteroide[i * (RES_ASTEROIDE+2) + j + 1];
			Vec3 n2;
			if (j == res+1)
				n2 = (v3 - v1).cross(v2 - v1)*-1;
			else
				n2 = (v3 - v1).cross(v2 - v1);

			n2.normalize();
			glNormal3f(n2.x, n2.y, n2.z);
			glTexCoord2f(float(i) / RES_ASTEROIDE, float(j) / RES_ASTEROIDE);
			glVertex3f(v2.x, v2.y, v2.z);
			
			Vec3 n1;
			if (j == res + 1)
				n1 = (v3 - v1).cross(v2 - v1) * -1;
			else
				n1 = (v3 - v1).cross(v2 - v1);

			n1.normalize();
			glNormal3f(n1.x, n1.y, n1.z);
			glTexCoord2f(float(i+1) / RES_ASTEROIDE, float(j) / RES_ASTEROIDE);
			glVertex3f(v1.x, v1.y, v1.z);
		}
		glEnd();
	}
	glBegin(GL_TRIANGLE_STRIP);
	
	for (int j = res + 1; j >= 1; j--) {
		//float a = (cos(j * 8 * 2 * PI / res) * 0.1 + 0.9);

		glNormal3f(0, 0, -1);
		glTexCoord2f(float((RES_ASTEROIDE + 2) + 1) / RES_ASTEROIDE, 0);
		glVertex3f(0, 0, -1);
		Vec3 v1 = verticesAsteroide[(RES_ASTEROIDE+1) * (RES_ASTEROIDE + 2) + j];
		Vec3 n1 = v1;
		n1.normalize();
		glNormal3f(n1.x, n1.y, n1.z);
		glTexCoord2f(float((RES_ASTEROIDE + 1) + 1) / RES_ASTEROIDE, float(j) / RES_ASTEROIDE);
		glVertex3f(v1.x, v1.y, v1.z);
	}
	glEnd();

	glPopAttrib();
	glEndList();

	#pragma endregion

	#pragma region plano

	plano = glGenLists(1);
	glNewList(plano, GL_COMPILE);
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	//glColor3f(1, 1, 1);

	res = RES_ASTEROIDE;

	//glPolygonMode(GL_BACK, GL_LINE);

	float offsetAct = 1;
	float offsetSig = 1;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, AZUL);
	glMaterialfv(GL_FRONT, GL_SPECULAR, AZUL);
	glMaterialf(GL_FRONT, GL_SHININESS, 100);
	glNormal3f(0, 0, 1);
	for (int i = 0; i < res; i++) {
		glBegin(GL_TRIANGLE_STRIP);

		for (int j = 0; j <= res; j++) {

			float a = (cos(j * 8 * 2 * PI / res) * 0.1 + 0.9);

			Vec3 v2(
				(i+1)/res,
				j/res,
				a*0.5
			);
			v2 = v2 * offsetSig;
			glNormal3f(a, offsetSig, 1);
			glVertex3f(v2.x, v2.y, v2.z);


			Vec3 v1(
				i/res,
				j/res,
				a*0.5
			);
			v1 = v1 * offsetAct;
			glNormal3f(a, offsetSig, 1);
			glVertex3f(v1.x, v1.y, v1.z);
		}
		offsetAct = offsetSig;
		//offsetSig = offsetSig + random(-0.1, 0.11);
		offsetSig = (cos(i * 8 * 2 * PI / res) * 0.1 + 0.9);
		glEnd();
	}


	glPopAttrib();
	glEndList();

	#pragma endregion

	#pragma region nave

	nave = glGenLists(1);
	glNewList(nave, GL_COMPILE);
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	//glColor3f(1, 1, 1);
	//ejes();
	//glDisable(GL_CULL_FACE);

	int numVert = 50;
	Vec3 propulsores[] = { Vec3(-.25f,-1.25f,-0.21f),Vec3(.25f,-1.25f,-0.21f),Vec3(0,-1.25f,0.21f), };

	for (int i = 0; i < 3; i++) {
		glPushMatrix();
		glTranslatef(propulsores[i].x, propulsores[i].y, propulsores[i].z);
		glScalef(0.25f, 0.25f, 0.25f);
		glBegin(GL_TRIANGLE_STRIP);
		//Parte delantera
		for (int i = 0; i <= numVert; i++) {
			Vec3 v(-cos(i * 2 * PI / numVert), 1, sin(i * 2 * PI / numVert));
			Vec3 n = v;
			n.normalize();
			glNormal3f(n.x, n.y, n.z);
			glVertex3f(0, 2, 0);
			glVertex3f(v.x, v.y, v.z);
		}
		//Parte trasera
		for (int i = 0; i <= numVert; i++) {
			Vec3 v(-cos(i * 2 * PI / numVert), 1, sin(i * 2 * PI / numVert));
			Vec3 n = v;
			n.normalize();
			glNormal3f(n.x, n.y, n.z);
			glVertex3f(v.x, v.y, v.z);
			glVertex3f(0, 2, 0);
		}
		glEnd();
		glPopMatrix();
	}
	//----------------------------------------------------------------------------------------------------

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glBindTexture(GL_TEXTURE_2D, reflejo_fondo);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glEnable(GL_LIGHTING);
	GLfloat Dm_TV[] = { 0.8,0.8,0.8,1.0 };
	GLfloat Sm_TV[] = { 0.8,0.8,0.8,1.0 };
	GLfloat s_TV = 20.0;
	glColor3f(0.4f, 0.4f, 0.4f);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, Dm_TV);
	glMaterialfv(GL_FRONT, GL_SPECULAR, Sm_TV);
	glMaterialf(GL_FRONT, GL_SHININESS, s_TV);
	glPushMatrix();
	glScalef(0.99, 0.99, 0.99);
	glPushMatrix();
	//Arriba y abajo
	for (int i = 0; i < 2; i++) {
		GLfloat v0[3] = { .5f,.8f + 0.4 * i,0 };
		GLfloat v1[3] = { -.5f,.8f + 0.4 * i,0 };
		GLfloat v2[3] = { -.5f,-.8f,0 };
		GLfloat v3[3] = { .5f,-.8f,0 };
		glPushMatrix();
		glRotatef(180 * i, 0, 1, 0);
		glTranslatef(0, 0, .5f);
		quadtex(v0, v1, v2, v3, 0, 1, 0, 1, 20, 20);
		glPopMatrix();
	}
	//Derecha e izquierda
	for (int i = 0; i < 2; i++) {
		//Techo/suelo
		GLfloat v0[3] = { .5f,.8f + 0.4 * abs((i - 1) % 2),0 };
		GLfloat v1[3] = { -.5f,.8f + 0.4 * i,0 };
		GLfloat v2[3] = { -.5f,-.8f,0 };
		GLfloat v3[3] = { .5f,-.8f,0 };

		glPushMatrix();
		glRotatef(180 * i + 90, 0, 1, 0);
		glTranslatef(0, 0, .5f);
		quadtex(v0, v1, v2, v3, 0, 1, 0, 1, 20, 20);
		glPopMatrix();
	}

	//Delante y detrás
	for (int i = 1; i >= 0; i--) {
		//Pared
		GLfloat v0[3] = { .5f, 0.5f,+0.4f * i };
		GLfloat v1[3] = { -.5f,.5f,+0.4f * i };
		GLfloat v2[3] = { -.5f,-.5f, 0 };
		GLfloat v3[3] = { .5f,-.5f, 0 };

		glPushMatrix();
		glRotatef(180 * i + 90, 1, 0, 0);
		glTranslatef(0, 0, .8f);
		quadtex(v0, v1, v2, v3, 0, 1, 0, 1, 20, 20);
		glPopMatrix();
	}
	glPopMatrix();
	glPopMatrix(),
	glPopAttrib();
	//-----------------------------------------------------------------------------------

	glMaterialfv(GL_FRONT, GL_DIFFUSE, GRISCLARO);
	glMaterialfv(GL_FRONT, GL_SPECULAR, GRISCLARO);
	glMaterialf(GL_FRONT, GL_SHININESS, 100);

	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, suelo_techo_nave);
	//Arriba y abajo
	for (int i = 0; i < 2; i++) {
		GLfloat v0[3] = {.5f,.8f+0.4*i,0};
		GLfloat v1[3] = {-.5f,.8f+0.4*i,0};
		GLfloat v2[3] = {-.5f,-.8f,0};
		GLfloat v3[3] = {.5f,-.8f,0};
		glPushMatrix();
		glRotatef(180 * i, 0, 1, 0);
		glTranslatef(0, 0, .5f);
		quadtex(v0,v1,v2,v3,0,1,0,1,20,20);	
		glPopMatrix();
	}
	//Derecha e izquierda
	glBindTexture(GL_TEXTURE_2D, lado_nave_1);
	for (int i = 0; i < 2; i++) {
		//Techo/suelo
		GLfloat v0[3] = { .5f,.8f+0.4 * abs((i - 1) % 2),0 };
		GLfloat v1[3] = { -.5f,.8f+0.4 * i,0};
		GLfloat v2[3] = { -.5f,-.8f,0 };
		GLfloat v3[3] = { .5f,-.8f,0 };

		glPushMatrix();
		glRotatef(180 * i + 90, 0, 1, 0);
		glTranslatef(0, 0, .5f);
		quadtex(v0, v1, v2, v3, 0, 1, 0, 1, 20, 20);
		glPopMatrix();
	}
	glDisable(GL_LIGHT1);
	glDisable(GL_LIGHT2);
	//Delante y detrás
	glBindTexture(GL_TEXTURE_2D, Ventana_Nave_Exterior);
	for (int i = 1; i >= 0; i--) {
		//Pared
		GLfloat v0[3] = { .5f, 0.5f,+0.4f * i};
		GLfloat v1[3] = { -.5f,.5f,+0.4f * i};
		GLfloat v2[3] = { -.5f,-.5f, 0};
		GLfloat v3[3] = { .5f,-.5f, 0};

		glPushMatrix();
		glRotatef(180 * i + 90, 1, 0, 0);
		glTranslatef(0, 0, .8f);
		quadtex(v0, v1, v2, v3, 0, 1, 0, 1, 20, 20);
		glPopMatrix();
		glBindTexture(GL_TEXTURE_2D, suelo_techo_nave);
	}
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);

	//Alas
	glBindTexture(GL_TEXTURE_2D, suelo_techo_nave);
	glPushMatrix();
	glTranslatef(0, -0.5f, -0.4f);
	GLfloat v0A[3] = { -1,1,0 };
	GLfloat v1A[3] = { 1,1,0 };
	GLfloat v2A[3] = { 1.5f,0,0 };
	GLfloat v3A[3] = { -1.5f,0,0 };
	GLfloat v4A[3] = { -1.3,0,.25f };
	GLfloat v5A[3] = { 1.3,0,.25f };
	quadtex(v5A, v4A, v3A, v2A, 0, 1, 0, 1, 20, 20);
	quadtex(v1A, v0A, v4A, v5A, 0, 1, 0, 1, 20, 20);
	quadtex(v0A, v1A, v2A, v3A, 0, 1, 0, 1, 20, 20);

	quadtex(v0A, v3A, v3A, v4A, 0, 1, 0, 1, 20, 20);
	quadtex(v1A, v5A, v5A, v2A, 0, 1, 0, 1, 20, 20);
	glEnd();
	glPopMatrix();

	glPopMatrix();
	
	//glPolygonMode(GL_BACK, GL_LINE);

	glPopAttrib();
	glEndList();

	#pragma endregion

	#pragma region sonidos
	device = audiere::OpenDevice();
	musica = OpenSound(device, "tomp3.cc-Space.wav", false);
	disparo = OpenSound(device, "tomp3.cc-Blaster-sound-effect.wav", false);

	musica->play(); musica->setRepeat(true); musica->setVolume(.25f);
	#pragma endregion

	//Inicialización del jugador/nave
	player.seto(Vec3(0, 0, z0)); //Posicion
	player.setu(Vec3(1, 0, 0));  //x
	player.setw(Vec3(0, -1, 0)); //y
	player.setv(Vec3(0, 0, 1));  //z
	speed_Player = 0;

	primeraPersona.setu(Vec3(1, 0, 0));  //x
	primeraPersona.setw(Vec3(0, -1, 0)); //y
	primeraPersona.setv(Vec3(0, 0, 1));  //z
}

//Diferencia entre 2 álgulos
// https://stackoverflow.com/questions/21622956/how-to-convert-direction-vector-to-euler-angles
// https://stackoverflow.com/questions/21622956/how-to-convert-direction-vector-to-euler-angles
void lcs2Euler(){


}

void update() {

	//Coherente con el tiempo transcurrido (sin control de FPS)
	static int hora_anterior = glutGet(GLUT_ELAPSED_TIME);
	int hora_actual = glutGet(GLUT_ELAPSED_TIME);

	float delta = (hora_actual - hora_anterior) / 1000.0f;

	Vec3 w = player.getw();

	// cout << girox << ",\t" << giroy << ",\t" << delta << endl;

	//Giro
	switch (camaraActual)
	{
	case basica:
		player.rotar(-girox * delta, player.getu());
		player.rotar(-giroy * delta, player.getv());
		primeraPersona = player;
		terceraPersona3d = player;

		cabinaPrimeraPersona.setu(Vec3(1, 0, 0));  //x
		cabinaPrimeraPersona.setw(Vec3(0, -1, 0)); //y
		cabinaPrimeraPersona.setv(Vec3(0, 0, 1));  //z
	case primera_persona:
		primeraPersona.rotar(-girox * delta, primeraPersona.getu());
		primeraPersona.rotar(-giroy * delta, primeraPersona.getv());

		cabinaPrimeraPersona.rotar(-girox * delta, cabinaPrimeraPersona.getu());
		cabinaPrimeraPersona.rotar(-giroy * delta, cabinaPrimeraPersona.getv());

		break;
	case tercera_persona:
		player.rotar(-girox * delta, player.getu());
		player.rotar(-giroy * delta, player.getv());

		terceraPersona3d.rotar(-girox * delta, terceraPersona3d.getu());
		terceraPersona3d.rotar(-giroy * delta, terceraPersona3d.getv());

		//terceraPersonaNave3d.rotar(-girox * delta, terceraPersona3d.getu());
		//terceraPersonaNave3d.rotar(-giroy * delta, terceraPersona3d.getv());

		break;
	default:
		break;
	}

	//Reducimos el giro progresivamente para que parezca más suave
	if (abs(girox) > 0.01f) { girox -= signo(girox) * delta * abs(girox) * 10; }
	else girox = 0;
	if (abs(giroy) > 0.01f) { giroy -= signo(giroy) * delta * abs(giroy) * 10; }
	else giroy = 0;

	//Al final funciona con el sistema de referencia
	//posPlayer += Vec3(-w.x * speed_Player * delta, -w.y * speed_Player * delta, -w.z * speed_Player * delta);

	player.seto(player.geto() += Vec3(-w.x * speed_Player * delta, -w.y * speed_Player * delta, -w.z * speed_Player * delta));

	for (int i = 0; i < NUM_ASTEROIDES; i++) {
		asteroides[i].Actualizar(delta);
		if (asteroides[i].explosion == true) { 
			//cout << "Explosion solicitada, id:" << i << endl
			//	<< "Posicion" << asteroides[i].posicionExplosion.x << ','
			//	<< asteroides[i].posicionExplosion.y << ','
			//	<< asteroides[i].posicionExplosion.z;

			explosiones[explActual].Iniciar(asteroides[i].posicionExplosion, asteroides[i].size, 10);
			explActual = (explActual + 1) % NUM_EXPLOSIONES;
			asteroides[i].explosion = false; 
		}

		//Colisión de la nave con cada asteroide
		if ((player.geto() - asteroides[i].getPos()).norm() < asteroides[i].size) {
			speed_Player = 0;
			player.seto((player.geto() - asteroides[i].getPos()) * 1.001f + player.geto());
			girox += random(-10, 10);
			giroy += random(-10, 10);
		}
	}

	for (int i = 0; i < NUM_BLASTERS; i++) {
		blasters[i].Actualizar(delta, speed_Player);
	}

	for (int i = 0; i < NUM_EXPLOSIONES; i++) {
		explosiones[i].Actualizar(delta);
	}

	hora_anterior = hora_actual;

	glutPostRedisplay();
}

void cubemap(GLfloat posicionEstrella[3], GLuint textura, bool dibujarEstrella) {

	glDisable(GL_LIGHTING);
	glPushMatrix();

	
	glBindTexture(GL_TEXTURE_2D, estrella);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glMaterialf(GL_FRONT, GL_SHININESS, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	static float planoS[] = { 10,0,0,0 };
	static float planoT[] = { 0,10,0,0 };
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGenfv(GL_S, GL_OBJECT_PLANE, planoS);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGenfv(GL_T, GL_OBJECT_PLANE, planoT);

	Vec3 posEstrella(posicionEstrella[0], posicionEstrella[1], posicionEstrella[2]);
	posEstrella.normalize();
	posEstrella = posEstrella * 0.5f;
	glPushMatrix();
	glTranslatef(posEstrella.x, posEstrella.y, posEstrella.z);
	if (dibujarEstrella)
	glutSolidSphere(0.05f, 20, 20);
	//glutSolidTeapot(0.05f);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, textura);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	int caras[] = { 7,2,5,10,6,4 };

	for (int i = 0; i < 4; i++) {
		glPushMatrix();
		glRotatef(90 * i, 0, 0, 1);
		glTranslatef(0, 0.5f, 0);
		glRotatef(90 * i, 0, 1, 0);
		glTranslatef(-0.5f, 0, -0.5f);

		glBegin(GL_TRIANGLE_STRIP);
		GLfloat p[] = { float(caras[i] / 4) * 1 / 3.f, float(caras[i] % 4) * 1 / 4.f };
		glTexCoord2f(p[0], p[1]);
		glVertex3f(0, 0, 0);
		glTexCoord2f(1 / 3.f + p[0], p[1]);
		glVertex3f(1, 0, 0);
		glTexCoord2f(p[0], 1 / 4.f + p[1]);
		glVertex3f(0, 0, 1);
		glTexCoord2f(1 / 3.f + p[0], 1 / 4.f + p[1]);
		glVertex3f(1, 0, 1);
		glEnd();
		glPopMatrix();
	}

	for (int i = 4; i < 6; i++) {
		glPushMatrix();
		glTranslatef(-0.5f, -0.5f + 1, -0.5f);

		glTranslatef(0, 0, i - 4);
		glRotatef(180 * i, 1, 0, 0);
		glRotatef(90 + 180, 1, 0, 0);

		glTranslatef(0, 0, (i - 4) - 1);
		glBegin(GL_TRIANGLE_STRIP);
		GLfloat p[] = { float(caras[i] / 4) * 1 / 3.f, float(caras[i] % 4) * 1 / 4.f };
		glTexCoord2f(p[0], p[1]);
		glVertex3f(0, 0, 0);
		glTexCoord2f(1 / 3.f + p[0], p[1]);
		glVertex3f(1, 0, 0);
		glTexCoord2f(p[0], 1 / 4.f + p[1]);
		glVertex3f(0, 0, 1);
		glTexCoord2f(1 / 3.f + p[0], 1 / 4.f + p[1]);
		glVertex3f(1, 0, 1);
		glEnd();
		glPopMatrix();
	}

	glEnable(GL_LIGHTING);
	glPopMatrix();
}

void terceraPersona() {
	if (camaraActual == tercera_persona) {
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		//glDisable(GL_LIGHTING);
		//glDisable(GL_DEPTH_TEST);	//Profundidad

		glPushMatrix();
		glLoadIdentity();
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		//glOrtho(-1, 1, -1, 1, -1, 1);
		gluPerspective(90, windowWidth / float(windowHeight), 0.1, 10);
		glMatrixMode(GL_MODELVIEW);

		Vec3 lookAt;
		Vec3 up;
		up = Vec3(0, 0, 1);

		if (camaraActual == basica) {
			lookAt = Vec3(0, 1, 0);
			//up = Vec3(0, 0, 1);
		}

		//lookAt = player.getw() * -1;
		else if (camaraActual == tercera_persona) {
			lookAt = terceraPersonaNave3d.getw() * 2;
		}

		//gluLookAt(0, 0, 0, lookAt.x, lookAt.y, lookAt.z, up.x, up.y, up.z);
		gluLookAt(0, -5, 2, 0, 0, 0, up.x, up.y, up.z);

		//glBindTexture(GL_TEXTURE_2D, textura);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

		glPushMatrix();
		glTranslatef(0, -2, 0);
		glRotatef(girox * 2 * PI, 1, 0, 0);
		glRotatef(giroy * 2 * PI, 0, 0, 1);
		glCallList(nave);
		glPopMatrix();
		
		if (camaraActual == primera_persona) {
			glRotatef(offset_girox * 360 / 2 / PI, 1, 0, 0);
			glRotatef(offset_giroy * 360 / 2 / PI, 0, 1, 0);
		}

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		glPopAttrib();
	}
}

void cabina(GLuint textura) {
	if (mostrarCabina && camaraActual != tercera_persona) {
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);	//Profundidad

		glPushMatrix();
		glLoadIdentity();
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		//glOrtho(-1, 1, -1, 1, -1, 1);
		gluPerspective(90, windowWidth/float(windowHeight), 0.1, 2);
		glMatrixMode(GL_MODELVIEW);

		Vec3 lookAt;
		Vec3 up;

		if (camaraActual == basica) {
			lookAt = Vec3(0, 1, 0);
			up = Vec3(0, 0, 1);
		}

		//lookAt = player.getw() * -1;
		else if (camaraActual == primera_persona) {
			lookAt = cabinaPrimeraPersona.getw() * -1;
			up = cabinaPrimeraPersona.getv();
		}

		gluLookAt(0, 0, 0, lookAt.x, lookAt.y, lookAt.z, up.x, up.y, up.z);

		//glBindTexture(GL_TEXTURE_2D, textura);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		Vec3 punteroDireccion = Vec3(0, 0, -1);
		float rotx = girox, roty = giroy;
		if (abs(girox) > 1) rotx = 1 * signo(girox);
		if (abs(giroy) > 1) roty = 1 * signo(giroy);
		if (camaraActual == basica) {
			punteroDireccion.rotate(rotx * 0.1f, cabinaPrimeraPersona.getw());
			punteroDireccion.rotate(roty * 0.1f, cabinaPrimeraPersona.getu());
		}

		glPushMatrix();
		glRotatef(-90, 1, 0, 0);
		glRotatef(180, 0, 1, 0);
		glRotatef(90, 0, 0, 1);		
		glPushMatrix();
		glRotatef(girox / 2.f, 0, 1, 0);
		glRotatef(giroy / 2.f, 1, 0, 0);
		cubemap(Vec3(0, 0, 0), textura, false);
		
		//-----------------------------
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_CULL_FACE);

		//--------------------- RETICULA ----------------------------------------------------

		glPushMatrix();
		glTranslatef(0, 0, -1);
		glRotatef(90, 1, 0, 0);
		glRotatef(-180, 0, 1, 0);
		glRotatef(-90, 0, 0, 1);
		glColor4f(1, 1, 1, 1);
		glRotatef(90, 0, 1, 0);
		glutSolidTorus(0.0025f, 0.03f, 20, 20);
		glPopMatrix();

		//--------------------- INDICADORES DE LA NAVE --------------------------------------
		
		glPushMatrix();
		glColor3f(0, 0, 0);
		glTranslatef(0.55, -0.4, -1);
		glRotatef(-180, 1, 0, 0);
		glRotatef(-90, 0, 0, 1);
		glRotatef(180, 0, 1, 0);
		glutSolidSphere(0.1, 10, 10);
		glScalef(0.1f, 0.1f, 0.1f);
		player.drawLocal();
		glColor3f(1, 1, 1);
		glScalef(0.2f, 0.2f, 0.2f);
		/*Vec3 o = player.geto();
		Vec3 w = player.getw();
		Vec3 v = player.getv();
		Vec3 u = player.getu();

		GLfloat matrix_transformacion[16] = {
			u.x, u.y, u.z, 0.0f,
			-w.x, -w.y, -w.z, 0.0f,
			v.x, v.y, v.z, 0.0f,
			o.x, o.y, o.z, 0.0f,
		};
		glMultMatrixf(matrix_transformacion);

		glCallList(nave);*/
		
		glPopMatrix();

		//Velocidad
		Vec3 posVelocimetro(+0.55f,+0.4f,-1.0f);
		glColor3f(0, 0, 0);
		glBegin(GL_TRIANGLE_STRIP);
		glVertex3f(+0.12/2 + posVelocimetro.x, -0.025f + posVelocimetro.y, posVelocimetro.z);
		glVertex3f(+0.12/2 + posVelocimetro.x, +0.025f + posVelocimetro.y, posVelocimetro.z);
		glVertex3f(-0.11 + posVelocimetro.x, -0.025f + posVelocimetro.y, posVelocimetro.z);
		glVertex3f(-0.11 + posVelocimetro.x, +0.025f + posVelocimetro.y, posVelocimetro.z);
		glEnd();
		glColor3f(1,1,1);
		glBegin(GL_TRIANGLES);
		glVertex3f(posVelocimetro.x+ -0.1 * speed_Player / MAX_SPEED + 0.02, -0.02f + posVelocimetro.y - 0.02f, posVelocimetro.z);
		glVertex3f(posVelocimetro.x + -0.1 * speed_Player / MAX_SPEED - 0.02, -0.02f + posVelocimetro.y - 0.02f, posVelocimetro.z);
		glVertex3f(posVelocimetro.x + -0.1 * speed_Player / MAX_SPEED, -0.02f + posVelocimetro.y, posVelocimetro.z);

		glVertex3f(posVelocimetro.x + -0.1 * speed_Player / MAX_SPEED + 0.02, +0.02f + posVelocimetro.y + 0.02f, posVelocimetro.z);
		glVertex3f(posVelocimetro.x + -0.1 * speed_Player / MAX_SPEED - 0.02, +0.02f + posVelocimetro.y + 0.02f, posVelocimetro.z);
		glVertex3f(posVelocimetro.x + -0.1 * speed_Player / MAX_SPEED, +0.02f + posVelocimetro.y, posVelocimetro.z);
		glEnd();
		glColor3f(0.5f - speed_Player / MAX_SPEED, speed_Player / MAX_SPEED + 0.1f,0);
		glBegin(GL_TRIANGLE_STRIP);
		glVertex3f(+posVelocimetro.x, -0.02f + posVelocimetro.y, posVelocimetro.z);
		glVertex3f(+posVelocimetro.x, +0.02f + posVelocimetro.y, posVelocimetro.z);
		glVertex3f(-0.1 * speed_Player / MAX_SPEED + posVelocimetro.x, -0.02f + posVelocimetro.y, posVelocimetro.z);
		glVertex3f(-0.1 * speed_Player / MAX_SPEED + posVelocimetro.x, +0.02f + posVelocimetro.y, posVelocimetro.z);
		glEnd();

		//Base Joystick
		glColor3f(0, 0, 0);
		glBegin(GL_TRIANGLE_STRIP);
		glVertex3f(.8f, -0.02, -1);
		glVertex3f(.8f, +0.02, -1);
		glVertex3f(punteroDireccion.x + 0.5f, punteroDireccion.y - 0.02f, punteroDireccion.z);
		glVertex3f(punteroDireccion.x + 0.5f, punteroDireccion.y + 0.02f, punteroDireccion.z);
		glEnd();
		glPushMatrix();
		//Cabeza joystick
		glColor3f(1, 0, 0);
		glTranslatef(punteroDireccion.x + 0.5f, punteroDireccion.y, punteroDireccion.z);
		glutSolidSphere(0.05, 10, 10);
		glPopMatrix();

		glTranslatef(0.5, 0, -1);
		glPopMatrix();
		// ^^^Solidarios al giro
		// vvvIndependientes

		//--------------------- FIN INDICADORES DE LA NAVE --------------------------------------
		glPopAttrib();

		glPopMatrix();

		if (camaraActual == primera_persona) {
			glRotatef(offset_girox * 360 / 2 / PI, 1, 0, 0);
			glRotatef(offset_giroy * 360 / 2 / PI, 0, 1, 0);
		}

		glEnable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glBindTexture(GL_TEXTURE_2D, sofa);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glMaterialf(GL_FRONT, GL_SHININESS, 0);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
		static float planoS[] = { 0,5,0,0 };
		static float planoT[] = { 5,0,0,0 };
		glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
		glTexGenfv(GL_S, GL_OBJECT_PLANE, planoS);
		glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
		glTexGenfv(GL_T, GL_OBJECT_PLANE, planoT);

		glPushMatrix();
		glTranslatef(0, -0.1f, 0);
		glPushMatrix();
		glTranslatef(0, 0, -0.25f);
		glScalef(1.5f, 1.25f, 0.5f);
		glutSolidCube(0.2f);
		glPopMatrix();

		static float planoS_[] = { 0,0,5,0 };
		static float planoT_[] = { 5,0,0,0 };
		glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
		glTexGenfv(GL_S, GL_OBJECT_PLANE, planoS_);
		glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
		glTexGenfv(GL_T, GL_OBJECT_PLANE, planoT_);

		glPushMatrix();
		glTranslatef(0.0f, -0.2f, -0.1f);
		glScalef(1.5f, 0.5f, 1.0f);
		glutSolidCube(0.2f);
		glPopMatrix();
		glPopMatrix();
		
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);



		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		glPopAttrib();
	}
}
// Funcion de atencion al evento de dibujo
void display()
{
	//cout << luces;
	if (luces && camaraActual != cinematica) { glEnable(GL_LIGHT1); glEnable(GL_LIGHT2); }
	else { glDisable(GL_LIGHT1); glDisable(GL_LIGHT2);}

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glPolygonMode(GL_FRONT, GL_FILL);
	//glPolygonMode(GL_FRONT, GL_LINE);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	#pragma region Posiciones_faros
	/****************************************************************
	*	 Posiciones de los faros									*
	****************************************************************/

	
	Vec3 origen = player.geto();
	//origen += posPlayer;

	Vec3 lookAt = origen - player.getw();;
	Vec3 up;
	Vec3 offset3p(0,0,0);

	Vec3 faro1 = player.getu() * 0.25f;
	Vec3 faro2 = player.getu() * -0.25f;

	if (camaraActual == basica) {
		lookAt = origen - player.getw(); //w
		up = player.getv();
	
	}

	else if (camaraActual == primera_persona) {
		lookAt = origen - primeraPersona.getw();
		up = primeraPersona.getv();
	}
	
	else if (camaraActual == tercera_persona) {
		lookAt = origen - player.getw();
		up = terceraPersona3d.getv();
		offset3p = player.getw()*8 + player.getv() * 2;
		//offset3p = player.local2global(Vec3(0, 5, 2));
	}

	else if (camaraActual == cinematica) {
		lookAt = origen - player.getw();
		up = Vec3(0,0,1);
		//offset3p = player.getw() * 8;
		faro1 += offset3p * 8;
		faro2 += offset3p * 8;
	}

	static GLfloat posFaro1[] = { faro1.x, faro1.y, faro1.z, 1 }; //PosSpot
	glLightfv(GL_LIGHT1, GL_POSITION, posFaro1);
	GLfloat dir_central1[] = { 0.1f, 0, -1.0f };
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, dir_central1);

	//cout << faro1.x << faro1.y << faro1.z << endl;

	static GLfloat posFaro2[] = { faro2.x, faro2.y, faro2.z, 1 }; //PosSpot
	glLightfv(GL_LIGHT2, GL_POSITION, posFaro2);
	GLfloat dir_central2[] = { -0.1f, 0, -1.0f };
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, dir_central2);
	#pragma endregion
	
	if (camaraActual != tercera_persona && camaraActual != cinematica)
		gluLookAt(origen.x + offset3p.x, origen.y + offset3p.y, origen.z + offset3p.z, lookAt.x, lookAt.y, lookAt.z, up.x, up.y, up.z); //Desde el frente
	else if (camaraActual == tercera_persona)
		gluLookAt(origen.x + offset3p.x, origen.y + offset3p.y, origen.z + offset3p.z, origen.x, origen.y, origen.z, up.x, up.y, up.z); //Desde el frente
	else if (camaraActual == cinematica) {
		distAsteroideCamara = (asteroides[camAstActual].getPos() - player.geto()).norm();
		float cont = 0; int ite = 0;
		if (distAsteroideCamara > MAX_DIST_ASTEROIDES/2) {
			while (distAsteroideCamara > cont) {
				distAsteroideCamara = (asteroides[camAstActual].getPos() - player.geto()).norm();
				camAstActual = (camAstActual + 1) % NUM_ASTEROIDES;
				cont += 0.1f; ite++;
			}
			camAstActual = (camAstActual -1)%NUM_ASTEROIDES;
			//cout << camAstActual << "   " << cont << "   " << distAsteroideCamara << "        " << ite << endl;
		}
		Vec3 pos = asteroides[camAstActual].getPos();
		//cout << "a";
		gluLookAt(pos.x, pos.y, pos.z, origen.x, origen.y, origen.z, up.x, up.y, up.z); //Desde el frente
	}
	
	//gluLookAt(5, -5, 5, origen.x, origen.y, origen.z, up.x, up.y, up.z); //Desde el frente

	//ejes();

	static GLfloat posicionEstrella[] = { 50,0,100,0}; //Direccional
	glLightfv(GL_LIGHT0, GL_POSITION, posicionEstrella);

	/*Faros posiciones*/
	/*
	glPushMatrix();
	glTranslatef(faro1.x + origen.x, faro1.y + origen.y, faro1.z + origen.z);
	glutSolidSphere(0.2f, 8, 8);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(faro2.x + origen.x, faro2.y + origen.y, faro2.z + origen.z);
	glutSolidSphere(0.2f, 8, 8);
	glPopMatrix();
	*/

	/*Suelo*/
	glMaterialfv(GL_FRONT, GL_DIFFUSE, GRISOSCURO);
	glMaterialfv(GL_FRONT, GL_SPECULAR, GRISOSCURO);
	glMaterialf(GL_FRONT, GL_SHININESS, 0);

	glCallList(suelo);

	glPushMatrix();
	glTranslatef(0, 0, 1);
	for (int i = 0; i < 10; i++) {
		glTranslatef(5, 0, 0);
		glCallList(nave);
	}
	glPopMatrix();

	/*
	//Esfera
	glMaterialfv(GL_FRONT, GL_DIFFUSE, AZUL);
	glMaterialfv(GL_FRONT, GL_SPECULAR, AZUL);
	glMaterialf(GL_FRONT, GL_SHININESS, 1000);

	glPushMatrix();
	glTranslatef(0,30,0);
	glutSolidSphere(5,50,50);
	glPopMatrix();

	//Donut
	glMaterialfv(GL_FRONT, GL_DIFFUSE, VERDE);
	glMaterialfv(GL_FRONT, GL_SPECULAR, VERDE);
	glMaterialf(GL_FRONT, GL_SHININESS, 1000);

	glPushMatrix();
	glTranslatef(20, 10, 6);
	glutSolidTorus(1, 5, 50,50);
	glPopMatrix();

	//Taza
	glMaterialfv(GL_FRONT, GL_DIFFUSE, ROJO);
	glMaterialfv(GL_FRONT, GL_SPECULAR, ROJO);
	glMaterialf(GL_FRONT, GL_SHININESS, 1000);

	glPushMatrix();
	glTranslatef(10, 20, 10);
	glRotatef(90, 1, 0, 0);
	glutSolidTeapot(5);
	glPopMatrix();	
	
	*/
	//glPushMatrix();
	//glTranslatef(0, 2, 2);
	//glCallList(asteroide);
	//glPopMatrix();

	#pragma region cubemap
	glPushMatrix();
	glTranslatef(origen.x, origen.y, origen.z);
	glScalef(9000, 9000, 9000);
	cubemap(posicionEstrella, fondo, true);
	glPopMatrix();
	#pragma endregion


	for (int i = 0; i < NUM_ASTEROIDES; i++) {
		asteroides[i].Dibujar(asteroide);
		Vec3 pos = asteroides[i].getPos();
		Vec3 pos2 = pos;
		//Vec3 q = player.global2local(pos).normalize();
		float angulo = (pos-origen).normalize().dot(player.local2global(player.getw() * -1).normalize());

		//if (i == 0) {
			//cout << angulo << "\n";
			//Va malllllllll
			if (abs(angulo) < rad(15)) {
				cb2::texto(pos2.x, pos2.y, (char*)"Asteroide", pos2.z, VERDE);
			}
		//}

	}

	for (int i = 0; i < NUM_BLASTERS; i++) {
		blasters[i].Dibujar();
	}

	//glPushMatrix();
	//terceraPersona();
	//glPopMatrix();

	if (camaraActual == cinematica || camaraActual == tercera_persona) {
	#pragma region AAAAAAAAAAAAAA

		glEnable(GL_LIGHTING);

		glPushMatrix();
		Vec3 vecDir = player.getw() * -1;
		Vec3 vecUp = player.getv();

		// https://stackoverflow.com/questions/42554960/get-xyz-angles-between-vectors
		float angulo = acos(Vec3(0, 1, 0).dot(vecDir));
		Vec3 axis = Vec3(0, 1, 0).cross(vecDir);

		Vec3 transFV(0, 0, 1);
		transFV = transFV.rotate(angulo, axis);

		float anguloUP = transFV.dot(vecUp);
		if (isnan(anguloUP)) { cout << "check"; }
		//cout << anguloUP * 360 / 2 / PI << endl;

		
		glTranslatef(origen.x, origen.y, origen.z);
		glRotatef(angulo * 360 / 2 / PI, axis.x, axis.y, axis.z);
		glCallList(nave);
		glPopMatrix();
	#pragma endregion
	}

	for (int i = 0; i < NUM_EXPLOSIONES; i++) {
		explosiones[i].Dibujar(roca);
	}

	glPushMatrix();
	cabina(interiorNave);
	glPopMatrix();
	/*
	glPushMatrix();
	glTranslatef(lookAt.x, lookAt.y, lookAt.z);
	//player.drawGlobal();
	//player.drawLocal();
	glPopMatrix();
	*/

	#pragma region texto 
	texto(0, 0, (char*)"Hangar", 3);
	#pragma endregion


	glutSwapBuffers();
}

// Funcion de atencion al redimensionamiento
void reshape(GLint w, GLint h)
{
	//Relación de aspecto
	float ra = (float)w / (float)h;

	windowWidth = w;
	windowHeight = h;

	//fijado de dimensiones del marco
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//gluPerspective(apertura vertical, raz n de aspecto (alto:ancho), distancia del plano cercano, distancia del plano alejado); 
	gluPerspective(90, ra, 0.1, 10000);
}


void acelerar() {
	//cout << "acelerando";
	if (speed_Player < MAX_SPEED)
		speed_Player += ACCELERATION; //Tiene que ser coerente con el tiempo FALTA
}
void frenar() {
	//cout << "frenando";
	if (speed_Player > -MAX_SPEED / 2)
		speed_Player -= ACCELERATION; //Tiene que ser coerente con el tiempo FALTA
}
void esperaDisparo(int n) {
	disparando = false;
}
void disparar() {
	//NOTA: Poner sonido gracioso (PIU)

	if (!disparando) {

		disparo->play(); disparo->setRepeat(false); disparo->setVolume(.1f);

		disparando = true;
		//cout << "PUM!" << endl;
		blasters[blasterActual].Disparar(player, Vec3(0, -0.1f, -1));
		//blasters[blasterActual+1].Disparar(player, Vec3(1, 0, 0));
		blasterActual = (blasterActual + 1) % NUM_BLASTERS;
		//Llamar aquí al blaster
		glutTimerFunc(CADENCIA_DISPARO, esperaDisparo, CADENCIA_DISPARO);
	}
}

void onKey(unsigned char tecla, int x, int y) {
	switch (tecla) {
	case 'a':
	case 'A':
		acelerar();
		break;
	case 'z':
	case 'Z':
		frenar();
		break;

	case 'f':
	case 'F':
		disparar();
		break;

	default:
		break;
	}
	//glutPostRedisplay();

}void onKeyUP(unsigned char tecla, int x, int y) {
	switch (tecla) {
	
	case 'l':
	case 'L':
		luces = !luces;
		if (luces) {
			glEnable(GL_LIGHT1);
			glEnable(GL_LIGHT2);
		}
		else {
			glDisable(GL_LIGHT1);
			glDisable(GL_LIGHT2);
		}
		break;
	case 'c':
	case 'C':
		mostrarCabina = !mostrarCabina;
		switch (camaraActual)
		{
		case basica:
			//primeraPersona.setu(Vec3(1, 0, 0));  //x
			//primeraPersona.setw(Vec3(0, -1, 0)); //y
			//primeraPersona.setv(Vec3(0, 0, 1));  //z
			primeraPersona.setu(player.getu());  //x
			primeraPersona.setw(player.getw());  //y
			primeraPersona.setv(player.getv());  //z
			break;
		case primera_persona:
			break;
		case tercera_persona:
			luces = false;
			break;
		case cinematica:
			glDisable(GL_LIGHT1);
			glDisable(GL_LIGHT2);
			luces = false;

			break;
		default:
			break;
		}
		break;
	case 'v':
	case 'V':
		camaraActual = tipoCamara((camaraActual+1)%4);
		break;
	case 27:
		exit(0);
	default:
		break;
	}
	//glutPostRedisplay();

}

void onPassiveMotiotion(int x, int y);

void onClick(int boton, int estado, int x, int y) {
	//Almacenar el pixel donde se hizo el click
	if (boton == GLUT_LEFT_BUTTON && estado == GLUT_DOWN) {
		disparar();
	}
}

void onDrag(int x, int y) {
	disparar();
	onPassiveMotiotion(x, y);
}


void onPassiveMotiotion(int x, int y) {
	static float pixel2grados = 0.05f;
	//glutWarpPointer(windowWidth / 2, windowHeight / 2);

	//Giramos siendo coherentes con el tiempo
	static int hora_anterior = glutGet(GLUT_ELAPSED_TIME);
	int hora_actual = glutGet(GLUT_ELAPSED_TIME);
	float delta = (hora_actual - hora_anterior)/100.0f;

	girox += (y - yanterior) * pixel2grados * delta;
	giroy += (x - xanterior) * pixel2grados * delta;

	//xanterior = x;
	//yanterior = y;

	//Fija el cursor en la posición central de la pantalla
	glutWarpPointer(windowWidth / 2, windowHeight / 2);
	glutSetCursor(GLUT_CURSOR_NONE);

	xanterior = windowWidth / 2;
	yanterior = windowHeight / 2;
	hora_anterior = hora_actual;
}

void estelaNave(int t) {
	//Estela detrás de la nave:
	if (speed_Player > 0) {
		explosiones[explActual].Iniciar(player.geto() + player.getw(), speed_Player / MAX_SPEED * 0.5f, 1.f);
		explActual = (explActual + 1) % NUM_EXPLOSIONES;
	}
	int r = min(max(abs(100/float(t)), 1), 1000);
	glutTimerFunc(r, estelaNave, speed_Player); //Partículas estela de la nave
}


int main(int argc, char** argv)
{
	// Inicializaciones
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(50, 50);

	// Crear ventana
	glutCreateWindow(PROYECTO);

	windowWidth = 500;
	windowHeight = 500;

	// Registrar callbacks
	glutKeyboardFunc(onKey);
	glutKeyboardUpFunc(onKeyUP);
	glutMotionFunc(onDrag);
	glutMouseFunc(onClick);

	glutPassiveMotionFunc(onPassiveMotiotion);

	glutIdleFunc(update);		//FPS flexibles
	glutTimerFunc(100, estelaNave, 100); //Partículas estela de la nave

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	// Inicio propio y del bucle de eventos
	init();

	glutMainLoop();
}
