
#pragma once
#ifndef EXPLOSION_H
#define EXPLOSION_H
#include "codebase2.h"
#include "Asteroide.h"
#include "globales.h"
#include <iostream>
#include <vector>

using namespace cb2;
using namespace std;

const int NUM_FRAGMENTOS = 3;
const int NUM_EXPLOSIONES = 10;

class Explosion
{
private:
	Vec3 posicion;
	float lifeTime;
	GLuint lista; //Lista de dibujo de los fragmentos
	float size;
	bool vivo;
	//Asteroide fragmentos[3];
public:
	Explosion(Vec3 posicion = Vec3(0, 0, 0), float lifeTime = 1, float size = 1, GLuint lista = 0);

	void Actualizar(float tiempo);

	void Dibujar();

	Explosion& Destruir();
};

#endif


