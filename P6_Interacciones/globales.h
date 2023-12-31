#pragma once
#ifndef GLOBALES_H
#define GLOBALES_H

#include <iostream>
#include "codebase2.h"
#include "Blaster.h"
#include "Asteroide.h"
#include "Explosion.h"

using namespace cb2;

extern Sistema3d player;

extern Blaster blasters[NUM_BLASTERS];

extern int explActual;
extern const int NUM_EXPLOSIONES;



//extern GLuint asteroide;

#endif // !GLOBALES_H
