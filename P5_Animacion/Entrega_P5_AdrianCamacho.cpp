/***************************************************
*	Practica 1
*
*	SGI
*
*	@author	A.Camacho <acamgar1@etsinf.upv.es
*
***************************************************/

#define PROYECTO "Camara"

#include <iostream>	
#include <codebase.h>

static int estadoAnimacion = 6;


// Distancia de la cámara
static float distCam = 2;
static float alturaCam = 2;

// Listas

// P3 y P4 estrella + esferaEstrella
static GLuint estrella;
static GLuint esferaEstrellaP4;

// Pulpo
static GLuint asiento;
static GLuint vagoneta;
static GLuint dedos;
static GLuint parteBrazo;
static GLuint brazo;
static GLuint parteCentral;
static GLuint pulpo;
static GLuint tree;

static float lastx;
static float lasty;
static float lastz;



static const float radio = 1;

static float alfa = 0;
static float rotacionDedos = 0;
static float anglBrazo = 0;
static float rot = 0;

void init() {

	int res = 0; //Variable auxiliar utilizada para calcular resoluciones

	glEnable(GL_DEPTH_TEST);

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
	glTranslatef(-0.75, 0, -0.75);
	glCallList(asiento);
	glTranslatef(1.5, 0, 0);
	glCallList(asiento);
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
	glCallList(esferaEstrellaP4);
	glTranslatef(0, 1, 0);
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

}

