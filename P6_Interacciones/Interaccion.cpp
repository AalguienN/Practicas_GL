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
#include <codebase.h>

using namespace std;
using namespace cb;

//Listas de dibujo
static GLuint suelo;

//Globales
const int DIM_PLATAFORMA = 200;
const int VERTICES_POR_UNIDAD = 4;
static float z0 = 1;
static Sistema3d player;

static float speedForward;
static const float ACCELERATION = 0.1;
static const float MAX_SPEED = 10;

static Vec3 posPlayer;

float girox; float giroy;

static int xanterior;
static int yanterior;

static int windowWidth;
static int windowHeight;

static bool luces = true;

// Funcion de inicializacion propia
void init()
{
	cout << glGetString(GL_VERSION) << endl;
	
	#pragma region Luces

	static const GLfloat Ambient[4] = { 0.2,0.2,0.2,1 };

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, Ambient);

	glLightfv(GL_LIGHT0, GL_AMBIENT, GRISOSCURO);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, GRISCLARO);
	glLightfv(GL_LIGHT0, GL_SPECULAR, GRISCLARO);	
	
	glLightfv(GL_LIGHT1, GL_AMBIENT, NEGRO);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, GRISCLARO);
	glLightfv(GL_LIGHT1, GL_SPECULAR, GRISCLARO);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 10.0f);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 10.0F);


	glLightfv(GL_LIGHT2, GL_AMBIENT, NEGRO);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, GRISCLARO);
	glLightfv(GL_LIGHT2, GL_SPECULAR, GRISCLARO);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 10.0f);
	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 10.0F);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);


	#pragma endregion

	glEnable(GL_DEPTH_TEST);	//Profundidad
	glEnable(GL_LIGHTING);		//Iluminación
	glEnable(GL_NORMALIZE);

	#pragma region suelo lista

	suelo = glGenLists(1);
	glNewList(suelo, GL_COMPILE);
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	//glColor3f(1, 1, 1);

	for (int i = 0; i < DIM_PLATAFORMA * VERTICES_POR_UNIDAD; i++) {
		glBegin(GL_QUAD_STRIP);
		for (int j = 0; j < DIM_PLATAFORMA * VERTICES_POR_UNIDAD; j++) {
			glNormal3f(0, 0, 1);
			glVertex3f((i-DIM_PLATAFORMA/2)/ float(VERTICES_POR_UNIDAD), (j - DIM_PLATAFORMA / 2) / float(VERTICES_POR_UNIDAD), 0);
			glNormal3f(0, 0, 1);
			glVertex3f((i + 1 - DIM_PLATAFORMA / 2) / float(VERTICES_POR_UNIDAD), (j - DIM_PLATAFORMA / 2) / float(VERTICES_POR_UNIDAD), 0);
		}
		glEnd();
	}


	glPopAttrib();
	glEndList();

	#pragma endregion

	player.seto(Vec3(0, 0, 0));  //Posicion Local
	player.setu(Vec3(1, 0, 0));  //x
	player.setw(Vec3(0, -1, 0)); //y
	player.setv(Vec3(0, 0, 1));  //z
	speedForward = 0;

	posPlayer = Vec3(0,0,z0); //Posicion inicial global player
}

void update() {

	//Coherente con el tiempo transcurrido (sin control de FPS)
	static int hora_anterior = glutGet(GLUT_ELAPSED_TIME);
	int hora_actual = glutGet(GLUT_ELAPSED_TIME);

	float delta = (hora_actual - hora_anterior) / 1000.0f;

	Vec3 w = player.getw();
	posPlayer += Vec3(-w.x * speedForward * delta, -w.y * speedForward * delta, -w.z * speedForward * delta);

	hora_anterior = hora_actual;
	
	glutPostRedisplay();
}

