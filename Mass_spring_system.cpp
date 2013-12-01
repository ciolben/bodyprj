//=============================================================================
//
//   Exercise code for the lecture
//   "Advanced Computer Graphics"
//
//   Adapted from Prof. Dr. Mario Botsch, Bielefeld University
//
//   Copyright (C) 2013 LGG, epfl
//
//   DO NOT REDISTRIBUTE
//=============================================================================


//== INCLUDES =================================================================

#include "Mass_spring_system.h"
#include <gl/glut.h>

//== IMPLEMENTATION ========================================================== 


void Mass_spring_system::clear()
{
    particles.clear();
    springs.clear();
    triangles.clear();
}


//-----------------------------------------------------------------------------


void Mass_spring_system::add_particle(vec3 position, vec3 velocity, float mass, bool locked)
{
    particles.push_back( Particle(position, velocity, mass, locked) );
}

void Mass_spring_system::add_particle(vec2 position, vec2 velocity, float mass, bool locked)
{
    particles.push_back( Particle(
                             vec3(position[0], position[1])
                            , vec3(velocity[0], velocity[1]), mass, locked) );
}
//-----------------------------------------------------------------------------


void Mass_spring_system::add_spring(unsigned int i0, unsigned int i1, bool breakable)
{
    assert(i0 < particles.size());
    assert(i1 < particles.size());
    springs.push_back( Spring(particles[i0], particles[i1], breakable ));
}


//-----------------------------------------------------------------------------


void Mass_spring_system::add_triangle(unsigned int i0, unsigned int i1, unsigned int i2)
{
    assert(i0 < particles.size());
    assert(i1 < particles.size());
    assert(i2 < particles.size());
    triangles.push_back( Triangle(particles[i0], particles[i1], particles[i2]) );
}


//-----------------------------------------------------------------------------


void Mass_spring_system::draw(float particle_radius, bool show_forces, int selected) const
{
    // draw particles
    //glEnable(GL_DEPTH_TEST);
  //  glEnable(GL_LIGHTING);
    //projection
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    gluPerspective(60, 1, 0.1, 100);

    //camera
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
//    gluLookAt(0,0,5 ,0,0,0 ,0,0,1);

   // glPushMatrix();

//    glShadeModel(GL_SMOOTH);
//    glEnable(GL_COLOR_MATERIAL);


    GLfloat specular_term[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat specular_exponant[] = { 100.0 };
    GLfloat light_position[] = { -1.0, 1.0, 1.0, 0.0 };
    GLfloat light_term[] = { 0.8, 0.8, 0.8 };
    GLfloat ambient_term[] = { 0.3, 0.3, 0.3, 1.0 };
    glShadeModel(GL_SMOOTH);

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_term);
    glMaterialfv(GL_FRONT, GL_SHININESS, specular_exponant);

    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_term );
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_term );
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_term);

    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);


    for (unsigned int i=0; i<particles.size(); ++i)
    {
        const Particle& p = particles[i];
        GLfloat ambient_material_term[] = { 0.2, 0.2, 0.2, 1.0 };
        GLfloat diffuse_material_term[] = { 0.2, 0.2, 0.2, 1.0 };

        if (p.locked) {
            glColor3f(0.6, 0.0, 0.0);
            ambient_material_term[0] = 0.6;
            diffuse_material_term[0] = 0.6;
        } else {
            glColor3f(0.0, 0.6, 0.0);
            ambient_material_term[1] = 0.6;
            diffuse_material_term[1] = 0.6;
        }

        if (i == selected) {
            ambient_material_term[0] = 0.6;
            ambient_material_term[1] = 0.6;
            ambient_material_term[2] = 0.6;
            diffuse_material_term[0] = 0.6;
            diffuse_material_term[1] = 0.6;
            diffuse_material_term[2] = 0.6;
        }

        glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_material_term);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_material_term);

        glEnable(GL_LIGHTING);
        glPushMatrix();
        glTranslatef( p.position.x, p.position.y, p.position.z);
        glPushName(i); //*********************************************************
        glutSolidSphere( particle_radius + 0.f, 20, 20 );
        glPopName();

        glPopMatrix();
        glDisable(GL_LIGHTING);

        if (show_forces)
        {
            glLineWidth(1.0);
            glBegin(GL_LINES);
            glVertex3f(p.position.x, p.position.y, p.position.z);
            glVertex3f(p.position[0] + 0.05 * p.force[0]
                    , p.position[1] + 0.05 * p.force[1]
                    , p.position[2] + 0.05 * p.force[2]);
            glEnd();
        }
    }
    glDisable(GL_COLOR_MATERIAL);

    // draw springs
    glDisable(GL_LIGHTING);
    glLineWidth(5.0);
    glColor3f(0,0,0);
    glBegin(GL_LINES);
    for (unsigned int i=0; i<springs.size(); ++i)
    {
        if (!springs[i].is_broken) {
            glVertex3fv( springs[i].particle0->position.data() );
            glVertex3fv( springs[i].particle1->position.data() );
        }
    }
    glEnd();

    // draw area constraints
    glDisable(GL_LIGHTING);
    glColor3f(0.8, 1.0, 0.8);
    glBegin(GL_TRIANGLES);
    for (unsigned int i=0; i<triangles.size(); ++i)
    {
        glVertex3fv( triangles[i].particle0->position.data() );
        glVertex3fv( triangles[i].particle1->position.data() );
        glVertex3fv( triangles[i].particle2->position.data() );
    }
    glEnd();
}

//=============================================================================
