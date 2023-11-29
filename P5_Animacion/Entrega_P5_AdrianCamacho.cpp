/***************************************************
*	Practica 5 entrega Animacion
*
*	SGI
*
*	@author	A.Camacho <acamgar1@etsinf.upv.es>
*
* 
*  Notas Para la corrección:-------------------------
*	El código referente a la estructura de personas se lo he pasado a un compañero:
*   Brian Valiente Ródenas, quien a partir de la estructura y puntos clave a generado
*   un código similar. En cuanto a generación estructura y movimiento de las personas.
*   Eso sí, él sólo tomó los conceptos clave y desarrolló por su cuenta el código.
* 
*   Te lo comentamos en clase, pero como nos lo digiste lo comentamos por aquí, 
*   así lo hacemos para evitar confusiones. Un saludo.
***************************************************/

#define PROYECTO "Animacion"

#include <iostream>	
#include <codebase.h>

using namespace std;
using namespace cb;

// Controla la posición de la cámara
static int estadoAnimacion = 0;

// Controla la velocidad de la animación en general
static float speed = 0.75f;

// Distancia de la cámara
static float distCam = 2;
static float alturaCam = 2;

/*******Listas de dibujo*******/

// P3 y P4 estrella + esferaEstrella
static GLuint estrella;
static GLuint esferaEstrellaP4;

// P5
static GLuint asiento;
static GLuint vagoneta;
static GLuint dedos;
static GLuint parteBrazo;
static GLuint brazo;
static GLuint parteCentral;
static GLuint pulpo;
static GLuint tree;
static GLuint person;
static GLuint semaforo;
static GLuint hierba;

// Para la posición del conjunto de vagonetas respecto al brazo.
static float lastx;
static float lasty;
static float lastz;

//Posición de la cámara situada en la vagoneta, se actualiza más adelante
static GLfloat asientox = 5; //Default
static GLfloat asientoy = 5; //Default
static GLfloat asientoz = 5; //Default


enum {
	funcionando,   // Pulpo en funcionamiento
	bajandoBrazos, // Pulpo pausado bajando brazos
	subiendoBrazos // Pulpo pausado esperando a volver a empezar
}funcionamiento;

//Radio de la esfera de la práctica 4
static const float radio = 1;

static float alfa = 0;
static float rotacionDedos = 0;
static float rot = 0;

/********* Parada y reanudado de la atracción *********/
static float rotacionDedSUBIR_BAJAR_PARES;
static float rotacionDedSUBIR_BAJAR_IMPARES;

//Variables globales auxiliares en la subida y bajada
static bool brazosParesBajados;
static bool brazosImpaBajados;
static bool brazosParesSubidos;
static bool brazosImpaSubidos;
/*****************************************************************/

//Struct que representa una persona
struct PersonStruc
{
	Vec3 position;
	Vec3 direction;
	Vec3 color_camiseta;
};
//Cantidad de personas (funciona bien hasta 10000 en mi ordenador)
static int const NUM_PEOPLE = 100;
static PersonStruc people[NUM_PEOPLE];

// Número de dibujado de listas de hierba
static int const NUM_HIERBA = 200;

struct HierbaStruct
{
	Vec3 position;
	float rotation;
	float colorVerde;
};

static HierbaStruct hierbaLista[NUM_HIERBA];

//Inicializa el vector de personas aleatoriamiente
void setPeople(int n) {
	
	for (int i = 0; i < n; i++) {
		PersonStruc person = PersonStruc();
		
		person.position = Vec3((rand() % 100 - 50)/5.f, 0, (rand() % 100 - 50)/5.f).normalize()*(rand() % 100) / 10.f;
		while (person.position.norm() < 2.5) {
			person.position = Vec3((rand() % 100 - 50) / 5.f, 0, (rand() % 100 - 50) / 5.f).normalize() * (rand() % 100) / 10.f;
		}
		person.direction = Vec3(rand() % 10 - 5, 0, rand() % 10 - 5);
		person.color_camiseta = Vec3(rand() % 100 / 100.f, rand() % 100 / 100.f, rand() % 100 / 100.f);
		people[i] = person;

		//Poner a true para ver el estado inicial
		if (false)
		cout << person.position.x << "\t" << person.position.y << "\t" << person.position.z << "\t|"
			<< person.direction.x << "\t" << person.direction.y << "\t" << person.direction.z << "\t|"
			<< person.color_camiseta.x << "\t" << person.color_camiseta.y << "\t" << person.color_camiseta.z << "\n";
	}	
}

//Inicializa aleatoriamente el vector de hierbas
void setHierba() {
	for (int i = 0; i < NUM_HIERBA; i++) {
		hierbaLista[i].position = Vec3(rand() % 2000 / 100.f - 10.f, 0, rand() % 2000 / 100.f - 10.f);
		while (hierbaLista[i].position.norm() < 3.5f || hierbaLista[i].position.norm() > 8.5f)
		{
			hierbaLista[i].position = Vec3(rand() % 2000 / 100.f - 10.f, 0, rand() % 2000 / 100.f - 10.f);
		}
		hierbaLista[i].rotation = float(rand() % 360);
		hierbaLista[i].colorVerde = float(rand() % 25) / 100.f + 0.4f;
	}
}

