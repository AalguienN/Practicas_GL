#pragma once
#include "Asteroide.h"
#include <codebase.h>
using namespace cb;

//Asteroide.h
#pragma once
#include <codebase.h>
#include <iostream>
#include "Auxiliares.h"
#include "Blaster.h"


using namespace cb;
using namespace aux;
using namespace blasterNS;



		Asteroide::Asteroide(
			Vec3 position = randomVec() * MAX_DIST_ASTEROIDES,
			Vec3 velocidad = randomVec() * velIniAst,
			Vec3 rotacion = randomVec() * 180.f,
			Vec3 velocidadAngular = randomVec() * 1,
			Vec3 deformaciones = Vec3(1, 1, 1) + randomVec() * 0.5,
			float size = random(0.5f, 5.f),
			int res = RES_ASTEROIDE) :
			position(position), velocidad(velocidad),
			rotacion(rotacion), velocidadAngular(velocidadAngular),
			deformaciones(deformaciones),
			size(size),
			verticesAsteroide(), resolucion(res),
			rotAcionInterna(randomVec() * 180.f)
		{
			SetOffset();
			//cout << velocidadAngular.x << velocidadAngular.y << velocidadAngular.z << endl;
		};
		void SetOffset() {
		}
		void SetOffset(GLfloat va[RES_ASTEROIDE * RES_ASTEROIDE]) {
			cout << "Seteando offset de asteroide ..." << this << endl;
			for (int i = 0; i < RES_ASTEROIDE * RES_ASTEROIDE; i++) {
				this->verticesAsteroide[i] = va[i];
			}
		}
		void Dibujar() {
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
				glCallList(asteroide);

				glPopMatrix();
			}
		}
		Vec3 getPos() { return this->position; }
		void Actualizar(float tiempo) {
			this->position += this->velocidad * tiempo;

			if ((this->position - player.geto()).norm() > MAX_DIST_ASTEROIDES) {
				while ((this->position - player.geto()).norm() > MAX_DIST_ASTEROIDES) {
					this->position = Vec3(random(-1, 1), random(-1, 1), random(-1, 1)).normalize() * MAX_DIST_ASTEROIDES * 0.99f + player.geto();
					this->velocidad = Vec3(random(-velIniAst, velIniAst), random(-velIniAst, velIniAst), random(-velIniAst, velIniAst));
				}
			}

			for (Blaster b : blasters) {
				if (b.isAlive()) {
					if ((b.GetPosition() - this->position).norm() < this->size) {
						this->position = Vec3(random(-1, 1), random(-1, 1), random(-1, 1)).normalize() * MAX_DIST_ASTEROIDES * 0.99f + player.geto();
						this->velocidad = Vec3(random(-velIniAst, velIniAst), random(-velIniAst, velIniAst), random(-velIniAst, velIniAst));
						b.Destruir();
					}
				}
			}

			this->rotacion += velocidadAngular;
		}

	};
}




