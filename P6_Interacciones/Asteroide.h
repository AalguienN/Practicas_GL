//Asteroide.h
#pragma once
#include <codebase.h>
#include <iostream>
#include "Auxiliares.h"
#include "globales.h"

using namespace cb;
using namespace aux;

#ifndef ASTEROIDE_H
#define ASTEROIDE_H

static const int RES_ASTEROIDE = 5;
static const float velIniAst = 1.0f; //Velocidad inicial de los asteroides
static const Vec3 velIniAngularMax = 1.0f; //Velocidad inicial de los asteroides
static const float MAX_DIST_ASTEROIDES = 200;
static const int NUM_ASTEROIDES = 400;
static const float SUPERFICIE_BASE = 1.0f;
static const float SUPERFICIE_VARIACION = 0.1f;

class Asteroide {
	private:
		Vec3 position;
		Vec3 velocidad;
		Vec3 rotacion;
		Vec3 velocidadAngular;
		Vec3 deformaciones;
		float size;
		Vec3 rotAcionInterna;
		//Radio de cada punto del asteroide respecto a su centro
		Vec3 verticesAsteroide[(RES_ASTEROIDE + 2) * (RES_ASTEROIDE + 2)];
		//Resolución
		float resolucion;

	public:
		Asteroide(
			Vec3 position = randomVec() * MAX_DIST_ASTEROIDES,
			Vec3 velocidad = randomVec() * velIniAst,
			Vec3 rotacion = randomVec() * 180.f,
			Vec3 velocidadAngular = randomVec() * 1,
			Vec3 deformaciones = Vec3(1, 1, 1) + randomVec() * 0.5,
			float size = random(0.5f, 5.f),
			int res = RES_ASTEROIDE);
		void Dibujar();
		Vec3 getPos();
		void Actualizar(float tiempo);
};

#endif


