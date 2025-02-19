﻿#include "codebase2.h"

using namespace cb2;

Vec3::Vec3(float x, float y, float z) : x(x), y(y), z(z) {};
Vec3 Vec3::operator+(const Vec3& v) const { return Vec3(x + v.x, y + v.y, z + v.z); }
Vec3 Vec3::operator-(const Vec3& v) const { return Vec3(x - v.x, y - v.y, z - v.z); }
Vec3 Vec3::operator*(float f) const { return Vec3(x * f, y * f, z * f); }
Vec3 Vec3::operator/(float f) const { return Vec3(x / f, y / f, z / f); }
Vec3& Vec3::operator+=(const Vec3& v) { x += v.x; y += v.y; z += v.z; return *this; }
Vec3& Vec3::normalize() { return *this = *this * (1.0f / sqrtf(x * x + y * y + z * z)); }
float Vec3::norm() const { return sqrtf(x * x + y * y + z * z); }
float Vec3::dot(const Vec3& other) const { return x * other.x + y * other.y + z * other.z; }
Vec3 Vec3::cross(const Vec3& other) const { return Vec3(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x); }
Vec3& Vec3::rotX(float a) { float c = cosf(a), s = sinf(a), y0 = y; y = y0 * c - z * s; z = y0 * s + z * c; return *this; }
Vec3& Vec3::rotY(float a) { float c = cosf(a), s = sinf(a), z0 = z; z = z0 * c - x * s; x = z0 * s + x * c; return *this; }
Vec3& Vec3::rotZ(float a) { float c = cosf(a), s = sinf(a), x0 = x; x = x0 * c - y * s; y = x0 * s + y * c; return *this; }
Vec3& Vec3::rotate(float a, const Vec3& axis) {
	Vec3 u = axis / axis.norm();
	// Formula de Rodrigues
	*this = *this * cosf(a) + u.cross(*this) * sinf(a) + u * u.dot(*this) * (1 - cosf(a));
	return *this;
}
array<float, 3> Vec3::toArray() const { return { x,y,z }; }
//Esto estaba especificado con static, problema?
GLfloat* Vec3::to3fv(const Vec3& p) { return (GLfloat*)&p; }
Vec3::operator GLfloat* () { return to3fv(*this); }
Vec3::operator const GLfloat* () const { return to3fv(*this); }


float cb2::random(float min, float max) {
	// Devuelve un numero aleatorio entre min y max
	return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
}

Sistema3d::Sistema3d(Vec3 u, Vec3 v, Vec3 w, Vec3 origen) : u(u), v(v), w(w), origen(origen) {};
Sistema3d::Sistema3d(Vec3 origen) : u(1, 0, 0), v(0, 1, 0), w(0, 0, 1), origen(origen) {};
Sistema3d::Sistema3d() : u(1, 0, 0), v(0, 1, 0), w(0, 0, 1), origen(0, 0, 0) {};

Vec3 Sistema3d::getu() const { return u; };
Vec3 Sistema3d::getv() const { return v; };
Vec3 Sistema3d::getw() const { return w; };
Vec3 Sistema3d::geto() const { return origen; };
void Sistema3d::setu(Vec3 u) { this->u = u; };
void Sistema3d::setv(Vec3 v) { this->v = v; };
void Sistema3d::setw(Vec3 w) { this->w = w; };
void Sistema3d::seto(Vec3 origen) { this->origen = origen; };
void Sistema3d::rotar(float angulo, Vec3 eje) {
	// Rota el sistema de referencia alrededor del eje un angulo en radianes
	u = u.rotate(angulo, eje);
	v = v.rotate(angulo, eje);
	w = w.rotate(angulo, eje);
};

