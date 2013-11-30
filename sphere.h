#ifndef SPHERE_H
#define SPHERE_H

#include "utils/vec3.h"
#include "object3d.h"

class Sphere : public Object3D
{
private:
    float m_radius;
    vec3 m_center;

public:
    Sphere(vec3 center, float radius);

    virtual void draw();
    virtual void collisionResponse(Particle& particle, const float& particleRadius);

    vec3 center() const { return m_center; }
    void setCenter(const vec3 &center) { m_center = center; }
    float radius() const {return m_radius; }
    void setRadius(float radius) {m_radius = radius; }
};

#endif // SPHERE_H
