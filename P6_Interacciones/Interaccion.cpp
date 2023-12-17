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
#include <codebase.h>

using namespace std;
using namespace cb;

//Listas de dibujo
static GLuint suelo;
static GLuint asteroide;
static GLuint plano;

//Globales
const int DIM_PLATAFORMA = 200;
const int VERTICES_POR_UNIDAD = 4;
static float z0 = 1;
static Sistema3d player;

static float speedForward;
static const float ACCELERATION = 0.2;
static const float MAX_SPEED = 10;


//Innecesario?
//static Vec3 posPlayer;

float girox; float giroy;

static int xanterior;
static int yanterior;

static int windowWidth;
static int windowHeight;

static bool luces = true;

static const int resAsteroide = 50;
static const float velIniAst = 1.0f; //Velocidad inicial de los asteroides
static const Vec3 velIniAngularMax = 1.0f; //Velocidad inicial de los asteroides
static const float max_dist_asteroides = 20;
static const int numAsteroides = 100;
static const float SUPERFICIE_BASE = 1.0f;
static const float SUPERFICIE_VARIACION = 0.01f;


static Vec3 verticesAsteroide[(resAsteroide+2) * (resAsteroide+2)];

Vec3 randomVec() {
	
	return Vec3(random(-1, 1), random(-1, 1), random(-1, 1));
}

class Asteroide {
private:
	Vec3 position;
	Vec3 velocidad;
	Vec3 rotacion;
	Vec3 velocidadAngular;
	Vec3 escala;
	Vec3 rotAcionInterna;
	//Radio de cada punto del asteroide respecto a su centro
	Vec3 verticesAsteroide[(resAsteroide+2) * (resAsteroide+2)];
	//Resolución
	float resolucion;
	
public:
	Asteroide(
		Vec3 position = randomVec() * max_dist_asteroides,
		Vec3 velocidad = randomVec()* velIniAst,
		Vec3 rotacion = randomVec() * 180.f,
		Vec3 velocidadAngular = randomVec() * 1,
		Vec3 escala = Vec3(1,1,1) + randomVec() * 0.5,
		int res = resAsteroide) :
		position(position), velocidad(velocidad), 
		rotacion(rotacion), velocidadAngular(velocidadAngular),
		escala(escala),
		verticesAsteroide(), resolucion(res) ,
		rotAcionInterna(randomVec()*180.f)
	{
		SetOffset();
		cout << velocidadAngular.x << velocidadAngular.y << velocidadAngular.z << endl;
	};
	void SetOffset() {
	}
	void SetOffset(GLfloat va[resAsteroide * resAsteroide]) {
		cout << "Seteando offset de asteroide ..." << this << endl;
		for (int i = 0; i < resAsteroide * resAsteroide; i++) {
			this->verticesAsteroide[i] = va[i];
		}
	}
	void Dibujar() {
		glPushMatrix();
		glTranslatef(position.x,position.y, position.z);
		glRotatef(rotacion.x, 1,0,0);
		glRotatef(rotacion.y, 0,1,0);
		glRotatef(rotacion.z, 0,0,1);
		glScalef(escala.x, escala.y, escala.z);
		glRotatef(rotAcionInterna.x, 1, 0, 0);
		glRotatef(rotAcionInterna.y, 0, 1, 0);
		glRotatef(rotAcionInterna.z, 0, 0, 1);
		glCallList(asteroide);
		
		glPopMatrix();
		/*
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		float res = this -> resolucion;


		glPolygonMode(GL_BACK, GL_LINE);

		int pos = 0;

		//Opción arreglar : Meter plano de corte

		for (int i = 0; i < res; i++) {
			glBegin(GL_TRIANGLE_STRIP);
			for (int j = 0; j <= res; j++) {

				Vec3 v2(
					cos(j * 2 * PI / res) * sin((i + 1) * PI / res),
					sin(j * 2 * PI / res) * sin((i + 1) * PI / res),
					sin((i + 1) * PI / res - PI / 2)
				);
				Vec3 n2 = v2;
				n2.normalize();
				glNormal3f(n2.x, n2.y, n2.z);
				pos = (i+1) * res + j;
				v2 = v2 * offset[pos];
				glVertex3f(v2.x, v2.y, v2.z);


				Vec3 v1(
					cos(j * 2 * PI / res) * sin(i * PI / res),
					sin(j * 2 * PI / res) * sin(i * PI / res),
					sin(i * PI / res - PI / 2)
				);
				Vec3 n1 = v1;
				n1.normalize();
				glNormal3f(n1.x, n1.y, n1.z);
				pos = i * res + j;
				v1 = v1 * offset[pos];
				glVertex3f(v1.x, v1.y, v1.z);
			}

			glEnd();
		}


		glPopAttrib();
		*/
	}
	Vec3 getPos() { return this->position; }
	void updatePos(float tiempo) { 
		this->position += this->velocidad * tiempo; 
		if (this->position.z < 0) {
			this->velocidad = Vec3(random(-velIniAst, velIniAst), random(-velIniAst, velIniAst), random(-velIniAst, velIniAst));
			this->position = Vec3(position.x, position.y, 0);
		} 
		if ((this->position - player.geto()).norm() > max_dist_asteroides) {
			this->position = Vec3(random(-1, 1), random(-1, 1), random(-1, 1)).normalize() * max_dist_asteroides * 0.99f + player.geto();
			this->velocidad = Vec3(random(-velIniAst, velIniAst), random(-velIniAst, velIniAst), random(-velIniAst, velIniAst));
		}
		this->rotacion += velocidadAngular;
	}
};