Vec3 Sistema3d::local2global(Vec3 p) const {
	// Transforma un punto del sistema de referencia al sistema de referencia global
	return origen + u * p.x + v * p.y + w * p.z;
};
Vec3 Sistema3d::global2local(Vec3 p) const {
	// Transforma un punto del sistema de referencia global al sistema de referencia
	return Vec3((p - origen).dot(u), (p - origen).dot(v), (p - origen).dot(w));
};
bool Sistema3d::enCono(const Vec3& p, float angulo) const {
	// Comprueba si un punto esta dentro del cono de angulo radianes del eje -w
	Vec3 q = global2local(p).normalize();
	return q.dot(w) < -cosf(angulo);
};
Vec3 Sistema3d::puntoCono(float angulo, float maxDistance) const {
	// Devuelve un punto aleatorio dentro del cono de angulo radianes del eje -w
	// y a una distancia maxima del origen
	float r = maxDistance * tanf(angulo); // radio del cono
	Vec3 k(random(-r, r), random(-r, r), -random(maxDistance * 0.6f, maxDistance));
	return local2global(k);
};
void Sistema3d::drawLocal() const {
	// Dibuja el sistema de referencia en el origen fijo
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	glVertex3fv(Vec3());
	glVertex3fv(u);
	glColor3f(0, 1, 0);
	glVertex3fv(Vec3());
	glVertex3fv(v);
	glColor3f(0, 0, 1);
	glVertex3fv(Vec3());
	glVertex3fv(w);
	glEnd();
	glPopAttrib();
};
void Sistema3d::drawGlobal() const {
	// Dibuja el sistema fijo en el de la camara
	Vec3 origenglobal = global2local(Vec3());
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	glVertex3fv(Vec3());
	glVertex3fv(global2local(Vec3(1, 0, 0)) - origenglobal);
	glColor3f(0, 1, 0);
	glVertex3fv(Vec3());
	glVertex3fv(global2local(Vec3(0, 1, 0)) - origenglobal);
	glColor3f(0, 0, 1);
	glVertex3fv(Vec3());
	glVertex3fv(global2local(Vec3(0, 0, 1)) - origenglobal);
	glEnd();
	glPopAttrib();
};

	/********** ADICIONES ADRIÁN **********************************************************************************************/


	//Mis Adiciones:
	Vec3 cb2::randomVec() {
		return Vec3(-1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1 - ( - 1)))),
			-1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1 - (-1)))),
			-1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1 - (-1)))));
	}

	/********** IMPLEMENTACION ************************************************************************************************/

	void cb2::planoXY(int resolucion)
		// resolucion: numero de divisiones del lado (>0)
		/* dibuja el cuadrado unidad (-0.5,-0.5)(0.5,0.5) con
		   "resolucion" divisiones usando strips horizontales.
		   Las normales se generan como (0,0,1)            */
	{
		if (resolucion < 1) resolucion = 1;				    //resolucion minima
		static const int NP = resolucion + 1;			    //numero de puntos en una dimension
		static const float delta = 1.0f / resolucion;		//distancia entre dos consecutivos
		// generacion de coordenadas
		GLfloat* vertices = new GLfloat[NP * NP * 3];
		int cont = 0;
		for (int i = 0; i < NP; i++)
			for (int j = 0; j < NP; j++) {
				vertices[cont++] = -0.5f + delta * j;		//x
				vertices[cont++] = -0.5f + delta * i;		//y
				vertices[cont++] = 0.0;				//z
			}
		// generacion de normales
		GLfloat* normales = new GLfloat[NP * NP * 3];
		cont = 0;
		for (int i = 0; i < NP; i++)
			for (int j = 0; j < NP; j++) {
				normales[cont++] = 0.0;					//nx
				normales[cont++] = 0.0;					//ny
				normales[cont++] = 1.0;					//nz
			}
		// generacion de coordenadas de textura
		GLfloat* texcoor = new GLfloat[NP * NP * 2];
		cont = 0;
		for (int i = 0; i < NP; i++)
			for (int j = 0; j < NP; j++) {
				texcoor[cont++] = 0 + delta * j;			//s
				texcoor[cont++] = 0 + delta * i;			//t
			}
		// carga en gpu
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, vertices);
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, 0, normales);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, 0, texcoor);
		// generacion de indices para cada strip horizontal
		GLuint* indices = new GLuint[2 * NP];				// indices por strip
		for (int k = 0; k < resolucion; k++) {			// k: strip corriente
			cont = 0;
			for (int i = 0; i < NP; i++) {
				indices[cont++] = i + (k + 1) * NP;			// counterclock
				indices[cont++] = i + k * NP;
			}
			glDrawElements(GL_TRIANGLE_STRIP, 2 * NP, GL_UNSIGNED_INT, indices);
		}
		// limpiando
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		delete[] vertices;
		delete[] normales;
		delete[] texcoor;
		delete[] indices;
	}

	void cb2::quad(GLfloat v0[3], GLfloat v1[3], GLfloat v2[3], GLfloat v3[3], int M, int N)
		// Dibuja un cuadrilatero con resolucion MxN y fija la normal 
	{
		if (M < 1) M = 1; if (N < 1) N = 1;	// Resolucion minima
		GLfloat ai[3], ci[3], bj[3], dj[3], p0[3], p1[3];
		// calculo de la normal (v1-v0)x(v3-v0) unitaria 
		GLfloat v01[] = { v1[0] - v0[0], v1[1] - v0[1], v1[2] - v0[2] };
		GLfloat v03[] = { v3[0] - v0[0], v3[1] - v0[1], v3[2] - v0[2] };
		GLfloat normal[] = { v01[1] * v03[2] - v01[2] * v03[1] ,
							 v01[2] * v03[0] - v01[0] * v03[2] ,
							 v01[0] * v03[1] - v01[1] * v03[0] };
		float norma = sqrt(normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]);
		glNormal3f(normal[0] / norma, normal[1] / norma, normal[2] / norma);
		// ai: punto sobre el segmento v0v1, bj: v1v2, ci: v3v2, dj: v0v3
		for (int i = 0; i < M; i++) {
			// puntos sobre segmentos a y c
			for (int k = 0; k < 3; k++) {
				ai[k] = v0[k] + i * (v1[k] - v0[k]) / M;
				ci[k] = v3[k] + i * (v2[k] - v3[k]) / M;
			}
			// strip vertical. i=s, j=t
			glBegin(GL_QUAD_STRIP);
			for (int j = 0; j <= N; j++) {
				for (int k = 0; k < 3; k++) {
					// puntos sobre los segmentos b y d
					bj[k] = v1[k] + j * (v2[k] - v1[k]) / N;
					dj[k] = v0[k] + j * (v3[k] - v0[k]) / N;

					// p0= ai + j/N (ci-ai)
					p0[k] = ai[k] + j * (ci[k] - ai[k]) / N;
					// p1= p0 + 1/M (bj-dj)
					p1[k] = p0[k] + (bj[k] - dj[k]) / M;
				}
				// punto izquierdo
				glTexCoord2f(i * 1.0f / M, j * 1.0f / N);  // s,t
				glVertex3f(p0[0], p0[1], p0[2]);
				// punto derecho
				glTexCoord2f((i + 1) * 1.0f / M, j * 1.0f / N);
				glVertex3f(p1[0], p1[1], p1[2]);
			}
			glEnd();
		}
	}
	void cb2::quadtex(GLfloat v0[3], GLfloat v1[3], GLfloat v2[3], GLfloat v3[3],
		GLfloat smin, GLfloat smax, GLfloat tmin, GLfloat tmax,
		int M, int N)
		// Dibuja un cuadrilatero con resolucion MxN con normales y coordenadas de textura
	{
		if (M < 1) M = 1; if (N < 1) N = 1;	// Resolucion minima
		GLfloat ai[3], ci[3], bj[3], dj[3], p0[3], p1[3];
		// calculo de la normal (v1-v0)x(v3-v0) unitaria 
		GLfloat v01[] = { v1[0] - v0[0], v1[1] - v0[1], v1[2] - v0[2] };
		GLfloat v03[] = { v3[0] - v0[0], v3[1] - v0[1], v3[2] - v0[2] };
		GLfloat normal[] = { v01[1] * v03[2] - v01[2] * v03[1] ,
							 v01[2] * v03[0] - v01[0] * v03[2] ,
							 v01[0] * v03[1] - v01[1] * v03[0] };
		float norma = sqrt(normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]);
		glNormal3f(normal[0] / norma, normal[1] / norma, normal[2] / norma);
		// ai: punto sobre el segmento v0v1, bj: v1v2, ci: v3v2, dj: v0v3
		for (int i = 0; i < M; i++) {
			// puntos sobre segmentos a y c
			for (int k = 0; k < 3; k++) {
				ai[k] = v0[k] + i * (v1[k] - v0[k]) / M;
				ci[k] = v3[k] + i * (v2[k] - v3[k]) / M;
			}
			// strip vertical. i=s, j=t
			glBegin(GL_QUAD_STRIP);
			for (int j = 0; j <= N; j++) {
				for (int k = 0; k < 3; k++) {
					// puntos sobre los segmentos b y d
					bj[k] = v1[k] + j * (v2[k] - v1[k]) / N;
					dj[k] = v0[k] + j * (v3[k] - v0[k]) / N;

					// p0= ai + j/N (ci-ai)
					p0[k] = ai[k] + j * (ci[k] - ai[k]) / N;
					// p1= p0 + 1/M (bj-dj)
					p1[k] = p0[k] + (bj[k] - dj[k]) / M;
				}
				// punto izquierdo
				glTexCoord2f(smin + (smax - smin) * i / M, tmin + (tmax - tmin) * j / N);  // s,t
				glVertex3f(p0[0], p0[1], p0[2]);
				// punto derecho
				glTexCoord2f(smin + (smax - smin) * (i + 1) / M, tmin + (tmax - tmin) * j / N);
				glVertex3f(p1[0], p1[1], p1[2]);
			}
			glEnd();
		}
	}

	void cb2::ejes()
	{
		// Construye unos ejes de longitud 1
		/// Por precaucion no usar listas con client arrays!

		//Construye los ejes
		glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
		//Eje X en rojo
		glColor3fv(ROJO);
		glPushMatrix();
		glRotatef(-90, 0, 0, 1);
		glBegin(GL_LINES);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 1, 0);
		glEnd();
		//Punta de la flecha
		glPushMatrix();
		glTranslatef(0, 1, 0);
		glRotatef(-90, 1, 0, 0);
		glTranslatef(0.0, 0.0, -1 / 10.0f);
		glutSolidCone(1 / 50.0, 1 / 10.0, 10, 1);
		glPopMatrix();
		glPopMatrix();
		//Eje Y en verde
		glColor3fv(VERDE);
		glPushMatrix();
		glBegin(GL_LINES);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 1, 0);
		glEnd();
		//Punta de la flecha
		glPushMatrix();
		glTranslatef(0, 1, 0);
		glRotatef(-90, 1, 0, 0);
		glTranslatef(0.0, 0.0, -1 / 10.0f);
		glutSolidCone(1 / 50.0, 1 / 10.0, 10, 1);
		glPopMatrix();
		glPopMatrix();
		//Eje Z en azul
		glColor3fv(AZUL);
		glPushMatrix();
		glRotatef(90, 1, 0, 0);
		glBegin(GL_LINES);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 1, 0);
		glEnd();
		//Punta de la flecha
		glPushMatrix();
		glTranslatef(0, 1, 0);
		glRotatef(-90, 1, 0, 0);
		glTranslatef(0.0, 0.0, -1 / 10.0f);
		glutSolidCone(1 / 50.0, 1 / 10.0, 10, 1);
		glPopMatrix();
		glPopMatrix();
		//Esferita en el origen
		glColor3f(0.5, 0.5, 0.5);
		glutSolidSphere(0.05, 8, 8);
		glPopAttrib();
		glPopClientAttrib();
	}
	//Pequeña modificación en el texto, ahora puede modificarse la z

	//Muestra texto en pantalla o en world space
	void cb2::texto(unsigned int x, unsigned int y, char* text, unsigned int z,const GLfloat* color, void* font, bool WCS)
	{
		//glPushAttrib(GL_DEPTH_BUFFER_BIT | GL_ENABLE_BIT);
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_TEXTURE_2D);

		glColor3fv(color);

		if (!WCS) {
			int viewport[4];
			glGetIntegerv(GL_VIEWPORT, viewport);

			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			gluOrtho2D(viewport[0], viewport[2], viewport[1], viewport[3]);

			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();

			glRasterPos2i(x, y);

			while (*text)
			{
				glutBitmapCharacter(font, *text++);
			}
			glPopMatrix();
			glMatrixMode(GL_PROJECTION);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
		}
		else {
			glRasterPos3i(x, y, z);
			while (*text)
			{
				glutBitmapCharacter(font, *text++);
			}
		}
		glEnable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);
		glPopAttrib();
	}

	void cb2::loadImageFile(char* nombre)
	{
		// Detecci�n del formato, lectura y conversion a BGRA
		FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(nombre, 0);
		FIBITMAP* imagen = FreeImage_Load(formato, nombre);
		if (imagen == NULL) cerr << "Fallo carga de imagen " << nombre << endl;
		FIBITMAP* imagen32b = FreeImage_ConvertTo32Bits(imagen);

		// Lectura de dimensiones y colores
		int w = FreeImage_GetWidth(imagen32b);
		int h = FreeImage_GetHeight(imagen32b);
		GLubyte* texeles = FreeImage_GetBits(imagen32b);

		// Carga como textura actual
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, texeles);

		// Liberar recursos
		FreeImage_Unload(imagen);
		FreeImage_Unload(imagen32b);
	}

	void cb2::saveScreenshot(char* nombre, int ancho, int alto)
	{
		int pix = ancho * alto;
		BYTE* pixels = new BYTE[3 * pix];
		glReadBuffer(GL_FRONT);
		glReadPixels(0, 0, ancho, alto, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);
		FIBITMAP* img = FreeImage_ConvertFromRawBits(pixels, ancho, alto, ancho * 3, 24, 0xFF0000, 0x00FF00, 0x0000FF, false);
		FreeImage_Save(FIF_PNG, img, nombre, 0);
		delete[] pixels;
	}

	void cb2::texturarFondo()
	{
		glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT | GL_TEXTURE_BIT);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);		//Texel menor que pixel
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		//Texel mayor que pixel
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);			//La textura se repite en abcisas
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);			//La textura se repite en ordenadas
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);				//Asigna solo el color de la textura al fragmento

		//Cargar el fondo con la textura corriente
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(-1, 1, -1, 1, -10, 10);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glBegin(GL_POLYGON);
		glTexCoord2f(0, 0);
		glVertex3f(-1, -1, 0);
		glTexCoord2f(1, 0);
		glVertex3f(1, -1, 0);
		glTexCoord2f(1, 1);
		glVertex3f(1, 1, 0);
		glTexCoord2f(0, 1);
		glVertex3f(-1, 1, 0);
		glEnd();
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);

		glPopAttrib();
	}