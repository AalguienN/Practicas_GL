#pragma once
#ifndef GLOBALES_H
#define GLOBALES_H

#include "Blaster.h"
#include <codebase.h>
#include <iostream>

using namespace blasterNS;
using namespace cb;


extern Sistema3d player;

extern float speed_Player;				//Velocidad de la nave

extern const int NUM_BLASTERS = 4;
extern Blaster blasters[];

extern GLuint asteroide;


#endif // !GLOBALES_H
