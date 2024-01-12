
#include "Explosion.h"

using namespace cb2;

int explActual = 0;
//Explosion explosiones[NUM_EXPLOSIONES];
//Asteroide Explosion::fragmentos[NUM_FRAGMENTOS];

Explosion::Explosion(Vec3 posicion, float lifeTime, float size, float crecimiento, GLuint textura) :
	posicion(posicion), lifeTime(lifeTime), size(size), textura(textura), vivo(false) , crecimiento(crecimiento)
{};

void Explosion::Iniciar(Vec3 posicion, float size, float crecimiento) {
	this->vivo = true;
	this->lifeTime = 1;
	this->size = size;
	this->posicion = posicion;
	this->crecimiento = crecimiento;
}

void Explosion::Actualizar(float tiempo) {
	if (vivo) {
		lifeTime -= tiempo;
		size += tiempo*crecimiento;
		for (int i = 0; i < NUM_FRAGMENTOS; i++) {
			//fragmentos[i].Actualizar(tiempo);
		}
		if (lifeTime < 0) { Destruir(); }
	}
}

void Explosion::Dibujar(GLuint text) {
	if (vivo) {

		glPushAttrib(GL_ALL_ATTRIB_BITS);
		//glDisable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);

		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glPushMatrix();
		glTranslatef(posicion.x,posicion.y,posicion.z);
		
		glColor4f(0.8f, 0.5f, 0, lifeTime/2);

		glPushMatrix();
		//glScalef(size, size, size);
		glutSolidSphere(size, 20, 20);
		glPopMatrix();

		for (int i = 0; i < 5; i++) {
			glPushMatrix();
			glColor4f(1, 1, 1, lifeTime / 2);
			glTranslatef(random(-1, 1) * size / 3, random(-1, 1) * size / 3, random(-1, 1) * size / 3);
			float r = random(0.9f, 1.f);
			glScalef(r, r, r);
			glutSolidSphere(size / 3, 20, 20);
			glPopMatrix();
		}
		
		glPopMatrix();
		glEnable(GL_LIGHTING);
		glPopAttrib();
		for (int i = 0; i < NUM_FRAGMENTOS; i++) {
			//fragmentos[i].Dibujar(lista);
		}
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
	}
}

void Explosion::Destruir() {
	vivo = false;
}