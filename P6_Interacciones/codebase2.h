#pragma once
#ifndef CODEBASE2_H
#define CODEBASE2_H

#include <iostream>
#include <cmath>
#include <array>

#include <GL/freeglut.h>
#include <GL/glext.h>
#include <freeimage/FreeImage.h>

using namespace std;

namespace cb2 {

	#ifndef PI
	#define PI 3.1415926f
	#endif
	#ifndef max
	#define max(a,b) (a>b?a:b)
	#define min(a,b) (a<b?a:b)
	#endif
	#define signo(a) (a<0?-1:1)
	#define deg(a) (a*180/PI)
	#define rad(a) (a*PI/180)

	const GLfloat BLANCO[] = { 1,1,1,1 };
	const GLfloat NEGRO[] = { 0,0,0,1 };
	const GLfloat GRISCLARO[] = { 0.7f,0.7f,0.7f,1 };
	const GLfloat GRISOSCURO[] = { 0.2f,0.2f,0.2f,1 };
	const GLfloat ROJO[] = { 1,0,0,1 };
	const GLfloat VERDE[] = { 0,1,0,1 };
	const GLfloat AZUL[] = { 0,0,1,1 };
	const GLfloat AMARILLO[] = { 1,1,0,1 };
	const GLfloat BRONCE[] = { 140.0f / 255,120.0f / 255,83.0f / 255,1 };
	const GLfloat MARINO[] = { 0,0,0.5f,1 };
	const GLfloat ORO[] = { 218.0f / 255,165.0f / 255,32.0f / 255,1 };


	// Clase basica punto/vector 
	// Angulos en radianes
	class Vec3
	{
	public:
		float x; float y; float z;
		Vec3(float x = 0.0f, float y = 0.0f, float z = 0.0f);
		Vec3 operator+(const Vec3& v) const;
		Vec3 operator-(const Vec3& v) const;
		Vec3 operator*(float f) const;
		Vec3 operator/(float f) const;
		Vec3& operator+=(const Vec3& v);
		Vec3& normalize();
		float norm() const;
		float dot(const Vec3& other) const;
		Vec3 cross(const Vec3& other) const;
		Vec3& rotX(float a);
		Vec3& rotY(float a);
		Vec3& rotZ(float a);
		Vec3& rotate(float a, const Vec3& axis);
		array<float, 3> toArray() const;
		static GLfloat* to3fv(const Vec3& p);
		operator GLfloat* ();
		operator const GLfloat* () const;
	};

	float random(float min, float max);

	class Sistema3d {
	private:
		Vec3 u, v, w;
		Vec3 origen;
	public:
		Sistema3d(Vec3 u, Vec3 v, Vec3 w, Vec3 origen);
		Sistema3d(Vec3 origen);
		Sistema3d();
		Vec3 getu() const;
		Vec3 getv() const;
		Vec3 getw() const;
		Vec3 geto() const;
		void setu(Vec3 u);
		void setv(Vec3 v);
		void setw(Vec3 w);
		void seto(Vec3 origen);
		void rotar(float angulo, Vec3 eje);
		Vec3 local2global(Vec3 p) const;
		Vec3 global2local(Vec3 p) const;
		bool enCono(const Vec3& p, float angulo) const;
		Vec3 puntoCono(float angulo, float maxDistance) const;
		void drawLocal() const;
		void drawGlobal() const;
	};

	//Modificación de Adrián
	//Devuelve un vector aleatorio con parámetros entre -1 y 1
	Vec3 randomVec();

	void planoXY(int resolucion = 10);
	/* resolucion: numero de divisiones opcional del lado (por defecto 10)
	   Dibuja el cuadrado unidad (-0.5,-0.5)(0.5,0.5) con
	   "resolucion" divisiones usando strips horizontales.
	   Las normales se generan como (0,0,1)            */

	void quad(GLfloat v0[3], GLfloat v1[3], GLfloat v2[3], GLfloat v3[3], int M = 10, int N = 10);
	/* v0,v1,v2,v3: vertices del quad
	   NxM: resolucion opcional (por defecto 10x10)
	   Dibuja el cuadrilatero de entrada con resolucion MxN y normales.
	   Se asume counterclock en la entrada                            */

	void quadtex(GLfloat v0[3], GLfloat v1[3], GLfloat v2[3], GLfloat v3[3],
		GLfloat smin = 0, GLfloat smax = 1, GLfloat tmin = 0, GLfloat tmax = 1,
		int M = 10, int N = 10);
	/* v0,v1,v2,v3: vertices del quad
	   (smin,tmin),(smax,tmax): coordenadas de textura de v0 y v2 respectivamente
	   NxM: resolucion opcional (por defecto 10x10)
	   Dibuja el cuadrilatero de entrada con resolucion MxN, normales y ccordenadas
	   de textura segun rango dado.
	   Se asume antihorario en la entrada para caras frontales                      */

	void ejes();
	/* Dibuja unos ejes de longitud 1 y una esferita en el origen */

	void texto(unsigned int x, unsigned int y, char* text, const GLfloat* color = ROJO, void* font = GLUT_BITMAP_HELVETICA_18, bool WCS = true);
	/* Muestra en la posicion (x, y) del viewport la cadena de texto con la fuente y el color indicados
	   Los tres ultimos parametros son opcionales.
	   @WCS: x,y en coordenadas del mundo -true- o del viewport -false-.
	   Ver en glut.h las posibles fuentes que se pueden utilizar.
	   Por ejemplo: texto(30, 30, "Hola mundo"); */

	void loadImageFile(char* nombre);
	/* Uso de FreeImage para cargar la imagen en cualquier formato
	   nombre: nombre del fichero con extension en el mismo directorio que el proyecto o con su path completo */

	void saveScreenshot(char* nombre, int ancho, int alto);
	/* Utiliza FreeImage para grabar un png
	   nombre: Nombre del fichero con extension p.e. salida.png
	   ancho: Ancho del viewport en pixeles
	   alto: Alto del viewport en pixeles */

	void texturarFondo();
	/* Carga como fondo del area de dibujo la textura corriente.
	   Debe llamarse inmediatamente despues del glClear()       */
}



#endif