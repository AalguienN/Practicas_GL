#include "Asteroide.h"
#include "Explosion.h"

using namespace cb2;

Asteroide::Asteroide(
	Vec3 position,
	Vec3 velocidad,
	Vec3 rotacion,
	Vec3 velocidadAngular,
	Vec3 deformaciones,
	float size,
	Vec3 rotacionInterna,
	int res) :
	position(position), velocidad(velocidad),
	rotacion(rotacion), velocidadAngular(velocidadAngular),
	deformaciones(deformaciones),
	size(size),
	resolucion(res),
	rotAcionInterna(rotacionInterna),
	explosion(false),
	posicionExplosion(Vec3(0,0,0))
{
	//cout << velocidadAngular.x << velocidadAngular.y << velocidadAngular.z << endl;
};
void Asteroide::setPos(Vec3 posicion) {
	this->position = posicion;
}

void Asteroide::Dibujar(GLuint lista) {
	if ((this->position - player.geto()).norm() < MAX_DIST_ASTEROIDES - 4) {
		glPushMatrix();
		glTranslatef(position.x, position.y, position.z);
		(rotacion.x, 1, 0, 0);
		glRotatef(rotacion.y, 0, 1, 0);
		glRotatef(rotacion.z, 0, 0, 1);
		glScalef(deformaciones.x, deformaciones.y, deformaciones.z);
		glRotatef(rotAcionInterna.x, 1, 0, 0);
		glRotatef(rotAcionInterna.y, 0, 1, 0);
		glRotatef(rotAcionInterna.z, 0, 0, 1);
		glScalef(size, size, size);
		glCallList(lista);

		glPopMatrix();
	}
}
Vec3 Asteroide::getPos() { return this->position; }
void Asteroide::Actualizar(float tiempo) {
	this->position += this->velocidad * tiempo;

	if ((this->position - player.geto()).norm() > MAX_DIST_ASTEROIDES) {
		while ((this->position - player.geto()).norm() > MAX_DIST_ASTEROIDES) {
			this->position = Vec3(random(-1, 1), random(-1, 1), random(-1, 1)).normalize() * MAX_DIST_ASTEROIDES * 0.99f + player.geto();
			this->velocidad = Vec3(random(-velIniAst, velIniAst), random(-velIniAst, velIniAst), random(-velIniAst, velIniAst));
		}
	}

	for (int i = 0; i < NUM_BLASTERS; i++) {
		if (blasters[i].isAlive()) {
			if ((blasters[i].GetPosition() - this->position).norm() < this->size) {
				blasters[i].Destruir();
				Explotar();
			}
		}
	}

	this->rotacion += velocidadAngular;
}

void Asteroide::Explotar() {
	cout << "Asteroide muelto" << endl;
	explosion = true;
	this->posicionExplosion = this->position;
	this->position = Vec3(random(-1, 1), random(-1, 1), random(-1, 1)).normalize() * MAX_DIST_ASTEROIDES * 0.99f + player.geto();
	this->velocidad = Vec3(random(-velIniAst, velIniAst), random(-velIniAst, velIniAst), random(-velIniAst, velIniAst));
	//explosiones[expActual] = Explosion(position, 1, size / NUM_FRAGMENTOS, 0);
}






