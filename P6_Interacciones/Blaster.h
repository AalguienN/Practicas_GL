#pragma once
#ifndef BLASTER_H
#define BLASTER_H

#include "codebase2.h"
#include <iostream>

using namespace cb2;


class Blaster;

const int NUM_BLASTERS = 8;

class Blaster {
private:
	Vec3 position;
	Vec3 direction;
	float speed;
	float lifeSpan;
	bool vivo = false;

	//Poner luces aquí en el futuro

public:
	Blaster(
		Vec3 position = Vec3(0, 0, 0),
		Vec3 direction = Vec3(0, 1, 0),
		float speed = 50,
		float lifeSpan = 5
	);

	void Dibujar();

	void Actualizar(float tiempo, float speed_player);

	void Disparar(Sistema3d player, Vec3 offset);

	Blaster& Destruir();

	Vec3 GetPosition();

	bool isAlive();
};

#endif //BLASTER_H


