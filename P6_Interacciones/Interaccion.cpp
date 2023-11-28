/***************************************************
*	Interaccion.cpp
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
static float z0 = 1;
static cb::Sistema3d player;

static float speedForward;
static const float ACCELERATION = 0.001;
static const float MAX_SPEED = 0.1;

static Vec3 posPlayer;

float girox; float giroy;

int xanterior;
int yanterior;

// Funcion de inicializacion propia
void init()
{
	cout << glGetString(GL_VERSION) << endl;
	// Canfigurar el motor
	glEnable(GL_DEPTH_TEST);

	#pragma region suelo lista
	suelo = glGenLists(1);
	glNewList(suelo, GL_COMPILE);
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glColor3f(1, 1, 1);

	for (int i = 0; i < DIM_PLATAFORMA; i++) {
		glBegin(GL_TRIANGLE_STRIP);
		for (int j = 0; j < DIM_PLATAFORMA; j++) {
			glVertex3f(i-DIM_PLATAFORMA/2, j - DIM_PLATAFORMA / 2, 0);
			glVertex3f(i + 1 - DIM_PLATAFORMA / 2, j - DIM_PLATAFORMA / 2, 0);
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

	posPlayer = Vec3(0,0,z0); //Posicion global player
}

void update() {

	//Animación coherente con el tiempo transcurrido (sin control de FPS)
	static const float omega = 0.1; //omega grados por segundo
	static int hora_anterior = glutGet(GLUT_ELAPSED_TIME);
	int hora_actual = glutGet(GLUT_ELAPSED_TIME);

	float delta = (hora_actual - hora_anterior) / 1000.0f;

	Vec3 w = player.getw();
	posPlayer += Vec3(-w.x * speedForward * delta, -w.y * speedForward * delta, -w.z * speedForward * delta);
	
	cout << speedForward << "\n";
	cout << "rotaciones " << girox << ":" << giroy;
	glutPostRedisplay();
}

// Funcion de atencion al evento de dibujo
void display()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	Vec3 origen = player.geto();
	origen += posPlayer;

	Vec3 lookAt = origen - player.getw(); //w
	Vec3 up = player.getv();


	//Posici n de la c mara (eyex, eyey, eyez) | centro punto al que se mira (centerx, centery, centerz) | (upx, upy, upz)
	//gluLookAt(origen.x, origen.y, origen.z, lookAt.x - giroy, lookAt.y, lookAt.z - girox, up.x, up.y, up.z); //Desde el frente

	gluLookAt(origen.x, origen.y, origen.z, lookAt.x, lookAt.y, lookAt.z, up.x, up.y, up.z); //Desde el frente
	//gluLookAt(0, 2, 0, 0, 0, 0, 1, 0, 0); //Desde arriba
	ejes();

	glCallList(suelo);

	glutSwapBuffers();
}

// Funcion de atencion al redimensionamiento
void reshape(GLint w, GLint h)
{
	//problema se deforma (no hay isometria == las medidas no se corresponden al variar el  rea)
	//soluci n:
	//Relaci n de aspecto
	float ra = (float)w / (float)h;

	//fijado de dimensiones del marco
	glViewport(0, 0, w, h);

	//fija en la esquina superior derecha
	//glViewport(w / 2, h / 2, w / 2, h / 2);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//gluPerspective(apertura vertical, raz n de aspecto (alto:ancho), distancia del plano cercano, distancia del plano alejado); 
	gluPerspective(60, ra, 0.1, 30);
}

void select() {
	// Hace lo mismo que display pero dibujando los objetos que son seleccionablesde un color diferente cada uno
	// y no intercabia los buffers
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Posici n de la c mara (eyex, eyey, eyez) | centro punto al que se mira (centerx, centery, centerz) | (upx, upy, upz)
	gluLookAt(2, 2, 2, 0, 0, 0, 0, 1, 0); //Desde el frente
	//gluLookAt(0, 2, 0, 0, 0, 0, 1, 0, 0); //Desde arriba
	//ejes() no son seleccionables;

	

	//Siempre solida
	glutSolidTeapot(0.5);

	glPopMatrix();

	//Notese que no dibujamos cambio de buffers ni redisplay
}

void acelerar() {
	if (speedForward < MAX_SPEED)
		speedForward += ACCELERATION; //Tiene que ser coerente con el tiempo FALTA
}
void frenar() {
	if (speedForward > -MAX_SPEED)
		speedForward -= ACCELERATION; //Tiene que ser coerente con el tiempo FALTA
}

void onKey(unsigned char tecla, int x, int y) {
	switch (tecla) {
	case 'a':
		acelerar();
		break;
	case 'z':
		frenar();
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
	select();

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

	girox = (y - yanterior) * pixel2grados;
	giroy = (x - xanterior) * pixel2grados;

	player.rotar(-girox, Vec3(player.getu()));
	player.rotar(-giroy, Vec3(player.getv()));

	xanterior = x;
	yanterior = y;
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

	// Registrar callbacks
	glutKeyboardFunc(onKey);
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