void init() {

	int res = 0; //Variable auxiliar utilizada para calcular resoluciones

	glEnable(GL_DEPTH_TEST);

	//Listas de dibujo

	#pragma region person
	person = glGenLists(1);
	glNewList(person, GL_COMPILE);

	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	glutSolidCylinder(0.05f, 0.20f, 10, 1);
	glRotatef(90, 1, 0, 0);
	glColor3f(0.9, 0.75, 0.5);
	glTranslatef(0, 0.25f, 0);
	glutSolidSphere(0.075f, 8, 8);
	glPopMatrix();

	glEndList();
	#pragma endregion

	# pragma region Listas de dibujo Practica 3 y 4 estrella + estrella con esfera
	// INI Estrella 3 ---------------------------------------------------------------------------------------------------------------
	estrella = glGenLists(1);
	glNewList(estrella, GL_COMPILE);
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glBegin(GL_TRIANGLE_STRIP);
	for (int i = 0; i <= 3; i++) {
		glVertex3f(radio * cos(i * 2 * PI / 3 + PI / 2), radio * sin(i * 2 * PI / 3 + PI / 2), 0);
		glVertex3f(radio * 0.7 * cos(i * 2 * PI / 3 + PI / 2), radio * 0.75 * sin(i * 2 * PI / 3 + PI / 2), 0);
	}
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	for (int i = 0; i <= 3; i++) {
		glVertex3f(radio * cos(i * 2 * PI / 3 + 3 * PI / 2), radio * sin(i * 2 * PI / 3 + 3 * PI / 2), 0);
		glVertex3f(radio * 0.7 * cos(i * 2 * PI / 3 + 3 * PI / 2), radio * 0.75 * sin(i * 2 * PI / 3 + 3 * PI / 2), 0);
	}
	glEnd();
	glPopAttrib();

	glEndList();
	// FIN Estrella P3 ---------------------------------------------------------------------------------------------------------------

	// INI Esfera + estrella P4 ------------------------------------------------------------------------------------------------------
	esferaEstrellaP4 = glGenLists(1);
	glNewList(esferaEstrellaP4, GL_COMPILE);
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glColor3f(0, 0, 0.8);
	glutWireSphere(1, 20, 20);

	for (int i = 0; i <= 5; i++)
	{
		glColor3f(1 - ((float)i) * (1. / 6.), 1 - ((float)i) * (1. / 6.), ((float)i) * (1. / 6.));
		glPushMatrix();
		glRotatef(30 * i, 0, 1, 0);
		glCallList(estrella);
		glPopMatrix();
	}

	glPopAttrib();

	glEndList();
	// INI Esfera + estrella P4 ------------------------------------------------------------------------------------------------------

	#pragma endregion

	#pragma region Lista Asiento
	asiento = glGenLists(1);
	glNewList(asiento, GL_COMPILE);
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glColor3f(0, 0.75, 0.50);
	glBegin(GL_TRIANGLE_STRIP);
	for (int i = 0; i <= 4; i++) {
		glVertex3f(radio * cos(i * 2 * PI / 4 + PI / 4), 1.5, radio * sin(i * 2 * PI / 4 + PI / 4) / 2);
		glVertex3f(radio * cos(i * 2 * PI / 4 + PI / 4), 0, radio * sin(i * 2 * PI / 4 + PI / 4) / 2);
	}
	glEnd();

	glColor3f(0, 1, 1);
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(radio * cos(0 * 2 * PI / 4 + PI / 4), 1.5, radio * sin(0 * 2 * PI / 4 + PI / 4) / 2);
	glVertex3f(radio * cos(1 * 2 * PI / 4 + PI / 4), 1.5, radio * sin(1 * 2 * PI / 4 + PI / 4) / 2);
	glVertex3f(radio * cos(3 * 2 * PI / 4 + PI / 4), 1.5, radio * sin(3 * 2 * PI / 4 + PI / 4) / 2);
	glVertex3f(radio * cos(2 * 2 * PI / 4 + PI / 4), 1.5, radio * sin(2 * 2 * PI / 4 + PI / 4) / 2);
	glEnd();

	glColor3f(0, 0.75, 0.50);
	glBegin(GL_TRIANGLE_STRIP);
	for (int i = 0; i <= 4; i++) {
		glVertex3f(radio * cos(i * 2 * PI / 4 + PI / 4), 0.75, radio * sin(i * 2 * PI / 4 + PI / 4) + 0.6);
		glVertex3f(radio * cos(i * 2 * PI / 4 + PI / 4), 0, radio * sin(i * 2 * PI / 4 + PI / 4) + 0.6);
	}
	glEnd();

	glColor3f(0, 1, 1);
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(radio * cos(0 * 2 * PI / 4 + PI / 4), 0.75, radio * sin(0 * 2 * PI / 4 + PI / 4) + 0.6);
	glVertex3f(radio * cos(1 * 2 * PI / 4 + PI / 4), 0.75, radio * sin(1 * 2 * PI / 4 + PI / 4) + 0.6);
	glVertex3f(radio * cos(3 * 2 * PI / 4 + PI / 4), 0.75, radio * sin(3 * 2 * PI / 4 + PI / 4) + 0.6);
	glVertex3f(radio * cos(2 * 2 * PI / 4 + PI / 4), 0.75, radio * sin(2 * 2 * PI / 4 + PI / 4) + 0.6);
	glEnd();

	glPopAttrib();
	glEndList();
	#pragma endregion

	#pragma region Lista vagoneta

	vagoneta = glGenLists(1);
	glNewList(vagoneta, GL_COMPILE);
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	res = 20;
	float alturavagoneta = 0.5f; //Altura de las vagonetas

	glPushMatrix();
	glScalef(0.5, 0.5, 0.5);
	glTranslatef(-0.75, 0.01, -0.75);
	glCallList(asiento);
	glTranslatef(1.5, 0, 0);
	glCallList(asiento);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.75/2, 0.01, 0);
	glPushMatrix();
	glScalef(4, 4, 4);
	glColor3f(0, 0, 1);
	glCallList(person);
	glPopMatrix();
	glTranslatef(1.5/2, 0, 0);
	glPushMatrix();
	glScalef(4, 4, 4);
	glColor3f(0, 0, 1);
	glCallList(person);
	glPopMatrix();
	glPopMatrix();



	//Paredes exteriores Color mas claro
	glColor3f(1, 1, 0);
	glBegin(GL_TRIANGLE_STRIP);
	for (int i = -res/6; i <= res/6; i++) {
		glVertex3f(sin(i * 2 * PI / res), alturavagoneta, cos(i * 2 * PI / res));
		glVertex3f(sin(i * 2 * PI / res), 0, cos(i * 2 * PI / res));
	}
	for (int i = -res / 6; i <= res / 6; i++) {
		glVertex3f(sin(i * 2 * PI / res + PI), alturavagoneta, cos(i * 2 * PI / res + PI));
		glVertex3f(sin(i * 2 * PI / res + PI), 0, cos(i * 2 * PI / res + PI));
	}
	glVertex3f(sin(-res / 6 * 2 * PI / res), alturavagoneta, cos(-res/6 * 2 * PI / res));
	glVertex3f(sin(-res / 6 * 2 * PI / res), 0, cos(-res/6 * 2 * PI / res));
	glEnd();

	//Paredes interiores Color mas apagado
	glColor3f(0.75, 0.75, 0);
	glBegin(GL_TRIANGLE_STRIP);
	for (int i = -res / 6; i <= res / 6; i++) {
		glVertex3f(sin(i * 2 * PI / res) * 0.99, alturavagoneta, cos(i * 2 * PI / res) * 0.99);
		glVertex3f(sin(i * 2 * PI / res) * 0.99, 0, cos(i * 2 * PI / res) * 0.99);
	}
	for (int i = -res / 6; i <= res / 6; i++) {
		glVertex3f(sin(i * 2 * PI / res + PI) * 0.99, alturavagoneta, cos(i * 2 * PI / res + PI) * 0.99);
		glVertex3f(sin(i * 2 * PI / res + PI) * 0.99, 0, cos(i * 2 * PI / res + PI) * 0.99);
	}
	glVertex3f(sin(-res / 6 * 2 * PI / res) * 0.99, alturavagoneta, cos(-res / 6 * 2 * PI / res) * 0.99);
	glVertex3f(sin(-res / 6 * 2 * PI / res) * 0.99, 0, cos(-res / 6 * 2 * PI / res) * 0.99);

	glEnd();

	//Suelo
	glColor3f(0.5, 0.5, 0);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0, 0, 0);
	for (int i = -res / 6; i <= res / 6; i++) {
		//if (i % 10 < res / 3 || i % 10 > 2 * res / 3)
			glVertex3f(sin(i * 2 * PI / res), 0, cos(i * 2 * PI / res));
	}
	for (int i = -res / 6; i <= res / 6; i++) {
		//if (i % 10 < res / 3 || i % 10 > 2 * res / 3)
		glVertex3f(sin(i * 2 * PI / res + PI) * 0.99, 0, cos(i * 2 * PI / res + PI) * 0.99);
	}
	glVertex3f(sin(-res / 6 * 2 * PI / res), 0, cos(-res / 6 * 2 * PI / res));

	glEnd();

	//frente
	glColor3f(1, 1, 0);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0, alturavagoneta+0.25f, cos(res / 6 * 2 * PI / res));
	for (int i = -res/6; i <= res/6; i++) {
			glVertex3f(sin(i * 2 * PI / res), alturavagoneta, cos(i * 2 * PI / res));
	}
	glEnd();

	glPopAttrib();

	glEndList();
	#pragma endregion

	#pragma region Lista dedos
	dedos = glGenLists(1);
	glNewList(dedos, GL_COMPILE);
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glColor3f(0, 0, 0.75);

	glBegin(GL_TRIANGLE_STRIP);
	for (int i = 0; i <= 4; i++) {
		glVertex3f(cos(i * 2 * PI / 4), 0, sin(i * 2 * PI / 4));
		glVertex3f(0.5 * cos(i * 2 * PI / 4), 0, 0.5 * sin(i * 2 * PI / 4));
	}
	glEnd();

	for (int i = 0; i < 4; i++) {
		glPushMatrix();
		glTranslatef(cos(i * 2 * PI / 4 + PI / 2), 0.01, sin(i * 2 * PI / 4 + PI / 2));
		if (i%2==1)
			glRotatef(90*i-45, 0, 1, 0);
		else
			glRotatef(90 * i - 45+180, 0, 1, 0);
		glScalef(0.5, 0.5, 0.5);
		glCallList(vagoneta);
		glPopMatrix();
	}
	glPushMatrix();
	//glTranslatef(0, 0.5, 0);
	glScalef(0.4f, 0.4f, 0.4f);
	glTranslatef(0, 0.5f, 0);
	glCallList(esferaEstrellaP4);
	glTranslatef(0, 0.5f, 0);
	glRotatef(-90,4,0,0);
	glutSolidCylinder(1, 2, 10, 10);

	glPopMatrix();

	


	glPopAttrib();
	glEndList();
	#pragma endregion

	#pragma region Lista parteBrazo
	parteBrazo = glGenLists(1);
	glNewList(parteBrazo, GL_COMPILE);
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glColor3f(0, 0, 0.75);

	glPushMatrix();

	glTranslatef(-sin(0),0,-cos(0));
	glRotatef(90, 0, 0, 1);
	glTranslatef(0, -0.1, 0);

	//Pared
	glBegin(GL_TRIANGLE_STRIP);
	for (int i = 0; i <= 5; i++) {
		glVertex3f(sin(i * 2 * PI / 15) * 0.5, 0.2, cos(i * 2 * PI / 15));
		glVertex3f(sin(i * 2 * PI / 15) * 0.5, 0, cos(i * 2 * PI / 15));
	}
	glVertex3f(sin(0), 0.2, cos(0));
	glVertex3f(sin(0), 0, cos(0));
	glEnd();


	//Tapas
	glColor3f(0, 0, 1.f);
	glBegin(GL_TRIANGLE_FAN);
	for (int i = 0; i <= 5; i++) {
		glVertex3f(sin(i * 2 * PI / 15) * 0.5, 0.2, cos(i * 2 * PI / 15));
	}
	glVertex3f(sin(0), 0.2, cos(0));
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	for (int i = 0; i <= 5; i++) {
		glVertex3f(sin(i * 2 * PI / 15) * 0.5, 0, cos(i * 2 * PI / 15));
	}
	glVertex3f(sin(0), 0, cos(0));
	glEnd();

	glPopMatrix();

	glPopAttrib();
	glEndList();
	#pragma endregion

	#pragma region Lista brazo
	brazo = glGenLists(1);
	glNewList(brazo, GL_COMPILE);
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glPushMatrix();
	glTranslatef(0, 0.3, 0);
	glRotatef(4,1,0,0);
	glCallList(parteBrazo);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0.3, -2.2*cos(0));
	glRotatef(-4, 1, 0, 0);
	glRotatef(180, 0, 1, 0);
	glCallList(parteBrazo);
	glPopMatrix();

	lastx = 0;
	lasty = 0;
	lastz = -2.2 * cos(0);
	
	/*
	glPushMatrix();
	glTranslatef(0, 0, -cos(0) - 0.75);
	glCallList(dedos);
	glPopMatrix();
	*/

	glPopAttrib();
	glEndList();
	#pragma endregion
	
	#pragma region Pulpo
	pulpo = glGenLists(1);
	glNewList(pulpo, GL_COMPILE);
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	
	glPushMatrix();
	glRotatef(-30, 1, 0, 0);
	glutSolidSphere(0.5, 20, 20);
	glPushMatrix();
	glTranslatef(0, 0.20, -0.3);
	glutSolidSphere(0.45, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -0.20, 0.3);
	glScalef(1, 1, 0.5);
	glutSolidSphere(0.45, 20, 20);
	glPopMatrix();

	glColor3f(1, 1, 0);
	glPushMatrix();
	glTranslatef(0.4, 0.1, 0.1);
	glutSolidSphere(0.1, 20, 20);
	glColor3f(0, 0, 0);
	glScalef(1.1, 0.5, 1);
	glutSolidSphere(0.1, 20, 20);
	glPopMatrix();

	glColor3f(1, 1, 0);
	glPushMatrix();
	glTranslatef(-0.4, 0.1, 0.1);
	glutSolidSphere(0.1, 20, 20);
	glColor3f(0, 0, 0);
	glScalef(1.1, 0.5, 1);
	glutSolidSphere(0.1, 20, 20);
	glPopMatrix();
	glPopMatrix();

	/*
	glColor3f(0, 0, 0.75);
	glPushMatrix();
	glTranslatef(0, -0.75, 0);
	glRotatef(-90, 1, 0, 0);
	glScalef(1, 1, 1.5);
	glutSolidCone(1, 0.75, 10, 10);
	//glutSolidSphere(0.45, 20, 20);
	glPopMatrix();
	*/

	glPopAttrib();
	glEndList();
	#pragma endregion

	#pragma region partecentral
	parteCentral = glGenLists(1);
	glNewList(parteCentral, GL_COMPILE);
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glBegin(GL_TRIANGLE_STRIP);
	glColor3f(0.75, 0, 0);
	glBegin(GL_TRIANGLE_STRIP);
	for (int i = 0; i <= 15; i++) {
		glVertex3f(sin(i * 2 * PI / 15)/8, 0.5, cos(i * 2 * PI / 15)/8);
		glVertex3f(sin(i * 2 * PI / 15)/2, 0, cos(i * 2 * PI / 15)/2);
	}
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	for (int i = 0; i <= 15; i++) {
		glVertex3f(sin(i * 2 * PI / 15) / 4, 1.0, cos(i * 2 * PI / 15) / 4);
		glVertex3f(sin(i * 2 * PI / 15) / 8, 0.5, cos(i * 2 * PI / 15) / 8);
	}
	glEnd();

	glColor3f(0, 0, 0.75);

	glBegin(GL_TRIANGLE_STRIP);
	for (int i = 0; i <= 15; i++) {
		glVertex3f(sin(i * 2 * PI / 15) / 4, 1.5, cos(i * 2 * PI / 15) / 4);
		glVertex3f(sin(i * 2 * PI / 15) / 4, 1.0, cos(i * 2 * PI / 15) / 4);
	}
	glEnd();
	/*
	glPushMatrix();
	glTranslatef(0, 2.5, 0);
	glutSolidSphere(0.55,20,20);
	glPopMatrix();
	
	*/

	glPopAttrib();
	glEndList();
	#pragma endregion

	#pragma region tree
	tree = glGenLists(1);
	glNewList(tree, GL_COMPILE);
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glPushMatrix();
	glColor3f(0, 1, 0);
	glTranslatef(0, 0.5, 0);
	glutSolidSphere(0.25, 10, 10);	
	glTranslatef(0, 0.25, 0);
	glScalef(0.75, 0.75, 0.75);
	glutSolidSphere(0.25, 10, 10);
	glTranslatef(0, 0.25, 0);
	glScalef(0.75, 0.75, 0.75);
	glutSolidSphere(0.25, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	glColor3f(0.5, 0.5, 0);
	glutSolidCylinder(0.05,1,10,10);
	glPopMatrix();

	glPopAttrib();
	glEndList();
	#pragma endregion

	#pragma region semaforo
	semaforo = glGenLists(1);
	glNewList(semaforo, GL_COMPILE);
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	//Luz con color depende del color que se defina antes de llamar
	glPushMatrix();
	glTranslatef(0.125, 0.9f, 0);
	glScalef(0.2f, 1, 1);
	glutSolidSphere(0.1f,8,8);
	glTranslatef(0,0.25f,0);
	glutSolidSphere(0.1f, 8, 8);
	glPopMatrix();

	glColor3f(0.1f, 0.1f, 0.1f);
	glPushMatrix();
	glRotatef(-90,1,0,0);
	glutSolidCylinder(0.05f,1,4,1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 1, 0);
	glScalef(0.5f, 1.25f, 0.5f);
	glutSolidCube(0.5f);
	glPopMatrix();	

	glPopAttrib();
	glEndList();
	#pragma endregion

	#pragma region hierba
	hierba = glGenLists(1);
	glNewList(hierba, GL_COMPILE);
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glPushMatrix();
	Vec3 pos[] = {Vec3(0,0,0), Vec3(0.5,0,0), Vec3(0.5,0,0.5), Vec3(0,0,0.5), Vec3(-0.5,0,0.5), Vec3(-0.5,0,0), Vec3(-0.5,0,-0.5), Vec3(0,0,-0.5), Vec3(0.5,0,-0.5) };
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			glBegin(GL_TRIANGLE_STRIP);
			glVertex3f(pos[i].x + pos[j].x, 0, pos[i].z + pos[j].z);
			glVertex3f(0.05f + pos[i].x + pos[j].x, 0, pos[i].z + pos[j].z);
			glVertex3f(0.05f / 2 + pos[i].x + pos[j].x, 0.05f, pos[i].z + pos[j].z);
			glEnd();

			glBegin(GL_TRIANGLE_STRIP);
			glVertex3f(pos[i].x + pos[j].x, 0, pos[i].z + pos[j].z);
			glVertex3f(-0.05f + pos[i].x + pos[j].x, 0, 0.05f + pos[i].z + pos[j].z);
			glVertex3f(-0.05f / 2 + pos[i].x + pos[j].x, 0.05f, 0.05f/2 + pos[i].z + pos[j].z);
			glEnd();

			glBegin(GL_TRIANGLE_STRIP);
			glVertex3f(pos[i].x + pos[j].x, 0, pos[i].z + pos[j].z);
			glVertex3f(pos[i].x + pos[j].x, 0, -0.05f + pos[i].z + pos[j].z);
			glVertex3f(+ pos[i].x + pos[j].x, 0.05f, -0.05f/2 + pos[i].z + pos[j].z);
			glEnd();
		}
	}
	glPopMatrix();

	glPopAttrib();
	glEndList();
	#pragma endregion

	//Inicializadores
	setPeople(NUM_PEOPLE);
	setHierba();
}

