
#include "Explosion.h"

using namespace cb2;

int explActual = 0;
//Explosion explosiones[NUM_EXPLOSIONES];
//Asteroide Explosion::fragmentos[NUM_FRAGMENTOS];

Explosion::Explosion(Vec3 posicion, float lifeTime, float size, GLuint textura) :
	posicion(posicion), lifeTime(lifeTime), size(size), textura(textura), vivo(false) //, fragmentos()
{

	for (int i = 0; i < NUM_FRAGMENTOS; i++) {
		//fragmentos
		
	}
};

void Explosion::Iniciar(Vec3 posicion, float size) {
	this->vivo = true;
	this->lifeTime = 1;
	this->size = size;
	this->posicion = posicion;
}

void Explosion::Actualizar(float tiempo) {
	if (vivo) {
		lifeTime -= tiempo;
		//size += tiempo*10;
		for (int i = 0; i < NUM_FRAGMENTOS; i++) {
			//fragmentos[i].Actualizar(tiempo);
		}
		if (lifeTime < 0) { Destruir(); }
	}
}

void Explosion::Dibujar(GLuint text) {
	if (vivo) {

		glPushAttrib(GL_ALL_ATTRIB_BITS);
		//glDisable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

		glBindTexture(GL_TEXTURE_2D, text);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
		static float planoS[] = { player.getu().x,player.getu().y,player.getu().z,0};
		static float planoT[] = { -player.getw().x,-player.getw().y,-player.getw().z,0 };
		glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
		glTexGenfv(GL_S, GL_OBJECT_PLANE, planoS);
		glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
		glTexGenfv(GL_T, GL_OBJECT_PLANE, planoT);
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
		GLfloat mdf[] = { 1,1,1,lifeTime };
		GLfloat msp[] = { 1,1,1,lifeTime };
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mdf);
		glMaterialfv(GL_FRONT, GL_SPECULAR, msp);
		glMaterialf(GL_FRONT, GL_SHININESS, 1000);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glPushMatrix();
		glTranslatef(posicion.x,posicion.y,posicion.z);
		
		glColor4f(0.8f, 0.5f, 0, lifeTime);

		glPushMatrix();
		glTranslatef(random(-1, 1) * size / 2, random(-1, 1) * size / 2, random(-1, 1) * size / 2);
		float r = random(0.9f, 1.f);
		glScalef(r, r, r);
		glutSolidSphere(size / 2, 20, 20);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(random(-1, 1) * size / 2, random(-1, 1) * size / 2, random(-1, 1) * size / 2);
		r = random(0.9f, 1.f);
		glScalef(r, r, r);
		glutSolidSphere(size / 2, 20, 20);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(random(-1, 1) * size / 2, random(-1, 1) * size / 2, random(-1, 1) * size / 2);
		r = random(0.9f, 1.f);
		glScalef(r, r, r);
		glutSolidSphere(size / 2, 20, 20);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(random(-1, 1) * size / 2, random(-1, 1) * size / 2, random(-1, 1) * size / 2);
		r = random(0.9f, 1.f);
		glScalef(r, r, r);
		glutSolidSphere(size / 2, 20, 20);
		glPopMatrix();

		glPopMatrix();
		glEnable(GL_LIGHTING);
		glPopAttrib();
		for (int i = 0; i < NUM_FRAGMENTOS; i++) {
			//fragmentos[i].Dibujar(lista);
		}
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
	}
}

void Explosion::Destruir() {
	vivo = false;
}