// Funcion de atencion al evento de dibujo
void display()
{
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
	origen += posPlayer;

	Vec3 lookAt = origen - player.getw(); //w
	Vec3 up = player.getv();

	Vec3 faro1 =  Vec3(0,0,0) + player.getu()*0.25f;
	Vec3 faro2 =  Vec3(0,0,0) - player.getu()*0.25f;

	static GLfloat posFaro1[] = { faro1.x, faro1.y, faro1.z, 1 }; //PosSpot
	glLightfv(GL_LIGHT1, GL_POSITION, posFaro1);
	GLfloat dir_central1[] = { 0.1f, 0, -1.0f };
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, dir_central1);

	static GLfloat posFaro2[] = { faro2.x, faro2.y, faro2.z, 1 }; //PosSpot
	glLightfv(GL_LIGHT2, GL_POSITION, posFaro2);
	GLfloat dir_central2[] = { -0.1f, 0, -1.0f };
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, dir_central2);
	#pragma endregion

	gluLookAt(origen.x, origen.y, origen.z, lookAt.x, lookAt.y, lookAt.z, up.x, up.y, up.z); //Desde el frente

	//ejes();

	static GLfloat posicionEstrella[] = { 50,0,100,0}; //Direccional
	glLightfv(GL_LIGHT0, GL_POSITION, posicionEstrella);

	/*Faros posiciones*/
	/*
	glPushMatrix();
	glTranslatef(faro1.x + posPlayer.x, faro1.y + posPlayer.y, faro1.z + posPlayer.z);
	glutSolidSphere(0.2f, 8, 8);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(faro2.x + posPlayer.x, faro2.y + posPlayer.y, faro2.z + posPlayer.z);
	glutSolidSphere(0.2f, 8, 8);
	glPopMatrix();
	*/

	/*Suelo*/
	glMaterialfv(GL_FRONT, GL_DIFFUSE, GRISOSCURO);
	glMaterialfv(GL_FRONT, GL_SPECULAR, GRISOSCURO);
	glMaterialf(GL_FRONT, GL_SHININESS, 0);

	glCallList(suelo);

	/*Esfera*/
	glMaterialfv(GL_FRONT, GL_DIFFUSE, AZUL);
	glMaterialfv(GL_FRONT, GL_SPECULAR, AZUL);
	glMaterialf(GL_FRONT, GL_SHININESS, 1000);

	glPushMatrix();
	glTranslatef(0,10,0);
	glutSolidSphere(5,50,50);
	glPopMatrix();

	/*Donut*/
	glMaterialfv(GL_FRONT, GL_DIFFUSE, VERDE);
	glMaterialfv(GL_FRONT, GL_SPECULAR, VERDE);
	glMaterialf(GL_FRONT, GL_SHININESS, 1000);

	glPushMatrix();
	glTranslatef(20, 10, 6);
	glutSolidTorus(1, 5, 50,50);
	glPopMatrix();

	/*Taza*/
	glMaterialfv(GL_FRONT, GL_DIFFUSE, ROJO);
	glMaterialfv(GL_FRONT, GL_SPECULAR, ROJO);
	glMaterialf(GL_FRONT, GL_SHININESS, 1000);

	glPushMatrix();
	glTranslatef(10, 20, 10);
	glRotatef(90, 1, 0, 0);
	glutSolidTeapot(5);
	glPopMatrix();

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
	gluPerspective(90, ra, 0.1, 1000);
}


void acelerar() {
	//cout << "acelerando";
	if (speedForward < MAX_SPEED)
		speedForward += ACCELERATION; //Tiene que ser coerente con el tiempo FALTA
}
void frenar() {
	//cout << "frenando";
	if (speedForward > -MAX_SPEED)
		speedForward -= ACCELERATION; //Tiene que ser coerente con el tiempo FALTA
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

	default:
		break;
	}
	glutPostRedisplay();

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
	case 27:
		exit(0);
	default:
		break;
	}
	glutPostRedisplay();

}
void onClick(int boton, int estado, int x, int y) {
	//Almacenar el pixel donde se hizo el click
	if (boton == GLUT_LEFT_BUTTON && estado == GLUT_DOWN) {

	}

	// Ver que punto del viewport corresponde al p xel del click
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport); // origen y extensi n del viewport
	//Si en alg n momento necesit ramos emplear otro viewport...
	GLfloat xv = x;
	GLfloat yv = viewport[3]/*Alto*/ - y;

	//Dibujar en el backbuffer los objetos s lidos y de un color diferente cada uno
	//select();

	//Leer del backbuffer el color que hay debajo del punto xv, yv
	//glReadPixels(xv, yv, 1, 1, GL_BLUE, GL_UNSIGNED_BYTE, objeto);
	glutPostRedisplay();
}

void onDrag(int x, int y) {
	static float pixel2grados = 1.0f;
	/*
	//al mover el raton hacia la derecha la x del p xel aumenta y el giro es alrededor del eje y positivo
	giroy += (x - xanterior) * pixel2grados;

	//al movel el raton hacia abajo la y de los p xeles aumenta (recordemos que ba de arriba a abajo) y el giro es alrededor de x positivo
	girox += (y - yanterior) * pixel2grados;
	cout << "x:" << girox << "\t y:" << giroy << "\n";
	*/

	glutPostRedisplay();
}


void onPassiveMotiotion(int x, int y) {
	static float pixel2grados = 0.01f;
	glutWarpPointer(windowWidth / 2, windowHeight / 2);

	//Giramos siendo coherentes con el tiempo
	static int hora_anterior = glutGet(GLUT_ELAPSED_TIME);
	int hora_actual = glutGet(GLUT_ELAPSED_TIME);
	float delta = (hora_actual - hora_anterior)/100.0f;

	girox = (y - yanterior) * pixel2grados;
	giroy = (x - xanterior) * pixel2grados;

	player.rotar(-girox*delta, Vec3(player.getu()));
	player.rotar(-giroy*delta, Vec3(player.getv()));

	//xanterior = x;
	//yanterior = y;

	//Fija el cursor en la posición central de la pantalla
	glutWarpPointer(windowWidth / 2, windowHeight / 2);

	xanterior = windowWidth / 2;
	yanterior = windowHeight / 2;
	hora_anterior = hora_actual;
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

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	// Inicio propio y del bucle de eventos
	init();
	glutMainLoop();
}