void movePeople(float delta) {
	for (int i = 0; i < NUM_PEOPLE; i++) {
		people[i].position += people[i].direction * delta * speed / 4;
		if (people[i].position.norm() > 10) { //si se salen de los límites
			people[i].position = people[i].position.normalize() * 10;		 //Les fijamos en los límites
			people[i].direction = Vec3(rand() % 10 - 5, 0, rand() % 10 - 5); //Nueva dirección
		}
		if (people[i].position.norm() < 3) {  //si se acercan mucho al pulpo
			people[i].position = people[i].position.normalize() * 3;		 //Les fijamos en los límites
			people[i].direction = Vec3(rand() % 10 - 5, 0, rand() % 10 - 5); //Nueva dirección
		}
	}
}

void update() {

	//Animación coherente con el tiempo transcurrido (sin control de FPS)
	static const float omega = 0.1; //omega grados por segundo
	static int hora_anterior = glutGet(GLUT_ELAPSED_TIME);
	int hora_actual = glutGet(GLUT_ELAPSED_TIME);
	float delta = (hora_actual - hora_anterior) / 1000.0f;

	delta *= speed;

	alfa += omega * delta;

	//La rotación va dictada por el estado de funcionamiento de la máquina
	if (funcionamiento == funcionando) {
		rotacionDedos += 1 * delta;
		rot += 0.3 * delta;
	}
	else if (funcionamiento == bajandoBrazos) {
		if (!brazosParesBajados) {
			rotacionDedSUBIR_BAJAR_PARES += 4 * delta;
		}		
		if (!brazosImpaBajados) {
			rotacionDedSUBIR_BAJAR_IMPARES += 4 * delta;
		}
	}
	else if (funcionamiento == subiendoBrazos) {
		if (!brazosParesSubidos) {
			rotacionDedSUBIR_BAJAR_PARES -= 4 * delta;
		}
		if (!brazosImpaSubidos) {
			rotacionDedSUBIR_BAJAR_IMPARES -= 4 * delta;
		}
	}
	hora_anterior = hora_actual;

	//Movemos las personas una vez cada frame
	movePeople(delta);

	glutPostRedisplay();
}

