
#include "Explosion.h"

using namespace cb2;

int explActual = 0;
//Explosion explosiones[NUM_EXPLOSIONES];
//Asteroide Explosion::fragmentos[NUM_FRAGMENTOS];

Explosion::Explosion(Vec3 posicion, float lifeTime, float size, GLuint textura) :
	posicion(posicion), lifeTime(lifeTime), size(size), textura(textura), vivo(false) //, fragmentos()
{

	for (int i = 0; i < NUM_FRAGMENTOS; i++) {
		//fragmentos
		
	}
};

void Explosion::Iniciar(Vec3 posicion, float size) {
	this->vivo = true;
	this->lifeTime = 1;
	this->size = size;
	this->posicion = posicion;
}

void Explosion::Actualizar(float tiempo) {
	if (vivo) {
		lifeTime -= tiempo;
		size += tiempo*10;
		for (int i = 0; i < NUM_FRAGMENTOS; i++) {
			//fragmentos[i].Actualizar(tiempo);
		}
		if (lifeTime < 0) { Destruir(); }
	}
}

void Explosion::Dibujar() {
	if (vivo) {

		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glPushMatrix();
		glTranslatef(posicion.x,posicion.y,posicion.z);
		glColor4f(1, 0, 0, lifeTime);
		glutSolidSphere(size,10,10);
		glPopMatrix();
		glEnable(GL_LIGHTING);
		glPopAttrib();
		for (int i = 0; i < NUM_FRAGMENTOS; i++) {
			//fragmentos[i].Dibujar(lista);
		}
	}
}

void Explosion::Destruir() {
	vivo = false;
}