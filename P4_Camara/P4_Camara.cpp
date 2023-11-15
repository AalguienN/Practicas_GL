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

static GLuint estrella;
static const float radio = 1;

static float a;


void init() {
	estrella = glGenLists(1);

	glEnable(GL_DEPTH_TEST);

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
}

void display() {
	glClearColor(1, 1, 1, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	a += PI / 2 / 90;

	gluLookAt(2*cos(a), 1, 2*sin(a), 0, 0, 0, 0, 1, 0); //Desde el frente
	//gluLookAt(0, 0, 5, 0, 0, 0, sin(a), cos(a), 0); //Desde el frente

	glColor3f(0, 0, 0.8);
	glutWireSphere(1, 20, 20);

	// 1
	for (int i = 0; i <= 5; i++)
	{
		glColor3f(1 - ((float)i) * (1. / 6.), 1- ((float)i) * (1. / 6.), ((float)i) * (1. / 6.));
		glPushMatrix();
		glRotatef(30 * i, 0, 1, 0);
		glCallList(estrella);
		glPopMatrix();
	}

	glFinish();
}

void fun(int value) {
	glutPostRedisplay();
	glutTimerFunc(10, fun, 0);
}

void reshape(GLint w, GLint h) {
	float ra = (float)w / (float)h;

	//fijado de dimensiones del marco
	glViewport(0, 0, w, h);

	//fija en la esquina superior derecha
	//glViewport(w / 2, h / 2, w / 2, h / 2);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	/*
	if (ra < 1)
		glOrtho(-1, 1, -1 / ra, 1 / ra, -10, 30);
	else
		glOrtho(-1 * ra, 1 * ra, -1, 1, -10, 30);

	*/
	
	gluPerspective(60, ra, 0.1, 10);

	
}

int main(int argc, char** argv) {
	
	a = 0;

	// Inicializaciones
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(50, 50);
	glutTimerFunc(100, fun, 0);

	// Crear ventana
	glutCreateWindow(PROYECTO);

	init();
	//Registrar callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	//Bucle de atención a eventos
	glutMainLoop();


}