void update() {

	//Animación coherente con el tiempo transcurrido (sin control de FPS)
	static const float omega = 0.1; //omega grados por segundo
	static int hora_anterior = glutGet(GLUT_ELAPSED_TIME);
	int hora_actual = glutGet(GLUT_ELAPSED_TIME);
	float delta = (hora_actual - hora_anterior) / 1000.0f;

	alfa += omega * delta;
	rotacionDedos += 1 * delta;
	anglBrazo += 0.1 * delta;
	rot += 0.3 * delta;
	hora_anterior = hora_actual;

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

	float posx = 0;
	float posy = 0;
	float posz = 0;

	float vistax = 0;
	float vistay = 1;
	float vistaz = 0;

	//cout << estadoAnimacion;
	// Posición de la cámara en cada posible animación
	// El estado de animación se inicia arriba y se modifica con la funcion onTimer
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
		case 4: //Cara del pulpo
			distCam = 2; alturaCam = 3;
			gluLookAt(cos(-sin(rot) * 2 * PI + PI / 2) * distCam, alturaCam, sin(-sin(rot) * 2 * PI + PI / 2) * distCam, 0, 1, 0, 0, 1, 0);
			break;
		case 5: //Dedos
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
		case 6: //POV vagoneta
			//Hice pruebas y no funcionabaaaaaaaaaa alguna idea? Puedo realizar transformaciones de matriz sobre la cámara?
			distCam = -sin(0) * 0.1 -2.2 * cos(0)*0.75;
			alturaCam = 0.75 - sin(rotacionDedos) * 15 * PI / 180; cout << 0.5 << "+" << sin(rotacionDedos) * 15 * PI / 180  << "=" << alturaCam<< "\n";

			posx = cos(-sin(rot) * 2 * PI + 15 * PI / 180) * distCam + 0.35 * cos(-cos(rotacionDedos)*2*PI);
			posz = sin(-sin(rot) * 2 * PI + 15 * PI / 180) * distCam + 0.35 * sin(-cos(rotacionDedos)*2*PI);

			vistax = cos(-sin(rot) * 2 * PI + 15 * PI / 180) * distCam + 0.4 * cos(-cos(rotacionDedos) * 2 * PI - PI / 4);
			vistaz = sin(-sin(rot) * 2 * PI + 15 * PI / 180) * distCam + 0.4 * sin(-cos(rotacionDedos) * 2 * PI - PI / 4);

			//-4 - sin(30 * PI / 180), 4, 0 - cos(30 * PI / 180),

			gluLookAt(
				posx, alturaCam, posz,
				vistax, alturaCam, vistaz,
				0, 1, 0
			);

			//gluLookAt(5, 2, 0,0, 0, 0,0, 1, 0);
			glPushAttrib(GL_ALL_ATTRIB_BITS);
			glColor3f(0, 0, 0);
			glPushMatrix();
			glTranslatef(posx, alturaCam, posz);
			glutSolidCube(0.1);
			glPopMatrix();
			glPopAttrib();

			break;

		default:
			gluLookAt(2 * cos(alfa) * distCam, alturaCam, 2 * sin(alfa) * distCam, 0, 1, 0, 0, 1, 0);
			estadoAnimacion = 0;
			break;
	
	}

	float expansion = 15;
	int num = 8;
	glCallList(parteCentral);

	glPushMatrix();
	glRotatef(sin(rot)*360, 0, 1, 0); //Rotación de todo (brazos + pulpo central)
	glPushMatrix();
	glTranslatef(0, 1.8, 0);
	glColor3f(0, 0, 0.75);
	glCallList(pulpo);
	glPopMatrix();
	glRotatef(27.5, 0, 1, 0);

	# pragma region Dibujado de brazos + dedos

	//Conjunto de brazos + dedos
	for (int i = 0; i <= num; i++) {
		glPushMatrix();
		glTranslatef(-sin(i * 2 * PI / num) * 0.1, 0.9, -cos(i * 2 * PI / num) * 0.1);

		glRotatef(i * 360/ num, 0, 1, 0);
		glRotatef(-15, 1, 0, 0);

		//Rotación de cada dedo del pulpo
		if (i % 2 == 0)
			glRotatef(sin(rotacionDedos) * expansion, 1, 0, 0);
		else
			glRotatef(sin(rotacionDedos+PI) * expansion, 1, 0, 0);

		glScalef(0.75, 0.75, 0.75);

		glCallList(brazo);

		glTranslatef(lastx, lasty, lastz);
		glRotatef(15, 1, 0, 0); //Hacemos que estén recas

		if (i % 2 == 0)
			glRotatef(-sin(rotacionDedos) * expansion, 1, 0, 0);
		else
			glRotatef(-sin(rotacionDedos + PI) * expansion, 1, 0, 0);
		if (i % 2 == 0)
			glRotatef(sin(rotacionDedos) * 360, 0, 1, 0);
		else
			glRotatef(cos(rotacionDedos) * 360, 0, 1, 0);
		glScalef(0.5, 0.5, 0.5);
		glCallList(dedos);
		
		glPopMatrix();
	}
	glPopMatrix();

	# pragma endregion

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
	#pragma endregion
	glEnd();

	for (int i = 0; i <= 100; i++) {
		glPushMatrix();
		glTranslatef(sin(i * 2 * PI / 100) * 10, 0, cos(i * 2 * PI / 100) * 10);
		glCallList(tree);
		glPopMatrix();
	}

	glPushMatrix();
	glTranslatef(3, 0, 1);
	glCallList(tree);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1, 0, -4);
	glCallList(tree);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1, 0, 6);
	glCallList(tree);
	glPopMatrix();


;	//glCallList(brazo);
	//glCallList(dedos);
	//glCallList(vagoneta);
	//glCallList(asiento);

	glutSwapBuffers(); //Para los 2 buffers (incorpora glflush())
}
/*
void fun(int value) {
	glutPostRedisplay();
	glutTimerFunc(10, fun, 0);
}*/


void reshape(GLint w, GLint h) {
	float ra = (float)w / (float)h;

	//fijado de dimensiones del marco
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(60, ra, 0.1, 200);
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
	//glutTimerFunc(10000, onTimer, 10000); //Cambio de animación

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(update);		//FPS flexibles

	//Bucle de atención a eventos
	glutMainLoop();


}