#include "Blaster.h"

Blaster::Blaster(
	Vec3 position,
	Vec3 direction,
	float speed,
	float lifeSpan
) : position(position), direction(direction.normalize()), speed(speed), vivo(false), lifeSpan(lifeSpan) {}

void Blaster::Dibujar() {
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

void Blaster::Actualizar(float tiempo, float speed_Player) {
	if (vivo) {
		position += direction * speed * tiempo;
		lifeSpan -= tiempo;
		if (lifeSpan < 0) {
			Destruir();
		}
		speed = speed_Player + 50;
	}
}

void Blaster::Disparar(Sistema3d player) {
	position = player.geto();
	direction = player.getw() * -1;
	lifeSpan = 5;
	vivo = true;
}

Blaster& Blaster::Destruir() { 
	vivo = false; 
	speed = 0; 
	lifeSpan = 0; 
	return *this;
}

Vec3 Blaster::GetPosition() { return position; }

bool Blaster::isAlive() { return vivo; }