void onTimer(int cuentaAtras) {
	glutTimerFunc(cuentaAtras, onTimer, cuentaAtras);
	estadoAnimacion += 1;
}


void display() {
	glClearColor(0.1, 0.9, 1, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glPolygonMode(GL_BACK, GL_LINE);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	// Variables auxiliares para la contrucción del pulpo

	float posx = 0;
	float posy = 0;
	float posz = 0;

	float vistax = 0;
	float vistay = 1;
	float vistaz = 0;

	Vec3 posDedos(0, 0, 0);
	Vec3 posVagon(0, 0, 0);
	Vec3 posLookAt(0, 0, 0);

	/*********************************************************************************
	** Posición de la cámara en cada posible animación								**
	** El estado de animación se inicia arriba y se modifica con la funcion onTimer **
	*********************************************************************************/

	#pragma region posicion de camara
	switch (estadoAnimacion) {
		case 0: //Rotación básica sentido horario
			distCam = 2; alturaCam = 2;
			gluLookAt(2 * cos(alfa) * distCam, alturaCam, 2 * sin(alfa) * distCam, 0, 1, 0, 0, 1, 0);
			break;
		case 1: //Rotación top down
			distCam = 5; alturaCam = 15;
			gluLookAt(2 * cos(alfa/2) * distCam, alturaCam, 2 * sin(alfa/2) * distCam, 0, 1, 0, 0, 1, 0);
			break;
		case 2: //Rotación básica sentido antihorario
			distCam = 2; alturaCam = 2;
			gluLookAt(2 * cos(-alfa) * distCam, alturaCam, 2 * sin(-alfa) * distCam, 0, 1, 0, 0, 1, 0);
			break;
		case 3: //Desde el suelo
			distCam = 2.5f; alturaCam = 0.25f;
			gluLookAt(distCam, alturaCam, -distCam * 2, 0, 1, 0, 0, 1, 0);
			break;

		case 4: //Cámara desde la vagoneta

			//Transformaciones requeridas para colocar la cámara en un vagón de manera aproximada
			//Vector relativo a la posición de los dedos respecto al centro
			posDedos += Vec3(-sin(0) * 0.1f, 0.9f + 1.8f, -cos(0) * 0.1f);
			posDedos += Vec3(lastx * 2, lastx * 2, lastz * 2);
			posDedos.rotY(sin(rot) * 2 * PI);
			posDedos.rotY(27.5f * PI / 180);

			// En el caso que la máquina esté en funcionamiento o parada
			if (funcionamiento == funcionando)
				posDedos += Vec3(0, -sin(-rotacionDedos) - 0.5f, 0);
			else
				posDedos += Vec3(0, -sin(-rotacionDedSUBIR_BAJAR_PARES) - 0.5f, 0);

			posDedos = Vec3(posDedos.x * 0.75 * 0.5, posDedos.y * 0.75 * 0.5, posDedos.z * 0.75 * 0.5);

			//Vector relativo a la posición de la vagoneta respecto al centro de los dedos
			posVagon += Vec3(cos(PI / 2), 0, sin(PI / 2));

			// En el caso que la máquina esté en funcionamiento o parada
			if (funcionamiento == funcionando)
				posVagon.rotY(sin(rotacionDedos) * 2 * PI);

			posVagon.rotY(sin(rot) * 2 * PI);
			posVagon.rotY((27.5f) * PI / 180);
			posVagon += Vec3(0, -.4f, 0);
			posVagon = Vec3(posVagon.x / 3, posVagon.y / 3, posVagon.z / 3);

			//Dirección hacia la que miraremos
			posLookAt = posVagon;
			posLookAt.rotY(PI / 8);

			//cout << posDedos.x << " \t " << posDedos.y << " \t " << posDedos.z << "\n";

			asientox = posDedos.x + posVagon.x;
			asientoy = posDedos.y + posVagon.y;
			asientoz = posDedos.z + posVagon.z;

			//-----------------------------------------------------Posicion Camara Vagoneta----------------------------------------------------------------//
			gluLookAt(asientox, asientoy, asientoz, posDedos.x + posLookAt.x, posDedos.y + posLookAt.y, posDedos.z + posLookAt.z, 0, 1, 0);
			//-----------------------------------------------------Posicion Camara Vagoneta----------------------------------------------------------------//

			break;

		case 5: //Cara del pulpo
			distCam = 2; alturaCam = 3;
			gluLookAt(cos(-sin(rot) * 2 * PI + PI / 2) * distCam, alturaCam, sin(-sin(rot) * 2 * PI + PI / 2) * distCam, 0, 1, 0, 0, 1, 0);
			break;
		case 6: //Dedos
			distCam = 2.2 - 2.2 * cos(0) / 5;
			alturaCam = -sin(rotacionDedos) / 2 + 0.75 + 4;

			posx = cos(-sin(rot) * 2 * PI + PI / 10) * distCam;
			posy = alturaCam;
			posz = sin(-sin(rot) * 2 * PI + PI / 10) * distCam;

			vistax = cos(-sin(rot) * 2 * PI + PI / 10) * distCam;
			vistay = -1000;
			vistaz = sin(-sin(rot) * 2 * PI + PI / 10) * distCam;

			gluLookAt(posx, alturaCam, posz, 0, vistay, 0, 0, 1, 0);
			break;

		default:
			distCam = 2; alturaCam = 2;
			gluLookAt(2 * cos(alfa) * distCam, alturaCam, 2 * sin(alfa) * distCam, 0, 1, 0, 0, 1, 0);
			estadoAnimacion = 0;
			break;
	
	}
	#pragma endregion

	float expansion = 15;
	int num = 8;

	glCallList(parteCentral);

	glPushMatrix(); // INICIO Matriz que contiene cabeza de pulpo y brazos

	#pragma region cabeza pulpo

	glRotatef(sin(rot) * 360, 0, 1, 0); //Rotación de todo (brazos + pulpo central)
	glPushMatrix();
	glTranslatef(0, 1.8, 0);
	glColor3f(0, 0, 0.75);
	glCallList(pulpo);
	glPopMatrix();

	#pragma endregion

	glRotatef(27.5, 0, 1, 0);

	# pragma region Dibujado de brazos + dedos

	//Dibujado y movimiento de brazos y dedos del pulpo
	for (int i = 0; i <= num; i++) {

		glPushMatrix();
		glTranslatef(-sin(i * 2 * PI / num) * 0.1, 0.9, -cos(i * 2 * PI / num) * 0.1);

		glRotatef(i * 360/ num, 0, 1, 0);
		glRotatef(-15, 1, 0, 0);

		//Rotación de cada dedo del pulpo
		if (funcionamiento == funcionando) {
			if (i % 2 == 0)
				glRotatef(sin(rotacionDedos) * expansion, 1, 0, 0);
			else
				glRotatef(sin(rotacionDedos+PI) * expansion, 1, 0, 0);
		}
		else if (funcionamiento == bajandoBrazos) {
			if (i % 2 == 0 && sin(rotacionDedSUBIR_BAJAR_PARES) > -0.9f) {
				glRotatef(sin(rotacionDedSUBIR_BAJAR_PARES) * expansion, 1, 0, 0);
			}
			else if (i % 2 == 0 && funcionamiento == bajandoBrazos && sin(rotacionDedSUBIR_BAJAR_PARES) <= -0.9f) {
				if (!brazosParesBajados) {
					brazosParesBajados = true;
					cout << "brazosParesBajados\n";
				}
				glRotatef(sin(rotacionDedSUBIR_BAJAR_PARES)* expansion, 1, 0, 0);
			}

			if (i % 2 == 1 && sin(rotacionDedSUBIR_BAJAR_IMPARES+PI) > -0.9f) {
				glRotatef(sin(rotacionDedSUBIR_BAJAR_IMPARES+PI) * expansion, 1, 0, 0);
			}
			else if (i % 2 == 1 && funcionamiento == bajandoBrazos && sin(rotacionDedSUBIR_BAJAR_IMPARES+PI) <= -0.9f) {
				if (!brazosImpaBajados) {
					brazosImpaBajados = true;
					cout << "brazosImparesBajados\n";
				}
				glRotatef(sin(rotacionDedSUBIR_BAJAR_IMPARES+PI)* expansion, 1, 0, 0);
			}
		}
		else if (funcionamiento == subiendoBrazos) {
			if (i % 2 == 0 && !(sin(rotacionDedSUBIR_BAJAR_PARES) < sin(rotacionDedos) + 0.1f && sin(rotacionDedSUBIR_BAJAR_PARES) > sin(rotacionDedos) - 0.1f)) {
				glRotatef(sin(rotacionDedSUBIR_BAJAR_PARES) * expansion, 1, 0, 0);
			}
			else if (i % 2 == 0 && funcionamiento == subiendoBrazos && sin(rotacionDedSUBIR_BAJAR_PARES) < sin(rotacionDedos) + 0.1f && sin(rotacionDedSUBIR_BAJAR_PARES) > sin(rotacionDedos) - 0.1f) {
				if (!brazosParesSubidos) {
					brazosParesSubidos = true;
					cout << "brazosParesSubidos" << rotacionDedos << ", " << rotacionDedSUBIR_BAJAR_PARES << "\n";
				}
				glRotatef(sin(rotacionDedos) * expansion, 1, 0, 0);
			}
			if (i % 2 == 1 &&  !(sin(rotacionDedSUBIR_BAJAR_IMPARES + PI) < sin(rotacionDedos + PI) + 0.1f && sin(rotacionDedSUBIR_BAJAR_IMPARES + PI) > sin(rotacionDedos + PI) - 0.1f)) {
				glRotatef(sin(rotacionDedSUBIR_BAJAR_IMPARES + PI) * expansion, 1, 0, 0);
			}
			else if (i % 2 == 1 && funcionamiento == subiendoBrazos && sin(rotacionDedSUBIR_BAJAR_IMPARES + PI) < sin(rotacionDedos + PI) + 0.1f && sin(rotacionDedSUBIR_BAJAR_IMPARES + PI) > sin(rotacionDedos + PI) - 0.1f) {
				if (!brazosImpaSubidos) {
					brazosImpaSubidos = true;
					cout << "brazosImpaSubidos" << rotacionDedos + PI << "," << rotacionDedSUBIR_BAJAR_IMPARES << "\n";
				}
				glRotatef(sin(rotacionDedos + PI) * expansion, 1, 0, 0);
			}
		}
		glScalef(0.75, 0.75, 0.75);

		glCallList(brazo);

		glTranslatef(lastx, lasty, lastz);
		glRotatef(15, 1, 0, 0); //Hacemos que estén recas

		

		if (i % 2 == 0) {
			if (funcionamiento == funcionando) {
				glRotatef(-sin(rotacionDedos) * expansion, 1, 0, 0);
			}
			else {
				glRotatef(-sin(rotacionDedSUBIR_BAJAR_PARES) * expansion, 1, 0, 0);
			}
		}
		else if (i % 2 == 1) {
			if (funcionamiento == funcionando) {
				glRotatef(-sin(rotacionDedos + PI) * expansion, 1, 0, 0);
			}
			else {
				glRotatef(-sin(rotacionDedSUBIR_BAJAR_IMPARES + PI) * expansion, 1, 0, 0);
			}
		}
		if (i % 2 == 0)
			glRotatef(sin(rotacionDedos) * 360, 0, 1, 0);
		else
			glRotatef(cos(rotacionDedos) * 360, 0, 1, 0);
		glScalef(0.5, 0.5, 0.5);

		glCallList(dedos);
		
		glPopMatrix();
	}

	# pragma endregion

	glPopMatrix(); // FIN Matriz que contiene cabeza de pulpo y brazos

	#pragma region Suelo
	glColor3f(0.1,0.5, 0);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0, 0, 0);

	for (int i = 0; i <= 50; i++) {
		glVertex3f(sin(i * 2 * PI / 50)*10, 0, cos(i * 2 * PI / 50)*10);
	}
	glEnd();

	glColor3f(0.6, 0.6, 0.1);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0, 0, 0);

	for (int i = 0; i <= 50; i++) {
		glVertex3f(sin(i * 2 * PI / 50) * 2, 0.01, cos(i * 2 * PI / 50) * 2);
	}
	glEnd();
	#pragma endregion

	#pragma region arboles
	for (int i = 0; i <= 100; i++) {
		glPushMatrix();
		glTranslatef(sin(i * 2 * PI / 100) * 10, 0, cos(i * 2 * PI / 100) * 10);
		glCallList(tree);
		glPopMatrix();
	}

	Vec3 posTrees[7] = {Vec3(3,0,1), Vec3(-1,0,-4), Vec3(-1,0,6), Vec3(-7,0,3), Vec3(-8,0,-3), Vec3(7,0,4) , Vec3(5,0,-5)};

	for (int t = 0; t < 7; t++) {
		glPushMatrix();
		glTranslatef(posTrees[t].x, posTrees[t].y, posTrees[t].z);
		glCallList(tree);
		glPopMatrix();
	}
	#pragma endregion

	#pragma region personas
	for (int i = 0; i < NUM_PEOPLE; i++) {
		PersonStruc pers = people[i];

		glPushMatrix();
		glTranslatef(pers.position.x, pers.position.y, pers.position.z);
		glColor3f(pers.color_camiseta.x, pers.color_camiseta.y, pers.color_camiseta.z);
		glCallList(person);

		glPopMatrix();
		glPopAttrib();
		glPopMatrix();
	}
	#pragma endregion

	#pragma region Semaforo
	for (int i = 0; i < 4; i++){
		glPushMatrix();
		glTranslatef(cos(i * 2 * PI / 4) * 2.5f, 0, sin(i * 2 * PI / 4) * 2.5f);
		if (funcionamiento == funcionando)
			glColor3f(0, 1, 0);
		else if (funcionamiento == bajandoBrazos)
			glColor3f(1, 0, 0);
		else if (funcionamiento == subiendoBrazos)
			glColor3f(1, 1, 0);
		glRotatef((i-2) * 360 / 4 * float(i % 2) + i * 360 / 4 * float((i+1) % 2), 0, 1, 0);
		glScalef(0.5f,0.5f,0.5f);
		glCallList(semaforo);
		glPopMatrix();
	}
	#pragma endregion

	for (int i = 0; i < NUM_HIERBA; i++) {
		glPushMatrix();
		glTranslatef(hierbaLista[i].position.x, 0, hierbaLista[i].position.z);
		glRotatef(hierbaLista[i].rotation,0,1,0);
		glColor3f(0, hierbaLista[i].colorVerde, 0);
		glCallList(hierba);
		glPopMatrix();
	}

	glutSwapBuffers(); //Para los 2 buffers (incorpora glflush())
}