static Asteroide asteroides[numAsteroides];

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


	#pragma endregion

	glEnable(GL_DEPTH_TEST);	//Profundidad
	glEnable(GL_LIGHTING);		//Iluminación
	glEnable(GL_NORMALIZE);

	#pragma region suelo lista

	suelo = glGenLists(1);
	glNewList(suelo, GL_COMPILE);
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	//glColor3f(1, 1, 1);

	for (int i = 0; i < DIM_PLATAFORMA * VERTICES_POR_UNIDAD - 1; i++) {
		glBegin(GL_TRIANGLE_STRIP);
		glNormal3f(0, 0, 1);
		for (int j = 0; j < DIM_PLATAFORMA * VERTICES_POR_UNIDAD; j++) {
			glTexCoord2f(i / (DIM_PLATAFORMA * VERTICES_POR_UNIDAD), j);
			glVertex3f((i-DIM_PLATAFORMA/2)/ float(VERTICES_POR_UNIDAD), (j - DIM_PLATAFORMA / 2) / float(VERTICES_POR_UNIDAD), 0);
			glTexCoord2f((i+1) / (DIM_PLATAFORMA * VERTICES_POR_UNIDAD), j);
			glVertex3f((i + 1 - DIM_PLATAFORMA / 2) / float(VERTICES_POR_UNIDAD), (j - DIM_PLATAFORMA / 2) / float(VERTICES_POR_UNIDAD), 0);
		}
		glEnd();
	}


	glPopAttrib();
	glEndList();

	#pragma endregion

	// Inicializar Asteroide ---------------------------------------------------------------------

	#pragma region iniAsteroide

	float res = resAsteroide;
	float offset;
	offset = 1;

	for (int i = 1; i < resAsteroide + 1; i++) {
		for (int j = 1; j <= resAsteroide + 1; j++) {
			//cout <<'('<< i << ',' << j <<')' << (i+1) * (resAsteroide) + j;
			float a = (cos(j * 8 * 2 * PI / res) * SUPERFICIE_VARIACION + SUPERFICIE_BASE);

			verticesAsteroide[i * (resAsteroide+2) + j] = Vec3(
				cos(j * 2 * PI / res) * a * sin(i * PI / res),
				sin(j * 2 * PI / res) * a * sin(i * PI / res),
				sin(i * PI / res - PI / 2)
			) * offset;
		}
		//cout << endl;
		offset = (cos(i * 8 * 2 * PI / res) * SUPERFICIE_VARIACION + SUPERFICIE_BASE);
	}

	//Derecha izquierda
	for (int i = 1; i < resAsteroide + 1; i++) {
		verticesAsteroide[i * (resAsteroide + 2)] = verticesAsteroide[i * (resAsteroide + 2) + resAsteroide];
		verticesAsteroide[i * (resAsteroide + 2) + resAsteroide + 1] = verticesAsteroide[i * (resAsteroide + 2) + 1];
	}
	//Arriba abajo
	for (int j = 1; j < resAsteroide + 1; j++) {
		verticesAsteroide[j] = verticesAsteroide[(resAsteroide) * (resAsteroide + 2) + j];
		verticesAsteroide[(resAsteroide + 1) * (resAsteroide + 2) + j] = verticesAsteroide[resAsteroide + 2 + j];
	}
	//Esquinas
	verticesAsteroide[0] = verticesAsteroide[(resAsteroide) * (resAsteroide + 2)];
	verticesAsteroide[(resAsteroide + 1) * (resAsteroide + 2) + resAsteroide + 1] = verticesAsteroide[(resAsteroide + 2) + 1];
	verticesAsteroide[(resAsteroide + 1) * (resAsteroide + 2)] = verticesAsteroide[(resAsteroide + 2) + resAsteroide+1];
	#pragma endregion

	#pragma region asteroide

	asteroide = glGenLists(1);
	glNewList(asteroide, GL_COMPILE);
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	//glColor3f(1, 1, 1);

	// Dibujar ---------------------------------------------------------------------
	res = resAsteroide;

	//glPolygonMode(GL_BACK, GL_LINE);

	for (int i = 1; i < res; i++) {
		glBegin(GL_TRIANGLE_STRIP);
		for (int j = 1; j <= res + 1; j++) {
			//float a = (cos(j * 8 * 2 * PI / res) * 0.1 + 0.9);
			Vec3 v1 = verticesAsteroide[i * (resAsteroide+2) + j];
			Vec3 v2 = verticesAsteroide[(i + 1) * (resAsteroide + 2) + j];
			Vec3 v3 = verticesAsteroide[i * (resAsteroide+2) + j + 1];

			Vec3 n2;
			if (j == res+1)
				n2 = (v3 - v1).cross(v2 - v1)*-1;
			else
				n2 = (v3 - v1).cross(v2 - v1);

			n2.normalize();
			glNormal3f(n2.x, n2.y, n2.z);
			glVertex3f(v2.x, v2.y, v2.z);
			
			Vec3 n1;
			if (j == res + 1)
				n1 = (v3 - v1).cross(v2 - v1) * -1;
			else
				n1 = (v3 - v1).cross(v2 - v1);

			n1.normalize();
			glNormal3f(n1.x, n1.y, n1.z);
			glVertex3f(v1.x, v1.y, v1.z);
		}
		glEnd();
	}
	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0, 0, -1);
	glVertex3f(0,0,-1);
	for (int j = res + 1; j >= 1; j--) {
		//float a = (cos(j * 8 * 2 * PI / res) * 0.1 + 0.9);
		Vec3 v1 = verticesAsteroide[(resAsteroide+1) * (resAsteroide + 2) + j];

		Vec3 n1 = v1;
		n1.normalize();
		glNormal3f(n1.x, n1.y, n1.z);
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

	res = resAsteroide;

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


	//Inicialización del jugador/nave
	player.seto(Vec3(0, 0, z0)); //Posicion
	player.setu(Vec3(1, 0, 0));  //x
	player.setw(Vec3(0, -1, 0)); //y
	player.setv(Vec3(0, 0, 1));  //z
	speedForward = 0;

}

