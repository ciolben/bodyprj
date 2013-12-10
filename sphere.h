#ifndef SPHERE_H
#define SPHERE_H

#include "utils/vec3.h"
#include "object3d.h"

class Sphere : public Object3D
{
private:
    float m_radius;

public:
    Sphere(vec3 center, float radius, float m = 0.0f, bool locked = false, vec3 v = vec3(0));

    virtual void draw();
    virtual void collisionResponse(Particle& particle, const float& particleRadius);

    vec3 center() const { return position; }
    void setCenter(const vec3 &center) { position = center; }
    float radius() const {return m_radius; }
    void setRadius(float radius) {m_radius = radius; }
    bool locked;
};

#endif // SPHERE_H
