/***************************************************
*	Practica 1
*
*	SGI
*
*	@author	A.Camacho <acamgar1@etsinf.upv.es
*
***************************************************/

#define PROYECTO "Banana"

#include <iostream>	
#include <codebase.h>

static GLuint banana;
static const float radio = 1;

static float a;


void init() {
	banana = glGenLists(1);

	glEnable(GL_DEPTH_TEST);

	glNewList(banana, GL_COMPILE);

	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glColor3f(0.25, 0.5, 0);

	float posx = -0.25;

	glPushMatrix();
	glTranslatef(0.25 + posx, 1, 0);
	glutSolidSphere(0.1, 20, 20);
	glPopMatrix();

	glColor3f(1, 1, 0);

	glBegin(GL_TRIANGLE_STRIP);

	
	glVertex3f(0.25+posx, 1, 0);

	glVertex3f(0 + posx, 0.25, 0);
	glVertex3f(-0.25 + posx, 0.5, 0);

	glVertex3f(0 + posx, 0, 0);
	glVertex3f(-0.3 + posx, 0, 0);

	glVertex3f(0 + posx, -0.25, 0);
	glVertex3f(-0.25 + posx, -0.5, 0);

	glVertex3f(0.25 + posx, -1, 0);
	
	glEnd();
	glPopAttrib();

	glEndList();
}

void display() {
	glClearColor(0, 0, 0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	a += PI / 2 / 90;

	gluLookAt(2 * (double)cos(a), 0, 2 * (double)sin(a), 0, 0, 0, 0, 1, 0); //Desde el frente
	//gluLookAt(0, 0, 5, 0, 0, 0, sin(a), cos(a), 0); //Desde el frente



	// 1
	for (int i = 0; i <= 90; i++)
	{
		glColor3f(((float)i) / 90 * 0.8, ((float)i) / 90 * 0.8, 0);
		glPushMatrix();
		glRotatef(((float)i)/((float)2), 0, 1, 0);
		glCallList(banana);
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