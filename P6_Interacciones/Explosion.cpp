
#include "Explosion.h"

using namespace cb2;

int expActual = 0;
//Explosion explosiones[NUM_EXPLOSIONES];
//Asteroide Explosion::fragmentos[NUM_FRAGMENTOS];

Explosion::Explosion(Vec3 posicion, float lifeTime, float size, GLuint lista) :
	posicion(posicion), lifeTime(lifeTime), size(size), lista(lista), vivo(false) //, fragmentos()
{

	for (int i = 0; i < NUM_FRAGMENTOS; i++) {
		//fragmentos
		
	}
};

void Explosion::Actualizar(float tiempo) {
	if (vivo) {
		lifeTime -= tiempo;
		for (int i = 0; i < NUM_FRAGMENTOS; i++) {
			//fragmentos[i].Actualizar(tiempo);
		}
		if (lifeTime < 0) { Destruir(); }
	}
}

void Explosion::Dibujar() {
	if (vivo) {
		for (int i = 0; i < NUM_FRAGMENTOS; i++) {
			//fragmentos[i].Dibujar(lista);
		}
	}
}

Explosion& Explosion::Destruir() {
	vivo = false;
	return *this;
}