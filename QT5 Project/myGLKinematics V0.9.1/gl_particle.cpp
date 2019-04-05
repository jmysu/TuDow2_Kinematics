#include "gl_particle.h"


Particle::Particle( ) {
    init();
    }

void Particle::setPosition(qreal x, qreal y, qreal z)
{
    pos0.x = x;
    pos0.y = y;
    pos0.z = z;
    init();
}
void Particle::animate() {
  speed.z += 0.1f;
  pos += 0.1f * speed;
  if (++age == ageMax) init();
}

void Particle::draw() {
  float fA = age/(float)ageMax;
  glColor3f(0.95 , fA , fA);
  glVertex3fv(pos);   
}

void Particle::init() {
  //pos = Vec(2,2,1);
  pos = pos0;

  float angle = 3.0 * M_PI * rand() / RAND_MAX;
  float norm  = 0.35  * rand() / RAND_MAX;
  speed = Vec(norm * cos(angle), norm * sin(angle),  rand() / static_cast<float>(RAND_MAX));
  age = 0;
  ageMax = 5.0 + static_cast<int>(10.0 * rand() / RAND_MAX);
}
