/***************************************************
*	Practica 1
*
*	SGI
*
*	@author	A.Camacho <acamgar1@etsinf.upv.es
*
***************************************************/

#define PROYECTO "Mosaico"

#include <iostream>	
#include <codebase.h>

static GLuint estrella;
static GLuint estrellas;
static const float radio = 1;
float r;


void init() {
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


	//Lista estrellas

	estrellas = glGenLists(1);
	glNewList(estrellas, GL_COMPILE);

	glPushAttrib(GL_CURRENT_BIT);

	glPushMatrix();

	glRotatef(90, 0, 0, 1);
	glScalef(0.4, 0.4, 0.4);
	glCallList(estrella);

	glPopMatrix();

	glPushMatrix();

	glCallList(estrella);

	glPopMatrix();

	glPopAttrib();

	glEndList();





}

void display() {
	r += 10;
	glClearColor(1, 1, 1, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glPolygonMode(GL_BACK, GL_FILL);
	glColor3f(0, 0, 0.8);

	// 1
	glPushMatrix();
	glTranslatef(-0.5, 0.5, 0);
	glRotatef(15+r, 0, 0, 1);
	glScalef(0.5, 0.5, 0.5);
	glCallList(estrellas);
	glPopMatrix();
	// 2
	glPushMatrix();
	glTranslatef(0.5, 0.5, 0);
	glRotatef(-15 - r, 0, 0, 1);
	glScalef(0.5, 0.5, 0.5);
	glCallList(estrellas);
	glPopMatrix();
	// 3
	glPushMatrix();
	glTranslatef(0.5, -0.5, 0);
	glRotatef(15 + r, 0, 0, 1);
	glScalef(0.5, 0.5, 0.5);
	glCallList(estrellas);
	glPopMatrix();
	// 4
	glPushMatrix();
	glTranslatef(-0.5, -0.5, 0);
	glRotatef(-15 - r, 0, 0, 1);
	glScalef(0.5, 0.5, 0.5);
	glCallList(estrellas);
	glPopMatrix();




	glFinish();
}


void reshape(GLint w, GLint h) {

}

int main(int argc, char** argv) {
	r = 0;
	// Inicializaciones
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(50, 50);

	// Crear ventana
	glutCreateWindow(PROYECTO);

	init();
	//Registrar callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	//Bucle de atención a eventos
	glutMainLoop();


}