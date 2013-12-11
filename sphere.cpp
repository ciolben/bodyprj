#include "sphere.h"

Sphere::Sphere(vec3 center, float radius, float m, bool locked, vec3 v)
    : Object3D(center, v, m, locked), m_radius(radius)
{
}

void Sphere::draw()
{

    GLfloat specular_term[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat specular_exponant[] = { 100.0 };
    GLfloat light_position[] = { -1.0, 1.0, 1.0, 0.0 };
    GLfloat light_term[] = { 0.8, 0.8, 0.8 };
    GLfloat ambient_term[] = { 0.3, 0.3, 0.3, 1.0 };

    GLfloat ambient_material_term[] = { 0.6, 0.6, 0.6, 1.0 };
    GLfloat diffuse_material_term[] = { 0.6, 0.6, 0.6, 1.0 };

    glShadeModel(GL_SMOOTH);

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_material_term);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_material_term);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_term);
    glMaterialfv(GL_FRONT, GL_SHININESS, specular_exponant);

    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_term );
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_term );
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_term);

    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    glutSolidSphere(m_radius, 50, 50);
    glPopMatrix();

    glDisable(GL_COLOR_MATERIAL);
    glDisable(GL_LIGHTING);
}

void Sphere::collisionResponse(Particle &particle, const float& particleRadius)
{
    //basic sphere collision

    vec3 dir = particle.position - position;
    float penDist = norm(dir) - (m_radius + particleRadius);

    dir = normalize(dir);

    vec3 p = dir * penDist;

    if(penDist < 0)
    {
        particle.position -= p;
    }
}