void update() {

	//Coherente con el tiempo transcurrido (sin control de FPS)
	static int hora_anterior = glutGet(GLUT_ELAPSED_TIME);
	int hora_actual = glutGet(GLUT_ELAPSED_TIME);

	float delta = (hora_actual - hora_anterior) / 1000.0f;

	Vec3 w = player.getw();

	//Al final funciona con el sistema de referencia
	//posPlayer += Vec3(-w.x * speedForward * delta, -w.y * speedForward * delta, -w.z * speedForward * delta);

	player.seto(player.geto() += Vec3(-w.x * speedForward * delta, -w.y * speedForward * delta, -w.z * speedForward * delta));

	for (int i = 0; i < numAsteroides; i++) {
		asteroides[i].updatePos(delta);
	}

	hora_anterior = hora_actual;

	//texto(10, 10, a.c_str());

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
	//origen += posPlayer;

	Vec3 lookAt = origen - player.getw(); //w
	Vec3 up = player.getv();

	Vec3 faro1 =  player.getu()*0.25f;
	Vec3 faro2 =  player.getu()*-0.25f;

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
	glPushMatrix();
	glTranslatef(0, 2, 1);
	glCallList(asteroide);
	glPopMatrix();

	for (int i = 0; i < numAsteroides; i++) {
		asteroides[i].Dibujar();
	}

	/*
	glPushMatrix();
	glTranslatef(lookAt.x, lookAt.y, lookAt.z);
	//player.drawGlobal();
	//player.drawLocal();
	glPopMatrix();
	*/

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
	case 27:
		exit(0);
	default:
		break;
	}
	//glutPostRedisplay();

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
	//glutWarpPointer(windowWidth / 2, windowHeight / 2);

	//Giramos siendo coherentes con el tiempo
	static int hora_anterior = glutGet(GLUT_ELAPSED_TIME);
	int hora_actual = glutGet(GLUT_ELAPSED_TIME);
	float delta = (hora_actual - hora_anterior)/100.0f;

	girox = (y - yanterior) * pixel2grados;
	giroy = (x - xanterior) * pixel2grados;

	player.rotar(-girox*delta, player.getu());
	player.rotar(-giroy*delta, player.getv());

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
