#ifndef OBJECT3D_H
#define OBJECT3D_H

#include <qgl.h>
#include <gl/glut.h>
#include "Mass_spring_system.h"

class Object3D
{
public:
    virtual void draw() = 0;
    virtual void collisionResponse(Particle& particle, const float& particleRadius) = 0;
};

#endif // OBJECT3D_H