void reshape(GLint w, GLint h) {
	float ra = (float)w / (float)h;

	//fijado de dimensiones del marco
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	gluPerspective(60, ra, 0.125f, 200);
}

void subirBrazos(int n) {
	cout << "subiendo brazos " << n << "\n";
	brazosParesSubidos = false;
	brazosImpaSubidos = false;
	funcionamiento = subiendoBrazos;

}

void parar(int n) {
	cout << "bajando brazos" << n << "\n";
	brazosParesBajados = false;
	brazosImpaBajados = false;
	rotacionDedSUBIR_BAJAR_IMPARES = rotacionDedos; cout << rotacionDedSUBIR_BAJAR_IMPARES << "\n";
	rotacionDedSUBIR_BAJAR_PARES = rotacionDedos; cout << rotacionDedSUBIR_BAJAR_PARES << "\n";
	funcionamiento = bajandoBrazos;
	glutTimerFunc(n/2, subirBrazos, n/2);
}


void vuelta(int n) {
	cout << "funcionando " << n << "\n";
	funcionamiento = funcionando;
	glutTimerFunc(n-n/3, parar, n-2*n/3);
	glutTimerFunc(n, vuelta, n);
	
}

int main(int argc, char** argv) {
	// Inicializaciones
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(50, 50);

	// Crear ventana
	glutCreateWindow(PROYECTO);

	init();
	//Registrar callbacks
	//glutTimerFunc(1000, onTimer, 1000); //Cambio de animación
	glutTimerFunc(10000, onTimer, 10000); //Cambio de animación
	glutTimerFunc(0, vuelta, 27000); //Cambio de animación

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(update);		//FPS flexibles

	//Bucle de atención a eventos
	glutMainLoop();


}