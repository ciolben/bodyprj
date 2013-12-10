#ifndef OBJECT3D_H
#define OBJECT3D_H

#include <qgl.h>
#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <gl/glut.h>
#endif
#include "Mass_spring_system.h"
#include "utils/vec3.h"

class Object3D
{
public:
    virtual void draw() = 0;
    virtual void collisionResponse(Particle& particle, const float& particleRadius) = 0;

    Object3D(vec3 p, vec3 v, float m, bool l)
        : position(p), velocity(v), force(0), mass(m), acceleration(0), locked(l){

    }

    vec3 velocity;
    vec3  position;
    vec3  acceleration;
    vec3  force;
    float mass;

    bool locked;
};

#endif // OBJECT3D_H
