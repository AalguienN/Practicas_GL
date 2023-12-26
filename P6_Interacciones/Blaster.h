#pragma once
#include <codebase.h>
#include <iostream>
#include "Auxiliares.h"
#include "globales.h"

using namespace cb;

namespace blasterNS {

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
		) : position(position), direction(direction.normalize()), speed(speed), vivo(false), lifeSpan(lifeSpan) {}

		void Dibujar() {
			if (vivo) {
				glPushAttrib(GL_ALL_ATTRIB_BITS);
				//glDisable(GL_LIGHTING);
				glDisable(GL_LIGHTING);
				glDisable(GL_TEXTURE_2D);
				glColor3f(1.0f, 0.0f, 0.0f);
				glPushMatrix();
				glTranslatef(position.x, position.y, position.z);
				glutSolidSphere(0.2, 8, 8);
				glPopMatrix();
				//glEnable(GL_LIGHTING);
				glEnable(GL_TEXTURE_2D);
				glEnable(GL_LIGHTING);
				glPopAttrib();
			}
		}

		void Actualizar(float tiempo) {
			if (vivo) {
				position += direction * speed * tiempo;
				lifeSpan -= tiempo;
				if (lifeSpan < 0) {
					Destruir();
				}
				speed = speed_Player + 50;
			}
		}

		void Disparar() {
			position = player.geto();
			direction = player.getw() * -1;
			lifeSpan = 5;
			vivo = true;
		}

		void Destruir() { vivo = false; speed = 0; }

		Vec3 GetPosition() { return position; }

		bool isAlive() { return vivo; }
	};
}

