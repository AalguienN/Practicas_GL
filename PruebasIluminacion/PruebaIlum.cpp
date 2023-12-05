/***************************************************
*	Primer_Proyecto.cpp
*
*	Codigo ejemplo inicial para practicas con OpenGL
*	y GLUT para ETSINF/UPV
*
*	@author	R.Vivo' <rvivo@upv.es>
*
***************************************************/
#define PROYECTO "Primer programa en OpenGL"

#include <iostream>	
#include <codebase.h>

using namespace cb;

void init() {
	cout << glGetString(GL_VERSION) << endl;

	static const GLfloat Ambient[4] = { 0.2,0.2,0.2,1 };

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, Ambient);

	glLightfv(GL_LIGHT0, GL_AMBIENT, NEGRO);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, GRISCLARO);
	glLightfv(GL_LIGHT0, GL_SPECULAR, GRISCLARO);

	glLightfv(GL_LIGHT1, GL_AMBIENT, NEGRO);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, BLANCO);
	glLightfv(GL_LIGHT1, GL_SPECULAR, BLANCO);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 5.0);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 100.0f);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_DEPTH_TEST);	//Profundidad
	glEnable(GL_LIGHTING);		//Iluminación


}
// Funcion de atencion al evento de dibujo
void display()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPolygonMode(GL_FRONT, GL_FILL);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	static GLfloat pos[] = { -0.5f, 0, 0, 1 }; //PosSpot
	glLightfv(GL_LIGHT1, GL_POSITION, pos);
	//static GLfloat dir[] = { 0,0,0 }; //DirSpot
	//glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, dir);

	gluLookAt(0, 2, 2, 0, 0, 0, 0, 1, 0); //Desde el frente

	static GLfloat posicionEstrella[] = { 100,20,0,0 }; //Direccional
	glLightfv(GL_LIGHT0, GL_POSITION, posicionEstrella);

	glPushMatrix();
	glutSolidTeapot(0.5);
	glPopMatrix();
	glutSwapBuffers();
}

void update() {
	static int hora_anterior = glutGet(GLUT_ELAPSED_TIME);
	int hora_actual = glutGet(GLUT_ELAPSED_TIME);

	float delta = (hora_actual - hora_anterior) / 1000.0f;

	glutPostRedisplay();
}
void reshape(GLint w, GLint h)
{
	float ra = (float)w / (float)h;

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(60, ra, 0.1, 100);
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
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(update);		//FPS flexibles

	// Bucle de atencion a eventos
	init();
	glutMainLoop();
}
