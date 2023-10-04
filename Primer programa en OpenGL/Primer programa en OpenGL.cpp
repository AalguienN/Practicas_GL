/***************************************************
*	Practica 1
*
*	SGI
*
*	@author	A.Camacho <acamgar1@etsinf.upv.es
*
***************************************************/

#define PROYECTO "Primer programa en OpenGL"

#include <iostream>	
#include <codebase.h>

float r; float g; float b;

void display() {
	//glClearColor(0.0f, 0.0f, 0.3f, 1.0f);
	
	glClearColor(r, g, b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	

	int ran = rand() % 3;
	glutWireTeapot(0.5);
	/*cout << ran;
	switch (ran)
	{
	case 0:
		cout << " teapot" << endl;
		glutWireTeapot(0.5);
		break;
	case 1:
		cout << " teaspoon" << endl;
		glutWireTeaspoon(0.5);
		break;
	case 2:
		cout << " tetrahedron" << endl;
		glutWireTetrahedron();
		break;
	default:
		break;
	}*/
	//lFlush();  //Pasa directamente a la tubería gráfica sin esperar a que se cargue todo
	glFinish(); //Pasa la tubería gráfica todos los 


}

void loop() {
	while (true) {
		r += ((float)(rand() % 100)-50) / 500;
		g += ((float)(rand() % 100)-50) / 500;
		b += ((float)(rand() % 100)-50) / 500;

		if (r < 0 || r>1) r = 0.5;
		if (g < 0 || r>1) g = 0.5;
		if (b < 0 || r>1) b = 0.5;

		Sleep(50);
		display();
		glRotatef(1, 1,1, 1);
	}
}

void reshape(GLint w, GLint h) {

}

int main(int argc, char** argv) {

	r, g, b = 0.5;

	// Inicializaciones
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 400);
	glutInitWindowPosition(50, 50);

	// Crear ventana
	glutCreateWindow(PROYECTO);

	//Registrar callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);


	loop();

	//Bucle de atención a eventos
	glutMainLoop();


}